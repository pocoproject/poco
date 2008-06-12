#
# Makefile
#
# $Id: //poco/Main/WebWidgets/Makefile#26 $
#
# Makefile for Poco WebWidgets
#

include $(POCO_BASE)/build/rules/global

objects = Renderable Renderer RenderContext LookAndFeel Type Delegate \
	Formatter IntFormatter DoubleFormatter \
	View ContainerView Control Cell Button ButtonCell TextField TextFieldCell \
	Label Form Table Page BoolFormatter DataRetriever \
	DateField DateFieldCell NumberField NumberFieldCell \
	DateFormatter ComboBox ComboBoxCell ImageButton ImageButtonCell \
	TextEdit TextEditCell ToggleButton CheckButton RadioButton \
	ToggleButtonCell CheckButtonCell RadioButtonCell Panel \
	Layout GridLayout HorizontalLayout VerticalLayout \
	TabView HTML Image ProgressIndicator \
	Frame Collapsible ListBox ListBoxCell \
	TableModel TableColumn SimpleTableModel \
	PasswordField PasswordFieldCell TimeField TimeFieldCell \
	WebApplication RequestProcessor RequestHandler \
	SubmitButton SubmitButtonCell ResourceManager \
	StringFormatter WebWidgetsException JSDelegate

target         = PocoWebWidgets
target_version = $(LIBVERSION)
target_libs    = PocoNet PocoUtil PocoXML PocoFoundation

include $(POCO_BASE)/build/rules/lib
