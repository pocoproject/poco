add_executable(HTTPSTimeServer-win src/HTTPSTimeServer.cpp)
target_link_libraries(HTTPSTimeServer-win Poco::NetSSLWin Poco::Crypto Poco::Util Poco::Net Poco::XML Poco::Foundation)
add_custom_command(TARGET HTTPSTimeServer-win POST_BUILD
				   COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/any.pfx ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
				   )
