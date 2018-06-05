add_executable(WebSocketServer src/WebSocketServer.cpp)
if(WINCE)
	set_target_properties(WebSocketServer PROPERTIES LINK_FLAGS "/ENTRY:wmainCRTStartup")
endif()

target_link_libraries(WebSocketServer PUBLIC Poco::Net Poco::Util Poco::JSON)

POCO_ENABLE_EXE_WMAIN(WebSocketServer)
