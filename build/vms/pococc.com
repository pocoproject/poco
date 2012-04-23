$ !
$ ! POCOCC.COM
$ !
$ ! $Id: //poco/1.4/build/vms/pococc.com#1 $
$ !
$ ! C compile procedure for Poco libraries
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
$     ptropt = "/POINTER=64"
$     objdir = "OBJ64"
$ ENDIF
$
$ IF "''buildMode'" .EQS. "RELEASE"
$ THEN
$     modeopt = "/NODEBUG"
$     modedef = "NDEBUG"
$     objdir  = objdir + "R"
$ ELSE
$     modeopt = "/DEBUG=ALL/NOOPTIMIZE"
$     modedef = "_DEBUG"
$     objdir  = objdir + "D"
$ ENDIF
$
$ baseopt = "/STANDARD=RELAXED/ANSI_ALIAS/FLOAT=IEEE_FLOAT/OBJECT=[.''objdir'].OBJ"
$ defines = "/DEFINE=(''modedef' ''projectDefines')"
$
$ IF "''verbose'" .EQS. "1"
$ THEN
$     WRITE SYS$OUTPUT "CC ''baseopt' ''ptropt' ''modeopt' ''defines' ''P1'"
$ ENDIF
$
$ CC 'baseopt' 'ptropt' 'modeopt' 'defines' 'p1'
