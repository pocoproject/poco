add_executable(SampleApp src/SampleApp.cpp)
target_link_libraries(SampleApp PUBLIC Poco::Util Poco::JSON Poco::XML)
target_compile_options(SampleApp PUBLIC 
    $<$<BOOL:${MINGW}>-municode>
    $<$<BOOL:${WINCE}>/ENTRY:wmainCRTStartup>
)
