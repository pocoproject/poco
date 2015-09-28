target_compile_definitions(${BII_BLOCK_TARGET} INTERFACE -DTHREADSAFE)
target_link_libraries( ${BII_BLOCK_TARGET} INTERFACE ${POSTGRESQL_LIB})
