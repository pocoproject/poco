set(POCO_EXENAME "PageCompiler")

# Set flags to enable unicode
if(MINGW)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -municode")
endif()

# Sources
file(GLOB SRCS_G "src/*.cpp")
POCO_SOURCES_AUTO( SRCS ${SRCS_G})

add_executable( "${POCO_EXENAME}" ${SRCS} )
set_target_properties( "${POCO_EXENAME}"
    PROPERTIES
    OUTPUT_NAME cpspc
    )

target_link_libraries( "${POCO_EXENAME}" ${CMAKE_THREAD_LIBS_INIT} Net Util XML JSON Foundation)

install(
    TARGETS "${POCO_EXENAME}" EXPORT "${POCO_EXENAME}Targets"
    LIBRARY DESTINATION lib${LIB_SUFFIX}
    ARCHIVE DESTINATION lib${LIB_SUFFIX}
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
    )
if (POCO_ENABLE_SAMPLES)
#    add_subdirectory(samples)
endif ()

