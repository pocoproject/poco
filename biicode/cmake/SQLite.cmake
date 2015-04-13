target_compile_definitions(${BII_BLOCK_TARGET} INTERFACE -DSQLITE_THREADSAFE=1
                                                         -DSQLITE_DISABLE_LFS
                                                         -DSQLITE_OMIT_UTF16
                                                         -DSQLITE_OMIT_PROGRESS_CALLBACK
                                                         -DSQLITE_OMIT_COMPLETE
                                                         -DSQLITE_OMIT_TCL_VARIABLE
                                                         -DSQLITE_OMIT_DEPRECATED)
