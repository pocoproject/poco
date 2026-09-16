// 
// CheckVatApprox.cpp
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#include "CheckVat/Types/CheckVatApprox.h"


namespace CheckVat {
namespace Types {


CheckVatApprox::CheckVatApprox():
	_countryCode(),
	_vatNumber(),
	_traderName(),
	_traderCompanyType(),
	_traderStreet(),
	_traderPostcode(),
	_traderCity(),
	_requesterCountryCode(),
	_requesterVatNumber()
{
}


CheckVatApprox::CheckVatApprox(const std::string& countryCode, const std::string& vatNumber, const Poco::Optional<std::string>& traderName, const Poco::Optional<std::string>& traderCompanyType, const Poco::Optional<std::string>& traderStreet, const Poco::Optional<std::string>& traderPostcode, const Poco::Optional<std::string>& traderCity, const Poco::Optional<std::string>& requesterCountryCode, const Poco::Optional<std::string>& requesterVatNumber):
	_countryCode(countryCode),
	_vatNumber(vatNumber),
	_traderName(traderName),
	_traderCompanyType(traderCompanyType),
	_traderStreet(traderStreet),
	_traderPostcode(traderPostcode),
	_traderCity(traderCity),
	_requesterCountryCode(requesterCountryCode),
	_requesterVatNumber(requesterVatNumber)
{
}


CheckVatApprox::~CheckVatApprox()
{
}


} } // CheckVat::Types


