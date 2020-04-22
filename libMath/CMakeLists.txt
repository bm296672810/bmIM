# 项目信息
set(projectName lib_math)

project (${projectName})
# 查找当前目录下的所有源文件

FILE (GLOB ALL_SOURCES "*.cpp" "*.c" )
FILE (GLOB ALL_INCLUDES "*.hpp" "*.h" )
SET(ALL_FILES ${ALL_SOURCES} ${ALL_INCLUDES})

aux_source_directory(. DIR_SRCS)

IF(CMAKE_CL_64)
    set(platform x64)
ELSE(CMAKE_CL_64)
    set(platform x86)
ENDIF(CMAKE_CL_64)

# include_directories("${MYSQL_ROOT_PATH}/include")
# 指定生成目标 
add_library(${projectName} ${ALL_FILES})
target_link_libraries(${projectName} ${RELAY_LIBS})