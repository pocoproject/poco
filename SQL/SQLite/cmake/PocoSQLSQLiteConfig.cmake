include(CMakeFindDependencyMacro)
find_dependency(PocoFoundation)
find_dependency(PocoSQL)
include("${CMAKE_CURRENT_LIST_DIR}/PocoSQLSQLiteTargets.cmake")
