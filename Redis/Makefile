#
# Makefile
#
# $Id$
#
# Makefile for Poco Redis
#

include $(POCO_BASE)/build/rules/global

INCLUDE += -I $(POCO_BASE)/Redis/include/Poco/Redis

objects = AsyncReader Array Client Command Error Exception RedisStream RedisEventArgs Type

target         = PocoRedis
target_version = $(LIBVERSION)
target_libs    = PocoFoundation PocoNet

include $(POCO_BASE)/build/rules/lib
