# 项目信息 项目名
set(PROJECT_NAME bm_server)

project (${PROJECT_NAME})
# 查找当前目录下的所有源文件
# 并将名称保存到 DIR_SRCS 变量
FILE (GLOB ALL_SOURCES "*.cpp" "*.c" "sql/*.c" "sql/*.cpp" "connect/*.cpp" )
FILE (GLOB ALL_INCLUDES "*.hpp" "*.h" "sql/*.h" "common/*.h" "connect/*.h" )
SET(ALL_FILES ${ALL_SOURCES} ${ALL_INCLUDES})
# aux_source_directory(. DIR_SRCS)

IF(CMAKE_CL_64)
    set(platform x64)
ELSE(CMAKE_CL_64)
    set(platform x86)
ENDIF(CMAKE_CL_64)

if(WIN32)
    set(MYSQL_ROOT_PATH "C:/Program Files/MySQL/MySQL Server 8.0")
    set(RELEASE_LIBS ${MYSQL_ROOT_PATH}/lib/libmysql.lib)
elseif(APPLE)
    set(MYSQL_ROOT_PATH "C:/Program Files/MySQL/MySQL Server 8.0")
    set(RELEASE_LIBS ${MYSQL_ROOT_PATH}/lib/libmysql.a)
elseif(UNIX)
    set(MYSQL_ROOT_PATH "C:/Program Files/MySQL/MySQL Server 8.0")
    set(RELEASE_LIBS ${MYSQL_ROOT_PATH}/lib/libmysql.a)
endif()

set(LIBS libcrypto_static.lib libssl_static.lib ${RELEASE_LIBS})
link_directories("${CMAKE_CURRENT_SOURCE_DIR}/../libOpenssl/lib")
link_libraries(${LIBS})

# 指定生成目标 
add_executable(${PROJECT_NAME} ${ALL_FILES})

#FIND_LIBRARY(COMM_LIB "${CMAKE_CURRENT_SOURCE_DIR}/../libSql/lib/${platform}/Debug" NO_DEFAULT_PATH)
#message(${COMM_LIB})
#message("type:${CMAKE_BUILD_TYPE}")
include_directories(
${CMAKE_CURRENT_SOURCE_DIR}
${CMAKE_CURRENT_SOURCE_DIR}/../libNetwork
${CMAKE_CURRENT_SOURCE_DIR}/../libOpenssl
${CMAKE_CURRENT_SOURCE_DIR}/../libOpenssl/include
${CMAKE_CURRENT_SOURCE_DIR}/../libSql
${CMAKE_CURRENT_SOURCE_DIR}/../libJson
"${MYSQL_ROOT_PATH}/include"
)
# 添加链接库
target_link_libraries(${PROJECT_NAME} lib_network lib_openssl lib_sql lib_json)

if(MSVC)
  # Force to always compile with W4
	if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
		string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
	else()
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
	endif()

	if(IMPORT_DLL)
		ADD_DEFINITIONS(-D_AFXDLL)#如果使用动态库的MFC，则加上_AFXDLL
	endif(IMPORT_DLL)
	#SET(CMAKE_MFC_FLAG 1)
	SET(CMAKE_SHARED_LINKER_FLAGS ${CMAKE_SHARED_LINKER_FLAGS} "/NODEFAULTLIBS:MSVCRT /NODEFAULTLIBS:MSVCRTD")
elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
  # Update if necessary
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-long-long -pedantic")
endif()
