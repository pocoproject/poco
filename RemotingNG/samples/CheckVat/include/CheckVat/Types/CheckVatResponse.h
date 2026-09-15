// 
// CheckVatResponse.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef CheckVat_Types_CheckVatResponse_INCLUDED
#define CheckVat_Types_CheckVatResponse_INCLUDED


#include "Poco/DateTime.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>


namespace CheckVat {
namespace Types {


//@ name="#checkVatResponse"
//@ namespace="urn:ec.europa.eu:taxud:vies:services:checkVat:types"
//@ serialize
class CheckVatResponse
{
public:
	CheckVatResponse();

	CheckVatResponse(
		const std::string& countryCode, 
		const std::string& vatNumber, 
		const Poco::DateTime& requestDate, 
		bool valid, 
		const Poco::Nullable<std::string>& name, 
		const Poco::Nullable<std::string>& address);

	virtual ~CheckVatResponse();

	const Poco::Nullable<std::string>& getAddress() const;

	const std::string& getCountryCode() const;

	const Poco::Nullable<std::string>& getName() const;

	//@ return={xsdType=date}
	const Poco::DateTime& getRequestDate() const;

	bool getValid() const;

	const std::string& getVatNumber() const;

	void setAddress(const Poco::Nullable<std::string>& val);

	void setCountryCode(const std::string& val);

	void setName(const Poco::Nullable<std::string>& val);

	//@ $val={xsdType=date}
	void setRequestDate(const Poco::DateTime& val);

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
	//@ name=name
	//@ order=4
	Poco::Nullable<std::string> _name;

	//@ mandatory=false
	//@ name=address
	//@ order=5
	Poco::Nullable<std::string> _address;

};


inline const Poco::Nullable<std::string>& CheckVatResponse::getAddress() const
{
	return _address;
}


inline const std::string& CheckVatResponse::getCountryCode() const
{
	return _countryCode;
}


inline const Poco::Nullable<std::string>& CheckVatResponse::getName() const
{
	return _name;
}


inline const Poco::DateTime& CheckVatResponse::getRequestDate() const
{
	return _requestDate;
}


inline bool CheckVatResponse::getValid() const
{
	return _valid;
}


inline const std::string& CheckVatResponse::getVatNumber() const
{
	return _vatNumber;
}


inline void CheckVatResponse::setAddress(const Poco::Nullable<std::string>& val)
{
	_address = val;
}


inline void CheckVatResponse::setCountryCode(const std::string& val)
{
	_countryCode = val;
}


inline void CheckVatResponse::setName(const Poco::Nullable<std::string>& val)
{
	_name = val;
}


inline void CheckVatResponse::setRequestDate(const Poco::DateTime& val)
{
	_requestDate = val;
}


inline void CheckVatResponse::setValid(bool val)
{
	_valid = val;
}


inline void CheckVatResponse::setVatNumber(const std::string& val)
{
	_vatNumber = val;
}


} } // CheckVat::Types


#endif // CheckVat_Types_CheckVatResponse_INCLUDED
