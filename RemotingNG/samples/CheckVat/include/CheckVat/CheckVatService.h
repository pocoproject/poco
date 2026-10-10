// 
// CheckVatService.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef CheckVat_CheckVatService_INCLUDED
#define CheckVat_CheckVatService_INCLUDED


#include "Poco/DateTime.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>


namespace CheckVat {


//@ name=checkVatService
//@ namespace="urn:ec.europa.eu:taxud:vies:services:checkVat"
//@ remote
class CheckVatService
{
public:
	virtual ~CheckVatService();

	//@ action=""
	//@ name=checkVat
	//@ namespace="urn:ec.europa.eu:taxud:vies:services:checkVat:types"
	//@ replyName=checkVatResponse
	//@ $countryCode={direction=in, name=countryCode}
	//@ $vatNumber={direction=in, name=vatNumber}
	//@ $countryCode2={direction=out, name=countryCode}
	//@ $vatNumber2={direction=out, name=vatNumber}
	//@ $requestDate={direction=out, name=requestDate, xsdType=date}
	//@ $valid={direction=out, name=valid}
	//@ $name={direction=out, name=name}
	//@ $address={direction=out, name=address}
	virtual void checkVat(
		const std::string& countryCode, 
		const std::string& vatNumber, 
		std::string& countryCode2, 
		std::string& vatNumber2, 
		Poco::DateTime& requestDate, 
		bool& valid, 
		Poco::Nullable<std::string>& name, 
		Poco::Nullable<std::string>& address) = 0;

	//@ action=""
	//@ name=checkVatApprox
	//@ namespace="urn:ec.europa.eu:taxud:vies:services:checkVat:types"
	//@ replyName=checkVatApproxResponse
	//@ $countryCode={direction=in, name=countryCode}
	//@ $vatNumber={direction=in, name=vatNumber}
	//@ $traderName={direction=in, name=traderName}
	//@ $traderCompanyType={direction=in, name=traderCompanyType}
	//@ $traderStreet={direction=in, name=traderStreet}
	//@ $traderPostcode={direction=in, name=traderPostcode}
	//@ $traderCity={direction=in, name=traderCity}
	//@ $requesterCountryCode={direction=in, name=requesterCountryCode}
	//@ $requesterVatNumber={direction=in, name=requesterVatNumber}
	//@ $countryCode2={direction=out, name=countryCode}
	//@ $vatNumber2={direction=out, name=vatNumber}
	//@ $requestDate={direction=out, name=requestDate, xsdType=date}
	//@ $valid={direction=out, name=valid}
	//@ $traderName2={direction=out, name=traderName}
	//@ $traderCompanyType2={direction=out, name=traderCompanyType}
	//@ $traderAddress={direction=out, name=traderAddress}
	//@ $traderStreet2={direction=out, name=traderStreet}
	//@ $traderPostcode2={direction=out, name=traderPostcode}
	//@ $traderCity2={direction=out, name=traderCity}
	//@ $traderNameMatch={direction=out, name=traderNameMatch}
	//@ $traderCompanyTypeMatch={direction=out, name=traderCompanyTypeMatch}
	//@ $traderStreetMatch={direction=out, name=traderStreetMatch}
	//@ $traderPostcodeMatch={direction=out, name=traderPostcodeMatch}
	//@ $traderCityMatch={direction=out, name=traderCityMatch}
	//@ $requestIdentifier={direction=out, name=requestIdentifier}
	virtual void checkVatApprox(
		const std::string& countryCode, 
		const std::string& vatNumber, 
		const Poco::Optional<std::string>& traderName, 
		const Poco::Optional<std::string>& traderCompanyType, 
		const Poco::Optional<std::string>& traderStreet, 
		const Poco::Optional<std::string>& traderPostcode, 
		const Poco::Optional<std::string>& traderCity, 
		const Poco::Optional<std::string>& requesterCountryCode, 
		const Poco::Optional<std::string>& requesterVatNumber, 
		std::string& countryCode2, 
		std::string& vatNumber2, 
		Poco::DateTime& requestDate, 
		bool& valid, 
		Poco::Nullable<std::string>& traderName2, 
		Poco::Nullable<std::string>& traderCompanyType2, 
		Poco::Optional<std::string>& traderAddress, 
		Poco::Optional<std::string>& traderStreet2, 
		Poco::Optional<std::string>& traderPostcode2, 
		Poco::Optional<std::string>& traderCity2, 
		Poco::Optional<std::string>& traderNameMatch, 
		Poco::Optional<std::string>& traderCompanyTypeMatch, 
		Poco::Optional<std::string>& traderStreetMatch, 
		Poco::Optional<std::string>& traderPostcodeMatch, 
		Poco::Optional<std::string>& traderCityMatch, 
		std::string& requestIdentifier) = 0;
};


} // CheckVat


#endif // CheckVat_CheckVatService_INCLUDED
