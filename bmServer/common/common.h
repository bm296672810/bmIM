#pragma one
#include <vector>
#include <string>

namespace bm
{
namespace server
{
inline std::vector<std::string> StringSplit(const std::string& str, const std::string& sp)
{
    std::string::size_type startPos = 0, findPos;
    std::vector <std::string> result;
    do {
        findPos = str.find(sp, startPos);
        if (findPos != std::string::npos)
        {
            result.push_back(str.substr(startPos, findPos - startPos));
            startPos = findPos + sp.length();
        }
        else
        {
            result.push_back(str.substr(startPos));
        }
    } while (findPos != std::string::npos);

    return result;
}
}
}