// 
// GetWaitTimeReply.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef Pizzeria_GetWaitTimeReply_INCLUDED
#define Pizzeria_GetWaitTimeReply_INCLUDED


#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include "Poco/Types.h"


namespace Pizzeria {


//@ name=getWaitTimeReply
//@ namespace="http://www.appinf.com/webservices/PizzaDeliveryService/"
//@ serialize
class GetWaitTimeReply
{
public:
	GetWaitTimeReply();

	GetWaitTimeReply(Poco::Int64 return_);

	virtual ~GetWaitTimeReply();

	Poco::Int64 getReturn() const;

	void setReturn(Poco::Int64 val);

private:
	//@ name=return
	//@ order=0
	Poco::Int64 _return;

};


inline Poco::Int64 GetWaitTimeReply::getReturn() const
{
	return _return;
}


inline void GetWaitTimeReply::setReturn(Poco::Int64 val)
{
	_return = val;
}


} // Pizzeria


#endif // Pizzeria_GetWaitTimeReply_INCLUDED
