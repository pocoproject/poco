macro(ENABLE_LIBRARIES )
    if(NOT BII_LIB_SRC)
      return()
    endif()
    string(FIND "${BII_LIB_SRC}" "Data/MySQL/include"  MySQL_SINGLE_MATCHED )
    IF(MySQL_SINGLE_MATCHED)
      SET(ENABLE_DATA_MYSQL ON)
    ENDIF()

    string(FIND "${BII_LIB_SRC}" "Data/ODBC/include"  ODBC_SINGLE_MATCHED)
    IF(ODBC_SINGLE_MATCHED)
      SET(ENABLE_DATA_ODBC  ON)
    ENDIF()

    string(FIND "${BII_LIB_SRC}" "Data/SQLite/include"  SQLite_SINGLE_MATCHED )
    IF(SQLite_SINGLE_MATCHED)
      SET(ENABLE_DATA_SQLITE  ON)
    ENDIF()

    string(FIND "${BII_LIB_SRC}" "JSON/include"  JSON_SINGLE_MATCHED )
    IF(JSON_SINGLE_MATCHED)
      SET(ENABLE_JSON ON)
    ENDIF()

    string(FIND "${BII_LIB_SRC}" "MongoDB/include"  MONGODB_SINGLE_MATCHED )
    IF(MONGODB_SINGLE_MATCHED)
      SET(ENABLE_MONGODB ON)
    ENDIF()

    string(FIND "${BII_LIB_SRC}" "Util/include"  UTIL_SINGLE_MATCHED )
    IF(UTIL_SINGLE_MATCHED)
      SET(ENABLE_UTIL ON)
    ENDIF()

    string(FIND "${BII_LIB_SRC}" "Net/include"  NET_SINGLE_MATCHED )
    IF(NET_SINGLE_MATCHED)
      SET(ENABLE_NET ON)
    ENDIF()

    string(FIND "${BII_LIB_SRC}" "Zip/include"  ZIP_SINGLE_MATCHED )
    IF(ZIP_SINGLE_MATCHED)
      SET(ENABLE_ZIP ON)
    ENDIF()

    string(FIND "${BII_LIB_SRC}" "ApacheConnector/include"  APACHECONNECTOR_SINGLE_MATCHED )
    IF(APACHECONNECTOR_SINGLE_MATCHED)
      SET(ENABLE_APACHECONNECTOR  ON)
    ENDIF()

    string(FIND "${BII_LIB_SRC}" "CppParser/include"  CPPPARSER_SINGLE_MATCHED )
    IF(CPPPARSER_SINGLE_MATCHED)
      SET(ENABLE_CPPPARSER ON)
    ENDIF()

    string(FIND "${BII_LIB_SRC}" "XML/include"  XML_SINGLE_MATCHED )
    IF(XML_SINGLE_MATCHED)
      SET(ENABLE_XML ON)
    ENDIF()

    string(FIND "${BII_LIB_SRC}" "Crypto/include"  CRYPTO_SINGLE_MATCHED )
    IF(CRYPTO_SINGLE_MATCHED)
        SET(ENABLE_CRYPTO  ON)
    ENDIF()

    string(FIND "${BII_LIB_SRC}" "Data/include"  DATA_SINGLE_MATCHED )
    IF(DATA_SINGLE_MATCHED)
        SET(ENABLE_DATA ON)
    ENDIF()

    string(FIND "${BII_LIB_SRC}" "NetSSL_OpenSSL/include"  NETSSL_OPENSSL_SINGLE_MATCHED )
    IF(NETSSL_OPENSSL_SINGLE_MATCHED)
        SET(ENABLE_NETSSL ON)
    ENDIF()

    string(FIND "${BII_LIB_SRC}" "NetSSL_Win/include"  NETSSL_WIN_SINGLE_MATCHED )
    IF(NETSSL_WIN_SINGLE_MATCHED)
        SET(ENABLE_NETSSL_WIN ON)
    ENDIF()

    string(FIND "${BII_LIB_SRC}" "PDF/include"  PDF_SINGLE_MATCHED )
    IF(PDF_SINGLE_MATCHED)
        SET(ENABLE_PDF ON)
    ENDIF()

    string(FIND "${BII_LIB_SRC}" "SevenZip/include"  SEVENZIP_SINGLE_MATCHED )
    IF(SEVENZIP_SINGLE_MATCHED)
        SET(ENABLE_SEVENZIP  ON)
    ENDIF()
endmacro()

macro(FILTER_BII_LIB_SRC PATTERN)
    if(NOT BII_LIB_SRC)
      return()
    endif()
    SET(ALL_MATCHES )
    foreach(RESOURCE_FILE ${BII_LIB_SRC})
        set(SINGLE_MATCHED )
        string(REGEX MATCH "${PATTERN}" SINGLE_MATCHED "${RESOURCE_FILE}")
        IF(DEFINED SINGLE_MATCHED)
            SET(ALL_MATCHES ${ALL_MATCHES} ${SINGLE_MATCHED})
        ENDIF(DEFINED SINGLE_MATCHED)
    endforeach(RESOURCE_FILE ${BII_LIB_SRC})

    FOREACH(BAD_ITEM ${ALL_MATCHES})
      LIST(REMOVE_ITEM BII_LIB_SRC ${BAD_ITEM})
    ENDFOREACH(BAD_ITEM ${ALL_MATCHES})
endmacro()

# Delete all the bad implementations which biicode detects
macro(FILTER_BAD_DEPENDENCIES)
    if(NOT BII_LIB_SRC)
      return()
    endif()
    SET(BAD_DEPS_PATTERN "(.*)(_WIN32U|_UNIX|_POSIX|_STD|_C99|_DEC|_DUMMY|_SUN|_VMS|_WINCE|_WIN32|_VX|_Android|_HPUX)(.*)")
    # First filter: selected the pattern "_WIN32" or similars
    FILTER_BII_LIB_SRC(${BAD_DEPS_PATTERN})

    # Second filter: special cases
    SET(SPECIAL_BAD_DEPENDENCIES_WIN Foundation/include/Poco/SyslogChannel.h
                     Foundation/src/SyslogChannel.cpp
                     Foundation/include/Poco/OpcomChannel.h
                     Foundation/src/OpcomChannel.cpp)

    SET(SPECIAL_BAD_DEPENDENCIES_UNIX Foundation/include/Poco/EventLogChannel.h
                      Foundation/src/EventLogChannel.cpp
                      Foundation/include/Poco/WindowsConsoleChannel.h
                      Foundation/src/WindowsConsoleChannel.cpp
                      Foundation/include/Poco/OpcomChannel.h
                      Foundation/src/OpcomChannel.cpp
                      Util/src/WinRegistryKey.cpp
                      Util/src/WinRegistryConfiguration.cpp
                      Util/src/WinService.cpp)
    IF(WIN32)
      FOREACH(SPECIAL_DEP ${SPECIAL_BAD_DEPENDENCIES_WIN})
        list(FIND BII_LIB_SRC ${SPECIAL_DEP} DEP_MATCH)
        if(DEP_MATCH)
          LIST(REMOVE_ITEM BII_LIB_SRC ${SPECIAL_DEP})
        endif()
      ENDFOREACH()
    ELSEIF(UNIX)
      FOREACH(SPECIAL_DEP ${SPECIAL_BAD_DEPENDENCIES_UNIX})
        list(FIND BII_LIB_SRC ${SPECIAL_DEP} DEP_MATCH)
        if(DEP_MATCH)
          LIST(REMOVE_ITEM BII_LIB_SRC ${SPECIAL_DEP})
        endif()
      ENDFOREACH()
    ENDIF()
endmacro()
