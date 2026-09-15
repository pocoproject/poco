//
// Topping.h
//
// Definition of the Topping class.
//
// Copyright (c) 2006-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties, 
// copied or duplicated in any form, in whole or in part.
//


#ifndef Topping_INCLUDED
#define Topping_INCLUDED


#include "Poco/Foundation.h"
#include <vector>


namespace Pizzeria { 


typedef Poco::UInt32 Cents;


//@ serialize
class Topping
{
public:
	Topping();

	Topping(const std::string& name, Cents price);
		/// Creates the Topping.

	virtual ~Topping();
		/// Destroys the Topping.

	const std::string& getName() const;
	void setName(const std::string& name);

	Cents getPrice() const;
	void setPrice(Cents price);

	std::string toString() const;

private:
	std::string _name;
	Cents       _price;
};


typedef std::vector<Topping> Toppings;


//@ serialize
class ExtTopping: public Topping
{
public:
	ExtTopping();

	ExtTopping(const std::string& name, Cents price);
		/// Creates the ExtTopping.

	virtual ~ExtTopping();
		/// Destroys the ExtTopping.

	void setAddInfo(const std::string& addInfo);

	const std::string& getAddInfo() const;

private:
	std::string _addInfo;
};


typedef std::vector<ExtTopping> ExtToppings;


inline const std::string& Topping::getName() const
{
	return _name;
}


inline Cents Topping::getPrice() const
{
	return _price;
}


inline void Topping::setName(const std::string& name)
{
	_name = name;
}


inline void Topping::setPrice(Cents price)
{
	_price = price;
}


inline void ExtTopping::setAddInfo(const std::string& addInfo)
{
	_addInfo = addInfo;
}


inline const std::string& ExtTopping::getAddInfo() const
{
	return _addInfo;
}


} // namespace Pizzeria


#endif // Topping_INCLUDED
