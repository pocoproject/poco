#
# Makefile
#
# Makefile for Poco Serial library
#

include $(POCO_BASE)/build/rules/global

# BeagleBone needs a special hack to enable
# RS-485 RTS via GPIO. Try to detect BeagleBone
# and enable hack.
ifeq ($(findstring bone,$(shell uname -r)),bone)
$(info BeagleBone detected - enabling RS485 hack)
POCO_SERIAL_ENABLE_BEAGLEBONE_RS485_HACK = 1
endif

ifdef POCO_SERIAL_ENABLE_BEAGLEBONE_RS485_HACK
SYSFLAGS += -DPOCO_SERIAL_ENABLE_BEAGLEBONE_RS485_HACK
endif

objects = SerialPort

target         = PocoSerial
target_version = 1
target_libs    = PocoFoundation

include $(POCO_BASE)/build/rules/lib
