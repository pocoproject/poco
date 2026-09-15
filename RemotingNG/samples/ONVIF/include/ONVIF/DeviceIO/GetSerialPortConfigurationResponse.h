// 
// GetSerialPortConfigurationResponse.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_DeviceIO_GetSerialPortConfigurationResponse_INCLUDED
#define ONVIF_DeviceIO_GetSerialPortConfigurationResponse_INCLUDED


#include "ONVIF/DeviceIO/SerialPortConfiguration.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace DeviceIO {


//@ name="#GetSerialPortConfigurationResponse"
//@ namespace="http://www.onvif.org/ver10/deviceIO/wsdl"
//@ serialize
class ONVIF_API GetSerialPortConfigurationResponse
{
public:
	GetSerialPortConfigurationResponse();

	GetSerialPortConfigurationResponse(const SerialPortConfiguration& serialPortConfiguration);

	virtual ~GetSerialPortConfigurationResponse();

	const SerialPortConfiguration& getSerialPortConfiguration() const;

	void setSerialPortConfiguration(const SerialPortConfiguration& val);

	void setSerialPortConfiguration(SerialPortConfiguration&& val);

private:
	//@ name=SerialPortConfiguration
	//@ order=0
	SerialPortConfiguration _serialPortConfiguration;

};


inline const SerialPortConfiguration& GetSerialPortConfigurationResponse::getSerialPortConfiguration() const
{
	return _serialPortConfiguration;
}


inline void GetSerialPortConfigurationResponse::setSerialPortConfiguration(const SerialPortConfiguration& val)
{
	_serialPortConfiguration = val;
}


inline void GetSerialPortConfigurationResponse::setSerialPortConfiguration(SerialPortConfiguration&& val)
{
	_serialPortConfiguration = std::move(val);
}


} } // ONVIF::DeviceIO


#endif // ONVIF_DeviceIO_GetSerialPortConfigurationResponse_INCLUDED
