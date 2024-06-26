#pragma once
#include "Dispatcher.h"
#include "Channel.h"
#include <thread>
#include <queue>
#include <map>
#include <mutex>
using namespace std;

enum class ElemType :char { ADD, DELETE, MODIFY };
struct ChannelElement
{
    ElemType type;   // ��δ���ýڵ��е�channel
    Channel* channel;
};
class Dispatcher;

class EventLoop
{
public:
    EventLoop();
    EventLoop(const string threadName);
    ~EventLoop();
    // ������Ӧ��ģ��
    int run();
    // ����𼤻���ļ�fd
    int eventActive(int fd, int event);
    // ��������������
    int addTask(struct Channel* channel, ElemType type);
    // ������������е�����
    int processTaskQ();
    // ����dispatcher�еĽڵ�
    int add(Channel* channel);
    int remove(Channel* channel);
    int modify(Channel* channel);
    // �ͷ�channel
    int freeChannel(Channel* channel);
    int readMessage();
    // �����߳�ID
    inline thread::id getThreadID()
    {
        return m_threadID;
    }
    inline string getThreadName()
    {
        return m_threadName;
    }
    static int readLocalMessage(void* arg);

private:
    void taskWakeup();
private:
    bool m_isQuit;
    // ��ָ��ָ�������ʵ�� epoll, poll, select
    Dispatcher* m_dispatcher;
    // �������
    queue<ChannelElement*> m_taskQ;
    // map
    map<int, Channel*> m_channelMap;
    // �߳�id, name, mutex
    thread::id m_threadID;
    string m_threadName;
    mutex m_mutex;
    int m_socketPair[2];  // �洢����ͨ�ŵ�fd ͨ��socketpair ��ʼ��


};

