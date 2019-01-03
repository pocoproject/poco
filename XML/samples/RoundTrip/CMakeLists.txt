add_executable(RoundTrip src/RoundTrip.cpp)
target_link_libraries(RoundTrip PUBLIC Poco::XML Poco::Foundation )
