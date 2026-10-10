#
# Makefile
#
# Makefile for RemotingNG REST PizzeriaClient sample.
#

include $(POCO_BASE)/build/rules/global

EXTRA_CXXFLAGS += -I../PizzeriaREST/include

objects = \
	DeliveryAddress \
	IOrderCollectionEndpoint \
	IOrderEndpoint \
	IPizzaCollectionEndpoint \
	IToppingCollectionEndpoint \
	OrderCollectionEndpointClientHelper \
	OrderCollectionEndpointProxy \
	OrderCollectionEndpointProxyFactory \
	OrderEndpointClientHelper \
	OrderEndpointProxy \
	OrderEndpointProxyFactory \
	PizzaCollectionEndpointClientHelper \
	PizzaCollectionEndpointProxy \
	PizzaCollectionEndpointProxyFactory \
	Pizza \
	PizzeriaClient \
	ToppingCollectionEndpointClientHelper \
	ToppingCollectionEndpointProxy \
	ToppingCollectionEndpointProxyFactory \
	Topping

target         = PizzeriaClient
target_version = 1
target_libs    = PocoRemotingNGREST PocoRemotingNG PocoUtil PocoNet PocoXML PocoFoundation

include $(POCO_BASE)/build/rules/exec
