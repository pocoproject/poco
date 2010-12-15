$ !
$ ! BUILDVMS.COM
$ !
$ ! $Id: //poco/1.4/buildvms.com#1 $
$ !
$ ! OpenVMS build procedure
$ !
$
$ options == "''p1' ''p2' ''p3' ''p4' ''p5'"
$
$ base = F$ENVIRONMENT("DEFAULT")
$ base = base - "]" + ".]"
$ define/nolog POCO_BASE 'base'
$ build :== @POCO_BASE:[build.vms]build.com
$
$ set def [.CppUnit]
$ build 'options'
$
$ set def [-.Foundation]
$ build 'options'
$
$ set def [.testsuite]
$ build testlibrary 'options'
$ build testsuite 'options'
$
$ set def [--.XML]
$ build 'options'
$
$ set def [.testsuite]
$ build 'options'
$
$ set def [--]
