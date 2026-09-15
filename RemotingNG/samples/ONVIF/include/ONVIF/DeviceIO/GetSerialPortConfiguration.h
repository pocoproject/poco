// 
// GetSerialPortConfiguration.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_DeviceIO_GetSerialPortConfiguration_INCLUDED
#define ONVIF_DeviceIO_GetSerialPortConfiguration_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace DeviceIO {


//@ name="#GetSerialPortConfiguration"
//@ namespace="http://www.onvif.org/ver10/deviceIO/wsdl"
//@ serialize
class ONVIF_API GetSerialPortConfiguration
{
public:
	GetSerialPortConfiguration();

	GetSerialPortConfiguration(const std::string& serialPortToken);

	virtual ~GetSerialPortConfiguration();

	const std::string& getSerialPortToken() const;

	void setSerialPortToken(const std::string& val);

	void setSerialPortToken(std::string&& val);

private:
	//@ name=SerialPortToken
	//@ order=0
	std::string _serialPortToken;

};


inline const std::string& GetSerialPortConfiguration::getSerialPortToken() const
{
	return _serialPortToken;
}


inline void GetSerialPortConfiguration::setSerialPortToken(const std::string& val)
{
	_serialPortToken = val;
}


inline void GetSerialPortConfiguration::setSerialPortToken(std::string&& val)
{
	_serialPortToken = std::move(val);
}


} } // ONVIF::DeviceIO


#endif // ONVIF_DeviceIO_GetSerialPortConfiguration_INCLUDED
