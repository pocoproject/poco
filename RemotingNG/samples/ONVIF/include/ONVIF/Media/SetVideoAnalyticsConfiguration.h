// 
// SetVideoAnalyticsConfiguration.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Media_SetVideoAnalyticsConfiguration_INCLUDED
#define ONVIF_Media_SetVideoAnalyticsConfiguration_INCLUDED


#include "ONVIF/ONVIF.h"
#include "ONVIF/VideoAnalyticsConfiguration.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Media {


//@ name="#SetVideoAnalyticsConfiguration"
//@ namespace="http://www.onvif.org/ver10/media/wsdl"
//@ serialize
class ONVIF_API SetVideoAnalyticsConfiguration
{
public:
	SetVideoAnalyticsConfiguration();

	SetVideoAnalyticsConfiguration(
		const ONVIF::VideoAnalyticsConfiguration& configuration, 
		bool forcePersistence);

	virtual ~SetVideoAnalyticsConfiguration();

	const ONVIF::VideoAnalyticsConfiguration& getConfiguration() const;

	bool getForcePersistence() const;

	void setConfiguration(const ONVIF::VideoAnalyticsConfiguration& val);

	void setConfiguration(ONVIF::VideoAnalyticsConfiguration&& val);

	void setForcePersistence(bool val);

private:
	//@ name=Configuration
	//@ order=0
	ONVIF::VideoAnalyticsConfiguration _configuration;

	//@ name=ForcePersistence
	//@ order=1
	bool _forcePersistence;

};


inline const ONVIF::VideoAnalyticsConfiguration& SetVideoAnalyticsConfiguration::getConfiguration() const
{
	return _configuration;
}


inline bool SetVideoAnalyticsConfiguration::getForcePersistence() const
{
	return _forcePersistence;
}


inline void SetVideoAnalyticsConfiguration::setConfiguration(const ONVIF::VideoAnalyticsConfiguration& val)
{
	_configuration = val;
}


inline void SetVideoAnalyticsConfiguration::setConfiguration(ONVIF::VideoAnalyticsConfiguration&& val)
{
	_configuration = std::move(val);
}


inline void SetVideoAnalyticsConfiguration::setForcePersistence(bool val)
{
	_forcePersistence = val;
}


} } // ONVIF::Media


#endif // ONVIF_Media_SetVideoAnalyticsConfiguration_INCLUDED
