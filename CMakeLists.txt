# CMake 最低版本号要求
cmake_minimum_required (VERSION 2.8)
# 项目信息
project (bmim)

# 添加子目录
add_subdirectory(libNetwork)
add_subdirectory(libOpenssl)
add_subdirectory(libSql)
add_subdirectory(test)
add_subdirectory(bmServer)
add_subdirectory(libJson)
