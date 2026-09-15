//
// DeliveryAddress.cpp
//
// Copyright (c) 2006-2016, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties,
// copied or duplicated in any form, in whole or in part.
//


#include "Pizzeria/DeliveryAddress.h"
#include "Poco/NumberFormatter.h"


namespace Pizzeria {


DeliveryAddress::DeliveryAddress():
	m_personName(),
	m_Street(),
	_housenumber(0),
	_cityName(),
	phoneNumber()
{
	_details.milesAway = 0.0;
}


DeliveryAddress::DeliveryAddress(const std::string& personName, const std::string& street, Poco::Int16 houseNumber, const std::string& cityName, const std::string& phoneNumb):
	m_personName(personName),
	m_Street(street),
	_housenumber(houseNumber),
	_cityName(cityName),
	phoneNumber(phoneNumb)
{
	_details.milesAway = 0.0;
}


DeliveryAddress::~DeliveryAddress()
{
}


std::string DeliveryAddress::toString() const
{
	std::string res(getPersonName());
	res.append(", ");
	res.append(getStreet());
	res.append(" ");
	res.append(Poco::NumberFormatter::format(getHouseNumber()));
	res.append(", ");
	res.append(getCityName());
	res.append(", ");
	res.append(getZip());
	if (!getState().empty())
	{
		res.append(", ");
		res.append(getState());
	}
	if (!getPhoneNumber().empty())
	{
		res.append(", Phone: ");
		res.append(getPhoneNumber());
	}
	return res;
}


} // namespace Pizzeria
