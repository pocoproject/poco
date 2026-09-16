// 
// GetZeroConfigurationResponse.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Device_GetZeroConfigurationResponse_INCLUDED
#define ONVIF_Device_GetZeroConfigurationResponse_INCLUDED


#include "ONVIF/NetworkZeroConfiguration.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Device {


//@ name="#GetZeroConfigurationResponse"
//@ namespace="http://www.onvif.org/ver10/device/wsdl"
//@ serialize
class ONVIF_API GetZeroConfigurationResponse
{
public:
	GetZeroConfigurationResponse();

	GetZeroConfigurationResponse(const ONVIF::NetworkZeroConfiguration& zeroConfiguration);

	virtual ~GetZeroConfigurationResponse();

	const ONVIF::NetworkZeroConfiguration& getZeroConfiguration() const;

	void setZeroConfiguration(const ONVIF::NetworkZeroConfiguration& val);

	void setZeroConfiguration(ONVIF::NetworkZeroConfiguration&& val);

private:
	//@ name=ZeroConfiguration
	//@ order=0
	ONVIF::NetworkZeroConfiguration _zeroConfiguration;

};


inline const ONVIF::NetworkZeroConfiguration& GetZeroConfigurationResponse::getZeroConfiguration() const
{
	return _zeroConfiguration;
}


inline void GetZeroConfigurationResponse::setZeroConfiguration(const ONVIF::NetworkZeroConfiguration& val)
{
	_zeroConfiguration = val;
}


inline void GetZeroConfigurationResponse::setZeroConfiguration(ONVIF::NetworkZeroConfiguration&& val)
{
	_zeroConfiguration = std::move(val);
}


} } // ONVIF::Device


#endif // ONVIF_Device_GetZeroConfigurationResponse_INCLUDED
