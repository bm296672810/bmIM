# 项目信息
project (test)
# 查找当前目录下的所有源文件
# 并将名称保存到 DIR_SRCS 变量
aux_source_directory(. DIR_SRCS)

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
add_executable(test ${DIR_SRCS})

#FIND_LIBRARY(COMM_LIB "${CMAKE_CURRENT_SOURCE_DIR}/../libSql/lib/${platform}/Debug" NO_DEFAULT_PATH)
#message(${COMM_LIB})
#message("type:${CMAKE_BUILD_TYPE}")
include_directories(
${CMAKE_CURRENT_SOURCE_DIR}/../libNetwork
${CMAKE_CURRENT_SOURCE_DIR}/../libOpenssl
${CMAKE_CURRENT_SOURCE_DIR}/../libOpenssl/include
${CMAKE_CURRENT_SOURCE_DIR}/../libSql
"${MYSQL_ROOT_PATH}/include"
)
# 添加链接库
target_link_libraries(test lib_network lib_openssl lib_sql)

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
	SET(CMAKE_SHARED_LINKER_FLAGS ${CMAKE_SHARED_LINKER_FLAGS} "/NODEFAULTLIBS:MSVCRTD")
	SET(CMAKE_SHARED_LINKER_FLAGS ${CMAKE_SHARED_LINKER_FLAGS} "/NODEFAULTLIBS:MSVCRT")
	ADD_DEFINITIONS(-DNODEFAULTLIB:library)
elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
  # Update if necessary
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-long-long -pedantic")
endif()
