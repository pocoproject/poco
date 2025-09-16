cmake_minimum_required(VERSION 3.28)

add_library(Modules)
add_library(Poco::Modules ALIAS Modules)

set(POCO_MODULES
	Poco.cppm
)

if(ENABLE_FOUNDATION)
	list(APPEND POCO_MODULES
		Poco/Foundation.cppm
		Poco/Dynamic.cppm
	)

	target_compile_definitions(Modules PUBLIC ENABLE_FOUNDATION)

	target_link_libraries(Modules PUBLIC Poco::Foundation)
	if(ENABLE_ENCODINGS)
		target_compile_definitions(Modules PUBLIC ENABLE_ENCODINGS)

		target_link_libraries(Modules PUBLIC Poco::Encodings)
	endif()
endif()

if(ENABLE_ACTIVERECORD)
	list(APPEND POCO_MODULES
		Poco/ActiveRecord.cppm
	)
	target_compile_definitions(Modules PUBLIC ENABLE_ACTIVERECORD)
	
	target_link_libraries(Modules PUBLIC Poco::ActiveRecord)
endif()

if(ENABLE_CRYPTO)
	list(APPEND POCO_MODULES
		Poco/Crypto.cppm
	)
	target_compile_definitions(Modules PUBLIC ENABLE_CRYPTO)

	target_link_libraries(Modules PUBLIC Poco::Crypto)
endif()

if(ENABLE_CPPPARSER)
	list(APPEND POCO_MODULES
		Poco/CppParser.cppm
	)
	target_compile_definitions(Modules PUBLIC ENABLE_CPPPARSER)

	target_link_libraries(Modules PUBLIC Poco::CppParser)
endif()

if(ENABLE_CPPUNIT)
	list(APPEND POCO_MODULES
		CppUnit.cppm
	)
	target_compile_definitions(Modules PUBLIC ENABLE_CPPUNIT)

	target_link_libraries(Modules PUBLIC Poco::CppUnit)
endif()

if(ENABLE_DATA)
	list(APPEND POCO_MODULES
		Poco/Data.cppm
	)
	target_compile_definitions(Modules PUBLIC ENABLE_DATA)

	target_link_libraries(Modules PUBLIC Poco::Data)

	if(ENABLE_DATA_MYSQL)
		list(APPEND POCO_MODULES
			Poco/Data/MySQL.cppm
		)
		target_compile_definitions(Modules PUBLIC ENABLE_DATA_MYSQL)

		target_link_libraries(Modules PUBLIC Poco::DataMySQL)
	endif()

	if(ENABLE_DATA_ODBC)
		list(APPEND POCO_MODULES
			Poco/Data/ODBC.cppm
		)
		target_compile_definitions(Modules PUBLIC ENABLE_DATA_ODBC)

		target_link_libraries(Modules PUBLIC Poco::DataODBC)
	endif()

	if(ENABLE_DATA_POSTGRESQL)
		list(APPEND POCO_MODULES
			Poco/Data/PostgreSQL.cppm
		)
		target_compile_definitions(Modules PUBLIC ENABLE_DATA_POSTGRESQL)

		target_link_libraries(Modules PUBLIC Poco::DataPostgreSQL)
	endif()

	if(ENABLE_DATA_SQLITE)
		list(APPEND POCO_MODULES
			Poco/Data/SQLite.cppm
		)
		target_compile_definitions(Modules PUBLIC ENABLE_DATA_SQLITE)

		target_link_libraries(Modules PUBLIC Poco::DataSQLite)
	endif()
endif()

if(ENABLE_DNSSD)
	list(APPEND POCO_MODULES
		Poco/DNSSD.cppm
	)
	target_compile_definitions(Modules PUBLIC ENABLE_DNSSD)

	target_link_libraries(Modules PUBLIC Poco::DNSSD)

	if(ENABLE_DNSSD_AVAHI)
		list(APPEND POCO_MODULES
			Poco/DNSSD/Avahi.cppm
		)
		target_compile_definitions(Modules PUBLIC ENABLE_DNSSD_AVAHI)

		target_link_libraries(Modules PUBLIC Poco::DNSSDAvahi)
	endif()

	if(ENABLE_DNSSD_BONJOUR)
		list(APPEND POCO_MODULES
			Poco/DNSSD/Bonjour.cppm
		)
		target_compile_definitions(Modules PUBLIC ENABLE_DNSSD_BONJOUR)

		target_link_libraries(Modules PUBLIC Poco::DNSSDBonjour)
	endif()
endif()

if(ENABLE_JSON)
	list(APPEND POCO_MODULES
		Poco/JSON.cppm
	)
	target_compile_definitions(Modules PUBLIC ENABLE_JSON)

	target_link_libraries(Modules PUBLIC Poco::JSON)
endif()

if(ENABLE_JWT)
	list(APPEND POCO_MODULES
		Poco/JWT.cppm
	)
	target_compile_definitions(Modules PUBLIC ENABLE_JWT)

	target_link_libraries(Modules PUBLIC Poco::JWT)
endif()

if(ENABLE_MONGODB)
	list(APPEND POCO_MODULES
		Poco/MongoDB.cppm
	)
	target_compile_definitions(Modules PUBLIC ENABLE_MONGODB)

	target_link_libraries(Modules PUBLIC Poco::MongoDB)
endif()

if(ENABLE_NET)
	list(APPEND POCO_MODULES
		Poco/Net.cppm
	)
	target_compile_definitions(Modules PUBLIC ENABLE_NET)

	target_link_libraries(Modules PUBLIC Poco::Net)

	if(ENABLE_NETSSL_OPENSSL)
		target_compile_definitions(Modules PUBLIC ENABLE_NETSSL_OPENSSL)
	endif()

	if(ENABLE_NETSSL_WIN)
		target_compile_definitions(Modules PUBLIC ENABLE_NETSSL_WIN)
	endif()
endif()

if(ENABLE_PDF)
	list(APPEND POCO_MODULES
		Poco/PDF.cppm
	)
	target_compile_definitions(Modules PUBLIC ENABLE_PDF)

	target_link_libraries(Modules PUBLIC Poco::PDF)
endif()

if(ENABLE_PROMETHEUS)
	list(APPEND POCO_MODULES
		Poco/Prometheus.cppm
	)
	target_compile_definitions(Modules PUBLIC ENABLE_PROMETHEUS)

	target_link_libraries(Modules PUBLIC Poco::Prometheus)
endif()

if(ENABLE_REDIS)
	list(APPEND POCO_MODULES
		Poco/Redis.cppm
	)
	target_compile_definitions(Modules PUBLIC ENABLE_REDIS)

	target_link_libraries(Modules PUBLIC Poco::Redis)
endif()

if(ENABLE_SEVENZIP)
	list(APPEND POCO_MODULES
		Poco/SevenZip.cppm
	)
	target_compile_definitions(Modules PUBLIC ENABLE_SEVENZIP)

	target_link_libraries(Modules PUBLIC Poco::SevenZip)
endif()

if(ENABLE_UTIL)
	list(APPEND POCO_MODULES
		Poco/Util.cppm
	)
	target_compile_definitions(Modules PUBLIC ENABLE_UTIL)

	target_link_libraries(Modules PUBLIC Poco::Util)
endif()

if(ENABLE_XML)
	list(APPEND POCO_MODULES
		Poco/XML.cppm
	)
	target_compile_definitions(Modules PUBLIC ENABLE_XML)

	target_link_libraries(Modules PUBLIC Poco::XML)
endif()

if(ENABLE_ZIP)
	list(APPEND POCO_MODULES
		Poco/Zip.cppm
	)
	target_compile_definitions(Modules PUBLIC ENABLE_ZIP)

	target_link_libraries(Modules PUBLIC Poco::Zip)
endif()

if(NOT COMMAND configure_cpp_module_target)
    function(configure_cpp_module_target target)
        target_sources(${target} PUBLIC FILE_SET CXX_MODULES FILES ${POCO_MODULES})
    endfunction()
endif()

configure_cpp_module_target(Modules)

set_target_properties(Modules
    PROPERTIES
    VERSION ${SHARED_LIBRARY_VERSION} 
    SOVERSION ${SHARED_LIBRARY_VERSION}
    OUTPUT_NAME Modules
    DEFINE_SYMBOL Modules_EXPORTS
)

target_include_directories(Modules
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    PRIVATE 
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_features(Modules PUBLIC cxx_std_20)

POCO_MODULES_INSTALL(Modules)

if(ENABLE_TESTS)
	add_subdirectory(samples)
endif()
