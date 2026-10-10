// 
// DeleteStorageConfiguration.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Device_DeleteStorageConfiguration_INCLUDED
#define ONVIF_Device_DeleteStorageConfiguration_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Device {


//@ name="#DeleteStorageConfiguration"
//@ namespace="http://www.onvif.org/ver10/device/wsdl"
//@ serialize
class ONVIF_API DeleteStorageConfiguration
{
public:
	DeleteStorageConfiguration();

	DeleteStorageConfiguration(const std::string& token);

	virtual ~DeleteStorageConfiguration();

	const std::string& getToken() const;

	void setToken(const std::string& val);

	void setToken(std::string&& val);

private:
	//@ name=Token
	//@ order=0
	std::string _token;

};


inline const std::string& DeleteStorageConfiguration::getToken() const
{
	return _token;
}


inline void DeleteStorageConfiguration::setToken(const std::string& val)
{
	_token = val;
}


inline void DeleteStorageConfiguration::setToken(std::string&& val)
{
	_token = std::move(val);
}


} } // ONVIF::Device


#endif // ONVIF_Device_DeleteStorageConfiguration_INCLUDED
