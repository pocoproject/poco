// 
// SendReceiveSerialCommandResponse.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_DeviceIO_SendReceiveSerialCommandResponse_INCLUDED
#define ONVIF_DeviceIO_SendReceiveSerialCommandResponse_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace DeviceIO {
class SerialData;
} } 


namespace ONVIF {
namespace DeviceIO {


//@ name="#SendReceiveSerialCommandResponse"
//@ namespace="http://www.onvif.org/ver10/deviceIO/wsdl"
//@ serialize
class ONVIF_API SendReceiveSerialCommandResponse
{
public:
	SendReceiveSerialCommandResponse();

	SendReceiveSerialCommandResponse(const Poco::SharedPtr<SerialData>& serialData);

	virtual ~SendReceiveSerialCommandResponse();

	const Poco::SharedPtr<SerialData>& getSerialData() const;

	void setSerialData(const Poco::SharedPtr<SerialData>& val);

	void setSerialData(Poco::SharedPtr<SerialData>&& val);

private:
	//@ mandatory=false
	//@ name=SerialData
	//@ order=0
	Poco::SharedPtr<SerialData> _serialData;

};


inline const Poco::SharedPtr<SerialData>& SendReceiveSerialCommandResponse::getSerialData() const
{
	return _serialData;
}


inline void SendReceiveSerialCommandResponse::setSerialData(const Poco::SharedPtr<SerialData>& val)
{
	_serialData = val;
}


inline void SendReceiveSerialCommandResponse::setSerialData(Poco::SharedPtr<SerialData>&& val)
{
	_serialData = std::move(val);
}


} } // ONVIF::DeviceIO


// The following headers are required for template instantiation.
#include "ONVIF/DeviceIO/SerialData.h"


#endif // ONVIF_DeviceIO_SendReceiveSerialCommandResponse_INCLUDED
