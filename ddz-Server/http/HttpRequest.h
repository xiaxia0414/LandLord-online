#pragma once
#include "Buffer.h"
#include <stdbool.h>
#include "HttpResponse.h"
#include <map>
#include <functional>
using namespace std;
enum class PrecessState :char
{
    ParseReqLine,
    ParseReqHeaders,
    ParseReqBody,
    ParseReqDone
};
class HttpRequest
{
public:
    HttpRequest();
    ~HttpRequest();
    void reset();// ����
    void addHeader(const string key, const string value);//�������ͷ
    string getHeader(const string key);//����key�õ�����ͷ��value
    bool parseRequestLine(Buffer* readBuf);//����������
    bool parseRequestHeader(Buffer* readBuf);//��������ͷ
    // ����http����Э��
    bool parseHttpRequest(Buffer* readBuf, HttpResponse* response, Buffer* sendBuf, int socket);
    // ����http����Э��
    bool processHttpRequest(HttpResponse* response);
    // �����ַ���
    string decodeMsg(string msg);
    const string getFileType(const string name);
    static void sendDir(string dirName, Buffer* sendBuf, int cfd);
    static void sendFile(string fileName, Buffer* sendBuf, int cfd);
    inline void setMethod(string method)
    {
        m_method = method;
    }
    inline void seturl(string url)
    {
        m_url = url;
    }
    inline void setVersion(string version)
    {
        m_version = version;
    }
    // ��ȡ����״̬
    inline PrecessState getState()
    {
        return m_curState;
    }
    inline void setState(PrecessState state)
    {
        m_curState = state;
    }
private:
    char* splitRequestLine(const char* start, const char* end,
        const char* sub, function<void(string)> callback);
    int hexToDec(char c);

private:
    string m_method;
    string m_url;
    string m_version;
    map<string, string> m_reqHeaders;
    PrecessState m_curState;
};

