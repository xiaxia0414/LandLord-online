#pragma once
#include "EventLoop.h"
#include <stdbool.h>
#include "WorkerThread.h"
#include <vector>
using namespace std;

class ThreadPool
{
public:
    ThreadPool(EventLoop* mainLoop, int count);
    ~ThreadPool();
    // �����̳߳�
    void run();
    // ȡ���̳߳��е�ĳ�����̵߳ķ�Ӧ��ʵ��
    EventLoop* takeWorkerEventLoop();
private:
    // ���̵߳ķ�Ӧ��ģ��
    EventLoop* m_mainLoop;
    bool m_isStart;
    int m_threadNum;
    vector<WorkerThread*> m_workerThreads;
    int m_index;
};