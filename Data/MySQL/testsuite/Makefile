#
# Makefile
#
# $Id: //poco/1.3/Data/MySQL/testsuite/Makefile#1 $
#
# Makefile for Poco MySQL testsuite
#

include $(POCO_BASE)/build/rules/global

INCLUDE += -I./../include -I/usr/local/include/mysql -I/usr/include/mysql/ -I/usr/mysql/include/mysql
SYSLIBS += -L/usr/local/lib/mysql -L/usr/lib/mysql -L/usr/mysql/lib/mysql -lmysqlclient -lz

objects = MySQLTestSuite Driver MySQLTest SQLExecutor

target         = testrunner
target_version = 1
target_libs    = PocoMySQL PocoData PocoFoundation CppUnit

include $(POCO_BASE)/build/rules/exec
