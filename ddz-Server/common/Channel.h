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
    // �ж��Ƿ���Ҫ����ļ���������д�¼�
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
    // �ļ�������
    int m_fd;
    // �¼�
    int m_events;
    // �ص������Ĳ���
    void* m_arg;

};

