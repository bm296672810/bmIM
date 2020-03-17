#include <iostream>
#include <assert.h>
#include <stdarg.h>
#include <regex>

#include "msqlite.h"

namespace bm {
namespace sql
{
namespace sqlite
{
    mutex g_sqlite_db_mutex;
    string g_sqlitePath = "database/database.db";
    string _aesKey = "0123456789abcdef";
    int _aesKeyLen = 16;
    string GetCurrentPath()
    {
        string exePath;
        char* path = new char[256];
        memset(path, 0, 256);
        _get_pgmptr(&path);
        exePath = path;
        // 内存泄漏
        //delete[] path;

        regex reg("\\\\");
        exePath = regex_replace(exePath, reg, "/");

        string::size_type pos = exePath.rfind("/");
        if (pos != string::npos)
            exePath = exePath.substr(0, pos);

        return exePath;
    }
    static int callback(void* result, int argc, char** argv, char** colName)
    {
        SqlResult* results = NULL;
        if (result)
            results = (SqlResult*)result;

        if (results)
        {
            vector<string> tmp;
            if (results->m_vecIds.empty())
                for (int i = 0; i < argc; i++)
                {
                    results->m_vecIds.push_back(colName[i]);

                    tmp.push_back(argv[i] ? argv[i] : "NULL");
                }
            else
                for (int i = 0; i < argc; i++)
                    tmp.push_back(argv[i] ? argv[i] : "NULL");

            results->m_vecValues.push_back(tmp);
        }

        return 0;
    }

    SqliteOperate::SqliteOperate(const string& filePath, const string& connectName)
    {
        _CreateDatabase(filePath, connectName);
    }
    int SqliteOperate::_CreateDatabase(const string& filePath, const string& connectName)
    {
        int r = -1;
        if (filePath.empty())
            return r;

        r = sqlite3_open(filePath.c_str(), &m_database);
        if (r)
            return r;
#if USE_DB_CRYPTO
        r = sqlite3_rekey(m_database, _aesKey.c_str(), _aesKeyLen);
        if (SQLITE_NOTADB == r)
            r = sqlite3_key(m_database, _aesKey.c_str(), _aesKeyLen);

        if (r)
            return r;
#endif
        m_bValid = true;

        m_strName = connectName;
        return r;
    }
    SqliteOperate::~SqliteOperate()
    {
        if (m_database)
        {
            sqlite3_close(m_database);
            m_database = NULL;
        }

        if (m_cErrorMsg)
            delete[] m_cErrorMsg;
    }
    int SqliteOperate::ExecSql(const string& sql, SqlResult& result)
    {
        return ExecSql(sql.c_str(), result);
    }
    int SqliteOperate::ExecSql(const char* sql, SqlResult& result)
    {
        return ExecSql(sql, callback, &result);
    }
    int SqliteOperate::ExecSql(const char* sql, int (*callback)(void*, int, char**, char**), SqlResult* result)
    {
        lock_guard<mutex> lock(g_sqlite_db_mutex);
        int r = sqlite3_exec(m_database, sql, callback, result, &m_cErrorMsg);
        if (SQLITE_OK != r)
            return r;

        return r;
    }
    int SqliteOperate::Translation()
    {
        int r = ExecSql("BEGIN", NULL, NULL);
        return r;
    }
    int SqliteOperate::Rollback()
    {
        int r = ExecSql("ROLLBACK", NULL, NULL);
        return r;
    }

    int SqliteOperate::Commit()
    {
        int r = ExecSql("COMMIT", NULL, NULL);
        return r;
    }

    // 执行sql语句
    int execSql(const string& sql, SqlResult& result)
    {
        return execSql(sql.c_str(), result);
    }
    int execSql(const char* sql, SqlResult& result)
    {
        return execSql(sql, &result, callback);
    }
    int execSql(const char* sql, SqlResult* result, int(*callback)(void*, int, char**, char**), SqliteOperate* operate)
    {
        SqliteOperate* op = NULL;
        if (operate)
            op = operate;
        else
            op = new SqliteOperate(g_sqlitePath, "mainConnect");

        int r = 0;
        if (op && op->CheckValid())
            r = op->ExecSql(sql, callback, result);

        return r;
    }

    int execSql(const string& sql, SqlResult& result, const string& dbPath)
    {
        SqliteOperate* op = new SqliteOperate(dbPath, "connect select");

        int r = op->ExecSql(sql, result);

        delete op;

        return r;
    }

    int inserts(const vector<string>& sqls, const string& dbPath)
    {
        SqliteOperate* op = new SqliteOperate(dbPath, "connect insert");

        int r = 0;
        op->Translation();
        for (auto i : sqls)
        {
            r = op->ExecSql(i.c_str(), NULL, NULL);
            if (r)
            {
                op->Rollback();
                return r;
            }
        }
        op->Commit();

        delete op;

        return r;
    }

    // 从某个表中查询相应的值
    //string selectResult(const string& tableName, const string& selectKey, const string& conditionKey, const string& conditionValue);
    int selectResult(const string& tableName, const string& selectKey, const string& conditionKey, const string& conditionValue, vector<string>& results)
    {
        char sql[256] = { 0 };
        sprintf_s(sql, "SELECT %s FROM %s WHERE %s='%s'", selectKey.c_str(), tableName.c_str(), conditionKey.c_str(), conditionValue.c_str());
        SqlResult result;

        int r = execSql(sql, result);
        if (SQLITE_OK != r)
            return r;

        assert(result.m_vecValues.size() == 1);

        results = result.m_vecValues.at(0);

        return r;
    }

    // 更新数据到数据库
    int update(const string& tableName, const string& opKey, const string& opValue, const string& conditionKey, const string& conditionValue)
    {
        char sql[256] = { 0 };
        sprintf_s(sql, "UPDATE %s SET %s='%s' WHERE %s='%s'",
            tableName.c_str(), opKey.c_str(), opValue.c_str(), conditionKey.c_str(), conditionValue.c_str());

        int r = execSql(sql, NULL);

        return r;
    }
    // 更新一条数据的多个列的值
    int update(const string& tableName, const vector<string>& opKeys, const vector<string>& opValues, const string& conditionKey, const string& conditionValue)
    {
        string strSet;
        for (size_t i = 0; i < opKeys.size(); i++)
            strSet += opKeys.at(i) + "=" + opValues.at(i) + ",";

        if (strSet.empty())
            return ErrorCode::CONNECT_INVALID;
        else
            strSet.pop_back();

        string sql = "UPDATE " + tableName + " SET " + strSet + " WHERE " + conditionKey + " = " + conditionValue;

        int r = execSql(sql.c_str(), NULL);

        return r;
    }

    int _updates(const string& tableName, const string& opKey, const vector<string>& opValues,
        const string& conditionKey, const vector<string>& conditionValues, SqliteOperate* op, bool isTransaction)
    {
        if (!op || !op->CheckValid())
            return ErrorCode::CONNECT_INVALID;

        assert(opValues.size() == conditionValues.size());

        int len = (int)opValues.size() * (50 * 2 + 128);
        char* sql = new char[len];
        memset(sql, 0, len);
        string sqlTmp;
        for (size_t i = 0; i < opValues.size(); i++)
            sqlTmp += "UPDATE " + tableName + " SET " + opKey + "='" + opValues.at(i) + "' WHERE " + conditionKey + "='" + conditionValues.at(i) + "';";

        int r = execSql(sqlTmp.c_str(), NULL, NULL, op);
        if (isTransaction && r)
            op->Rollback();

        return r;
    }
    int _updates(const string& tableName, const vector<string>& opKeys, const vector<string>& opValues,
        const string& conditionKey, const string& conditionValue, SqliteOperate* op, bool isTransaction)
    {
        if (!op || !op->CheckValid())
            return ErrorCode::CONNECT_INVALID;

        assert(opKeys.size() == opValues.size());

        string sqlTmp;
        for (size_t i = 0; i < opKeys.size(); i++)
            sqlTmp += opKeys.at(i) + "='" + opValues.at(i) + "',";
        if (!sqlTmp.empty())
            sqlTmp.pop_back();

        assert(!sqlTmp.empty());

        string sql = "UPDATE " + tableName + " SET " + sqlTmp + " WHERE " + conditionKey + "='" + conditionValue + "'";
        int r = execSql(sql.c_str(), NULL, NULL, op);
        if (isTransaction && r)
            op->Rollback();

        return r;
    }
    int updates(const string& tableName, const vector<vector<string>>& opKeys, const vector<vector<string>>& opValues,
        const string& conditionKey, const vector<string>& conditionValues)
    {

        SqliteOperate* op = new SqliteOperate(g_sqlitePath, "updateConnect");
        if (!op || !op->CheckValid())
            return ErrorCode::CONNECT_INVALID;

        bool isTranslation = true;

        assert(opKeys.size() == opValues.size());
        assert(opKeys.size() == conditionValues.size());

        int r = 0;

        if (isTranslation)
            op->Translation();

        for (size_t i = 0; i < opKeys.size(); i++)
        {
            r = _updates(tableName, opKeys.at(i), opValues.at(i), conditionKey, conditionValues.at(i), op, isTranslation);
            if (r)
                return r;
        }

        if (isTranslation)
            op->Commit();

        delete op;
        return 0;
    }
    // 更新多条数据的同一个字段的值
    int updates(const string& tableName, const string& opKey, const vector<string>& opValues,
        const string& conditionKey, const vector<string> conditionValues)
    {
        SqliteOperate* op = new SqliteOperate(g_sqlitePath, "updateConnect");
        if (!op || !op->CheckValid())
            return ErrorCode::CONNECT_INVALID;

        bool isTranslation = true;
        if (isTranslation)
            op->Translation();

        int r = _updates(tableName, opKey, opValues, conditionKey, conditionValues, op, isTranslation);

        if (!r && isTranslation)
            op->Commit();

        delete op;
        return r;
    }

    // 数据库错误码
    string sqliteError(int errorCode)
    {
        string r;
        switch (errorCode)
        {
        case SQLITE_BUSY:
            r = "数据库文件被锁";
            break;
        case SQLITE_LOCKED:
            r = "数据库表被锁";
            break;
        case SQLITE_CONSTRAINT:
            r = "数据冲突";
            break;
        default:
            r = "其他错误";
            break;
        }

        return r;
    }
}
}
}