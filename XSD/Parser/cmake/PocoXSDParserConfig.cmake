include(CMakeFindDependencyMacro)
find_dependency(PocoFoundation)
find_dependency(PocoXSDTypes)
include("${CMAKE_CURRENT_LIST_DIR}/PocoXSDParserTargets.cmake")
