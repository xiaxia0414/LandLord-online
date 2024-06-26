#include "TcpConnection.h"
#include "HttpRequest.h"
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <stdio.h>
#include "Codec.h"
#include "RsaCrypto.h"
#include "Log.h"
#include "Room.h"
TcpConnection::TcpConnection(int fd, EventLoop* evloop)
{
    m_evLoop = evloop;
    m_readBuf = new Buffer(10240);
    m_writeBuf = new Buffer(10240);
    m_reply = new Communication;
    auto delFunc = std::bind(&TcpConnection::addDeleteTask, this);
    auto writeFunc = std::bind(&TcpConnection::addWriteTask, this, std::placeholders::_1);
    m_reply->setCallback(writeFunc, delFunc);
    // http
//    m_request = new HttpRequest;
//    m_response = new HttpResponse;
    m_name = "Connection-" + to_string(fd);
    prepareSecretKey();
    printf("%s",m_writeBuf->data());
    m_channel = new Channel(fd, FDEvent::WriteEvent, processRead, processWrite, destroy, this);
    evloop->addTask(m_channel, ElemType::ADD);
}

TcpConnection::~TcpConnection()
{
    if (m_readBuf && m_readBuf->readableSize() == 0 &&
        m_writeBuf && m_writeBuf->readableSize() == 0)
    {
        delete m_readBuf;
        delete m_writeBuf;

        m_evLoop->freeChannel(m_channel);
    }
    Debug("���ӶϿ�, �ͷ���Դ, gameover, connName: %s", m_name.data());
}

int TcpConnection::processRead(void* arg)
{
    TcpConnection* conn = static_cast<TcpConnection*>(arg);
    // ��������
    int socket = conn->m_channel->getSocket();
    int count = conn->m_readBuf->socketRead(socket);
    if (count > 0)
    {//��������������
        conn->m_reply->parseRequest(conn->m_readBuf);
    }
    else if(count<=0)
    {
    conn->addDeleteTask();
        Debug("��ͻ��˵������ѶϿ�...");
    }

    return 0;
}

int TcpConnection::processWrite(void* arg)
{
    Debug("��ʼ����������....");
    TcpConnection* conn = static_cast<TcpConnection*>(arg);
    // ��������
    int count = conn->m_writeBuf->sendData(conn->m_channel->getSocket());

    if (count > 0)
    {
        // �ж������Ƿ�ȫ�����ͳ�ȥ��
        if (conn->m_writeBuf->readableSize() == 0)
        {
            // 1. ���ټ��д�¼� -- �޸�channel�б�����¼�
            conn->m_channel->setCurrentEvent(FDEvent::ReadEvent);
            // 2. �޸�dispatcher���ļ��� -- �������ڵ�
            conn->m_evLoop->addTask(conn->m_channel, ElemType::MODIFY);
            // 3. ɾ������ڵ�
            //��������Ҫ������
            //conn->m_evLoop->addTask(conn->m_channel, ElemType::DELETE);
            Debug("�������������....");
        }
    }
    return 0;
}

int TcpConnection::destroy(void* arg)
{
    TcpConnection* conn = static_cast<TcpConnection*>(arg);
    if (conn != nullptr)
    {
        delete conn;
    }
    return 0;
}

void TcpConnection::prepareSecretKey()
{
    Room redis;
    redis.initEnvironment();
    std::string pubkey = redis.rsaSecKey("PublicKey");
    Message msg;
    msg.rescode = RespondCode::RsaFenFa;
    msg.data1 = pubkey;
    RsaCrypto rsa;
    rsa.parseStringToKey(redis.rsaSecKey("PrivateKey"),RsaCrypto::PrivateKey);
    std::string data = rsa.sign(pubkey);
    msg.data2 = data;
    Codec codec(&msg);
    data = codec.encodeMsg();
    m_writeBuf->appendPackage(data);
}

void TcpConnection::addWriteTask(std::string data) {
    m_writeBuf->appendPackage(data);
#if 0
    m_channel->setCurrentEvent(FDEvent::WriteEvent);
    m_evLoop->addTask(m_channel,ElemType::MODIFY);
#else
    m_writeBuf->sendData(m_channel->getSocket());
#endif
}

void TcpConnection::addDeleteTask() {
    m_evLoop->addTask(m_channel,ElemType::DELETE);
    Debug("�Ͽ��˺ͷ�����������,connName = %s",m_name.data());
}
