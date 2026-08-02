poco_add_executable(MemoryDBBrowser src/MemoryDBBrowser.cpp)
target_link_libraries(MemoryDBBrowser PUBLIC Poco::DataSQLite Poco::Net Poco::JSON Poco::Util)

# Stage the web UI and the default configuration next to the binary so the
# sample runs straight from the build tree: resolveWebroot() falls back to
# <exe dir>/webui, and ServerApplication::loadConfiguration() picks up
# MemoryDBBrowser.properties from the same place.
add_custom_command(TARGET MemoryDBBrowser POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
		"${CMAKE_CURRENT_SOURCE_DIR}/webui" "$<TARGET_FILE_DIR:MemoryDBBrowser>/webui"
	COMMAND ${CMAKE_COMMAND} -E copy_if_different
		"${CMAKE_CURRENT_SOURCE_DIR}/MemoryDBBrowser.properties" "$<TARGET_FILE_DIR:MemoryDBBrowser>"
	COMMENT "Staging MemoryDBBrowser web UI")
