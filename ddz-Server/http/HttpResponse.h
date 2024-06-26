#pragma once
#include "Buffer.h"
#include <map>
#include <functional>
using namespace std;

enum class StatusCode
{
    Unknown,
    OK = 200,
    MovedPermanently = 301,
    MovedTemporarily = 302,
    BadRequest = 400,
    NotFound = 404
};


class HttpResponse
{
public:
    HttpResponse();
    ~HttpResponse();
    function<void(const string, struct Buffer*, int)> sendDataFunc;
    // �����Ӧͷ
    void addHeader(const string key, const string value);
    // ��֯http��Ӧ����
    void prepareMsg(Buffer* sendBuf, int socket);
    inline void setFileName(string name)
    {
        m_fileName = name;
    }
    inline void setStatusCode(StatusCode code)
    {
        m_statusCode = code;
    }
private:
    // ״̬��: ״̬��, ״̬����
    StatusCode m_statusCode;
    string m_fileName;
    // ��Ӧͷ - ��ֵ��
    map<string, string> m_headers;
    // ����״̬��������Ķ�Ӧ��ϵ
    const map<int, string> m_info = {
        {200, "OK"},
        {301, "MovedPermanently"},
        {302, "MovedTemporarily"},
        {400, "BadRequest"},
        {404, "NotFound"},
    };
};

