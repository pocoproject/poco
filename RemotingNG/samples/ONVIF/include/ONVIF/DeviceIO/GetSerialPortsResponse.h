// 
// GetSerialPortsResponse.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_DeviceIO_GetSerialPortsResponse_INCLUDED
#define ONVIF_DeviceIO_GetSerialPortsResponse_INCLUDED


#include "ONVIF/DeviceIO/SerialPort.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace DeviceIO {


//@ name="#GetSerialPortsResponse"
//@ namespace="http://www.onvif.org/ver10/deviceIO/wsdl"
//@ serialize
class ONVIF_API GetSerialPortsResponse
{
public:
	GetSerialPortsResponse();

	GetSerialPortsResponse(const std::vector<SerialPort>& serialPort);

	virtual ~GetSerialPortsResponse();

	const std::vector<SerialPort>& getSerialPort() const;

	std::vector<SerialPort>& getSerialPort();

	void setSerialPort(const std::vector<SerialPort>& val);

	void setSerialPort(std::vector<SerialPort>&& val);

private:
	//@ mandatory=false
	//@ name=SerialPort
	//@ order=0
	std::vector<SerialPort> _serialPort;

};


inline const std::vector<SerialPort>& GetSerialPortsResponse::getSerialPort() const
{
	return _serialPort;
}


inline std::vector<SerialPort>& GetSerialPortsResponse::getSerialPort()
{
	return _serialPort;
}


inline void GetSerialPortsResponse::setSerialPort(const std::vector<SerialPort>& val)
{
	_serialPort = val;
}


inline void GetSerialPortsResponse::setSerialPort(std::vector<SerialPort>&& val)
{
	_serialPort = std::move(val);
}


} } // ONVIF::DeviceIO


#endif // ONVIF_DeviceIO_GetSerialPortsResponse_INCLUDED
