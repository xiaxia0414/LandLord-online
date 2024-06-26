#include "taskqueue.h"
TaskQueue TaskQueue::m_taskQ;
TaskQueue::TaskQueue(QObject *parent)
    : QObject{parent}
{

}

TaskQueue *TaskQueue::getInstance()
{
    return &m_taskQ;
}

void TaskQueue::add(Task &t)
{
    m_mutex.lock();
    m_queue.enqueue(t);
    m_mutex.unlock();
    m_cond.wakeAll();
}

Task TaskQueue::take()
{
    Task t;
    m_mutex.lock();
    while(1)
    {
        if(m_queue.size() > 0)
        {
            t = m_queue.dequeue();
            break;
        }
        else
        {
            m_cond.wait(&m_mutex);
        }
    }
    m_mutex.unlock();
    return t;
}

int TaskQueue::size()
{
    m_mutex.lock();
    int size = m_queue.size();
    m_mutex.unlock();
    return size;
}

void TaskQueue::clear()
{
    m_mutex.lock();
    m_queue.clear();
    m_mutex.unlock();
}
