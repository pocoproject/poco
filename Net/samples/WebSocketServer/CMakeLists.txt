set(SAMPLE_NAME "WebSocketServer")

# Set flags to enable unicode
if(MINGW)
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -municode")
endif()

set(LOCAL_SRCS "")
aux_source_directory(src LOCAL_SRCS)

add_executable( ${SAMPLE_NAME} ${LOCAL_SRCS} )
if(WINCE)
set_target_properties( ${SAMPLE_NAME} PROPERTIES LINK_FLAGS "/ENTRY:wmainCRTStartup")
endif()
target_link_libraries( ${SAMPLE_NAME} PocoNet PocoUtil PocoJSON PocoXML PocoFoundation )
