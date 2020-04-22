# 项目信息
set(project_name lib_json)
project (${project_name})
# 查找当前目录下的所有源文件
FILE (GLOB ALL_SOURCES "*.cpp")
FILE (GLOB ALL_INCLUDES "*.h")
SET(ALL_FILES ${ALL_SOURCES} ${ALL_INCLUDES})

set(MY_DEFINE_MACRO
"-DJSON_USE_SIMPLE_INTERNAL_ALLOCATOR"
)
add_definitions(${MY_DEFINE_MACRO})
# 指定生成目标 
add_library(${project_name} ${ALL_FILES})

#link_directories(${CMAKE_CURRENT_BINARY_DIR}/lib/${platform}/${CMAKE_BUILD_TYPE})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
#target_link_libraries(${project_name} libNetwork)