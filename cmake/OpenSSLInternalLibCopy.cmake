if(MSVC AND USING_INTERNAL_OPENSSL)
  add_custom_command(TARGET "${LIBNAME}" PRE_BUILD
    COMMAND xcopy /Y /C ${OPENSSL_ROOT_DIR}\\win${OPENSSL_WIN_PLATFORM_BITS}\\${POCO_WIN_LIB_DIR}\\$(Configuration)\\*.dll ${WIN_BIN_OUTPUT_DIR}\\$(Configuration)\\*
    COMMAND xcopy /Y /C ${OPENSSL_ROOT_DIR}\\win${OPENSSL_WIN_PLATFORM_BITS}\\${POCO_WIN_LIB_DIR}\\$(Configuration)\\*.lib ${WIN_LIB_OUTPUT_DIR}\\$(Configuration)\\*
    COMMAND xcopy /Y /C ${OPENSSL_ROOT_DIR}\\win${OPENSSL_WIN_PLATFORM_BITS}\\${POCO_WIN_LIB_DIR}\\$(Configuration)\\*.pdb ${WIN_BIN_OUTPUT_DIR}\\$(Configuration)\\*
    COMMENT "Copying OpenSSL binaries" VERBATIM)
endif(MSVC AND USING_INTERNAL_OPENSSL)
