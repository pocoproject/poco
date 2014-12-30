#
# Makefile
#
# $Id: //poco/1.4/Net/samples/TwitterClient/Makefile#1 $
#
# Makefile for Poco Twitter Client
#

include $(POCO_BASE)/build/rules/global

objects = Twitter TweetApp

target         = tweet
target_version = 1
target_libs    = PocoUtil PocoJSON PocoNetSSL PocoCrypto PocoNet PocoXML PocoFoundation

include $(POCO_BASE)/build/rules/exec

ifdef POCO_UNBUNDLED
        SYSLIBS += -lz -lpcre -lexpat
endif
