// 
// SetVideoSourceConfiguration.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Media_SetVideoSourceConfiguration_INCLUDED
#define ONVIF_Media_SetVideoSourceConfiguration_INCLUDED


#include "ONVIF/ONVIF.h"
#include "ONVIF/VideoSourceConfiguration.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Media {


//@ name="#SetVideoSourceConfiguration"
//@ namespace="http://www.onvif.org/ver10/media/wsdl"
//@ serialize
class ONVIF_API SetVideoSourceConfiguration
{
public:
	SetVideoSourceConfiguration();

	SetVideoSourceConfiguration(
		const ONVIF::VideoSourceConfiguration& configuration, 
		bool forcePersistence);

	virtual ~SetVideoSourceConfiguration();

	const ONVIF::VideoSourceConfiguration& getConfiguration() const;

	bool getForcePersistence() const;

	void setConfiguration(const ONVIF::VideoSourceConfiguration& val);

	void setConfiguration(ONVIF::VideoSourceConfiguration&& val);

	void setForcePersistence(bool val);

private:
	//@ name=Configuration
	//@ order=0
	ONVIF::VideoSourceConfiguration _configuration;

	//@ name=ForcePersistence
	//@ order=1
	bool _forcePersistence;

};


inline const ONVIF::VideoSourceConfiguration& SetVideoSourceConfiguration::getConfiguration() const
{
	return _configuration;
}


inline bool SetVideoSourceConfiguration::getForcePersistence() const
{
	return _forcePersistence;
}


inline void SetVideoSourceConfiguration::setConfiguration(const ONVIF::VideoSourceConfiguration& val)
{
	_configuration = val;
}


inline void SetVideoSourceConfiguration::setConfiguration(ONVIF::VideoSourceConfiguration&& val)
{
	_configuration = std::move(val);
}


inline void SetVideoSourceConfiguration::setForcePersistence(bool val)
{
	_forcePersistence = val;
}


} } // ONVIF::Media


#endif // ONVIF_Media_SetVideoSourceConfiguration_INCLUDED
