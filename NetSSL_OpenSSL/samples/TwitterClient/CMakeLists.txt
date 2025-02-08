add_executable(TwitterClient
	src/TweetApp.cpp
	src/Twitter.cpp
)
target_link_libraries(TwitterClient PUBLIC Poco::NetSSL Poco::Util Poco::JSON Poco::XML)
if(MSVC)
	target_link_libraries(TwitterClient PRIVATE OpenSSL::applink)
endif()