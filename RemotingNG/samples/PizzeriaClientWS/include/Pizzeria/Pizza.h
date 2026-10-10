// 
// Pizza.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef Pizzeria_Pizza_INCLUDED
#define Pizzeria_Pizza_INCLUDED


#include "Pizzeria/ExtTopping.h"
#include "Pizzeria/PizzaDetails.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include "Poco/Types.h"
#include <string>
#include <vector>


namespace Pizzeria {


//@ name=Pizza
//@ namespace="http://www.appinf.com/webservices/PizzaDeliveryService/"
//@ serialize
class Pizza
{
public:
	Pizza();

	Pizza(
		Poco::UInt32 basePrice, 
		const PizzaDetails& details, 
		const std::string& name, 
		const std::vector<ExtTopping>& toppings);

	virtual ~Pizza();

	Poco::UInt32 getBasePrice() const;

	const PizzaDetails& getDetails() const;

	const std::string& getName() const;

	const std::vector<ExtTopping>& getToppings() const;

	std::vector<ExtTopping>& getToppings();

	void setBasePrice(Poco::UInt32 val);

	void setDetails(const PizzaDetails& val);

	void setName(const std::string& val);

	void setToppings(const std::vector<ExtTopping>& val);

private:
	//@ name=basePrice
	//@ order=0
	Poco::UInt32 _basePrice;

	//@ name=details
	//@ order=1
	PizzaDetails _details;

	//@ name=name
	//@ order=2
	std::string _name;

	//@ mandatory=false
	//@ name=toppings
	//@ order=3
	std::vector<ExtTopping> _toppings;

};


inline Poco::UInt32 Pizza::getBasePrice() const
{
	return _basePrice;
}


inline const PizzaDetails& Pizza::getDetails() const
{
	return _details;
}


inline const std::string& Pizza::getName() const
{
	return _name;
}


inline const std::vector<ExtTopping>& Pizza::getToppings() const
{
	return _toppings;
}


inline std::vector<ExtTopping>& Pizza::getToppings()
{
	return _toppings;
}


inline void Pizza::setBasePrice(Poco::UInt32 val)
{
	_basePrice = val;
}


inline void Pizza::setDetails(const PizzaDetails& val)
{
	_details = val;
}


inline void Pizza::setName(const std::string& val)
{
	_name = val;
}


inline void Pizza::setToppings(const std::vector<ExtTopping>& val)
{
	_toppings = val;
}


} // Pizzeria


#endif // Pizzeria_Pizza_INCLUDED
