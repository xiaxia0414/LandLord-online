#include "codec.h"
#include <QDebug>

Codec::Codec(Message *msg)
{
    reLoad(msg);
}

Codec::Codec(QByteArray msg)
{
    reLoad(msg);
}

QByteArray Codec::encodeMsg()
{
    std::string out;
    m_obj.SerializeToString(&out);
    return QByteArray::fromStdString(out);
}

QSharedPointer<Message> Codec::decodeMsg()
{
    std::string data = m_msg.toStdString();
    m_obj.ParseFromString(data);
    Message *msg = new Message;
    msg->userName = QByteArray::fromStdString(m_obj.username());
    msg->roomName = QByteArray::fromStdString(m_obj.roomname());
    msg->data1 = QByteArray::fromStdString(m_obj.data1());
    msg->data2 = QByteArray::fromStdString(m_obj.data2());
    msg->data3 = QByteArray::fromStdString(m_obj.data3());
    msg->reqcode = m_obj.reqcode();
    msg->rescode = m_obj.rescode();

    QSharedPointer<Message> ptr(msg, [this](Message* pt){
        delete pt;
        qDebug() << "Message 被析构了...";
    });
    return ptr;
}

void Codec::reLoad(QByteArray msg)
{
    m_msg = msg;
}

void Codec::reLoad(Message *msg)
{
    m_obj.set_username(msg->userName.toStdString());
    m_obj.set_roomname(msg->roomName.toStdString());
    m_obj.set_data1(msg->data1.toStdString());
    m_obj.set_data2(msg->data2.toStdString());
    m_obj.set_data3(msg->data3.toStdString());
    m_obj.set_reqcode(msg->reqcode);
    m_obj.set_rescode(msg->rescode);
}
