
# Sources
file(GLOB SRCS_G "src/*.cpp")
POCO_SOURCES_AUTO(DBLOGGER_SRCS ${SRCS_G})

# Headers
file(GLOB_RECURSE HDRS_G "src/*.h")
POCO_HEADERS_AUTO(DBLOGGER_SRCS ${HDRS_G})

add_executable(DBLogger ${DBLOGGER_SRCS})

target_link_libraries(DBLogger PUBLIC Poco::Util Poco::DataSQLite)

add_custom_target(DBLogger-properties SOURCES DBLogger.properties)
