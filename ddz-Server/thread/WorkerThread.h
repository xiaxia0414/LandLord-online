#pragma once
#include <thread>
#include <mutex>
#include <condition_variable>
#include "EventLoop.h"
using namespace std;
class WorkerThread
{
public:
    WorkerThread(int index);
    ~WorkerThread();
    // �����߳�
    void run();
    inline EventLoop* getEventLoop()
    {
        return m_evLoop;
    }
private:
    void running();

private:
    thread* m_thread;   // �����̵߳�ʵ��
    thread::id m_threadID; // ID
    string m_name;
    mutex m_mutex;  // ������
    condition_variable m_cond;    // ��������
    EventLoop* m_evLoop;   // ��Ӧ��ģ��

};

