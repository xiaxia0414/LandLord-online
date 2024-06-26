#pragma once
#include <functional>

enum class FDEvent
{
    TimeOut = 0x01,
    ReadEvent = 0x02,
    WriteEvent = 0x04
};

class Channel
{
public:
    using handleFunc = std::function<int(void*)>;
    Channel(int fd, FDEvent events, handleFunc readFunc, handleFunc writeFunc, handleFunc destroyFunc, void* arg);
    handleFunc readCallback;
    handleFunc writeCallback;
    handleFunc destroyCallback;
    // 判断是否需要检测文件描述符的写事件
    void writeEventEnable(bool flag);
    bool isWriteEventEnable();
    bool isReadEventEnable();
    void setCurrentEvent(FDEvent ev);
    inline int getEvent()
    {
        return m_events;
    }
    inline int getSocket()
    {
        return m_fd;
    }
    inline const void* getArg()
    {
        return m_arg;
    }

private:
    // 文件描述符
    int m_fd;
    // 事件
    int m_events;
    // 回调函数的参数
    void* m_arg;

};

