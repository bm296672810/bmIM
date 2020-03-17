#ifndef __M_MYSQL_H__
#define __M_MYSQL_H__
#include <string>
#include <iostream>
#include <vector>
#include <mutex>

#include "mysql.h"
#include "../common/common.h"

using namespace std;
namespace bm
{
namespace sql
{
namespace mysql
{
    extern mutex g_mysql_db_mutex;

    class SqlOperate
    {
    public:
        SqlOperate() = delete;
        SqlOperate(const std::string& user, const std::string& pwd, const std::string& ip, unsigned int port);
        ~SqlOperate();

        unsigned int ExecSql(const std::string& sql, bm::sql::SqlResult& result);
        unsigned int ExecSql(const char* sql, SqlResult* result);

        unsigned int Translation();
        unsigned int Rollback();
        unsigned int Commit();

        bool IsConnected() { return m_isConnected; }
    private:
        void initSession(const std::string& user, const std::string& pwd, const std::string& ip, unsigned int port);
    private:
        MYSQL mysql;    //一个数据库结构体
        unsigned int m_port = 3306;
        bool m_isConnected = false;
    };
    class ConnectPool
    {
    public:
        ConnectPool() = delete;
        ConnectPool(const std::string& user, const std::string& pwd, const std::string& host, unsigned int port, unsigned int num);
        ~ConnectPool();
        SqlOperate* GetOperate();
        void BackOperate(SqlOperate* op);
    private:
        void initPool(const std::string& user, const std::string& pwd, const std::string& host, unsigned int port, unsigned int num);
    private:
        vector<SqlOperate*> m_vecOperates;
        mutex m_lock;
        unsigned int max_size = 100;
    };
    extern ConnectPool* g_mysqlConnectPool;
    int initConnectPool(const std::string& user, const std::string& pwd, const std::string& host, unsigned int port, int num);
    int releseConnectPool();
    int execSql(const string& sql, bm::sql::SqlResult& result);
}
}
}
#endif