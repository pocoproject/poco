// 
// GetSerialPortConfigurationOptions.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_DeviceIO_GetSerialPortConfigurationOptions_INCLUDED
#define ONVIF_DeviceIO_GetSerialPortConfigurationOptions_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace DeviceIO {


//@ name="#GetSerialPortConfigurationOptions"
//@ namespace="http://www.onvif.org/ver10/deviceIO/wsdl"
//@ serialize
class ONVIF_API GetSerialPortConfigurationOptions
{
public:
	GetSerialPortConfigurationOptions();

	GetSerialPortConfigurationOptions(const std::string& serialPortToken);

	virtual ~GetSerialPortConfigurationOptions();

	const std::string& getSerialPortToken() const;

	void setSerialPortToken(const std::string& val);

	void setSerialPortToken(std::string&& val);

private:
	//@ name=SerialPortToken
	//@ order=0
	std::string _serialPortToken;

};


inline const std::string& GetSerialPortConfigurationOptions::getSerialPortToken() const
{
	return _serialPortToken;
}


inline void GetSerialPortConfigurationOptions::setSerialPortToken(const std::string& val)
{
	_serialPortToken = val;
}


inline void GetSerialPortConfigurationOptions::setSerialPortToken(std::string&& val)
{
	_serialPortToken = std::move(val);
}


} } // ONVIF::DeviceIO


#endif // ONVIF_DeviceIO_GetSerialPortConfigurationOptions_INCLUDED
