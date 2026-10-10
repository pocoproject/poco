// 
// DeliveryAddress.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef Pizzeria_DeliveryAddress_INCLUDED
#define Pizzeria_DeliveryAddress_INCLUDED


#include "Pizzeria/DeliveryAddressDetails.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include "Poco/Types.h"
#include <string>


namespace Pizzeria {


//@ name=DeliveryAddress
//@ namespace="http://www.appinf.com/webservices/PizzaDeliveryService/"
//@ serialize
class DeliveryAddress
{
public:
	DeliveryAddress();

	DeliveryAddress(
		const std::string& cityName, 
		const DeliveryAddressDetails& details, 
		Poco::Int16 houseNo, 
		const std::string& state, 
		const std::string& streetName, 
		const std::string& personName, 
		const std::string& phoneNo, 
		const std::string& zip);

	virtual ~DeliveryAddress();

	const std::string& getCityName() const;

	const DeliveryAddressDetails& getDetails() const;

	Poco::Int16 getHouseNo() const;

	const std::string& getPersonName() const;

	const std::string& getPhoneNo() const;

	const std::string& getState() const;

	const std::string& getStreetName() const;

	const std::string& getZip() const;

	void setCityName(const std::string& val);

	void setDetails(const DeliveryAddressDetails& val);

	void setHouseNo(Poco::Int16 val);

	void setPersonName(const std::string& val);

	void setPhoneNo(const std::string& val);

	void setState(const std::string& val);

	void setStreetName(const std::string& val);

	void setZip(const std::string& val);

private:
	//@ name=cityName
	//@ order=0
	std::string _cityName;

	//@ name=details
	//@ order=1
	DeliveryAddressDetails _details;

	//@ name=houseNo
	//@ order=2
	Poco::Int16 _houseNo;

	//@ name=state
	//@ order=3
	std::string _state;

	//@ name=streetName
	//@ order=4
	std::string _streetName;

	//@ name=personName
	//@ order=5
	std::string _personName;

	//@ name=phoneNo
	//@ order=6
	std::string _phoneNo;

	//@ name=zip
	//@ order=7
	std::string _zip;

};


inline const std::string& DeliveryAddress::getCityName() const
{
	return _cityName;
}


inline const DeliveryAddressDetails& DeliveryAddress::getDetails() const
{
	return _details;
}


inline Poco::Int16 DeliveryAddress::getHouseNo() const
{
	return _houseNo;
}


inline const std::string& DeliveryAddress::getPersonName() const
{
	return _personName;
}


inline const std::string& DeliveryAddress::getPhoneNo() const
{
	return _phoneNo;
}


inline const std::string& DeliveryAddress::getState() const
{
	return _state;
}


inline const std::string& DeliveryAddress::getStreetName() const
{
	return _streetName;
}


inline const std::string& DeliveryAddress::getZip() const
{
	return _zip;
}


inline void DeliveryAddress::setCityName(const std::string& val)
{
	_cityName = val;
}


inline void DeliveryAddress::setDetails(const DeliveryAddressDetails& val)
{
	_details = val;
}


inline void DeliveryAddress::setHouseNo(Poco::Int16 val)
{
	_houseNo = val;
}


inline void DeliveryAddress::setPersonName(const std::string& val)
{
	_personName = val;
}


inline void DeliveryAddress::setPhoneNo(const std::string& val)
{
	_phoneNo = val;
}


inline void DeliveryAddress::setState(const std::string& val)
{
	_state = val;
}


inline void DeliveryAddress::setStreetName(const std::string& val)
{
	_streetName = val;
}


inline void DeliveryAddress::setZip(const std::string& val)
{
	_zip = val;
}


} // Pizzeria


#endif // Pizzeria_DeliveryAddress_INCLUDED
