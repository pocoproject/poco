// 
// GetAnyPizzaReply.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef Pizzeria_GetAnyPizzaReply_INCLUDED
#define Pizzeria_GetAnyPizzaReply_INCLUDED


#include "Pizzeria/Pizza.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"


namespace Pizzeria {


//@ name=getAnyPizzaReply
//@ namespace="http://www.appinf.com/webservices/PizzaDeliveryService/"
//@ serialize
class GetAnyPizzaReply
{
public:
	GetAnyPizzaReply();

	GetAnyPizzaReply(const Pizza& return_);

	virtual ~GetAnyPizzaReply();

	const Pizza& getReturn() const;

	void setReturn(const Pizza& val);

private:
	//@ name=return
	//@ order=0
	Pizza _return;

};


inline const Pizza& GetAnyPizzaReply::getReturn() const
{
	return _return;
}


inline void GetAnyPizzaReply::setReturn(const Pizza& val)
{
	_return = val;
}


} // Pizzeria


#endif // Pizzeria_GetAnyPizzaReply_INCLUDED
