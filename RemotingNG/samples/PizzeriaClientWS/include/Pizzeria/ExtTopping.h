// 
// ExtTopping.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef Pizzeria_ExtTopping_INCLUDED
#define Pizzeria_ExtTopping_INCLUDED


#include "Pizzeria/Topping.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>


namespace Pizzeria {


//@ name=ExtTopping
//@ namespace="http://www.appinf.com/webservices/PizzaDeliveryService/"
//@ serialize
class ExtTopping: public Topping
{
public:
	ExtTopping();

	ExtTopping(
		const std::string& name, 
		Poco::UInt32 price, 
		const std::string& addInfo);

	virtual ~ExtTopping();

	const std::string& getAddInfo() const;

	void setAddInfo(const std::string& val);

private:
	//@ name=addInfo
	//@ order=0
	std::string _addInfo;

};


inline const std::string& ExtTopping::getAddInfo() const
{
	return _addInfo;
}


inline void ExtTopping::setAddInfo(const std::string& val)
{
	_addInfo = val;
}


} // Pizzeria


#endif // Pizzeria_ExtTopping_INCLUDED
