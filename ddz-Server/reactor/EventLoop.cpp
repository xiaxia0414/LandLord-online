#include "EventLoop.h"
#include "EpollDispatcher.h"
#include <assert.h>
#include "EventLoop.h"
#include <assert.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


EventLoop::EventLoop() : EventLoop(string())
{
}

EventLoop::EventLoop(const string threadName)
{
    m_isQuit = true;    // Ĭ��û������
    m_threadID = this_thread::get_id();
    m_threadName = threadName == string() ? "MainThread" : threadName;
    m_dispatcher = new EpollDispatcher(this);
    // map
    m_channelMap.clear();
    int ret = socketpair(AF_UNIX, SOCK_STREAM, 0, m_socketPair);
    if (ret == -1)
    {
        perror("socketpair");
        exit(0);
    }
#if 0
    // ָ������: evLoop->socketPair[0] ��������, evLoop->socketPair[1] ��������
    Channel* channel = new Channel(m_socketPair[1], FDEvent::ReadEvent,
        readLocalMessage, nullptr, nullptr, this);
#else
    // �� - bind
    auto obj = bind(&EventLoop::readMessage, this);
    Channel* channel = new Channel(m_socketPair[1], FDEvent::ReadEvent,
        obj, nullptr, nullptr, this);
#endif
    // channel ��ӵ��������
    addTask(channel, ElemType::ADD);
}

EventLoop::~EventLoop()
{
}

int EventLoop::run()
{
    m_isQuit = false;
    // �Ƚ��߳�ID�Ƿ�����
    if (m_threadID != this_thread::get_id())
    {
        return -1;
    }
    // ѭ�������¼�����
    while (!m_isQuit)
    {
        m_dispatcher->dispatch();    // ��ʱʱ�� 2s
        processTaskQ();
    }
    return 0;
}

int EventLoop::eventActive(int fd, int event)
{
    if (fd < 0)
    {
        return -1;
    }
    // ȡ��channel
    Channel* channel = m_channelMap[fd];
    assert(channel->getSocket() == fd);
    if (event & (int)FDEvent::ReadEvent && channel->readCallback)
    {
        channel->readCallback(const_cast<void*>(channel->getArg()));
    }
    if (event & (int)FDEvent::WriteEvent && channel->writeCallback)
    {
        channel->writeCallback(const_cast<void*>(channel->getArg()));
    }
    return 0;
}

int EventLoop::addTask(Channel* channel, ElemType type)
{
    // ����, ����������Դ
    m_mutex.lock();
    // �����½ڵ�
    ChannelElement* node = new ChannelElement;
    node->channel = channel;
    node->type = type;
    m_taskQ.push(node);
    m_mutex.unlock();
    // ����ڵ�
    /*
    * ϸ��:
    *   1. ��������ڵ�����: �����ǵ�ǰ�߳�Ҳ�����������߳�(���߳�)
    *       1). �޸�fd���¼�, ��ǰ���̷߳���, ��ǰ���̴߳���
    *       2). ����µ�fd, �������ڵ�Ĳ����������̷߳����
    *   2. ���������̴߳����������, ��Ҫ�ɵ�ǰ�����߳�ȡ����
    */
    if (m_threadID == this_thread::get_id())
    {
        // ��ǰ���߳�(�������̵߳ĽǶȷ���)
        processTaskQ();
    }
    else
    {
        // ���߳� -- �������̴߳�����������е�����
        // 1. ���߳��ڹ��� 2. ���̱߳�������:select, poll, epoll
        taskWakeup();
    }
    return 0;
}

int EventLoop::processTaskQ()
{
    // ȡ��ͷ���
    while (!m_taskQ.empty())
    {
        m_mutex.lock();
        ChannelElement* node = m_taskQ.front();
        m_taskQ.pop();  // ɾ���ڵ�
        m_mutex.unlock();
        Channel* channel = node->channel;
        if (node->type == ElemType::ADD)
        {
            // ���
            add(channel);
        }
        else if (node->type == ElemType::DELETE)
        {
            // ɾ��
            remove(channel);
        }
        else if (node->type == ElemType::MODIFY)
        {
            // �޸�
            modify(channel);
        }
        delete node;
    }
    return 0;
}

int EventLoop::add(Channel* channel)
{
    int fd = channel->getSocket();
    // �ҵ�fd��Ӧ������Ԫ��λ��, ���洢
    if (m_channelMap.find(fd) == m_channelMap.end())
    {
        m_channelMap.insert(make_pair(fd, channel));
        m_dispatcher->setChannel(channel);
        int ret = m_dispatcher->add();
        return ret;
    }
    return -1;
}

int EventLoop::remove(Channel* channel)
{
    int fd = channel->getSocket();
    if (m_channelMap.find(fd) == m_channelMap.end())
    {
        return -1;
    }
    m_dispatcher->setChannel(channel);
    int ret = m_dispatcher->remove();
    return ret;
}

int EventLoop::modify(Channel* channel)
{
    int fd = channel->getSocket();
    if (m_channelMap.find(fd) == m_channelMap.end())
    {
        return -1;
    }
    m_dispatcher->setChannel(channel);
    int ret = m_dispatcher->modify();
    return ret;
}

int EventLoop::readLocalMessage(void* arg)
{
    EventLoop* evLoop = static_cast<EventLoop*>(arg);
    char buf[256];
    read(evLoop->m_socketPair[1], buf, sizeof(buf));
    return 0;
}

void EventLoop::taskWakeup()
{
    const char* msg = "����Ҫ��Ϊ������������!!!";
    write(m_socketPair[0], msg, strlen(msg));
}

int EventLoop::freeChannel(Channel* channel)
{
    // ɾ�� channel �� fd �Ķ�Ӧ��ϵ
    auto it = m_channelMap.find(channel->getSocket());
    if (it != m_channelMap.end())
    {
        m_channelMap.erase(it);
        close(channel->getSocket());
        delete channel;
    }
    return 0;
}

int EventLoop::readMessage()
{
    char buf[256];
    read(m_socketPair[1], buf, sizeof(buf));
    return 0;
}
