# 项目信息
set(project_name lib_network)
project (${project_name})
# 查找当前目录下的所有源文件
# 并将名称保存到 DIR_SRCS 变量
# aux_source_directory(. DIR_SRCS)
FILE (GLOB DIR_SRCS "*.cpp" "*.h")

if(WIN32)
    FILE (GLOB ALL_SOURCES "win/*.cpp" "win/*.h")
elseif(APPLE)
    FILE (GLOB ALL_SOURCES "mac/*.cpp" "mac/*.h")
elseif(UNIX)
    FILE (GLOB ALL_SOURCES "unix/*.cpp" "unix/*.h")
endif()

IF(CMAKE_CL_64)
    set(platform x64)
    set(RELAY_LIBS)
ELSE(CMAKE_CL_64)
    set(platform x86)
    set(RELAY_LIBS)
ENDIF(CMAKE_CL_64)

# 指定生成目标 
add_library(${project_name} ${DIR_SRCS} ${ALL_SOURCES})

target_link_libraries(${project_name} ${RELAY_LIBS})
#target_link_libraries(libSql RELAY_LIBS libFins)
if(MSVC)
  # Force to always compile with W4
    message("before:${CMAKE_CXX_FLAGS}")
	if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
		string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
	else()
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
	endif()

	if(IMPORT_DLL)
		ADD_DEFINITIONS(-D_AFXDLL)#如果使用动态库的MFC，则加上_AFXDLL
	endif(IMPORT_DLL)
	#SET(CMAKE_MFC_FLAG 1)
	message("after:${CMAKE_CXX_FLAGS}")
elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
  # Update if necessary
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-long-long -pedantic")
endif()