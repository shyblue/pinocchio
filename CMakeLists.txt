cmake_minimum_required(VERSION 2.8)

project (pinocchio)

set(PROJECT_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
set(PROJECT_LIB_DIR ${PROJECT_SOURCE_DIR}/lib)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

find_package(Tcmalloc)
find_package(Threads)
find_package(Boost 1.52 COMPONENTS date_time filesystem system thread regex
	REQUIRED)

find_library(UCL_LIBRARY NAMES ucl libucl PATHS ~/work/pinocchio/lib )
if(UCL_LIBRARY)
	set(UCL_FOUND TRUE)
	set(Ucl_LIBRARIES ${UCL_LIBRARY})
	message(STATUS "Found libUcl: ${UCL_LIBRARIES}")
else()
	set(UCL_FOOUND FALSE)
	set(Ucl_LIBRARIES)
endif()

include_directories(${Boost_INCLUDE_DIR})

include_directories(${PROJECT_INCLUDE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/src)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2  -Wall -std=c++1y")

add_subdirectory(src)
