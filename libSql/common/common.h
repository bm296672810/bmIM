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

        void clear() 
        {
            m_vecIds.clear();
            m_vecValues.clear();
        }
    };

    inline std::vector<std::string> StringSplit(const std::string& str, const std::string& sp)
    {
        std::string::size_type startPos = 0, findPos;
        std::vector <std::string> result;
        do {
            findPos = str.find(sp, startPos);
            if (findPos != std::string::npos)
            {
                result.push_back(str.substr(startPos, findPos - startPos + sp.length()));
                startPos = findPos + sp.length();
            }
            else
            {
                if(startPos < str.length())
                    result.push_back(str.substr(startPos));
            }
        } while (findPos != std::string::npos);

        return result;
    }
}
}
#endif