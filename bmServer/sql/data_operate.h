#ifndef __DATA_OPERATE_H__
#define __DATA_OPERATE_H__

#include "lib_sql.h"
namespace bm
{
namespace data
{
    enum error_code
    {
        BD_SUCCESS          = 0,
        BD_DATABASE_ERROR   = -100,     // 数据库操作失败
        BD_PHONE_HAS_EXISET = -200,     // 手机号已经占用
    };
    int create_user(const string& phone, const string& name, const string& nickname, const string& pwd, string& id);
}
}

#endif