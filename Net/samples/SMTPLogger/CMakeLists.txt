add_executable(SMTPLogger src/SMTPLogger.cpp)
target_link_libraries(SMTPLogger PUBLIC Poco::Net)
