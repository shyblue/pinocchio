cmake_minimum_required(VERSION 2.8)

project (pinocchio)

set(PROJECT_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
set(PROJECT_LIB_DIR ${PROJECT_SOURCE_DIR}/lib)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

find_package(PkgConfig)
if(PKGCONFIG_FOUIND)
	pkg_search_module(OPENSSL REQUIRED openssl)
endif()

find_package(Tcmalloc)
find_package(Threads)
find_package(Boost 1.57 COMPONENTS date_time filesystem system thread regex
	REQUIRED)
find_package(TBB)

if(OPENSSL_FOUND)
	include_directories(${OPENSSL_INCLUDE_DIRS})
	message(STATUS "Using OpenSSL ${OPENSSL_VERSION}")
else()
	find_package(OPENSSL REQUIRED)
endif()
include_directories(${Boost_INCLUDE_DIR})
include_directories(${PROJECT_INCLUDE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/src)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g  -Wall -std=c++1y")

add_subdirectory(src)
