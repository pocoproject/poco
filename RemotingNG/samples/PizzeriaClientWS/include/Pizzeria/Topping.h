// 
// Topping.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef Pizzeria_Topping_INCLUDED
#define Pizzeria_Topping_INCLUDED


#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include "Poco/Types.h"
#include <string>


namespace Pizzeria {


//@ name=Topping
//@ namespace="http://www.appinf.com/webservices/PizzaDeliveryService/"
//@ serialize
class Topping
{
public:
	Topping();

	Topping(
		const std::string& name, 
		Poco::UInt32 price);

	virtual ~Topping();

	const std::string& getName() const;

	Poco::UInt32 getPrice() const;

	void setName(const std::string& val);

	void setPrice(Poco::UInt32 val);

private:
	//@ name=name
	//@ order=0
	std::string _name;

	//@ name=price
	//@ order=1
	Poco::UInt32 _price;

};


inline const std::string& Topping::getName() const
{
	return _name;
}


inline Poco::UInt32 Topping::getPrice() const
{
	return _price;
}


inline void Topping::setName(const std::string& val)
{
	_name = val;
}


inline void Topping::setPrice(Poco::UInt32 val)
{
	_price = val;
}


} // Pizzeria


#endif // Pizzeria_Topping_INCLUDED
