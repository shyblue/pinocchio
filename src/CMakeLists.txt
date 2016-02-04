cmake_minimum_required(VERSION 2.8)
project (pinocchio)

set(SOURCES
		redis_value.cpp
		redis_parser.cpp
		redis_client_impl.cpp
		redis_sync_client.cpp
		redis_async_client.cpp
		main.cpp
		pinocchio.cpp
		config.cpp
		logger.cpp
		database_manager.cpp
		user_manager.cpp)

add_executable(pinocchio ${SOURCES})
target_link_libraries(pinocchio ${TBB_LIBRARIES})
target_link_libraries(pinocchio ${Boost_LIBRARIES})
target_link_libraries(pinocchio ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(pinocchio ${LOG4CXX_LIBRARIES})

if (OPENSSL_FOUND)
	target_link_libraries(pinocchio ${OPENSSL_LIBRARIES})
endif(OPENSSL_FOUND)
if (Tcmalloc_FOUND)
	target_link_libraries(pinocchio ${Tcmalloc_LIBRARIES})
endif(Tcmalloc_FOUND)

add_custom_command(OUTPUT cert
	COMMAND ${CMAKE_COMMAND} -E
	copy ${PROJECT_SOURCE_DIR}/cert/*.crt ${CMAKE_CURRENT_BINARY_DIR}/
	DEPENDS ${PROJECT_SOURCE_DIR}/cert/*.crt)
add_custom_command(OUTPUT key
	COMMAND ${CMAKE_COMMAND} -E
	copy ${PROJECT_SOURCE_DIR}/cert/*.key ${CMAKE_CURRENT_BINARY_DIR}/
	DEPENDS ${PROJECT_SOURCE_DIR}/cert/*.key)
add_custom_target(cert_copy ALL DEPENDS cert key)

add_custom_command(OUTPUT conf
		COMMAND ${CMAKE_COMMAND} -E
		copy ${PROJECT_SOURCE_DIR}/conf/pinocchio.conf ${CMAKE_CURRENT_BINARY_DIR}/pinocchio.conf
		DEPENDS ${PROJECT_SOURCE_DIR}/conf/pinocchio.conf )
add_custom_command(OUTPUT log
		COMMAND ${CMAKE_COMMAND} -E
		copy ${PROJECT_SOURCE_DIR}/conf/log4cxx.xml ${CMAKE_CURRENT_BINARY_DIR}/log4cxx.xml
		DEPENDS ${PROJECT_SOURCE_DIR}/conf/log4cxx.xml )
add_custom_target(conf_copy ALL DEPENDS conf log)

add_custom_command(OUTPUT test
        COMMAND ${CMAKE_COMMAND} -E
        copy ${PROJECT_SOURCE_DIR}/test/send_msg.sh ${CMAKE_CURRENT_BINARY_DIR}/
        DEPENDS ${PROJECT_SOURCE_DIR}/test/send_msg.sh )
add_custom_command(OUTPUT script
		COMMAND ${CMAKE_COMMAND} -E
		copy ${PROJECT_SOURCE_DIR}/script/runserver.sh ${CMAKE_CURRENT_BINARY_DIR}/
		DEPENDS ${PROJECT_SOURCE_DIR}/script/runserver.sh )
add_custom_command(OUTPUT fabric
		COMMAND ${CMAKE_COMMAND} -E
		copy ${PROJECT_SOURCE_DIR}/script/fabfile.py ${CMAKE_CURRENT_BINARY_DIR}/
		DEPENDS ${PROJECT_SOURCE_DIR}/script/fabfile.py )
add_custom_target(script_copy ALL DEPENDS test script fabric)