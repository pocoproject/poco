$ !
$ ! POCOLIB.COM
$ !
$ ! $Id: //poco/1.4/build/vms/pocolib.com#1 $
$ !
$ ! C++ library creation procedure for Poco libraries
$ !
$ ! The logical POCO_BASE must be set and point to the
$ ! Poco base directory (e.g. USER1:[PROJECTS.POCO.])
$ !
$
$ libdir = "POCO_BASE:[LIB]"
$ lib = libdir + p1
$
$ IF "''pointerSize'" .EQS. "32"
$ THEN
$     objdir = "OBJ32"
$ ELSE
$     objdir = "OBJ64"
$     lib    = lib + "64"
$ ENDIF
$
$ IF "''buildMode'" .EQS. "RELEASE"
$ THEN
$     objdir = objdir + "R"
$ ELSE
$     objdir = objdir + "D"
$     lib    = lib + "D"
$ ENDIF
$
$ 
$ IF "''verbose'" .EQS. "1"
$ THEN 
$     WRITE SYS$OUTPUT "LIBRARY/OBJECT/CREATE ''lib' [.''objdir']*.OBJ"
$ ENDIF
$
$ LIBRARY/OBJECT/CREATE 'lib' [.'objdir']*.OBJ
