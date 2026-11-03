include(CMakeFindDependencyMacro)

foreach(_poco_dependency @POCO_MODULES_DEPENDENCIES@)
	find_dependency(Poco${_poco_dependency})
endforeach()

include("${CMAKE_CURRENT_LIST_DIR}/PocoModulesTargets.cmake")
