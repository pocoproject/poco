// 
// SerialPort.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_DeviceIO_SerialPort_INCLUDED
#define ONVIF_DeviceIO_SerialPort_INCLUDED


#include "ONVIF/DeviceEntity.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace DeviceIO {


//@ name=SerialPort
//@ namespace="http://www.onvif.org/ver10/deviceIO/wsdl"
//@ serialize
class ONVIF_API SerialPort: public ONVIF::DeviceEntity
{
public:
	SerialPort();

	SerialPort(const std::string& token);

	virtual ~SerialPort();

};


} } // ONVIF::DeviceIO


#endif // ONVIF_DeviceIO_SerialPort_INCLUDED
