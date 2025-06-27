cmake_minimum_required(VERSION 3.25)

project(
	libsth
	VERSION 0.0.1
	LANGUAGES C
)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

option(STH_WITH_MIMALLOC "use mimalloc as default memory allocator" ON)
# option(STH_WITH_MEM_DGB "enable memory allocation debugging features" OFF)
# option(STH_MEM_DEBUG_LOG "log memory allocations in *_dbg functions" OFF)

# Test if sth is the main project or added to another project as dependency
string(
	COMPARE EQUAL
	"${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_SOURCE_DIR}"
	STH_IS_MAIN
)

if (STH_IS_MAIN)
	set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")
	if(CMAKE_EXPORT_COMPILE_COMMANDS)
		set(CMAKE_C_STANDARD_INCLUDE_DIRECTORIES
			${CMAKE_C_IMPLICIT_INCLUDE_DIRECTORIES})
	endif()
endif()

if (NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "" FORCE)
endif()

set(
	STH_PUBLIC_HEADERS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include"
	CACHE STRING "" FORCE
)

add_subdirectory(src sth)
if (STH_IS_MAIN)
	add_subdirectory("test" sthtest)
endif()
