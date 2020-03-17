#ifndef __SQL_COMMON_H__
#define __SQL_COMMON_H__
#include <vector>
#include <string>

#ifndef M_SLEEP
#include <thread>
#include <chrono>
#define M_SLEEP(t) std::this_thread::sleep_for(std::chrono::milliseconds(t))
#define U_SLEEP(t) std::this_thread::sleep_for(std::chrono::microseconds(t))
#endif

namespace bm
{
namespace sql
{
    struct SqlResult {
        std::vector<std::string> m_vecIds;
        std::vector<std::vector<std::string>> m_vecValues;
    };
}
}
#endif