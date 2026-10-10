include(CMakeFindDependencyMacro)

if(ENABLE_FOUNDATION)
	find_dependency(PocoFoundation)
	if(ENABLE_ENCODINGS)
		find_dependency(PocoEncodings)
	endif()
endif()

if(ENABLE_ACTIVERECORD)
	find_dependency(PocoActiveRecord)
endif()

if(ENABLE_CRYPTO)
	find_dependency(PocoCrypto)
endif()

if(ENABLE_CPPPARSER)
	find_dependency(PocoCppParser)
endif()

if(ENABLE_CPPUNIT)
	find_dependency(PocoCppUnit)
endif()

if(ENABLE_DATA)
	find_dependency(PocoData)
	
	if(ENABLE_DATA_MYSQL)
		find_dependency(PocoDataMySQL)
	endif()
	
	if(ENABLE_DATA_ODBC)
		find_dependency(PocoDataODBC)
	endif()
	
	if(ENABLE_DATA_POSTGRESQL)
		find_dependency(PocoDataPostgreSQL)
	endif()
	
	if(ENABLE_DATA_SQLITE)
		find_dependency(PocoDataSQLite)
	endif()
endif()

if(ENABLE_DNSSD)
	find_dependency(PocoDNSSD)
	
	if(ENABLE_DNSSD_AVAHI)
		find_dependency(PocoDNSSDAvahi)
	endif()
	
	if(ENABLE_DNSSD_BONJOUR)
		find_dependency(PocoDNSSDBonjour)
	endif()
endif()

if(ENABLE_JSON)
	find_dependency(PocoJSON)
endif()

if(ENABLE_JWT)
	find_dependency(PocoJWT)
endif()

if(ENABLE_MONGODB)
	find_dependency(PocoMongoDB)
endif()

if(ENABLE_NET)
	find_dependency(PocoNet)
endif()

if(ENABLE_PDF)
	find_dependency(PocoPDF)
endif()

if(ENABLE_PROMETHEUS)
	find_dependency(PocoPrometheus)
endif()

if(ENABLE_REDIS)
	find_dependency(PocoRedis)
endif()

if(ENABLE_SEVENZIP)
	find_dependency(PocoSevenZip)
endif()

if(ENABLE_UTIL)
	find_dependency(PocoUtil)
endif()

if(ENABLE_XML)
	find_dependency(PocoXML)
endif()

if(ENABLE_ZIP)
	find_dependency(PocoZip)
endif()

include("${CMAKE_CURRENT_LIST_DIR}/PocoModulesTargets.cmake")
