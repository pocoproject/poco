// 
// GetSerialPortConfigurationOptionsResponse.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_DeviceIO_GetSerialPortConfigurationOptionsResponse_INCLUDED
#define ONVIF_DeviceIO_GetSerialPortConfigurationOptionsResponse_INCLUDED


#include "ONVIF/DeviceIO/SerialPortConfigurationOptions.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace DeviceIO {


//@ name="#GetSerialPortConfigurationOptionsResponse"
//@ namespace="http://www.onvif.org/ver10/deviceIO/wsdl"
//@ serialize
class ONVIF_API GetSerialPortConfigurationOptionsResponse
{
public:
	GetSerialPortConfigurationOptionsResponse();

	GetSerialPortConfigurationOptionsResponse(const SerialPortConfigurationOptions& serialPortOptions);

	virtual ~GetSerialPortConfigurationOptionsResponse();

	const SerialPortConfigurationOptions& getSerialPortOptions() const;

	void setSerialPortOptions(const SerialPortConfigurationOptions& val);

	void setSerialPortOptions(SerialPortConfigurationOptions&& val);

private:
	//@ name=SerialPortOptions
	//@ order=0
	SerialPortConfigurationOptions _serialPortOptions;

};


inline const SerialPortConfigurationOptions& GetSerialPortConfigurationOptionsResponse::getSerialPortOptions() const
{
	return _serialPortOptions;
}


inline void GetSerialPortConfigurationOptionsResponse::setSerialPortOptions(const SerialPortConfigurationOptions& val)
{
	_serialPortOptions = val;
}


inline void GetSerialPortConfigurationOptionsResponse::setSerialPortOptions(SerialPortConfigurationOptions&& val)
{
	_serialPortOptions = std::move(val);
}


} } // ONVIF::DeviceIO


#endif // ONVIF_DeviceIO_GetSerialPortConfigurationOptionsResponse_INCLUDED
