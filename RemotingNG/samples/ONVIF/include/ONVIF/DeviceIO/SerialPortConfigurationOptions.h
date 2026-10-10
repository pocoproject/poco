// 
// SerialPortConfigurationOptions.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_DeviceIO_SerialPortConfigurationOptions_INCLUDED
#define ONVIF_DeviceIO_SerialPortConfigurationOptions_INCLUDED


#include "ONVIF/DeviceIO/ParityBitList.h"
#include "ONVIF/FloatItems.h"
#include "ONVIF/IntItems.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace DeviceIO {


//@ name=SerialPortConfigurationOptions
//@ namespace="http://www.onvif.org/ver10/deviceIO/wsdl"
//@ serialize
class ONVIF_API SerialPortConfigurationOptions
{
public:
	SerialPortConfigurationOptions();

	SerialPortConfigurationOptions(
		const std::string& token, 
		const ONVIF::IntItems& baudRateList, 
		const ParityBitList& parityBitList, 
		const ONVIF::IntItems& characterLengthList, 
		const ONVIF::FloatItems& stopBitList);

	virtual ~SerialPortConfigurationOptions();

	const ONVIF::IntItems& getBaudRateList() const;

	const ONVIF::IntItems& getCharacterLengthList() const;

	const ParityBitList& getParityBitList() const;

	const ONVIF::FloatItems& getStopBitList() const;

	const std::string& getToken() const;

	void setBaudRateList(const ONVIF::IntItems& val);

	void setBaudRateList(ONVIF::IntItems&& val);

	void setCharacterLengthList(const ONVIF::IntItems& val);

	void setCharacterLengthList(ONVIF::IntItems&& val);

	void setParityBitList(const ParityBitList& val);

	void setParityBitList(ParityBitList&& val);

	void setStopBitList(const ONVIF::FloatItems& val);

	void setStopBitList(ONVIF::FloatItems&& val);

	void setToken(const std::string& val);

	void setToken(std::string&& val);

private:
	//@ name=token
	//@ order=0
	//@ type=attr
	std::string _token;

	//@ name=BaudRateList
	//@ order=1
	ONVIF::IntItems _baudRateList;

	//@ name=ParityBitList
	//@ order=2
	ParityBitList _parityBitList;

	//@ name=CharacterLengthList
	//@ order=3
	ONVIF::IntItems _characterLengthList;

	//@ name=StopBitList
	//@ order=4
	ONVIF::FloatItems _stopBitList;

};


inline const ONVIF::IntItems& SerialPortConfigurationOptions::getBaudRateList() const
{
	return _baudRateList;
}


inline const ONVIF::IntItems& SerialPortConfigurationOptions::getCharacterLengthList() const
{
	return _characterLengthList;
}


inline const ParityBitList& SerialPortConfigurationOptions::getParityBitList() const
{
	return _parityBitList;
}


inline const ONVIF::FloatItems& SerialPortConfigurationOptions::getStopBitList() const
{
	return _stopBitList;
}


inline const std::string& SerialPortConfigurationOptions::getToken() const
{
	return _token;
}


inline void SerialPortConfigurationOptions::setBaudRateList(const ONVIF::IntItems& val)
{
	_baudRateList = val;
}


inline void SerialPortConfigurationOptions::setBaudRateList(ONVIF::IntItems&& val)
{
	_baudRateList = std::move(val);
}


inline void SerialPortConfigurationOptions::setCharacterLengthList(const ONVIF::IntItems& val)
{
	_characterLengthList = val;
}


inline void SerialPortConfigurationOptions::setCharacterLengthList(ONVIF::IntItems&& val)
{
	_characterLengthList = std::move(val);
}


inline void SerialPortConfigurationOptions::setParityBitList(const ParityBitList& val)
{
	_parityBitList = val;
}


inline void SerialPortConfigurationOptions::setParityBitList(ParityBitList&& val)
{
	_parityBitList = std::move(val);
}


inline void SerialPortConfigurationOptions::setStopBitList(const ONVIF::FloatItems& val)
{
	_stopBitList = val;
}


inline void SerialPortConfigurationOptions::setStopBitList(ONVIF::FloatItems&& val)
{
	_stopBitList = std::move(val);
}


inline void SerialPortConfigurationOptions::setToken(const std::string& val)
{
	_token = val;
}


inline void SerialPortConfigurationOptions::setToken(std::string&& val)
{
	_token = std::move(val);
}


} } // ONVIF::DeviceIO


#endif // ONVIF_DeviceIO_SerialPortConfigurationOptions_INCLUDED
