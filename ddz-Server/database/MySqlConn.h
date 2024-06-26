#ifndef MYSQLCONN_H
#define MYSQLCONN_H
//你的代码写在这里
#include <iostream>
#include <mysql.h>
#include <chrono>
using namespace std;
using namespace chrono;

class MySqlConn
{
public:
    // 初始化数据库连接
    MySqlConn();
    // 释放数据库连接资源
    ~MySqlConn();
    // 连接数据库
    bool connect(string user, string dbname, 
    string password, string ip, unsigned short port = 3306);
    // 更新操作 insert、delete、update
    bool update(string sql);
    // 查询操作 select
    bool query(string sql);
    // 向下遍历结果集中的一条记录
    bool next();
    // 得到指定索引对应的字段值
    string value(int index);

    // 刷新一下连接的起始的空闲时间点
    void refreshAliveTime();
    // 返回存活的时间
    long long getAliveTime()const;
    // 事务 transaction
    void transaction();
    // 提交事务
    void commit();
    // 事务回滚
    void rollback();

private:
    void freeResult();

private:
    // 空指针需要使用C API, 需要使用C语言标准, 不能按照c++指定nullptr
    MYSQL* m_conn = NULL;          // 连接 MySQL 服务器的实例
    MYSQL_ROW m_row = NULL;        // 存储当前遍历到的结果集(二级指针 char**)
    MYSQL_RES* m_result = NULL;    // 存储查询到的结果集
    steady_clock::time_point m_alivetime;              // 记录进入空闲状态后的起始存活时间
};

#endif