if(ENABLE_FASTLOGGER)
	poco_add_executable(FastLogger src/FastLogger.cpp)
	target_link_libraries(FastLogger PUBLIC Poco::Foundation)
endif()
