#include "data_operate.h"
#include "mysql.h"

#include <string>
#include <assert.h>

namespace bm
{
namespace data
{
    int create_user(const string& phone, const string& name, const string& nickname, const string& pwd, string& id)
    {
        char sql[256] = { 0 };
        sprintf_s(sql, "SELECT id FROM t_users WHERE phone_nu = '%s';", phone.c_str());
        bm::sql::SqlResult result;
        int r = bm::sql::mysql::execSql(sql, result);
        if (r != 0)
            return error_code::BD_DATABASE_ERROR;

        if (!result.m_vecValues.empty())
            return error_code::BD_PHONE_HAS_EXISET;

        memset(sql, 0, 256);
        sprintf_s(sql, "INSERT INTO t_users VALUES(NULL, '%s','%s','%s','%s',NULL, CURRENT_TIMESTAMP,0,CURRENT_TIMESTAMP);SELECT id FROM t_users WHERE phone_nu = '%s';", 
            name.c_str(), nickname.c_str(), phone.c_str(), pwd.c_str(),phone.c_str());
        r = bm::sql::mysql::execSql(sql, result);
        if (r != 0)
            return error_code::BD_DATABASE_ERROR;

        assert(!result.m_vecValues.empty());
        id = result.m_vecValues.at(0).at(0);

        return error_code::BD_SUCCESS;
    }
}
}
