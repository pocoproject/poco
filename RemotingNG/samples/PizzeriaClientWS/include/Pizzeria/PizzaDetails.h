// 
// PizzaDetails.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef Pizzeria_PizzaDetails_INCLUDED
#define Pizzeria_PizzaDetails_INCLUDED


#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"


namespace Pizzeria {


//@ name=PizzaDetails
//@ namespace="http://www.appinf.com/webservices/PizzaDeliveryService/"
//@ serialize
class PizzaDetails
{
public:
	PizzaDetails();

	PizzaDetails(
		double calories, 
		double carboHydrates, 
		double percentFat);

	virtual ~PizzaDetails();

	double getCalories() const;

	double getCarboHydrates() const;

	double getPercentFat() const;

	void setCalories(double val);

	void setCarboHydrates(double val);

	void setPercentFat(double val);

private:
	//@ name=calories
	//@ order=0
	double _calories;

	//@ name=carboHydrates
	//@ order=1
	double _carboHydrates;

	//@ name=percentFat
	//@ order=2
	double _percentFat;

};


inline double PizzaDetails::getCalories() const
{
	return _calories;
}


inline double PizzaDetails::getCarboHydrates() const
{
	return _carboHydrates;
}


inline double PizzaDetails::getPercentFat() const
{
	return _percentFat;
}


inline void PizzaDetails::setCalories(double val)
{
	_calories = val;
}


inline void PizzaDetails::setCarboHydrates(double val)
{
	_carboHydrates = val;
}


inline void PizzaDetails::setPercentFat(double val)
{
	_percentFat = val;
}


} // Pizzeria


#endif // Pizzeria_PizzaDetails_INCLUDED
