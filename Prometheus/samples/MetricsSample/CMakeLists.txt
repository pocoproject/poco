add_executable(MetricsSample src/MetricsSample.cpp)
target_link_libraries(MetricsSample PUBLIC Poco::Prometheus Poco::Util Poco::Net)
