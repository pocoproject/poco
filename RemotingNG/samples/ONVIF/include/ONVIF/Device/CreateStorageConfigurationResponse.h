// 
// CreateStorageConfigurationResponse.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Device_CreateStorageConfigurationResponse_INCLUDED
#define ONVIF_Device_CreateStorageConfigurationResponse_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Device {


//@ name="#CreateStorageConfigurationResponse"
//@ namespace="http://www.onvif.org/ver10/device/wsdl"
//@ serialize
class ONVIF_API CreateStorageConfigurationResponse
{
public:
	CreateStorageConfigurationResponse();

	CreateStorageConfigurationResponse(const std::string& token);

	virtual ~CreateStorageConfigurationResponse();

	const std::string& getToken() const;

	void setToken(const std::string& val);

	void setToken(std::string&& val);

private:
	//@ name=Token
	//@ order=0
	std::string _token;

};


inline const std::string& CreateStorageConfigurationResponse::getToken() const
{
	return _token;
}


inline void CreateStorageConfigurationResponse::setToken(const std::string& val)
{
	_token = val;
}


inline void CreateStorageConfigurationResponse::setToken(std::string&& val)
{
	_token = std::move(val);
}


} } // ONVIF::Device


#endif // ONVIF_Device_CreateStorageConfigurationResponse_INCLUDED
