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
    void reset();// 重置
    void addHeader(const string key, const string value);//添加请求头
    string getHeader(const string key);//根据key得到请求头的value
    bool parseRequestLine(Buffer* readBuf);//解析请求行
    bool parseRequestHeader(Buffer* readBuf);//解析请求头
    // 解析http请求协议
    bool parseHttpRequest(Buffer* readBuf, HttpResponse* response, Buffer* sendBuf, int socket);
    // 处理http请求协议
    bool processHttpRequest(HttpResponse* response);
    // 解码字符串
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
    // 获取处理状态
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

