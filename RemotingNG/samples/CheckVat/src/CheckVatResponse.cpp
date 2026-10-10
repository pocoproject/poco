// 
// CheckVatResponse.cpp
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#include "CheckVat/Types/CheckVatResponse.h"


namespace CheckVat {
namespace Types {


CheckVatResponse::CheckVatResponse():
	_countryCode(),
	_vatNumber(),
	_requestDate(),
	_valid(),
	_name(),
	_address()
{
}


CheckVatResponse::CheckVatResponse(const std::string& countryCode, const std::string& vatNumber, const Poco::DateTime& requestDate, bool valid, const Poco::Nullable<std::string>& name, const Poco::Nullable<std::string>& address):
	_countryCode(countryCode),
	_vatNumber(vatNumber),
	_requestDate(requestDate),
	_valid(valid),
	_name(name),
	_address(address)
{
}


CheckVatResponse::~CheckVatResponse()
{
}


} } // CheckVat::Types


