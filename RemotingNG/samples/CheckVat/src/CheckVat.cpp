// 
// CheckVat.cpp
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#include "CheckVat/Types/CheckVat.h"


namespace CheckVat {
namespace Types {


CheckVat::CheckVat():
	_countryCode(),
	_vatNumber()
{
}


CheckVat::CheckVat(const std::string& countryCode, const std::string& vatNumber):
	_countryCode(countryCode),
	_vatNumber(vatNumber)
{
}


CheckVat::~CheckVat()
{
}


} } // CheckVat::Types


