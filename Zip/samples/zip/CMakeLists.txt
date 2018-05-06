set(SAMPLE_NAME "sample-zip")

set(LOCAL_SRCS "")
aux_source_directory(src LOCAL_SRCS)

add_executable(sample-zip src/zip.cpp)
if(WINCE)
	set_target_properties(sample-zip PROPERTIES LINK_FLAGS "/ENTRY:wmainCRTStartup")
endif()
target_link_libraries(sample-zip PUBLIC Poco::Zip Poco::Util Poco::XML)
