$ !
$ ! BUILD.COM
$ !
$ ! $Id: //poco/1.4/build/vms/build.com#1 $
$ !
$ ! OpenVMS build procedure
$ !
$ ! Requires the logical POCO_BASE to be set accordingly.
$ !
$ ! Usage: build [-32|-64] [-DEBUG|-RELEASE] [-CLEAN] [-VERBOSE]
$ !
$
$ clean          == 0
$ verbose        == 0
$ buildMode      == "DEBUG"
$ pointerSize    == 32
$ buildfile      == F$SEARCH("*.VMSBUILD")
$ projectDefines == ""
$ lib = ""
$ exe = ""
$ shr = ""
$
$ DEFINE/PROC/NOLOG CppUnit POCO_BASE:[CPPUNIT.INCLUDE.CPPUNIT]
$ DEFINE/PROC/NOLOG Foundation POCO_BASE:[FOUNDATION.INCLUDE.FOUNDATION]
$ DEFINE/PROC/NOLOG XML POCO_BASE:[XML.INCLUDE.XML]
$ DEFINE/PROC/NOLOG SAX POCO_BASE:[XML.INCLUDE.SAX]
$ DEFINE/PROC/NOLOG DOM POCO_BASE:[XML.INCLUDE.DOM]
$
$ CALL handleArgs "''P1'"
$ CALL handleArgs "''P2'"
$ CALL handleArgs "''P3'"
$ CALL handleArgs "''P4'"
$ CALL handleArgs "''P5'"
$
$ IF buildfile .EQS. ""
$ THEN
$     WRITE SYS$OUTPUT "Cannot find build file."
$     EXIT 1
$ ENDIF
$
$ IF pointerSize .EQ. 32
$ THEN
$     objdir = "OBJ32"
$ ELSE
$     objdir = "OBJ64"
$ ENDIF
$
$ IF buildMode .EQS. "DEBUG"
$ THEN
$     objdir = objdir + "D"
$ ELSE
$     objdir = objdir + "R"
$ ENDIF
$
$ objpath = "[.''objdir']"
$ exists = F$SEARCH("''objdir'.DIR")
$ IF exists .EQS. ""
$ THEN
$     CREATE/DIR/NOLOG 'objpath'
$     SET FILE/PROT=(O:RWED) 'objdir'.DIR;*
$ ENDIF
$
$ IF clean .EQ. 1
$ THEN
$     DELETE/NOLOG 'objpath'*.*;*
$ ENDIF
$
$ PCXX :== @POCO_BASE:[BUILD.VMS]POCOCXX.COM
$ PCC  :== @POCO_BASE:[BUILD.VMS]POCOCC.COM
$ PLIB :== @POCO_BASE:[BUILD.VMS]POCOLIB.COM
$ PLNK :== @POCO_BASE:[BUILD.VMS]POCOLNK.COM
$
$ OPEN fileList 'buildfile'
$ ON ERROR THEN GOTO EOF
$ ON CONTROL_Y THEN GOTO EOF
$ ReadLoop:
$     READ/END_OF_FILE=EOF fileList file
$     file = F$EDIT(file, "TRIM")
$     IF F$EXTRACT(0, 4, file) .EQS. "DEF="
$     THEN
$         projectDefines = F$EXTRACT(4, 64, file)
$         projectDefines = "," + F$EDIT(projectDefines, "TRIM")
$         GOTO ReadLoop
$     ENDIF
$     IF F$EXTRACT(0, 4, file) .EQS. "LIB="
$     THEN
$         lib = F$EXTRACT(4, 64, file)
$         lib = F$EDIT(lib, "TRIM")
$         GOTO ReadLoop
$     ENDIF
$     IF F$EXTRACT(0, 4, file) .EQS. "EXE="
$     THEN
$         exe = F$EXTRACT(4, 64, file)
$         exe = F$EDIT(exe, "TRIM")
$         GOTO ReadLoop
$     ENDIF
$     IF F$EXTRACT(0, 4, file) .EQS. "SHR="
$     THEN
$         shr = F$EXTRACT(4, 64, file)
$         shr = F$EDIT(shr, "TRIM")
$         GOTO ReadLoop
$     ENDIF
$     IF F$EXTRACT(0, 1, file) .NES. "#"
$     THEN
$         cfile   = F$SEARCH(F$PARSE(file, "[.SRC]", ".C"))
$         cppfile = F$SEARCH(F$PARSE(file, "[.SRC]", ".CPP"))
$         IF cppfile .NES. ""
$         THEN
$             srcfile = "[.SRC]''file'.CPP"
$         ELSE
$             srcfile = "[.SRC]''file'.C"
$         ENDIF
$         srctime = F$CVTIME(F$FILE_ATTRIBUTES(srcfile, "RDT"), "COMPARISON")
$         objfile = F$SEARCH(F$PARSE(file, objpath, ".OBJ"))
$         IF objfile .NES. ""
$         THEN
$             objtime = F$CVTIME(F$FILE_ATTRIBUTES(objfile, "RDT"), "COMPARISON")
$         ELSE
$             objtime = F$CVTIME("01-JAN-1970 00:00:00.00", "COMPARISON")
$         ENDIF
$         IF srctime .GTS. objtime
$         THEN
$             WRITE SYS$OUTPUT "** Compiling ",file
$             IF cppfile .NES. ""
$             THEN
$                 PCXX 'srcfile'
$             ELSE
$                 PCC 'srcfile'
$             ENDIF
$         ELSE
$             WRITE SYS$OUTPUT "** Skipping ",file," (up-to-date)"
$         ENDIF
$     ENDIF
$     GOTO ReadLoop
$ EOF:
$ CLOSE fileList
$
$ IF lib .NES. ""
$ THEN
$     WRITE SYS$OUTPUT "** Creating Library ",lib
$     PLIB 'lib'
$ ELSE
$     IF exe .NES. ""
$     THEN
$         WRITE SYS$OUTPUT "** Creating Executable ",exe
$         PLNK 'exe'
$     ELSE
$         IF shr .NES. ""
$         THEN
$              WRITE SYS$OUTPUT "** Creating Shareable ",shr
$              PLNK 'shr' "SHAREABLE"
$         ENDIF
$     ENDIF
$ ENDIF
$
$ handleArgs: SUBROUTINE
$     arg = F$EDIT("''P1'", "UPCASE")
$     IF arg .NES. ""
$     THEN
$         IF arg .EQS. "-CLEAN" THEN clean == 1
$         IF arg .EQS. "-32" THEN pointerSize == 32
$         IF arg .EQS. "-64" THEN pointerSize == 64
$         IF arg .EQS. "-DEBUG" THEN buildMode == "DEBUG"
$         IF arg .EQS. "-RELEASE" THEN buildMode == "RELEASE"
$         IF arg .EQS. "-VERBOSE" THEN verbose == 1
$         IF F$EXTRACT(0, 1, arg) .NES. "-" THEN buildfile == F$SEARCH("''arg'.VMSBUILD")
$     ENDIF
$ ENDSUBROUTINE
