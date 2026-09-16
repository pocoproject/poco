// 
// CheckVat.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef CheckVat_Types_CheckVat_INCLUDED
#define CheckVat_Types_CheckVat_INCLUDED


#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>


namespace CheckVat {
namespace Types {


//@ name="#checkVat"
//@ namespace="urn:ec.europa.eu:taxud:vies:services:checkVat:types"
//@ serialize
class CheckVat
{
public:
	CheckVat();

	CheckVat(
		const std::string& countryCode, 
		const std::string& vatNumber);

	virtual ~CheckVat();

	const std::string& getCountryCode() const;

	const std::string& getVatNumber() const;

	void setCountryCode(const std::string& val);

	void setVatNumber(const std::string& val);

private:
	//@ name=countryCode
	//@ order=0
	std::string _countryCode;

	//@ name=vatNumber
	//@ order=1
	std::string _vatNumber;

};


inline const std::string& CheckVat::getCountryCode() const
{
	return _countryCode;
}


inline const std::string& CheckVat::getVatNumber() const
{
	return _vatNumber;
}


inline void CheckVat::setCountryCode(const std::string& val)
{
	_countryCode = val;
}


inline void CheckVat::setVatNumber(const std::string& val)
{
	_vatNumber = val;
}


} } // CheckVat::Types


#endif // CheckVat_Types_CheckVat_INCLUDED
