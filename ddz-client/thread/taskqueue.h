#ifndef TASKQUEUE_H
#define TASKQUEUE_H

#include "cards.h"
#include <QObject>
#include <QQueue>
#include <QMutex>
#include <QWaitCondition>
struct Task
{
    int bet = 0;
    Cards cs;
};
class TaskQueue : public QObject
{
    Q_OBJECT
public:
    TaskQueue(const TaskQueue& t) = delete;
    TaskQueue& operator=(const TaskQueue& t) = delete;
    static TaskQueue* getInstance();

    void add(Task &t);
    // 2. 取出一个任务
    Task take();
    // 3. 得到任务队列中任务的数量
    int size();
    // 4. 清空任务队列
    void clear();

private:
    explicit TaskQueue(QObject *parent = nullptr);
private:
    static TaskQueue m_taskQ;
    QQueue<Task> m_queue;
    QMutex m_mutex;
    QWaitCondition m_cond;

signals:
};

#endif // TASKQUEUE_H
