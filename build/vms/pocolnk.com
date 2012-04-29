$ !
$ ! POCOLNK.COM
$ !
$ ! $Id: //poco/1.4/build/vms/pocolnk.com#1 $
$ !
$ ! C++ link procedure for Poco
$ !
$ ! The logical POCO_BASE must be set and point to the
$ ! Poco base directory (e.g. USER1:[PROJECTS.POCO.])
$ !
$
$ target  = P1
$ libdir  = "POCO_BASE:[LIB]"
$ bindir  = "[.BIN]"
$ baseopt = "/THREADS"
$
$ IF "''P2'" .EQS. "SHAREABLE"
$ THEN
$     outopt="/SHAREABLE"
$ ELSE
$     outopt="/EXECUTABLE"
$ ENDIF
$
$ IF "''pointerSize'" .EQS. "32"
$ THEN
$     libsuffix = ""
$     binsuffix = ""
$     ptropt    = "/MODEL=ARM"
$     objdir    = "OBJ32"
$ ELSE
$     libsuffix = "64"
$     binsuffix = "64"
$     ptropt    = "/MODEL=ANSI"
$     objdir    = "OBJ64"
$ ENDIF
$
$ IF "''buildMode'" .EQS. "RELEASE"
$ THEN
$     dbgopt    = "/NOTRACEBACK"
$     objdir    = objdir + "R"
$ ELSE
$     libsuffix = "''libsuffix'D"
$     binsuffix = "''binsuffix'D"
$     dbgopt    = "/TRACEBACK/debug"
$     objdir    = objdir + "D"
$ ENDIF
$
$ IF F$SEARCH("''target'.OPT") .NES. ""
$ THEN
$     COPY 'target'.OPT 'target'.TMPOPT
$     OPEN/APPEND optfile 'target'.TMPOPT
$ ELSE
$     OPEN/WRITE optfile 'target'.TMPOPT
$ ENDIF
$ ObjWriteLoop:
$     obj = F$SEARCH("[.''objdir']*.OBJ", 1)
$     IF obj .EQS. "" THEN GOTO LibWriteLoop1
$     WRITE optfile "''obj'"
$     GOTO ObjWriteLoop
$ LibWriteLoop1:
$     lib = F$SEARCH("''libdir'*''libsuffix'.OLB", 2)
$     IF lib .EQS. "" THEN GOTO LibWriteLoop2
$     WRITE optfile "''lib'/LIBRARY"
$     GOTO LibWriteLoop1
$ LibWriteLoop2:
$     lib = F$SEARCH("''libdir'*''libsuffix'.OLB", 3)
$     IF lib .EQS. "" THEN GOTO EndWriteLoop
$     WRITE optfile "''lib'/LIBRARY"
$     GOTO LibWriteLoop2
$ EndWriteLoop:
$ CLOSE optfile
$
$ bin = bindir + target + binsuffix + ".EXE"
$ baseopt = baseopt + "/REPOSITORY=[.''objdir']"
$
$ IF "''verbose'" .EQS. "1"
$ THEN
$     WRITE SYS$OUTPUT "CXXLINK''outopt'=''bin' ''baseopt' ''ptropt' ''dbgopt' ''target'.TMPOPT/OPTIONS"
$ ENDIF
$
$ IF F$SEARCH("BIN.DIR") .EQS. ""
$ THEN
$     CREATE/DIR/NOLOG 'bindir'
$ ENDIF
$
$ CXXLINK 'outopt'='bin' 'baseopt' 'ptropt' 'dbgopt' 'target'.TMPOPT/OPTIONS
$ PURGE 'target'.TMPOPT
