#include "mmysql.h"
namespace bm
{
namespace sql
{
namespace mysql
{
    ConnectPool* g_mysqlConnectPool = NULL;

    mutex g_mysql_db_mutex;

    SqlOperate::SqlOperate(const std::string& user, const std::string& pwd, const std::string& ip, unsigned int port)
    {
        initSession(user, pwd, ip, port);
    }

    SqlOperate::~SqlOperate()
    {
        mysql_close(&mysql);
    }
    void SqlOperate::initSession(const std::string& user, const std::string& pwd, const std::string& ip, unsigned int port)
    {
        m_port = port;
        mysql_init(&mysql);
        if (mysql_real_connect(&mysql, ip.c_str(), user.c_str(), pwd.c_str(), "imServer", m_port, NULL, 0) == NULL)
        {
            m_isConnected = false;
            printf("连接失败！\\n");
        }
        else
            m_isConnected = true;
    }

    unsigned int SqlOperate::ExecSql(const std::string& sql, SqlResult& result)
    {
        return ExecSql(sql.c_str(), &result);
    }
    unsigned int SqlOperate::ExecSql(const char* sql, SqlResult* result)
    {
        lock_guard<mutex> lock(g_mysql_db_mutex);

        mysql_query(&mysql, sql);
        unsigned int wr = mysql_errno(&mysql);
        if (wr != 0)
            cout << "error:" << mysql_error(&mysql) << endl;

        if (result == NULL)
            return wr;

        MYSQL_RES* res = mysql_store_result(&mysql);
        if (!res)
            return wr;

        MYSQL_FIELD* field = NULL;
        while (field = mysql_fetch_field(res))
        {
            field->name;
            result->m_vecIds.push_back(field->name);
        }

        MYSQL_ROW row = NULL;
        while (row = mysql_fetch_row(res))
        {
            vector<string> tmp;
            for (size_t i = 0; i < res->field_count; i++)
                tmp.push_back(row[i] ? row[i] : "");

            result->m_vecValues.push_back(tmp);
        }

        mysql_free_result(res);

        return wr;
    }
    unsigned int SqlOperate::Translation()
    {
        return ExecSql("BEGIN;", NULL);
    }
    unsigned int SqlOperate::Rollback()
    {
        return ExecSql("ROLLBACK;", NULL);
    }
    unsigned int SqlOperate::Commit()
    {
        return ExecSql("COMMIT;", NULL);
    }


    ConnectPool::ConnectPool(const std::string& user, const std::string& pwd, const std::string& host, unsigned int port, unsigned int num)
    {
        initPool(user, pwd, host, port, num);
    }
    ConnectPool::~ConnectPool()
    {
        for (size_t i = 0; i < m_vecOperates.size(); i++)
            if (m_vecOperates.at(i))
            {
                delete m_vecOperates.at(i);
                m_vecOperates.at(i) = NULL;
            }
    }
    void ConnectPool::initPool(const std::string& user, const std::string& pwd, const std::string& host, unsigned int port, unsigned int num)
    {
        unsigned int s = num;
        // 防止无限制初始化连接池
        if (num > max_size)
            s = max_size;

        for (unsigned int i = 0; i < s;)
        {
            SqlOperate* tmp = new SqlOperate(user, pwd, host, port);
            if (tmp && tmp->IsConnected())
                m_vecOperates.push_back(tmp);
            else
                continue;

            i++;
        }
    }
    SqlOperate* ConnectPool::GetOperate()
    {
        lock_guard<mutex> lock(m_lock);
        static int i = 0;
        do
        {
            if (!m_vecOperates.empty())
            {
                SqlOperate* tmp = m_vecOperates.at(0);
                m_vecOperates.pop_back();

                return tmp;
            }
            else if (i >= 100)
                break;
            else
            {
                i++;
                U_SLEEP(1000);
            }
        } while (m_vecOperates.empty());

        return NULL;
    }
    void ConnectPool::BackOperate(SqlOperate* op)
    {
        lock_guard<mutex> lock(m_lock);

        m_vecOperates.push_back(op);
    }

    int initConnectPool(const std::string& user, const std::string& pwd, const std::string& host, unsigned int port, int num)
    {
        if (!g_mysqlConnectPool)
            g_mysqlConnectPool = new ConnectPool(user, pwd, host, port, num);

        return 0;
    }
    int releseConnectPool()
    {
        if (g_mysqlConnectPool)
            delete g_mysqlConnectPool;

        return 0;
    }
    int execSql(const string& sql, bm::sql::SqlResult& result)
    {
        if (g_mysqlConnectPool)
        {
            SqlOperate* tmp = g_mysqlConnectPool->GetOperate();
            if (tmp)
            {
                unsigned int r = tmp->ExecSql(sql, result);

                g_mysqlConnectPool->BackOperate(tmp);

                return r;
            }
            else
                return -2;
        }

        return -1;
    }
}
}
}
