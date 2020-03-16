#ifndef __LIB_SQLITE_H__
#define __LIB_SQLITE_H__
#include <string>
#include <mutex>
#include <vector>
#include <list>
#include <chrono>

#include "db/sqlite3.h"

#ifndef M_SLEEP
#define M_SLEEP(n) std::this_thread::sleep_for(std::chrono::milliseconds(n))
#endif

using namespace std;

namespace bm{
namespace sqlite {
    extern mutex g_sqlite_db_mutex;
    extern string g_sqlitePath;
    string GetCurrentPath();
    static int callback(void* result, int argc, char** argv, char** colName);
    struct SqliteResult {
        vector<string> m_vecIds;
        vector<vector<string>> m_vecValues;
    };

    enum ErrorCode {
        CONNECT_INVALID = -1,   // ���ݿ�������Ч
    };
    class SqliteOperate {
    public:
        SqliteOperate() = delete;
        SqliteOperate(const SqliteOperate& ) = delete;
        SqliteOperate(const string& filePath, const string& connectName);
        ~SqliteOperate();

        int ExecSql(const string& sql, SqliteResult& result);
        int ExecSql(const char* sql, SqliteResult& result);
        int ExecSql(const char* sql, int (*callback)(void*, int, char**, char**), SqliteResult* result);

        int Translation();
        int Rollback();
        int Commit();

        string GetConnectName()const { return m_strName; }

        bool CheckValid() const { return m_bValid; }
    private:
        int _CreateDatabase(const string& filePath, const string& connectName);
    private:
        sqlite3*    m_database = NULL;
        string      m_strName;
        char*       m_cErrorMsg = NULL;
        bool        m_bValid = false;
    };

    // ִ��sql���
    int execSql(const string& sql, SqliteResult& result);
    int execSql(const char* sql, SqliteResult& result);
    //int execSql(const char* sql, SqliteResult* result, int(*callback)(void*, int, char**, char**) = NULL, shared_ptr<SqliteOperate> operate = NULL);
    int execSql(const char* sql, SqliteResult* result, int(*callback)(void*, int, char**, char**) = NULL, SqliteOperate* operate = NULL);
    int execSql(const string& sql, SqliteResult& result, const string& dbPath);

    int inserts(const vector<string>& sqls, const string& dbPath = "database/log.db");
    // ��ĳ�����в�ѯ��Ӧ��ֵ
    int selectResult(const string& tableName, const string& selectKey, const string& conditionKey, const string& conditionValue,vector<string>& results);

    // �������ݵ����ݿ�
    int update(const string& tableName, const string& opKey, const string& opValue, const string& conditionKey, const string& conditionValue);
    // ����һ�����ݵĶ���е�ֵ
    int update(const string& tableName, const vector<string>& opKeys, const vector<string>& opValues, const string& conditionKey, const string& conditionValue);
    //int update(const string& tableName, int groupNum, ...);
    int _updates(const string& tableName, const string& opKey, const vector<string>& opValues,
        const string& conditionKey, const vector<string>& conditionValues, SqliteOperate* op, bool isTransaction = true);
    int _updates(const string& tableName, const vector<string>& opKeys, const vector<string>& opValues,
        const string& conditionKey, const string& conditionValue, SqliteOperate* op, bool isTransaction = true);
    int updates(const string& tableName, const vector<vector<string>>& opKeys, const vector<vector<string>>& opValues,
        const string& conditionKey, const vector<string>& conditionValues);
    // ���¶������ݵ�ͬһ���ֶε�ֵ
    int updates(const string& tableName, const string& opKey, const vector<string>& opValues,
        const string& conditionKey, const vector<string> conditionValues);
    // ���ݿ������
    string sqliteError(int errorCode);
}
}

#endif