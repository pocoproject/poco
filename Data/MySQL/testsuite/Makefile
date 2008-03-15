#
# Makefile
#
# $Id: //poco/1.3/Data/MySQL/testsuite/Makefile#1 $
#
# Makefile for Poco MySQL testsuite
#

include $(POCO_BASE)/build/rules/global

INCLUDE += -I./../include -I/usr/local/include/mysql
SYSLIBS += -L/usr/local/lib/mysql -lmysqlclient -lz

objects = MySQLTestSuite Driver \
	MySQLTest SQLExecutor

target         = testrunner
target_version = 1
target_libs    = PocoMySQL PocoData PocoFoundation CppUnit

include $(POCO_BASE)/build/rules/exec
