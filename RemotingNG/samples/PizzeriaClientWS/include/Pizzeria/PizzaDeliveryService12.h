// 
// PizzaDeliveryService12.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef Pizzeria_PizzaDeliveryService12_INCLUDED
#define Pizzeria_PizzaDeliveryService12_INCLUDED


#include "Pizzeria/DeliveryAddress.h"
#include "Pizzeria/ExtTopping.h"
#include "Pizzeria/Pizza.h"
#include "Poco/DateTime.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include "Poco/Types.h"
#include <string>
#include <vector>


namespace Pizzeria {


//@ name=PizzaDeliveryService12
//@ namespace="http://www.appinf.com/webservices/PizzaDeliveryService/"
//@ remote
class PizzaDeliveryService12
{
public:
	virtual ~PizzaDeliveryService12();

	//@ action="http://www.appinf.com/webservices/PizzaDeliveryService/getAnyPizza"
	//@ name=getAnyPizza
	//@ replyName=getAnyPizzaReply
	//@ $return_={direction=out, name=return}
	virtual void getAnyPizza(Pizza& return_) = 0;

	//@ action="http://www.appinf.com/webservices/PizzaDeliveryService/getMostPopularTopping"
	//@ name=getMostPopularTopping
	//@ replyName=getMostPopularToppingReply
	//@ $return_={direction=out, name=return}
	virtual void getMostPopularTopping(ExtTopping& return_) = 0;

	//@ action="http://www.appinf.com/webservices/PizzaDeliveryService/getPizzaNames"
	//@ name=getPizzaNames
	//@ replyName=getPizzaNamesReply
	//@ $return_={direction=out, name=return}
	virtual void getPizzaNames(std::vector<std::string>& return_) = 0;

	//@ action="http://www.appinf.com/webservices/PizzaDeliveryService/getPizzas"
	//@ name=getPizzas
	//@ replyName=getPizzasReply
	//@ $return_={direction=out, name=return}
	virtual void getPizzas(std::vector<Pizza>& return_) = 0;

	//@ action="http://www.appinf.com/webservices/PizzaDeliveryService/getToppings"
	//@ name=getToppings
	//@ replyName=getToppingsReply
	//@ $return_={direction=out, name=return}
	virtual void getToppings(std::vector<ExtTopping>& return_) = 0;

	//@ action="http://www.appinf.com/webservices/PizzaDeliveryService/getWaitTime"
	//@ name=getWaitTime
	//@ replyName=getWaitTimeReply
	//@ $return_={direction=out, name=return}
	virtual void getWaitTime(Poco::Int64& return_) = 0;

	//@ action="http://www.appinf.com/webservices/PizzaDeliveryService/order"
	//@ name=order
	//@ replyName=orderReply
	//@ $deliverTo={direction=in, name=deliverTo}
	//@ $pizza={direction=in, name=pizza}
	//@ $return_={direction=out, name=return}
	virtual void order(
		const DeliveryAddress& deliverTo, 
		const Pizza& pizza, 
		Poco::DateTime& return_) = 0;
};


} // Pizzeria


#endif // Pizzeria_PizzaDeliveryService12_INCLUDED
