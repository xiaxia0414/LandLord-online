//
// Created by Hasee on 2024/4/24.
//
#include "iostream"
#include "Codec.h"
Codec::Codec(Message *msg)
{
    reLoad(msg);
}

Codec::Codec(std::string msg)
{
    reLoad(msg);
}

std::string Codec::encodeMsg()
{
    m_obj.SerializeToString(&m_msg);
    return m_msg;
}

std::shared_ptr<Message> Codec::decodeMsg()
{

    m_obj.ParseFromString(m_msg);
    Message *msg = new Message;
    msg->userName = m_obj.username();
    msg->roomName =m_obj.roomname();
    msg->data1 =m_obj.data1();
    msg->data2 = m_obj.data2();
    msg->data3 = m_obj.data3();
    msg->reqcode = m_obj.reqcode();
    msg->rescode = m_obj.rescode();

    std::shared_ptr <Message> ptr(msg, [this](Message* pt){
        delete pt;
        std::cout << "Message 被析构了...";
    });
    return ptr;
}

void Codec::reLoad(std::string  msg)
{
    m_msg = msg;
}

void Codec::reLoad(Message *msg)
{
    m_obj.set_username(msg->userName);
    m_obj.set_roomname(msg->roomName);
    m_obj.set_data1(msg->data1);
    m_obj.set_data2(msg->data2);
    m_obj.set_data3(msg->data3);
    m_obj.set_reqcode(msg->reqcode);
    m_obj.set_rescode(msg->rescode);
}
