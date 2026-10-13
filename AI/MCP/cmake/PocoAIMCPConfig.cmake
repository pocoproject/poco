include(CMakeFindDependencyMacro)
find_dependency(PocoFoundation)
find_dependency(PocoJSON)
include("${CMAKE_CURRENT_LIST_DIR}/PocoAIMCPTargets.cmake")
