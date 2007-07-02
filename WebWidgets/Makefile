#
# Makefile
#
# $Id: //poco/Main/WebWidgets/Makefile#7 $
#
# Makefile for Poco WebWidgets
#

include $(POCO_BASE)/build/rules/global

objects = AbstractButton Button CheckBox \
	ComboBox ComplexPanel FlowPanel Form \
	FormHandler GUIObject HTML HTMLVisitor \
	HyperLink Image ImageButton IndexedPanel \
	InputField Label ListView Panel Parser \
	ParserFactory RadioButton RootPanel \
	SimplePanel StackPanel Table TextArea \
	TextField Visitable Visitor Widget \
	WidgetHandler WidgetHandlerFactory XMLUtil \
	NamedWidget HidablePanel MouseEventArgs \
	JSDelegate JSEventHandler FileHandler \
	MouseMovementArgs KeyEventArgs PwdField
	
target         = PocoWebWidgets
target_version = $(LIBVERSION)
target_libs    = PocoNet PocoXML PocoFoundation

include $(POCO_BASE)/build/rules/lib
