# A consumer that does not get POCO's CMAKE_CXX_FLAGS, which add /Zc:__cplusplus for MSVC.
string(REPLACE "/Zc:__cplusplus" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

add_library(UnixLocalConsumer OBJECT UnixLocalConsumer.cpp)
target_link_libraries(UnixLocalConsumer PRIVATE Poco::Net)
