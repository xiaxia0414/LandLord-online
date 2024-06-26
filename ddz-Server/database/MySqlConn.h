#ifndef MYSQLCONN_H
#define MYSQLCONN_H
//��Ĵ���д������
#include <iostream>
#include <mysql.h>
#include <chrono>
using namespace std;
using namespace chrono;

class MySqlConn
{
public:
    // ��ʼ�����ݿ�����
    MySqlConn();
    // �ͷ����ݿ�������Դ
    ~MySqlConn();
    // �������ݿ�
    bool connect(string user, string dbname, 
    string password, string ip, unsigned short port = 3306);
    // ���²��� insert��delete��update
    bool update(string sql);
    // ��ѯ���� select
    bool query(string sql);
    // ���±���������е�һ����¼
    bool next();
    // �õ�ָ��������Ӧ���ֶ�ֵ
    string value(int index);

    // ˢ��һ�����ӵ���ʼ�Ŀ���ʱ���
    void refreshAliveTime();
    // ���ش���ʱ��
    long long getAliveTime()const;
    // ���� transaction
    void transaction();
    // �ύ����
    void commit();
    // ����ع�
    void rollback();

private:
    void freeResult();

private:
    // ��ָ����Ҫʹ��C API, ��Ҫʹ��C���Ա�׼, ���ܰ���c++ָ��nullptr
    MYSQL* m_conn = NULL;          // ���� MySQL ��������ʵ��
    MYSQL_ROW m_row = NULL;        // �洢��ǰ�������Ľ����(����ָ�� char**)
    MYSQL_RES* m_result = NULL;    // �洢��ѯ���Ľ����
    steady_clock::time_point m_alivetime;              // ��¼�������״̬�����ʼ���ʱ��
};

#endif