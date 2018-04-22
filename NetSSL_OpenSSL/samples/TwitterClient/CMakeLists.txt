add_executable(TwitterCLient
	src/TweetApp.cpp
	src/Twitter.cpp
)
target_link_libraries(TwitterCLient Poco::NetSSL Poco::Util Poco::JSON Poco::XML)
