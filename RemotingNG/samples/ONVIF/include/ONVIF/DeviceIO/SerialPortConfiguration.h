// 
// SerialPortConfiguration.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_DeviceIO_SerialPortConfiguration_INCLUDED
#define ONVIF_DeviceIO_SerialPortConfiguration_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace DeviceIO {


//@ name=SerialPortConfiguration
//@ namespace="http://www.onvif.org/ver10/deviceIO/wsdl"
//@ serialize
class ONVIF_API SerialPortConfiguration
{
public:
	SerialPortConfiguration();

	SerialPortConfiguration(
		const std::string& token, 
		const std::string& type, 
		int baudRate, 
		const std::string& parityBit, 
		int characterLength, 
		float stopBit);

	virtual ~SerialPortConfiguration();

	int getBaudRate() const;

	int getCharacterLength() const;

	const std::string& getParityBit() const;

	float getStopBit() const;

	const std::string& getToken() const;

	const std::string& getType() const;

	void setBaudRate(int val);

	void setCharacterLength(int val);

	void setParityBit(const std::string& val);

	void setParityBit(std::string&& val);

	void setStopBit(float val);

	void setToken(const std::string& val);

	void setToken(std::string&& val);

	void setType(const std::string& val);

	void setType(std::string&& val);

private:
	//@ name=token
	//@ order=0
	//@ type=attr
	std::string _token;

	//@ name=type
	//@ order=1
	//@ type=attr
	std::string _type;

	//@ name=BaudRate
	//@ order=2
	int _baudRate;

	//@ name=ParityBit
	//@ order=3
	std::string _parityBit;

	//@ name=CharacterLength
	//@ order=4
	int _characterLength;

	//@ name=StopBit
	//@ order=5
	float _stopBit;

};


inline int SerialPortConfiguration::getBaudRate() const
{
	return _baudRate;
}


inline int SerialPortConfiguration::getCharacterLength() const
{
	return _characterLength;
}


inline const std::string& SerialPortConfiguration::getParityBit() const
{
	return _parityBit;
}


inline float SerialPortConfiguration::getStopBit() const
{
	return _stopBit;
}


inline const std::string& SerialPortConfiguration::getToken() const
{
	return _token;
}


inline const std::string& SerialPortConfiguration::getType() const
{
	return _type;
}


inline void SerialPortConfiguration::setBaudRate(int val)
{
	_baudRate = val;
}


inline void SerialPortConfiguration::setCharacterLength(int val)
{
	_characterLength = val;
}


inline void SerialPortConfiguration::setParityBit(const std::string& val)
{
	_parityBit = val;
}


inline void SerialPortConfiguration::setParityBit(std::string&& val)
{
	_parityBit = std::move(val);
}


inline void SerialPortConfiguration::setStopBit(float val)
{
	_stopBit = val;
}


inline void SerialPortConfiguration::setToken(const std::string& val)
{
	_token = val;
}


inline void SerialPortConfiguration::setToken(std::string&& val)
{
	_token = std::move(val);
}


inline void SerialPortConfiguration::setType(const std::string& val)
{
	_type = val;
}


inline void SerialPortConfiguration::setType(std::string&& val)
{
	_type = std::move(val);
}


} } // ONVIF::DeviceIO


#endif // ONVIF_DeviceIO_SerialPortConfiguration_INCLUDED
