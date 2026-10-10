#
# Makefile
#
# Makefile for RemotingNG PizzeriaClient sample.
#

include $(POCO_BASE)/build/rules/global

EXTRA_CXXFLAGS += -I../Pizzeria/include

objects = \
	IPizzaDeliveryService \
	IPizzaPickupService \
	PizzaDeliveryServiceClientHelper \
	PizzaDeliveryServiceProxy \
	PizzaDeliveryServiceProxyFactory \
	PizzaPickupServiceClientHelper \
	PizzaPickupServiceProxy \
	PizzaPickupServiceProxyFactory \
	Pizza \
	Topping \
	DeliveryAddress \
	PizzeriaClient

target         = PizzeriaClient
target_version = 1
target_libs    = PocoRemotingNGSOAP PocoRemotingNG PocoUtil PocoNet PocoXML PocoFoundation

include $(POCO_BASE)/build/rules/exec
