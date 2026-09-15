//
// DeliveryAddress.h
//
// Definition of the DeliveryAddress class.
//
// Copyright (c) 2006-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties, 
// copied or duplicated in any form, in whole or in part.
//


#ifndef DeliveryAddress_INCLUDED
#define DeliveryAddress_INCLUDED


#include "Poco/Foundation.h"


namespace Pizzeria {


//@ serialize
class DeliveryAddress
{
public:
	//@ serialize
	struct Details
	{
		double milesAway;
	};
	
	DeliveryAddress();
		/// Creates the DeliveryAddress.
		
	DeliveryAddress(const std::string& personName, const std::string& street, Poco::Int16 houseNumber, const std::string& cityName, const std::string& phoneNumber = std::string());
		/// Creates the DeliveryAddress.

	~DeliveryAddress();
		/// Destroys the DeliveryAddress.

	const std::string& getPersonName() const;
	void setPersonName(const std::string& name);

	const std::string& getStreet() const;
	void setStreet(const std::string& street);

	Poco::Int16 getHouseNumber() const;
	void setHouseNumber(Poco::Int16 hn);

	const std::string& getCityName() const;
	void setCityName(const std::string& cn);
	
	const std::string& getZip() const;
	void setZip(const std::string& zip);

	const std::string& getState() const;
	void setState(const std::string& state);

	const std::string& getPhoneNumber() const;
	void setPhoneNumber(const std::string& pn);

	const Details& getDetails() const;
	void setDetails(const Details& d);

	std::string toString() const;

private:
	// We're showing different ways of naming member variables.
	std::string m_personName;
	//@ name=streetName
	std::string m_Street;
	//@ name=houseNo
	Poco::Int16 _housenumber;
	std::string _cityName;
	std::string zip_;
	std::string _state;
	//@ mandatory = false
	//@ name=phoneNo
	std::string phoneNumber; /// in case the delivery boy can't find it
	//@ mandatory = false
	Details _details;
};


inline const std::string& DeliveryAddress::getPersonName() const
{
	return m_personName;
}


inline const std::string& DeliveryAddress::getStreet() const
{
	return m_Street;
}


inline Poco::Int16 DeliveryAddress::getHouseNumber() const
{
	return _housenumber;
}


inline const std::string& DeliveryAddress::getCityName() const
{
	return _cityName;
}


inline const std::string& DeliveryAddress::getZip() const
{
	return zip_;
}


inline const std::string& DeliveryAddress::getState() const
{
	return _state;
}


inline void DeliveryAddress::setState(const std::string& state)
{
	_state = state;
}


inline const std::string& DeliveryAddress::getPhoneNumber() const
{
	return phoneNumber;
}


inline void DeliveryAddress::setPersonName(const std::string& name)
{
	m_personName = name;
}


inline void DeliveryAddress::setStreet(const std::string& street)
{
	m_Street = street;
}


inline void DeliveryAddress::setHouseNumber(Poco::Int16 hn)
{
	_housenumber = hn;
}


inline void DeliveryAddress::setCityName(const std::string& cn)
{
	_cityName = cn;
}


inline void DeliveryAddress::setZip(const std::string& zip)
{
	zip_ = zip;
}


inline void DeliveryAddress::setPhoneNumber(const std::string& pn)
{
	phoneNumber = pn;
}


inline const DeliveryAddress::Details& DeliveryAddress::getDetails() const
{
	return _details;
}


inline void DeliveryAddress::setDetails(const DeliveryAddress::Details& d)
{
	_details = d;
}


} // namespace Pizzeria


#endif // DeliveryAddress_INCLUDED
