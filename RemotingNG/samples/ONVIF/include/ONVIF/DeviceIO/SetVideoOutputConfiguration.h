// 
// SetVideoOutputConfiguration.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_DeviceIO_SetVideoOutputConfiguration_INCLUDED
#define ONVIF_DeviceIO_SetVideoOutputConfiguration_INCLUDED


#include "ONVIF/ONVIF.h"
#include "ONVIF/VideoOutputConfiguration.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace DeviceIO {


//@ name="#SetVideoOutputConfiguration"
//@ namespace="http://www.onvif.org/ver10/deviceIO/wsdl"
//@ serialize
class ONVIF_API SetVideoOutputConfiguration
{
public:
	SetVideoOutputConfiguration();

	SetVideoOutputConfiguration(
		const ONVIF::VideoOutputConfiguration& configuration, 
		bool forcePersistence);

	virtual ~SetVideoOutputConfiguration();

	const ONVIF::VideoOutputConfiguration& getConfiguration() const;

	bool getForcePersistence() const;

	void setConfiguration(const ONVIF::VideoOutputConfiguration& val);

	void setConfiguration(ONVIF::VideoOutputConfiguration&& val);

	void setForcePersistence(bool val);

private:
	//@ name=Configuration
	//@ order=0
	ONVIF::VideoOutputConfiguration _configuration;

	//@ name=ForcePersistence
	//@ order=1
	bool _forcePersistence;

};


inline const ONVIF::VideoOutputConfiguration& SetVideoOutputConfiguration::getConfiguration() const
{
	return _configuration;
}


inline bool SetVideoOutputConfiguration::getForcePersistence() const
{
	return _forcePersistence;
}


inline void SetVideoOutputConfiguration::setConfiguration(const ONVIF::VideoOutputConfiguration& val)
{
	_configuration = val;
}


inline void SetVideoOutputConfiguration::setConfiguration(ONVIF::VideoOutputConfiguration&& val)
{
	_configuration = std::move(val);
}


inline void SetVideoOutputConfiguration::setForcePersistence(bool val)
{
	_forcePersistence = val;
}


} } // ONVIF::DeviceIO


#endif // ONVIF_DeviceIO_SetVideoOutputConfiguration_INCLUDED
