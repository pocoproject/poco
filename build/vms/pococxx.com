$ !
$ ! POCOCXX.COM
$ !
$ ! $Id: //poco/1.4/build/vms/pococxx.com#1 $
$ !
$ ! C++ compile procedure for Poco libraries
$ !
$ ! The logical POCO_BASE must be set and point to the
$ ! Poco base directory (e.g. USER1:[PROJECTS.POCO.])
$ !
$
$ IF "''pointerSize'" .EQS. "32"
$ THEN
$     ptropt = "/POINTER=32"
$     objdir = "OBJ32"
$ ELSE
$     ptropt = "/MODEL=ANSI"
$     objdir = "OBJ64"
$ ENDIF
$
$ IF "''buildMode'" .EQS. "RELEASE"
$ THEN
$     modeopt = "/NODEBUG"
$     modedef = "NDEBUG"
$     objdir  = objdir + "R"
$ ELSE
$     modeopt = "/DEBUG=ALL/NOOPT"
$     modedef = "_DEBUG"
$     objdir  = objdir + "D"
$ ENDIF
$
$ node    = F$GETSYI("NODENAME")
$ baseopt = "/STANDARD=ANSI/ANSI/NOUSING/FLOAT=IEEE/IEEE=UNDER/REENT=MULTI/OBJ=[.''objdir'].OBJ"
$ repopt  = "/REP=[.''objdir']"
$ defines = "/DEF=(__USE_STD_IOSTREAM,__NEW_STARLET,POCO_BUILD_HOST=''node',''modedef' ''projectDefines')"
$
$ IF "''verbose'" .EQS. "1"
$ THEN
$     WRITE SYS$OUTPUT "CXX ''baseopt' ''ptropt' ''modeopt' ''defines' ''repopt' ''P1'"
$ ENDIF
$
$ CXX 'baseopt' 'ptropt' 'modeopt' 'defines' 'repopt' 'p1'
