// 
// OrderReply.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef Pizzeria_OrderReply_INCLUDED
#define Pizzeria_OrderReply_INCLUDED


#include "Poco/DateTime.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"


namespace Pizzeria {


//@ name=orderReply
//@ namespace="http://www.appinf.com/webservices/PizzaDeliveryService/"
//@ serialize
class OrderReply
{
public:
	OrderReply();

	OrderReply(const Poco::DateTime& return_);

	virtual ~OrderReply();

	const Poco::DateTime& getReturn() const;

	void setReturn(const Poco::DateTime& val);

private:
	//@ name=return
	//@ order=0
	Poco::DateTime _return;

};


inline const Poco::DateTime& OrderReply::getReturn() const
{
	return _return;
}


inline void OrderReply::setReturn(const Poco::DateTime& val)
{
	_return = val;
}


} // Pizzeria


#endif // Pizzeria_OrderReply_INCLUDED
