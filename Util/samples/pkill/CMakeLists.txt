add_executable(pkill src/pkill.cpp)
target_link_libraries(pkill PUBLIC Poco::Util Poco::JSON Poco::XML)
target_compile_options(pkill PUBLIC 
    $<$<BOOL:${MINGW}>-municode>
    $<$<BOOL:${WINCE}>/ENTRY:wmainCRTStartup>
)
