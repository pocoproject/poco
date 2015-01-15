#
# Makefile
#
# $Id: //poco/1.4/Net/Makefile#3 $
#
# Makefile for Poco NetEx
#

include $(POCO_BASE)/build/rules/global

SHAREDOPT_CXX += -DNetEx_EXPORTS

objects = \
	ICMPClient ICMPClientRaw ICMPEventArgs ICMPPacket ICMPPacketImpl \
	ICMPSocket ICMPSocketImpl ICMPv4PacketImpl \
	NTPClient NTPEventArgs NTPPacket \
	SNMPClient SNMPClientRawFactory SNMPClientTypes SNMPRawEventArgs \
	SNMPClientRaw SNMPClientRawTypes SNMPEventArgs

target         = PocoNetEx
target_version = $(LIBVERSION)
target_libs    = PocoASN PocoNet PocoFoundation 

include $(POCO_BASE)/build/rules/lib
