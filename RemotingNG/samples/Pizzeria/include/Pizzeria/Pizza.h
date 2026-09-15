//
// Pizza.h
//
// Definition of the Pizza class.
//
// Copyright (c) 2006-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties, 
// copied or duplicated in any form, in whole or in part.
//


#ifndef Pizza_INCLUDED
#define Pizza_INCLUDED


#include "Pizzeria/Topping.h"


namespace Pizzeria {


//@ serialize
class Pizza
	/// Sample Pizza class. Not thread-safe.
{
public:
	//@ serialize
	struct Details
	{
		double calories;
		double percentFat;
		double carboHydrates;
	};

	Pizza(const std::string& name=std::string(), Cents basePrice=400);
		/// Creates the Pizza.

	~Pizza();
		/// Destroys the Pizza.

	Cents price() const;
		/// Calculates the price of the pizza

	const std::string& getName() const;
	void setName(const std::string& name);

	void add(const ExtTopping& top);
	void remove(const std::string& toppingId);

	const ExtToppings& getToppings() const;
	void setToppings(const ExtToppings& top);

	Cents getBasePrice() const;
	void setBasePrice(Cents price);

	const Details& getDetails() const;
	Details& getDetails();
	void setDetails(const Details& d);

	std::string toString() const;

private:
	std::string _name;
	ExtToppings _toppings;
	Cents       _basePrice;
	Details     _details;
};


inline const std::string& Pizza::getName() const
{
	return _name;
}


inline void Pizza::setName(const std::string& name)
{
	_name = name;
}


inline const ExtToppings& Pizza::getToppings() const
{
	return _toppings;
}


inline void Pizza::setToppings(const ExtToppings& top)
{
	_toppings = top;
}


inline Cents Pizza::getBasePrice() const
{
	return _basePrice;
}


inline void Pizza::setBasePrice(Cents price)
{
	_basePrice = price;
}


inline void Pizza::add(const ExtTopping& top)
{
	// allow duplicate adds
	_toppings.push_back(top);
}

inline const Pizza::Details& Pizza::getDetails() const
{
	return _details;
}


inline Pizza::Details& Pizza::getDetails()
{
	return _details;
}


inline void Pizza::setDetails(const Pizza::Details& d)
{
	_details = d;
}


} // namespace Pizzeria


#endif // Pizza_INCLUDED
