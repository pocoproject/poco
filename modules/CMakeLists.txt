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
		Poco/ActiveRecord.cppm
		Poco/Crypto.cppm
		Poco/CppParser.cppm
		Poco/Data.cppm
		Poco/Data/MySQL.cppm
		Poco/Data/ODBC.cppm
		Poco/Data/PostgreSQL.cppm
		Poco/Data/SQLite.cppm
		Poco/DNSSD.cppm
		Poco/DNSSD/Avahi.cppm
		Poco/DNSSD/Bonjour.cppm
		Poco/JSON.cppm
		Poco/JWT.cppm
		Poco/MongoDB.cppm
		Poco/Net.cppm
		Poco/PDF.cppm
		Poco/Prometheus.cppm
		Poco/Redis.cppm
		Poco/SevenZip.cppm
		Poco/Util.cppm
		Poco/XML.cppm
		Poco/Zip.cppm
	)

	target_compile_definitions(Modules PUBLIC ENABLE_FOUNDATION)

	target_link_libraries(Modules PUBLIC Poco::Foundation)
	if(ENABLE_ENCODINGS)
		target_compile_definitions(Modules PUBLIC ENABLE_ENCODINGS)

		target_link_libraries(Modules PUBLIC Poco::Encodings)
	endif()
endif()

if(ENABLE_ACTIVERECORD)
	target_compile_definitions(Modules PUBLIC ENABLE_ACTIVERECORD)
	
	target_link_libraries(Modules PUBLIC Poco::ActiveRecord)
endif()

if(ENABLE_CRYPTO)
	target_compile_definitions(Modules PUBLIC ENABLE_CRYPTO)

	target_link_libraries(Modules PUBLIC Poco::Crypto)
endif()

if(ENABLE_CPPPARSER)
	target_compile_definitions(Modules PUBLIC ENABLE_CPPPARSER)

	target_link_libraries(Modules PUBLIC Poco::CppParser)
endif()

if(ENABLE_CPPUNIT)
	# CppUnit is test-only infrastructure and should not be part of the
	# exported Modules interface. Tests can use CppUnit directly without
	# it being included in POCO_MODULES or linked to the Modules library.
	# Note: CppUnit.cppm exists but is NOT added to POCO_MODULES to avoid
	# CMake export errors since Poco::CppUnit is not an exported target.
	target_compile_definitions(Modules PUBLIC ENABLE_CPPUNIT)
	# Do NOT link CppUnit to Modules - it's only needed by test executables
endif()

if(ENABLE_DATA)
	target_compile_definitions(Modules PUBLIC ENABLE_DATA)

	target_link_libraries(Modules PUBLIC Poco::Data)

	if(ENABLE_DATA_MYSQL)
		target_compile_definitions(Modules PUBLIC ENABLE_DATA_MYSQL)

		target_link_libraries(Modules PUBLIC Poco::DataMySQL)
	endif()

	if(ENABLE_DATA_ODBC)
		target_compile_definitions(Modules PUBLIC ENABLE_DATA_ODBC)

		target_link_libraries(Modules PUBLIC Poco::DataODBC)
	endif()

	if(ENABLE_DATA_POSTGRESQL)
		target_compile_definitions(Modules PUBLIC ENABLE_DATA_POSTGRESQL)

		target_link_libraries(Modules PUBLIC Poco::DataPostgreSQL)
	endif()

	if(ENABLE_DATA_SQLITE)
		target_compile_definitions(Modules PUBLIC ENABLE_DATA_SQLITE)

		target_link_libraries(Modules PUBLIC Poco::DataSQLite)
	endif()
endif()

if(ENABLE_DNSSD)
	target_compile_definitions(Modules PUBLIC ENABLE_DNSSD)

	target_link_libraries(Modules PUBLIC Poco::DNSSD)

	if(ENABLE_DNSSD_AVAHI)
		target_compile_definitions(Modules PUBLIC ENABLE_DNSSD_AVAHI)

		target_link_libraries(Modules PUBLIC Poco::DNSSDAvahi)
	endif()

	if(ENABLE_DNSSD_BONJOUR)
		target_compile_definitions(Modules PUBLIC ENABLE_DNSSD_BONJOUR)

		target_link_libraries(Modules PUBLIC Poco::DNSSDBonjour)
	endif()
endif()

if(ENABLE_JSON)
	target_compile_definitions(Modules PUBLIC ENABLE_JSON)

	target_link_libraries(Modules PUBLIC Poco::JSON)
endif()

if(ENABLE_JWT)
	target_compile_definitions(Modules PUBLIC ENABLE_JWT)

	target_link_libraries(Modules PUBLIC Poco::JWT)
endif()

if(ENABLE_MONGODB)
	target_compile_definitions(Modules PUBLIC ENABLE_MONGODB)

	target_link_libraries(Modules PUBLIC Poco::MongoDB)
endif()

if(ENABLE_NET)
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
	target_compile_definitions(Modules PUBLIC ENABLE_PDF)

	target_link_libraries(Modules PUBLIC Poco::PDF)
endif()

if(ENABLE_PROMETHEUS)
	target_compile_definitions(Modules PUBLIC ENABLE_PROMETHEUS)

	target_link_libraries(Modules PUBLIC Poco::Prometheus)
endif()

if(ENABLE_REDIS)
	target_compile_definitions(Modules PUBLIC ENABLE_REDIS)

	target_link_libraries(Modules PUBLIC Poco::Redis)
endif()

if(ENABLE_SEVENZIP)
	target_compile_definitions(Modules PUBLIC ENABLE_SEVENZIP)

	target_link_libraries(Modules PUBLIC Poco::SevenZip)
endif()

if(ENABLE_UTIL)
	target_compile_definitions(Modules PUBLIC ENABLE_UTIL)

	target_link_libraries(Modules PUBLIC Poco::Util)
endif()

if(ENABLE_XML)
	target_compile_definitions(Modules PUBLIC ENABLE_XML)

	target_link_libraries(Modules PUBLIC Poco::XML)
endif()

if(ENABLE_ZIP)
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
    OUTPUT_NAME PocoModules
    DEFINE_SYMBOL Modules_EXPORTS
)

target_include_directories(Modules
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    PRIVATE 
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
)

target_compile_features(Modules PUBLIC cxx_std_20)

POCO_INSTALL(Modules)
POCO_GENERATE_PACKAGE(Modules)

if(ENABLE_TESTS)
	add_subdirectory(samples)
endif()
