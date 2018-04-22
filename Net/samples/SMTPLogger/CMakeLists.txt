add_executable(SMTPLogger src/SMTPLogger.cpp)
target_link_libraries(SMTPLogger Poco::Net)
