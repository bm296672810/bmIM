# 项目信息
project (libSql)
# 查找当前目录下的所有源文件

FILE (GLOB ALL_SOURCES "*.cpp" "*.c" "db/*.c"  )
FILE (GLOB ALL_INCLUDES "*.hpp" "*.h" "db/*.h" )
SET(ALL_FILES ${ALL_SOURCES} ${ALL_INCLUDES})

aux_source_directory(. DIR_SRCS)

IF(CMAKE_CL_64)
    set(platform x64)
ELSE(CMAKE_CL_64)
    set(platform x86)
ENDIF(CMAKE_CL_64)

# 指定生成目标 
add_library(libSql ${ALL_FILES})