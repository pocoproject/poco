set(SAMPLE_NAME "")

set(LOCAL_SRCS "")
aux_source_directory(src LOCAL_SRCS)

add_executable(download-ssl-win src/download.cpp)
target_link_libraries(download-ssl-win PUBLIC Poco::NetSSLWin Poco::Crypto Poco::XML)
