set(procect_name lib_openssl)
project (${procect_name})

FILE (GLOB ALL_SOURCES "*.cpp" "*.c" )
FILE (GLOB ALL_INCLUDES "*.hpp" "*.h" )
# aux_source_directory(. DIR_SRCS)
add_library(${procect_name} ${ALL_INCLUDES} ${ALL_SOURCES})

include_directories(
${CMAKE_CURRENT_SOURCE_DIR}/include
)
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
	SET(CMAKE_SHARED_LINKER_FLAGS ${CMAKE_SHARED_LINKER_FLAGS} "/NODEFAULTLIBS:MSVCRT")
elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
  # Update if necessary
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-long-long -pedantic")
endif()