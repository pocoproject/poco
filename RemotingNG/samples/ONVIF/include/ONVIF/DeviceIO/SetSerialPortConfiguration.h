// 
// SetSerialPortConfiguration.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_DeviceIO_SetSerialPortConfiguration_INCLUDED
#define ONVIF_DeviceIO_SetSerialPortConfiguration_INCLUDED


#include "ONVIF/DeviceIO/SerialPortConfiguration.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace DeviceIO {


//@ name="#SetSerialPortConfiguration"
//@ namespace="http://www.onvif.org/ver10/deviceIO/wsdl"
//@ serialize
class ONVIF_API SetSerialPortConfiguration
{
public:
	SetSerialPortConfiguration();

	SetSerialPortConfiguration(
		const SerialPortConfiguration& serialPortConfiguration, 
		bool forcePersistance);

	virtual ~SetSerialPortConfiguration();

	bool getForcePersistance() const;

	const SerialPortConfiguration& getSerialPortConfiguration() const;

	void setForcePersistance(bool val);

	void setSerialPortConfiguration(const SerialPortConfiguration& val);

	void setSerialPortConfiguration(SerialPortConfiguration&& val);

private:
	//@ name=SerialPortConfiguration
	//@ order=0
	SerialPortConfiguration _serialPortConfiguration;

	//@ name=ForcePersistance
	//@ order=1
	bool _forcePersistance;

};


inline bool SetSerialPortConfiguration::getForcePersistance() const
{
	return _forcePersistance;
}


inline const SerialPortConfiguration& SetSerialPortConfiguration::getSerialPortConfiguration() const
{
	return _serialPortConfiguration;
}


inline void SetSerialPortConfiguration::setForcePersistance(bool val)
{
	_forcePersistance = val;
}


inline void SetSerialPortConfiguration::setSerialPortConfiguration(const SerialPortConfiguration& val)
{
	_serialPortConfiguration = val;
}


inline void SetSerialPortConfiguration::setSerialPortConfiguration(SerialPortConfiguration&& val)
{
	_serialPortConfiguration = std::move(val);
}


} } // ONVIF::DeviceIO


#endif // ONVIF_DeviceIO_SetSerialPortConfiguration_INCLUDED
