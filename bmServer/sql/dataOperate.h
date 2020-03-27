#ifndef __DATA_OPERATE_H__
#define __DATA_OPERATE_H__

#include "libSql.h"
namespace bm
{
namespace data
{
    enum ErrorCode
    {
        BD_SUCCESS          = 0,
        BD_DATABASE_ERROR   = -100,     // ���ݿ����ʧ��
        BD_PHONE_HAS_EXISET = -200,     // �ֻ����Ѿ�ռ��
    };
    int createUser(const string& phone, const string& name, const string& nickname, const string& pwd, string& id);
}
}

#endif