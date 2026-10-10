// 
// DeliveryAddressDetails.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef Pizzeria_DeliveryAddressDetails_INCLUDED
#define Pizzeria_DeliveryAddressDetails_INCLUDED


#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"


namespace Pizzeria {


//@ name=DeliveryAddressDetails
//@ namespace="http://www.appinf.com/webservices/PizzaDeliveryService/"
//@ serialize
class DeliveryAddressDetails
{
public:
	DeliveryAddressDetails();

	DeliveryAddressDetails(double milesAway);

	virtual ~DeliveryAddressDetails();

	double getMilesAway() const;

	void setMilesAway(double val);

private:
	//@ name=milesAway
	//@ order=0
	double _milesAway;

};


inline double DeliveryAddressDetails::getMilesAway() const
{
	return _milesAway;
}


inline void DeliveryAddressDetails::setMilesAway(double val)
{
	_milesAway = val;
}


} // Pizzeria


#endif // Pizzeria_DeliveryAddressDetails_INCLUDED
