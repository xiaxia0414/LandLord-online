//
// Created by Hasee on 2024/4/24.
//

#ifndef CLION_SOURCE_UPLOAD_MARKER_CODEC_H
#define CLION_SOURCE_UPLOAD_MARKER_CODEC_H


#include <string>
#include "infomation.pb.h"
#include <memory>
struct Message
{
    std::string userName;
    std::string roomName;
    std::string data1;
    std::string data2;
    std::string data3;
    RequestCode reqcode;
    RespondCode rescode;
};

class Codec
{
public:
    // 序列化
    Codec(Message* msg);
    // 反序列化
    Codec(std::string msg);
    // 数据编码
    std::string encodeMsg();
    // 数据解码
    std::shared_ptr <Message> decodeMsg();
    // 重新加载数据
    void reLoad(std::string msg);
    void reLoad(Message* msg);
private:
    std::string m_msg;
    Information m_obj;  // 通过这个对象处理数据的序列化和反序列化
};


#endif //CLION_SOURCE_UPLOAD_MARKER_CODEC_H
