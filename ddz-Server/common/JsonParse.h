//
// Created by Hasee on 2024/5/16.
//

#ifndef CLION_SOURCE_UPLOAD_MARKER_JSONPARSE_H
#define CLION_SOURCE_UPLOAD_MARKER_JSONPARSE_H
#include <string>
#include <json/json.h>
#include <memory>

// ����洢���ݵĽṹ��
struct DBInfo
{
    std::string ip;
    unsigned  short port;
    std::string user;
    std::string passwd;
    std::string dbname;
};

class JsonParse
{
public:
    enum DBType{Mysql, Redis};
    JsonParse(std::string fileName = "../config/config.json");
    // ��ȡ����
    std::shared_ptr<DBInfo> getDatabaseInfo(DBType type);
private:
    Json::Value m_root;
};

#endif //CLION_SOURCE_UPLOAD_MARKER_JSONPARSE_H
