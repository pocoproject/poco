// 
// ConvertTemperature.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef WebServiceX_ConvertTemperature_INCLUDED
#define WebServiceX_ConvertTemperature_INCLUDED


#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>


namespace WebServiceX {


//@ name=ConvertTemperature
//@ namespace="http://www.webserviceX.NET/"
//@ remote
class ConvertTemperature
{
public:
	virtual ~ConvertTemperature();

	//@ action="http://www.webserviceX.NET/ConvertTemp"
	//@ name=ConvertTemp
	//@ replyName=ConvertTempResponse
	//@ $temperature={direction=in, name=Temperature}
	//@ $fromUnit={direction=in, name=FromUnit}
	//@ $toUnit={direction=in, name=ToUnit}
	//@ $convertTempResult={direction=out, name=ConvertTempResult}
	virtual void convertTemp(
		double temperature, 
		const std::string& fromUnit, 
		const std::string& toUnit, 
		double& convertTempResult) = 0;
};


} // WebServiceX


#endif // WebServiceX_ConvertTemperature_INCLUDED
