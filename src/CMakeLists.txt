cmake_minimum_required(VERSION 2.8)
project (pinocchio)

set(SOURCES main.cpp pinocchio.cpp config.cpp redis_connector.cpp)

add_executable(pinocchio ${SOURCES})
target_link_libraries(pinocchio ${TBB_LIBRARIES})
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

add_custom_command(OUTPUT cert
	COMMAND ${CMAKE_COMMAND} -E
	copy ${PROJECT_SOURCE_DIR}/*.crt ${CMAKE_CURRENT_BINARY_DIR}/
	DEPENDS ${PROJECT_SOURCE_DIR}/*.crt)
add_custom_command(OUTPUT key
	COMMAND ${CMAKE_COMMAND} -E
	copy ${PROJECT_SOURCE_DIR}/*.key ${CMAKE_CURRENT_BINARY_DIR}/
	DEPENDS ${PROJECT_SOURCE_DIR}/*.key)

add_custom_target(certkey_copy ALL DEPENDS cert key)
