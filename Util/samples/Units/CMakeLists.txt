add_executable(Units src/Units.cpp)
target_link_libraries(Units Poco::Util Poco::JSON Poco::XML Poco::Foundation)
