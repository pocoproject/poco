// 
// SendReceiveSerialCommand.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_DeviceIO_SendReceiveSerialCommand_INCLUDED
#define ONVIF_DeviceIO_SendReceiveSerialCommand_INCLUDED


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


//@ name="#SendReceiveSerialCommand"
//@ namespace="http://www.onvif.org/ver10/deviceIO/wsdl"
//@ serialize
class ONVIF_API SendReceiveSerialCommand
{
public:
	SendReceiveSerialCommand();

	SendReceiveSerialCommand(
		const Poco::Optional<std::string>& token, 
		const Poco::SharedPtr<SerialData>& serialData, 
		const Poco::Optional<std::string>& timeOut, 
		Poco::Optional<int> dataLength, 
		const Poco::Optional<std::string>& delimiter);

	virtual ~SendReceiveSerialCommand();

	Poco::Optional<int> getDataLength() const;

	const Poco::Optional<std::string>& getDelimiter() const;

	const Poco::SharedPtr<SerialData>& getSerialData() const;

	const Poco::Optional<std::string>& getTimeOut() const;

	const Poco::Optional<std::string>& getToken() const;

	void setDataLength(Poco::Optional<int> val);

	void setDelimiter(const Poco::Optional<std::string>& val);

	void setDelimiter(Poco::Optional<std::string>&& val);

	void setSerialData(const Poco::SharedPtr<SerialData>& val);

	void setSerialData(Poco::SharedPtr<SerialData>&& val);

	void setTimeOut(const Poco::Optional<std::string>& val);

	void setTimeOut(Poco::Optional<std::string>&& val);

	void setToken(const Poco::Optional<std::string>& val);

	void setToken(Poco::Optional<std::string>&& val);

private:
	//@ mandatory=false
	//@ name=Token
	//@ order=0
	Poco::Optional<std::string> _token;

	//@ mandatory=false
	//@ name=SerialData
	//@ order=1
	Poco::SharedPtr<SerialData> _serialData;

	//@ mandatory=false
	//@ name=TimeOut
	//@ order=2
	Poco::Optional<std::string> _timeOut;

	//@ mandatory=false
	//@ name=DataLength
	//@ order=3
	Poco::Optional<int> _dataLength;

	//@ mandatory=false
	//@ name=Delimiter
	//@ order=4
	Poco::Optional<std::string> _delimiter;

};


inline Poco::Optional<int> SendReceiveSerialCommand::getDataLength() const
{
	return _dataLength;
}


inline const Poco::Optional<std::string>& SendReceiveSerialCommand::getDelimiter() const
{
	return _delimiter;
}


inline const Poco::SharedPtr<SerialData>& SendReceiveSerialCommand::getSerialData() const
{
	return _serialData;
}


inline const Poco::Optional<std::string>& SendReceiveSerialCommand::getTimeOut() const
{
	return _timeOut;
}


inline const Poco::Optional<std::string>& SendReceiveSerialCommand::getToken() const
{
	return _token;
}


inline void SendReceiveSerialCommand::setDataLength(Poco::Optional<int> val)
{
	_dataLength = val;
}


inline void SendReceiveSerialCommand::setDelimiter(const Poco::Optional<std::string>& val)
{
	_delimiter = val;
}


inline void SendReceiveSerialCommand::setDelimiter(Poco::Optional<std::string>&& val)
{
	_delimiter = std::move(val);
}


inline void SendReceiveSerialCommand::setSerialData(const Poco::SharedPtr<SerialData>& val)
{
	_serialData = val;
}


inline void SendReceiveSerialCommand::setSerialData(Poco::SharedPtr<SerialData>&& val)
{
	_serialData = std::move(val);
}


inline void SendReceiveSerialCommand::setTimeOut(const Poco::Optional<std::string>& val)
{
	_timeOut = val;
}


inline void SendReceiveSerialCommand::setTimeOut(Poco::Optional<std::string>&& val)
{
	_timeOut = std::move(val);
}


inline void SendReceiveSerialCommand::setToken(const Poco::Optional<std::string>& val)
{
	_token = val;
}


inline void SendReceiveSerialCommand::setToken(Poco::Optional<std::string>&& val)
{
	_token = std::move(val);
}


} } // ONVIF::DeviceIO


// The following headers are required for template instantiation.
#include "ONVIF/DeviceIO/SerialData.h"


#endif // ONVIF_DeviceIO_SendReceiveSerialCommand_INCLUDED
