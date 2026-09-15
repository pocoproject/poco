// 
// CheckVatApproxResponse.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef CheckVat_Types_CheckVatApproxResponse_INCLUDED
#define CheckVat_Types_CheckVatApproxResponse_INCLUDED


#include "Poco/DateTime.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>


namespace CheckVat {
namespace Types {


//@ name="#checkVatApproxResponse"
//@ namespace="urn:ec.europa.eu:taxud:vies:services:checkVat:types"
//@ serialize
class CheckVatApproxResponse
{
public:
	CheckVatApproxResponse();

	CheckVatApproxResponse(
		const std::string& countryCode, 
		const std::string& vatNumber, 
		const Poco::DateTime& requestDate, 
		bool valid, 
		const Poco::Nullable<std::string>& traderName, 
		const Poco::Nullable<std::string>& traderCompanyType, 
		const Poco::Optional<std::string>& traderAddress, 
		const Poco::Optional<std::string>& traderStreet, 
		const Poco::Optional<std::string>& traderPostcode, 
		const Poco::Optional<std::string>& traderCity, 
		const Poco::Optional<std::string>& traderNameMatch, 
		const Poco::Optional<std::string>& traderCompanyTypeMatch, 
		const Poco::Optional<std::string>& traderStreetMatch, 
		const Poco::Optional<std::string>& traderPostcodeMatch, 
		const Poco::Optional<std::string>& traderCityMatch, 
		const std::string& requestIdentifier);

	virtual ~CheckVatApproxResponse();

	const std::string& getCountryCode() const;

	//@ return={xsdType=date}
	const Poco::DateTime& getRequestDate() const;

	const std::string& getRequestIdentifier() const;

	const Poco::Optional<std::string>& getTraderAddress() const;

	const Poco::Optional<std::string>& getTraderCity() const;

	const Poco::Optional<std::string>& getTraderCityMatch() const;

	const Poco::Nullable<std::string>& getTraderCompanyType() const;

	const Poco::Optional<std::string>& getTraderCompanyTypeMatch() const;

	const Poco::Nullable<std::string>& getTraderName() const;

	const Poco::Optional<std::string>& getTraderNameMatch() const;

	const Poco::Optional<std::string>& getTraderPostcode() const;

	const Poco::Optional<std::string>& getTraderPostcodeMatch() const;

	const Poco::Optional<std::string>& getTraderStreet() const;

	const Poco::Optional<std::string>& getTraderStreetMatch() const;

	bool getValid() const;

	const std::string& getVatNumber() const;

	void setCountryCode(const std::string& val);

	//@ $val={xsdType=date}
	void setRequestDate(const Poco::DateTime& val);

	void setRequestIdentifier(const std::string& val);

	void setTraderAddress(const Poco::Optional<std::string>& val);

	void setTraderCity(const Poco::Optional<std::string>& val);

	void setTraderCityMatch(const Poco::Optional<std::string>& val);

	void setTraderCompanyType(const Poco::Nullable<std::string>& val);

	void setTraderCompanyTypeMatch(const Poco::Optional<std::string>& val);

	void setTraderName(const Poco::Nullable<std::string>& val);

	void setTraderNameMatch(const Poco::Optional<std::string>& val);

	void setTraderPostcode(const Poco::Optional<std::string>& val);

	void setTraderPostcodeMatch(const Poco::Optional<std::string>& val);

	void setTraderStreet(const Poco::Optional<std::string>& val);

	void setTraderStreetMatch(const Poco::Optional<std::string>& val);

	void setValid(bool val);

	void setVatNumber(const std::string& val);

private:
	//@ name=countryCode
	//@ order=0
	std::string _countryCode;

	//@ name=vatNumber
	//@ order=1
	std::string _vatNumber;

	//@ name=requestDate
	//@ order=2
	//@ xsdType=date
	Poco::DateTime _requestDate;

	//@ name=valid
	//@ order=3
	bool _valid;

	//@ mandatory=false
	//@ name=traderName
	//@ order=4
	Poco::Nullable<std::string> _traderName;

	//@ mandatory=false
	//@ name=traderCompanyType
	//@ order=5
	Poco::Nullable<std::string> _traderCompanyType;

	//@ mandatory=false
	//@ name=traderAddress
	//@ order=6
	Poco::Optional<std::string> _traderAddress;

	//@ mandatory=false
	//@ name=traderStreet
	//@ order=7
	Poco::Optional<std::string> _traderStreet;

	//@ mandatory=false
	//@ name=traderPostcode
	//@ order=8
	Poco::Optional<std::string> _traderPostcode;

	//@ mandatory=false
	//@ name=traderCity
	//@ order=9
	Poco::Optional<std::string> _traderCity;

	//@ mandatory=false
	//@ name=traderNameMatch
	//@ order=10
	Poco::Optional<std::string> _traderNameMatch;

	//@ mandatory=false
	//@ name=traderCompanyTypeMatch
	//@ order=11
	Poco::Optional<std::string> _traderCompanyTypeMatch;

	//@ mandatory=false
	//@ name=traderStreetMatch
	//@ order=12
	Poco::Optional<std::string> _traderStreetMatch;

	//@ mandatory=false
	//@ name=traderPostcodeMatch
	//@ order=13
	Poco::Optional<std::string> _traderPostcodeMatch;

	//@ mandatory=false
	//@ name=traderCityMatch
	//@ order=14
	Poco::Optional<std::string> _traderCityMatch;

	//@ name=requestIdentifier
	//@ order=15
	std::string _requestIdentifier;

};


inline const std::string& CheckVatApproxResponse::getCountryCode() const
{
	return _countryCode;
}


inline const Poco::DateTime& CheckVatApproxResponse::getRequestDate() const
{
	return _requestDate;
}


inline const std::string& CheckVatApproxResponse::getRequestIdentifier() const
{
	return _requestIdentifier;
}


inline const Poco::Optional<std::string>& CheckVatApproxResponse::getTraderAddress() const
{
	return _traderAddress;
}


inline const Poco::Optional<std::string>& CheckVatApproxResponse::getTraderCity() const
{
	return _traderCity;
}


inline const Poco::Optional<std::string>& CheckVatApproxResponse::getTraderCityMatch() const
{
	return _traderCityMatch;
}


inline const Poco::Nullable<std::string>& CheckVatApproxResponse::getTraderCompanyType() const
{
	return _traderCompanyType;
}


inline const Poco::Optional<std::string>& CheckVatApproxResponse::getTraderCompanyTypeMatch() const
{
	return _traderCompanyTypeMatch;
}


inline const Poco::Nullable<std::string>& CheckVatApproxResponse::getTraderName() const
{
	return _traderName;
}


inline const Poco::Optional<std::string>& CheckVatApproxResponse::getTraderNameMatch() const
{
	return _traderNameMatch;
}


inline const Poco::Optional<std::string>& CheckVatApproxResponse::getTraderPostcode() const
{
	return _traderPostcode;
}


inline const Poco::Optional<std::string>& CheckVatApproxResponse::getTraderPostcodeMatch() const
{
	return _traderPostcodeMatch;
}


inline const Poco::Optional<std::string>& CheckVatApproxResponse::getTraderStreet() const
{
	return _traderStreet;
}


inline const Poco::Optional<std::string>& CheckVatApproxResponse::getTraderStreetMatch() const
{
	return _traderStreetMatch;
}


inline bool CheckVatApproxResponse::getValid() const
{
	return _valid;
}


inline const std::string& CheckVatApproxResponse::getVatNumber() const
{
	return _vatNumber;
}


inline void CheckVatApproxResponse::setCountryCode(const std::string& val)
{
	_countryCode = val;
}


inline void CheckVatApproxResponse::setRequestDate(const Poco::DateTime& val)
{
	_requestDate = val;
}


inline void CheckVatApproxResponse::setRequestIdentifier(const std::string& val)
{
	_requestIdentifier = val;
}


inline void CheckVatApproxResponse::setTraderAddress(const Poco::Optional<std::string>& val)
{
	_traderAddress = val;
}


inline void CheckVatApproxResponse::setTraderCity(const Poco::Optional<std::string>& val)
{
	_traderCity = val;
}


inline void CheckVatApproxResponse::setTraderCityMatch(const Poco::Optional<std::string>& val)
{
	_traderCityMatch = val;
}


inline void CheckVatApproxResponse::setTraderCompanyType(const Poco::Nullable<std::string>& val)
{
	_traderCompanyType = val;
}


inline void CheckVatApproxResponse::setTraderCompanyTypeMatch(const Poco::Optional<std::string>& val)
{
	_traderCompanyTypeMatch = val;
}


inline void CheckVatApproxResponse::setTraderName(const Poco::Nullable<std::string>& val)
{
	_traderName = val;
}


inline void CheckVatApproxResponse::setTraderNameMatch(const Poco::Optional<std::string>& val)
{
	_traderNameMatch = val;
}


inline void CheckVatApproxResponse::setTraderPostcode(const Poco::Optional<std::string>& val)
{
	_traderPostcode = val;
}


inline void CheckVatApproxResponse::setTraderPostcodeMatch(const Poco::Optional<std::string>& val)
{
	_traderPostcodeMatch = val;
}


inline void CheckVatApproxResponse::setTraderStreet(const Poco::Optional<std::string>& val)
{
	_traderStreet = val;
}


inline void CheckVatApproxResponse::setTraderStreetMatch(const Poco::Optional<std::string>& val)
{
	_traderStreetMatch = val;
}


inline void CheckVatApproxResponse::setValid(bool val)
{
	_valid = val;
}


inline void CheckVatApproxResponse::setVatNumber(const std::string& val)
{
	_vatNumber = val;
}


} } // CheckVat::Types


#endif // CheckVat_Types_CheckVatApproxResponse_INCLUDED
