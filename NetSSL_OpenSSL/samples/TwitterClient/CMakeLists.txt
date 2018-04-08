add_executable(TwitterCLient
	src/TweetApp.cpp
	src/Twitter.cpp
)
target_link_libraries(TwitterCLient Poco::NetSSL Poco::Crypto Poco::Net Poco::Util Poco::JSON Poco::XML Poco::Foundation)
