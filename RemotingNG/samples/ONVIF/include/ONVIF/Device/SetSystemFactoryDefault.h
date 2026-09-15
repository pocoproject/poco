// 
// SetSystemFactoryDefault.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Device_SetSystemFactoryDefault_INCLUDED
#define ONVIF_Device_SetSystemFactoryDefault_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Device {


//@ name="#SetSystemFactoryDefault"
//@ namespace="http://www.onvif.org/ver10/device/wsdl"
//@ serialize
class ONVIF_API SetSystemFactoryDefault
{
public:
	SetSystemFactoryDefault();

	SetSystemFactoryDefault(const std::string& factoryDefault);

	virtual ~SetSystemFactoryDefault();

	const std::string& getFactoryDefault() const;

	void setFactoryDefault(const std::string& val);

	void setFactoryDefault(std::string&& val);

private:
	//@ name=FactoryDefault
	//@ order=0
	std::string _factoryDefault;

};


inline const std::string& SetSystemFactoryDefault::getFactoryDefault() const
{
	return _factoryDefault;
}


inline void SetSystemFactoryDefault::setFactoryDefault(const std::string& val)
{
	_factoryDefault = val;
}


inline void SetSystemFactoryDefault::setFactoryDefault(std::string&& val)
{
	_factoryDefault = std::move(val);
}


} } // ONVIF::Device


#endif // ONVIF_Device_SetSystemFactoryDefault_INCLUDED
