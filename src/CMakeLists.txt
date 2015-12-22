cmake_minimum_required(VERSION 2.8)
project (pinocchio_executable)

add_executable(pinocchio pinocchio.cpp)
target_link_libraries(pinocchio ${Boost_LIBRARIES})
target_link_libraries(pinocchio ${CMAKE_THREAD_LIBS_INIT})
if(UCL_FOUND)
	target_link_libraries(pinocchio ${Ucl_LIBRARIES})
endif(UCL_FOUND)

if (Tcmalloc_FOUND)
	target_link_libraries(pinocchio ${Tcmalloc_LIBRARIES})
endif(Tcmalloc_FOUND)

