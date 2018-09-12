
get_filename_component(UNITTEST_FILENAME ${UNITTEST} NAME)
message(STATUS "Cleanup /data/local/tmp ...")
execute_process(COMMAND ${ANDROID_NDK}/../platform-tools/adb shell "rm -r /data/local/tmp/*" OUTPUT_QUIET)
foreach(_TEST_DATA IN ITEMS ${TEST_FILES})
    message(STATUS "Push ${_TEST_DATA} to android ...")
    execute_process(COMMAND ${ANDROID_NDK}/../platform-tools/adb push ${_TEST_DATA} /data/local/tmp/ OUTPUT_QUIET)
endforeach()
message(STATUS "Push ignored.sh to android ...")
execute_process(COMMAND ${ANDROID_NDK}/../platform-tools/adb push ${CMAKE_CURRENT_LIST_DIR}/../travis/ignored.sh /data/local/tmp/ OUTPUT_QUIET)
message(STATUS "Push ${LIBRARY_DIR} to android ...")
execute_process(COMMAND ${ANDROID_NDK}/../platform-tools/adb push ${LIBRARY_DIR} /data/local/tmp/ OUTPUT_QUIET)                       
message(STATUS "Push ${UNITTEST} to android ...")
execute_process(COMMAND ${ANDROID_NDK}/../platform-tools/adb push ${UNITTEST} /data/local/tmp/ OUTPUT_QUIET)
message(STATUS "Execute ${UNITTEST_FILENAME} ${TEST_PARAMETER} on android ...")
execute_process(
   COMMAND ${ANDROID_NDK}/../platform-tools/adb shell "cd /data/local/tmp;su root sh -c '. /data/local/tmp/ignored.sh;LD_LIBRARY_PATH=/data/local/tmp:/data/local/tmp/lib:/data/local/tmp/data TMPDIR=/data/local/tmp HOME=/ ./${UNITTEST_FILENAME} ${TEST_PARAMETER};echo exit code $?'"
   RESULT_VARIABLE _RESULT
   OUTPUT_VARIABLE _OUT
   ERROR_VARIABLE _ERR
)
                
if(_RESULT)
    execute_process(COMMAND ls -l ${UNITTEST})
    execute_process(COMMAND ${ANDROID_NDK}/../platform-tools/adb shell "ls -R /data/local/tmp")
    message(FATAL_ERROR "Execution of ${UNITTEST_FILENAME} failed")
else()
    string(REGEX MATCH "exit code ([0-9]+)" _EXIT_CODE ${_OUT})
    if(NOT "${CMAKE_MATCH_1}" EQUAL 0)
        execute_process(COMMAND ls -l ${UNITTEST})
        execute_process(COMMAND ${ANDROID_NDK}/../platform-tools/adb shell "ls -R /data/local/tmp")
        string(REGEX REPLACE "exit code [0-9]+" "" _PRINT_OUT ${_OUT})
        message(FATAL_ERROR "${UNITTEST_FILENAME} execution error: ${_PRINT_OUT} ${_ERR}")
    endif()
endif()
