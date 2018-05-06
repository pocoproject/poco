add_executable(httpget src/httpget.cpp)
target_link_libraries(httpget PUBLIC Poco::Net Poco::Util Poco::JSON)
