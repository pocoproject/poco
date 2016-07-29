#
# Makefile
#
# $Id: //poco/1.4/FSM/samples/TrafficLight/Makefile#1 $
#
# Makefile for Poco FSM TrafficLight
#

include $(POCO_BASE)/build/rules/global
INCLUDE 		+= -I$(POCO_BASE)/FSM/include 
CXXFLAGS 		+= -fno-operator-names

# Cygwin Poco*.dll should be on PATH
ifeq ($(OSNAME), Cygwin)
    PATH       :=$(LIBPATH):$(PATH)
endif

# Where to find the Finite State Machine Compiler executable
FSMc   = $(POCO_BUILD)/FSM/bin/$(OSNAME)/$(OSARCH)/fsmc

objects = stoplight Stoplight_sm TrafficLight

target         = TrafficLight
target_version = 1
target_libs    = PocoUtil PocoXML PocoFoundation

include $(POCO_BASE)/build/rules/exec

ifdef POCO_UNBUNDLED
    SYSLIBS 	+= -lz -lpcre -lexpat
endif

# Rule for runnning Finite State Machine Compiler
src/Stoplight_sm.cpp: src/TrafficLight.sm
	@echo "** Compiling FSM " $<
	$(SET_LD_LIBRARY_PATH) $(FSMc) $<
