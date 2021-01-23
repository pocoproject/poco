add_library(FormServer src/FormServer.cpp)
target_link_libraries(FormServer PUBLIC Poco::Net)
