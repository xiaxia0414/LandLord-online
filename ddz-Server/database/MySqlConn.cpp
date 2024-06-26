#include "MySqlConn.h"
#include "Log.h"

MySqlConn::MySqlConn()
{
    // ��ʼ�����ݿ�����
    m_conn = mysql_init(NULL);
    // ���� utf8 ����
    mysql_set_character_set(m_conn, "utf8");
}

MySqlConn::~MySqlConn()
{
    freeResult();
    // �ͷ����ݿ�������Դ
    if (m_conn != NULL)
    {
        mysql_close(m_conn);
    }
    Debug("+++++++++++++++++++++++++++++++++++++++++++++++ mysql construct ***************************************************");
}

bool MySqlConn::connect(string user, string dbname, string password, string ip, unsigned short port)
{
    // �������ݿ�
    MYSQL* p = mysql_real_connect(m_conn, ip.c_str(), user.c_str(),
        password.c_str(), dbname.c_str(), port, NULL, 0);
    return p != NULL;
}

bool MySqlConn::update(string sql)
{
    // ���²��� insert��delete��update
    // ִ��ʧ��, ���ط� 0 ֵ
    if (mysql_query(m_conn, sql.c_str()))
    {
        // LOG("����ʧ��:" + sql);
        return false;
    }
    return true;
}

bool MySqlConn::query(string sql)
{
    // �ͷ���һ�β�ѯʱ������е�����
    freeResult();
    // ��ѯ���� select
    if (mysql_query(m_conn, sql.c_str()))
    {
        // LOG("��ѯʧ��:" + sql);
        return false;
    }
    // �����������ͻ���
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
    // ��ȡ������е�������
    int rowCount = mysql_num_fields(m_result);
    if (index >= rowCount || index < 0)
    {
        return string();
    }
    char* val = m_row[index];
    Debug("��ѯ��������Ϊ: %s", val);
    // ��ȡindex�ֶ����ݵ��ֽڳ���, ��֤char* -> string �����������(��ֹ����������ת��������)
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
    // auto_mode: 1-�Զ�, 0-�ֶ�
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
