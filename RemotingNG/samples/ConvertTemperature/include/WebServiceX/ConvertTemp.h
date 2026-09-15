// 
// ConvertTemp.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef WebServiceX_ConvertTemp_INCLUDED
#define WebServiceX_ConvertTemp_INCLUDED


#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>


namespace WebServiceX {


//@ name="#ConvertTemp"
//@ namespace="http://www.webserviceX.NET/"
//@ serialize
class ConvertTemp
{
public:
	ConvertTemp();

	ConvertTemp(
		double temperature, 
		const std::string& fromUnit, 
		const std::string& toUnit);

	virtual ~ConvertTemp();

	const std::string& getFromUnit() const;

	double getTemperature() const;

	const std::string& getToUnit() const;

	void setFromUnit(const std::string& val);

	void setTemperature(double val);

	void setToUnit(const std::string& val);

private:
	//@ name=Temperature
	//@ order=0
	double _temperature;

	//@ name=FromUnit
	//@ order=1
	std::string _fromUnit;

	//@ name=ToUnit
	//@ order=2
	std::string _toUnit;

};


inline const std::string& ConvertTemp::getFromUnit() const
{
	return _fromUnit;
}


inline double ConvertTemp::getTemperature() const
{
	return _temperature;
}


inline const std::string& ConvertTemp::getToUnit() const
{
	return _toUnit;
}


inline void ConvertTemp::setFromUnit(const std::string& val)
{
	_fromUnit = val;
}


inline void ConvertTemp::setTemperature(double val)
{
	_temperature = val;
}


inline void ConvertTemp::setToUnit(const std::string& val)
{
	_toUnit = val;
}


} // WebServiceX


#endif // WebServiceX_ConvertTemp_INCLUDED
