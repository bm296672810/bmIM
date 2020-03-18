# 项目信息
project (libSql)
# 查找当前目录下的所有源文件

FILE (GLOB ALL_SOURCES "*.cpp" "*.c" "sqlite/*.c" "sqlite/*.cpp" "mysql/*.cpp" )
FILE (GLOB ALL_INCLUDES "*.hpp" "*.h" "sqlite/*.h" "mysql/*.h" "common/*.h" )
SET(ALL_FILES ${ALL_SOURCES} ${ALL_INCLUDES})

aux_source_directory(. DIR_SRCS)

if(WIN32)
    set(MYSQL_ROOT_PATH "C:/Program Files/MySQL/MySQL Server 8.0")
    set(RELEASE_LIBS ${MYSQL_ROOT_PATH}/lib/libmysql.lib)
elseif(APPLE)
    set(MYSQL_ROOT_PATH )
    set(RELEASE_LIBS ${MYSQL_ROOT_PATH}/lib/libmysql.a)
elseif(UNIX)
    set(MYSQL_ROOT_PATH )
    set(RELEASE_LIBS ${MYSQL_ROOT_PATH}/lib/libmysql.a)
endif()


IF(CMAKE_CL_64)
    set(platform x64)
ELSE(CMAKE_CL_64)
    set(platform x86)
ENDIF(CMAKE_CL_64)

include_directories("${MYSQL_ROOT_PATH}/include")
# 指定生成目标 
add_library(libSql ${ALL_FILES})
target_link_libraries(libSql ${RELAY_LIBS})