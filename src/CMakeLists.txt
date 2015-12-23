cmake_minimum_required(VERSION 2.8)
project (pinocchio_executable)

set(SOURCES pinocchio.cpp config.cpp)

add_executable(pinocchio ${SOURCES})
target_link_libraries(pinocchio ${Boost_LIBRARIES})
target_link_libraries(pinocchio ${CMAKE_THREAD_LIBS_INIT})

if(UCL_FOUND)
	target_link_libraries(pinocchio ${UCL_LIBRARY})
endif(UCL_FOUND)

if (Tcmalloc_FOUND)
	target_link_libraries(pinocchio ${Tcmalloc_LIBRARIES})
endif(Tcmalloc_FOUND)

add_custom_command(OUTPUT pinocchio.conf
	COMMAND ${CMAKE_COMMAND} -E
	copy ${PROJECT_SOURCE_DIR}/pinocchio.conf ${CMAKE_CURRENT_BINARY_DIR}/pinocchio.conf
	DEPENDS ${PROJECT_SOURCE_DIR}/pinocchio.conf )

add_custom_target(conf_copy ALL DEPENDS pinocchio.conf)
