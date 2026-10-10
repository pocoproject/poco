// 
// CheckVatApproxResponse.cpp
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#include "CheckVat/Types/CheckVatApproxResponse.h"


namespace CheckVat {
namespace Types {


CheckVatApproxResponse::CheckVatApproxResponse():
	_countryCode(),
	_vatNumber(),
	_requestDate(),
	_valid(),
	_traderName(),
	_traderCompanyType(),
	_traderAddress(),
	_traderStreet(),
	_traderPostcode(),
	_traderCity(),
	_traderNameMatch(),
	_traderCompanyTypeMatch(),
	_traderStreetMatch(),
	_traderPostcodeMatch(),
	_traderCityMatch(),
	_requestIdentifier()
{
}


CheckVatApproxResponse::CheckVatApproxResponse(const std::string& countryCode, const std::string& vatNumber, const Poco::DateTime& requestDate, bool valid, const Poco::Nullable<std::string>& traderName, const Poco::Nullable<std::string>& traderCompanyType, const Poco::Optional<std::string>& traderAddress, const Poco::Optional<std::string>& traderStreet, const Poco::Optional<std::string>& traderPostcode, const Poco::Optional<std::string>& traderCity, const Poco::Optional<std::string>& traderNameMatch, const Poco::Optional<std::string>& traderCompanyTypeMatch, const Poco::Optional<std::string>& traderStreetMatch, const Poco::Optional<std::string>& traderPostcodeMatch, const Poco::Optional<std::string>& traderCityMatch, const std::string& requestIdentifier):
	_countryCode(countryCode),
	_vatNumber(vatNumber),
	_requestDate(requestDate),
	_valid(valid),
	_traderName(traderName),
	_traderCompanyType(traderCompanyType),
	_traderAddress(traderAddress),
	_traderStreet(traderStreet),
	_traderPostcode(traderPostcode),
	_traderCity(traderCity),
	_traderNameMatch(traderNameMatch),
	_traderCompanyTypeMatch(traderCompanyTypeMatch),
	_traderStreetMatch(traderStreetMatch),
	_traderPostcodeMatch(traderPostcodeMatch),
	_traderCityMatch(traderCityMatch),
	_requestIdentifier(requestIdentifier)
{
}


CheckVatApproxResponse::~CheckVatApproxResponse()
{
}


} } // CheckVat::Types


