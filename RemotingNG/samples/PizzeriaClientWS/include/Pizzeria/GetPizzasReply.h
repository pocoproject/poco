// 
// GetPizzasReply.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef Pizzeria_GetPizzasReply_INCLUDED
#define Pizzeria_GetPizzasReply_INCLUDED


#include "Pizzeria/Pizza.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <vector>


namespace Pizzeria {


//@ name=getPizzasReply
//@ namespace="http://www.appinf.com/webservices/PizzaDeliveryService/"
//@ serialize
class GetPizzasReply
{
public:
	GetPizzasReply();

	GetPizzasReply(const std::vector<Pizza>& return_);

	virtual ~GetPizzasReply();

	const std::vector<Pizza>& getReturn() const;

	std::vector<Pizza>& getReturn();

	void setReturn(const std::vector<Pizza>& val);

private:
	//@ mandatory=false
	//@ name=return
	//@ order=0
	std::vector<Pizza> _return;

};


inline const std::vector<Pizza>& GetPizzasReply::getReturn() const
{
	return _return;
}


inline std::vector<Pizza>& GetPizzasReply::getReturn()
{
	return _return;
}


inline void GetPizzasReply::setReturn(const std::vector<Pizza>& val)
{
	_return = val;
}


} // Pizzeria


#endif // Pizzeria_GetPizzasReply_INCLUDED
