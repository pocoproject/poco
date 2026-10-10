// 
// GetPizzaNamesReply.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef Pizzeria_GetPizzaNamesReply_INCLUDED
#define Pizzeria_GetPizzaNamesReply_INCLUDED


#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace Pizzeria {


//@ name=getPizzaNamesReply
//@ namespace="http://www.appinf.com/webservices/PizzaDeliveryService/"
//@ serialize
class GetPizzaNamesReply
{
public:
	GetPizzaNamesReply();

	GetPizzaNamesReply(const std::vector<std::string>& return_);

	virtual ~GetPizzaNamesReply();

	const std::vector<std::string>& getReturn() const;

	std::vector<std::string>& getReturn();

	void setReturn(const std::vector<std::string>& val);

private:
	//@ mandatory=false
	//@ name=return
	//@ order=0
	std::vector<std::string> _return;

};


inline const std::vector<std::string>& GetPizzaNamesReply::getReturn() const
{
	return _return;
}


inline std::vector<std::string>& GetPizzaNamesReply::getReturn()
{
	return _return;
}


inline void GetPizzaNamesReply::setReturn(const std::vector<std::string>& val)
{
	_return = val;
}


} // Pizzeria


#endif // Pizzeria_GetPizzaNamesReply_INCLUDED
