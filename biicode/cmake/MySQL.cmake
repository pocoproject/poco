target_compile_definitions(${BII_BLOCK_TARGET} INTERFACE -DTHREADSAFE -DNO_TCL)
target_link_libraries( ${BII_BLOCK_TARGET} INTERFACE ${MYSQL_LIB})
