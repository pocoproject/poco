add_executable(Units src/Units.cpp)
target_link_libraries(Units PUBLIC Poco::Util Poco::JSON Poco::XML)
