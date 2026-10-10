// 
// ConvertTemp.cpp
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#include "WebServiceX/ConvertTemp.h"


namespace WebServiceX {


ConvertTemp::ConvertTemp():
	_temperature(),
	_fromUnit(),
	_toUnit()
{
}


ConvertTemp::ConvertTemp(double temperature, const std::string& fromUnit, const std::string& toUnit):
	_temperature(temperature),
	_fromUnit(fromUnit),
	_toUnit(toUnit)
{
}


ConvertTemp::~ConvertTemp()
{
}


} // WebServiceX


