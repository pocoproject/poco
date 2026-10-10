#
# Makefile
#
# Makefile for RemotingNG REST Pizzeria sample.
#

include $(POCO_BASE)/build/rules/global

objects = \
	DeliveryAddress \
	IOrderCollectionEndpoint \
	IOrderEndpoint \
	IPizzaCollectionEndpoint \
	IToppingCollectionEndpoint \
	OrderCollection \
	OrderCollectionEndpoint \
	OrderCollectionEndpointRemoteObject \
	OrderCollectionEndpointServerHelper \
	OrderCollectionEndpointSkeleton \
	OrderEndpoint \
	OrderEndpointRemoteObject \
	OrderEndpointServerHelper \
	OrderEndpointSkeleton \
	Pizza \
	PizzaCollectionEndpoint \
	PizzaCollectionEndpointRemoteObject \
	PizzaCollectionEndpointServerHelper \
	PizzaCollectionEndpointSkeleton \
	PizzeriaServer \
	Topping \
	ToppingCollectionEndpoint \
	ToppingCollectionEndpointRemoteObject \
	ToppingCollectionEndpointServerHelper \
	ToppingCollectionEndpointSkeleton

target         = Pizzeria
target_version = 1
target_libs    = PocoRemotingNGREST PocoRemotingNG PocoUtil PocoNet PocoXML PocoFoundation

include $(POCO_BASE)/build/rules/exec
