// 
// GetToppingsReply.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef Pizzeria_GetToppingsReply_INCLUDED
#define Pizzeria_GetToppingsReply_INCLUDED


#include "Pizzeria/ExtTopping.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <vector>


namespace Pizzeria {


//@ name=getToppingsReply
//@ namespace="http://www.appinf.com/webservices/PizzaDeliveryService/"
//@ serialize
class GetToppingsReply
{
public:
	GetToppingsReply();

	GetToppingsReply(const std::vector<ExtTopping>& return_);

	virtual ~GetToppingsReply();

	const std::vector<ExtTopping>& getReturn() const;

	std::vector<ExtTopping>& getReturn();

	void setReturn(const std::vector<ExtTopping>& val);

private:
	//@ mandatory=false
	//@ name=return
	//@ order=0
	std::vector<ExtTopping> _return;

};


inline const std::vector<ExtTopping>& GetToppingsReply::getReturn() const
{
	return _return;
}


inline std::vector<ExtTopping>& GetToppingsReply::getReturn()
{
	return _return;
}


inline void GetToppingsReply::setReturn(const std::vector<ExtTopping>& val)
{
	_return = val;
}


} // Pizzeria


#endif // Pizzeria_GetToppingsReply_INCLUDED
