include(CMakeFindDependencyMacro)
find_dependency(PocoFoundation)
if(@POCO_UNBUNDLED@)
	find_dependency(ZLIB REQUIRED)
endif()

include("${CMAKE_CURRENT_LIST_DIR}/PocoPDFTargets.cmake")
