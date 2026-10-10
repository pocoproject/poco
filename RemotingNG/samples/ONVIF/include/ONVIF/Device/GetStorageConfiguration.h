// 
// GetStorageConfiguration.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Device_GetStorageConfiguration_INCLUDED
#define ONVIF_Device_GetStorageConfiguration_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Device {


//@ name="#GetStorageConfiguration"
//@ namespace="http://www.onvif.org/ver10/device/wsdl"
//@ serialize
class ONVIF_API GetStorageConfiguration
{
public:
	GetStorageConfiguration();

	GetStorageConfiguration(const std::string& token);

	virtual ~GetStorageConfiguration();

	const std::string& getToken() const;

	void setToken(const std::string& val);

	void setToken(std::string&& val);

private:
	//@ name=Token
	//@ order=0
	std::string _token;

};


inline const std::string& GetStorageConfiguration::getToken() const
{
	return _token;
}


inline void GetStorageConfiguration::setToken(const std::string& val)
{
	_token = val;
}


inline void GetStorageConfiguration::setToken(std::string&& val)
{
	_token = std::move(val);
}


} } // ONVIF::Device


#endif // ONVIF_Device_GetStorageConfiguration_INCLUDED
