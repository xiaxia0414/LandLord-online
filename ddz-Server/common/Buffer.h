#pragma once
#include <string>
using namespace std;
class Buffer
{
public:
    Buffer(int size);
    ~Buffer();
    // ����
    void extendRoom(int size);
    // �õ�ʣ��Ŀ�д���ڴ�����
    inline int writeableSize()
    {
        return m_capacity - m_writePos;
    }
    // �õ�ʣ��Ŀɶ����ڴ�����
    inline int readableSize()
    {
        return m_writePos - m_readPos;
    }
    // д�ڴ� 1. ֱ��д 2. �����׽�������
    int appendString(const char* data, int size);
    int appendString(const char* data);
    int appendString(const string data);
    int appendHead(const int length);
    int appendPackage(const string data);
    int socketRead(int fd);
    // ����\r\nȡ��һ��, �ҵ��������ݿ��е�λ��, ���ظ�λ��
    char* findCRLF();
    // ��������
    int sendData(int socket);    // ָ���ڴ��ָ��
    // �õ������ݵ���ʼλ��
    inline char* data()
    {
        return m_data + m_readPos;
    }
    inline string data(int length)
    {
       string msg(m_data+m_readPos,length);
       m_readPos += length;
        return  msg;
    }
    inline int readPosIncrease(int count)
    {
        m_readPos += count;
        return m_readPos;
    }
private:
    char* m_data;
    int m_capacity;
    int m_readPos = 0;
    int m_writePos = 0;
};

