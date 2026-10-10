// 
// OrderRequest.cpp
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#include "Pizzeria/OrderRequest.h"


namespace Pizzeria {


OrderRequest::OrderRequest():
	_deliverTo(),
	_pizza()
{
}


OrderRequest::OrderRequest(const DeliveryAddress& deliverTo, const Pizza& pizza):
	_deliverTo(deliverTo),
	_pizza(pizza)
{
}


OrderRequest::~OrderRequest()
{
}


} // Pizzeria


