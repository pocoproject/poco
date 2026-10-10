// 
// OrderRequest.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef Pizzeria_OrderRequest_INCLUDED
#define Pizzeria_OrderRequest_INCLUDED


#include "Pizzeria/DeliveryAddress.h"
#include "Pizzeria/Pizza.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"


namespace Pizzeria {


//@ name=orderRequest
//@ namespace="http://www.appinf.com/webservices/PizzaDeliveryService/"
//@ serialize
class OrderRequest
{
public:
	OrderRequest();

	OrderRequest(
		const DeliveryAddress& deliverTo, 
		const Pizza& pizza);

	virtual ~OrderRequest();

	const DeliveryAddress& getDeliverTo() const;

	const Pizza& getPizza() const;

	void setDeliverTo(const DeliveryAddress& val);

	void setPizza(const Pizza& val);

private:
	//@ name=deliverTo
	//@ order=0
	DeliveryAddress _deliverTo;

	//@ name=pizza
	//@ order=1
	Pizza _pizza;

};


inline const DeliveryAddress& OrderRequest::getDeliverTo() const
{
	return _deliverTo;
}


inline const Pizza& OrderRequest::getPizza() const
{
	return _pizza;
}


inline void OrderRequest::setDeliverTo(const DeliveryAddress& val)
{
	_deliverTo = val;
}


inline void OrderRequest::setPizza(const Pizza& val)
{
	_pizza = val;
}


} // Pizzeria


#endif // Pizzeria_OrderRequest_INCLUDED
