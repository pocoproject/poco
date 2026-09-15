set(SAMPLE "PizzeriaClientWS")

# Sources
file(GLOB SRCS_G "src/*.cpp")
POCO_SOURCES_AUTO(SAMPLE_SRCS ${SRCS_G})

# Headers
file(GLOB_RECURSE HDRS_G "include/*.h")
POCO_HEADERS_AUTO(SAMPLE_SRCS ${HDRS_G})

add_executable(${SAMPLE} ${SAMPLE_SRCS})
target_link_libraries(${SAMPLE} PUBLIC Poco::RemotingNGSOAP)
target_include_directories(${SAMPLE}
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include
)
