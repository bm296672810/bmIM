#ifndef __M_SQLITE_H__
#define __M_SQLITE_H__
#include <string>
#include <mutex>
#include <vector>
#include <list>
#include <chrono>

#include "sqlite3.h"
#include "../common/common.h"

using namespace std;

namespace bm {
namespace sql
{
namespace sqlite {
    extern mutex g_sqlite_db_mutex;
    extern string g_sqlitePath;
    string GetCurrentPath();
    static int callback(void* result, int argc, char** argv, char** colName);

    enum ErrorCode {
        CONNECT_INVALID = -1,   // 数据库连接无效
    };
    class SqliteOperate {
    public:
        SqliteOperate() = delete;
        SqliteOperate(const SqliteOperate&) = delete;
        SqliteOperate(const string& filePath, const string& connectName);
        ~SqliteOperate();

        int ExecSql(const string& sql, SqlResult& result);
        int ExecSql(const char* sql, SqlResult& result);
        int ExecSql(const char* sql, int (*callback)(void*, int, char**, char**), SqlResult* result);

        int Translation();
        int Rollback();
        int Commit();

        string GetConnectName()const { return m_strName; }

        bool CheckValid() const { return m_bValid; }
    private:
        int _CreateDatabase(const string& filePath, const string& connectName);
    private:
        sqlite3* m_database = NULL;
        string      m_strName;
        char* m_cErrorMsg = NULL;
        bool        m_bValid = false;
    };

    // 执行sql语句
    int execSql(const string& sql, SqlResult& result);
    int execSql(const char* sql, SqlResult& result);
    //int execSql(const char* sql, SqlResult* result, int(*callback)(void*, int, char**, char**) = NULL, shared_ptr<SqliteOperate> operate = NULL);
    int execSql(const char* sql, SqlResult* result, int(*callback)(void*, int, char**, char**) = NULL, SqliteOperate* operate = NULL);
    int execSql(const string& sql, SqlResult& result, const string& dbPath);

    int inserts(const vector<string>& sqls, const string& dbPath = "database/log.db");
    // 从某个表中查询相应的值
    int selectResult(const string& tableName, const string& selectKey, const string& conditionKey, const string& conditionValue, vector<string>& results);

    // 更新数据到数据库
    int update(const string& tableName, const string& opKey, const string& opValue, const string& conditionKey, const string& conditionValue);
    // 更新一条数据的多个列的值
    int update(const string& tableName, const vector<string>& opKeys, const vector<string>& opValues, const string& conditionKey, const string& conditionValue);
    //int update(const string& tableName, int groupNum, ...);
    int _updates(const string& tableName, const string& opKey, const vector<string>& opValues,
        const string& conditionKey, const vector<string>& conditionValues, SqliteOperate* op, bool isTransaction = true);
    int _updates(const string& tableName, const vector<string>& opKeys, const vector<string>& opValues,
        const string& conditionKey, const string& conditionValue, SqliteOperate* op, bool isTransaction = true);
    int updates(const string& tableName, const vector<vector<string>>& opKeys, const vector<vector<string>>& opValues,
        const string& conditionKey, const vector<string>& conditionValues);
    // 更新多条数据的同一个字段的值
    int updates(const string& tableName, const string& opKey, const vector<string>& opValues,
        const string& conditionKey, const vector<string> conditionValues);
    // 数据库错误码
    string sqliteError(int errorCode);
}
}
}
#endif