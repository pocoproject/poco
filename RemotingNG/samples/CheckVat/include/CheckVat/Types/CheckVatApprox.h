// 
// CheckVatApprox.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef CheckVat_Types_CheckVatApprox_INCLUDED
#define CheckVat_Types_CheckVatApprox_INCLUDED


#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>


namespace CheckVat {
namespace Types {


//@ name="#checkVatApprox"
//@ namespace="urn:ec.europa.eu:taxud:vies:services:checkVat:types"
//@ serialize
class CheckVatApprox
{
public:
	CheckVatApprox();

	CheckVatApprox(
		const std::string& countryCode, 
		const std::string& vatNumber, 
		const Poco::Optional<std::string>& traderName, 
		const Poco::Optional<std::string>& traderCompanyType, 
		const Poco::Optional<std::string>& traderStreet, 
		const Poco::Optional<std::string>& traderPostcode, 
		const Poco::Optional<std::string>& traderCity, 
		const Poco::Optional<std::string>& requesterCountryCode, 
		const Poco::Optional<std::string>& requesterVatNumber);

	virtual ~CheckVatApprox();

	const std::string& getCountryCode() const;

	const Poco::Optional<std::string>& getRequesterCountryCode() const;

	const Poco::Optional<std::string>& getRequesterVatNumber() const;

	const Poco::Optional<std::string>& getTraderCity() const;

	const Poco::Optional<std::string>& getTraderCompanyType() const;

	const Poco::Optional<std::string>& getTraderName() const;

	const Poco::Optional<std::string>& getTraderPostcode() const;

	const Poco::Optional<std::string>& getTraderStreet() const;

	const std::string& getVatNumber() const;

	void setCountryCode(const std::string& val);

	void setRequesterCountryCode(const Poco::Optional<std::string>& val);

	void setRequesterVatNumber(const Poco::Optional<std::string>& val);

	void setTraderCity(const Poco::Optional<std::string>& val);

	void setTraderCompanyType(const Poco::Optional<std::string>& val);

	void setTraderName(const Poco::Optional<std::string>& val);

	void setTraderPostcode(const Poco::Optional<std::string>& val);

	void setTraderStreet(const Poco::Optional<std::string>& val);

	void setVatNumber(const std::string& val);

private:
	//@ name=countryCode
	//@ order=0
	std::string _countryCode;

	//@ name=vatNumber
	//@ order=1
	std::string _vatNumber;

	//@ mandatory=false
	//@ name=traderName
	//@ order=2
	Poco::Optional<std::string> _traderName;

	//@ mandatory=false
	//@ name=traderCompanyType
	//@ order=3
	Poco::Optional<std::string> _traderCompanyType;

	//@ mandatory=false
	//@ name=traderStreet
	//@ order=4
	Poco::Optional<std::string> _traderStreet;

	//@ mandatory=false
	//@ name=traderPostcode
	//@ order=5
	Poco::Optional<std::string> _traderPostcode;

	//@ mandatory=false
	//@ name=traderCity
	//@ order=6
	Poco::Optional<std::string> _traderCity;

	//@ mandatory=false
	//@ name=requesterCountryCode
	//@ order=7
	Poco::Optional<std::string> _requesterCountryCode;

	//@ mandatory=false
	//@ name=requesterVatNumber
	//@ order=8
	Poco::Optional<std::string> _requesterVatNumber;

};


inline const std::string& CheckVatApprox::getCountryCode() const
{
	return _countryCode;
}


inline const Poco::Optional<std::string>& CheckVatApprox::getRequesterCountryCode() const
{
	return _requesterCountryCode;
}


inline const Poco::Optional<std::string>& CheckVatApprox::getRequesterVatNumber() const
{
	return _requesterVatNumber;
}


inline const Poco::Optional<std::string>& CheckVatApprox::getTraderCity() const
{
	return _traderCity;
}


inline const Poco::Optional<std::string>& CheckVatApprox::getTraderCompanyType() const
{
	return _traderCompanyType;
}


inline const Poco::Optional<std::string>& CheckVatApprox::getTraderName() const
{
	return _traderName;
}


inline const Poco::Optional<std::string>& CheckVatApprox::getTraderPostcode() const
{
	return _traderPostcode;
}


inline const Poco::Optional<std::string>& CheckVatApprox::getTraderStreet() const
{
	return _traderStreet;
}


inline const std::string& CheckVatApprox::getVatNumber() const
{
	return _vatNumber;
}


inline void CheckVatApprox::setCountryCode(const std::string& val)
{
	_countryCode = val;
}


inline void CheckVatApprox::setRequesterCountryCode(const Poco::Optional<std::string>& val)
{
	_requesterCountryCode = val;
}


inline void CheckVatApprox::setRequesterVatNumber(const Poco::Optional<std::string>& val)
{
	_requesterVatNumber = val;
}


inline void CheckVatApprox::setTraderCity(const Poco::Optional<std::string>& val)
{
	_traderCity = val;
}


inline void CheckVatApprox::setTraderCompanyType(const Poco::Optional<std::string>& val)
{
	_traderCompanyType = val;
}


inline void CheckVatApprox::setTraderName(const Poco::Optional<std::string>& val)
{
	_traderName = val;
}


inline void CheckVatApprox::setTraderPostcode(const Poco::Optional<std::string>& val)
{
	_traderPostcode = val;
}


inline void CheckVatApprox::setTraderStreet(const Poco::Optional<std::string>& val)
{
	_traderStreet = val;
}


inline void CheckVatApprox::setVatNumber(const std::string& val)
{
	_vatNumber = val;
}


} } // CheckVat::Types


#endif // CheckVat_Types_CheckVatApprox_INCLUDED
