// 
// GetMostPopularToppingReply.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef Pizzeria_GetMostPopularToppingReply_INCLUDED
#define Pizzeria_GetMostPopularToppingReply_INCLUDED


#include "Pizzeria/ExtTopping.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"


namespace Pizzeria {


//@ name=getMostPopularToppingReply
//@ namespace="http://www.appinf.com/webservices/PizzaDeliveryService/"
//@ serialize
class GetMostPopularToppingReply
{
public:
	GetMostPopularToppingReply();

	GetMostPopularToppingReply(const ExtTopping& return_);

	virtual ~GetMostPopularToppingReply();

	const ExtTopping& getReturn() const;

	void setReturn(const ExtTopping& val);

private:
	//@ name=return
	//@ order=0
	ExtTopping _return;

};


inline const ExtTopping& GetMostPopularToppingReply::getReturn() const
{
	return _return;
}


inline void GetMostPopularToppingReply::setReturn(const ExtTopping& val)
{
	_return = val;
}


} // Pizzeria


#endif // Pizzeria_GetMostPopularToppingReply_INCLUDED
