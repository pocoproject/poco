// 
// DeliveryAddress.cpp
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#include "Pizzeria/DeliveryAddress.h"


namespace Pizzeria {


DeliveryAddress::DeliveryAddress():
	_cityName(),
	_details(),
	_houseNo(),
	_state(),
	_streetName(),
	_personName(),
	_phoneNo(),
	_zip()
{
}


DeliveryAddress::DeliveryAddress(const std::string& cityName, const DeliveryAddressDetails& details, Poco::Int16 houseNo, const std::string& state, const std::string& streetName, const std::string& personName, const std::string& phoneNo, const std::string& zip):
	_cityName(cityName),
	_details(details),
	_houseNo(houseNo),
	_state(state),
	_streetName(streetName),
	_personName(personName),
	_phoneNo(phoneNo),
	_zip(zip)
{
}


DeliveryAddress::~DeliveryAddress()
{
}


} // Pizzeria


