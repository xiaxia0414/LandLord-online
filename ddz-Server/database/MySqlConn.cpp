#include "MySqlConn.h"
#include "Log.h"

MySqlConn::MySqlConn()
{
    // 初始化数据库连接
    m_conn = mysql_init(NULL);
    // 设置 utf8 编码
    mysql_set_character_set(m_conn, "utf8");
}

MySqlConn::~MySqlConn()
{
    freeResult();
    // 释放数据库连接资源
    if (m_conn != NULL)
    {
        mysql_close(m_conn);
    }
    Debug("+++++++++++++++++++++++++++++++++++++++++++++++ mysql construct ***************************************************");
}

bool MySqlConn::connect(string user, string dbname, string password, string ip, unsigned short port)
{
    // 连接数据库
    MYSQL* p = mysql_real_connect(m_conn, ip.c_str(), user.c_str(),
        password.c_str(), dbname.c_str(), port, NULL, 0);
    return p != NULL;
}

bool MySqlConn::update(string sql)
{
    // 更新操作 insert、delete、update
    // 执行失败, 返回非 0 值
    if (mysql_query(m_conn, sql.c_str()))
    {
        // LOG("更新失败:" + sql);
        return false;
    }
    return true;
}

bool MySqlConn::query(string sql)
{
    // 释放上一次查询时结果集中的数据
    freeResult();
    // 查询操作 select
    if (mysql_query(m_conn, sql.c_str()))
    {
        // LOG("查询失败:" + sql);
        return false;
    }
    // 保存结果集到客户端
    m_result = mysql_store_result(m_conn);
    return true;
}

bool MySqlConn::next()
{
    if (m_result != NULL)
    {
        m_row = mysql_fetch_row(m_result);
        if (m_row != NULL)
        {
            return true;
        }
    }
    return false;
}

string MySqlConn::value(int index)
{
    // 获取结果集中的总列数
    int rowCount = mysql_num_fields(m_result);
    if (index >= rowCount || index < 0)
    {
        return string();
    }
    char* val = m_row[index];
    Debug("查询出的数据为: %s", val);
    // 获取index字段数据的字节长度, 保证char* -> string 不会出现问题(防止二进制数据转换出问题)
    unsigned long length = mysql_fetch_lengths(m_result)[index];
    return string(val, length);
}

void MySqlConn::refreshAliveTime()
{
    m_alivetime = steady_clock::now();
}

long long MySqlConn::getAliveTime() const
{
    nanoseconds res = steady_clock::now() - m_alivetime;
    milliseconds millSec = duration_cast<milliseconds>(res);
    return millSec.count();
}

void MySqlConn::freeResult()
{
    if (m_result)
    {
        mysql_free_result(m_result);
        m_result = NULL;
    }
}

void MySqlConn::transaction()
{
    // auto_mode: 1-自动, 0-手动
    mysql_autocommit(m_conn, 0);
}

void MySqlConn::commit()
{
    mysql_commit(m_conn);
    mysql_autocommit(m_conn, 1);
}

void MySqlConn::rollback()
{
    mysql_rollback(m_conn);
    mysql_autocommit(m_conn, 1);
}
