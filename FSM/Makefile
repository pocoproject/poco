#
# Makefile
#
# $Id: //poco/1.4/FSM/Makefile#1 $
#
# Makefile for Poco C++ Finite State Machine Compiler
#

include $(POCO_BASE)/build/rules/global
INCLUDE 	+= -Isrc 
CXXFLAGS 	+= -std=c++11 -fno-operator-names

objects = \
Action \
ActionForCpp \
Actions \
Argument \
ArgumentForCpp \
BinaryOperation \
BinaryOperationForCpp \
Character \
Comment \
CompilableForCpp \
Dumper \
Element \
ElementForCpp \
Entry \
EntryForCpp \
Exit \
ExitForCpp \
Expression \
ExpressionForCpp \
FactoryForCpp \
FSM \
fsmc \
FSMForCpp \
Function \
FunctionForCpp \
Guard \
GuardForCpp \
Identifier \
IndentStream \
Keyword \
Literal \
LiteralForCpp \
Map \
MapForCpp \
Number \
Operation \
Operator \
Parameter \
ParameterForCpp \
Parser \
Print \
Reference \
ReferenceForCpp \
State \
StateForCpp \
String \
Token \
Tokenizer \
Transition \
TransitionForCpp \
UnaryOperation \
UnaryOperationForCpp \
Variable \
VariableForCpp
 
ifdef POCO_UNBUNDLED
	SYSLIBS += -lz -lpcre -lexpat
endif

target         = fsmc
target_version = 1
target_libs    = PocoUtil PocoXML PocoJSON PocoFoundation

include $(POCO_BASE)/build/rules/exec
