// 
// SetAudioOutputConfiguration.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Media_SetAudioOutputConfiguration_INCLUDED
#define ONVIF_Media_SetAudioOutputConfiguration_INCLUDED


#include "ONVIF/AudioOutputConfiguration.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Media {


//@ name="#SetAudioOutputConfiguration"
//@ namespace="http://www.onvif.org/ver10/media/wsdl"
//@ serialize
class ONVIF_API SetAudioOutputConfiguration
{
public:
	SetAudioOutputConfiguration();

	SetAudioOutputConfiguration(
		const ONVIF::AudioOutputConfiguration& configuration, 
		bool forcePersistence);

	virtual ~SetAudioOutputConfiguration();

	const ONVIF::AudioOutputConfiguration& getConfiguration() const;

	bool getForcePersistence() const;

	void setConfiguration(const ONVIF::AudioOutputConfiguration& val);

	void setConfiguration(ONVIF::AudioOutputConfiguration&& val);

	void setForcePersistence(bool val);

private:
	//@ name=Configuration
	//@ order=0
	ONVIF::AudioOutputConfiguration _configuration;

	//@ name=ForcePersistence
	//@ order=1
	bool _forcePersistence;

};


inline const ONVIF::AudioOutputConfiguration& SetAudioOutputConfiguration::getConfiguration() const
{
	return _configuration;
}


inline bool SetAudioOutputConfiguration::getForcePersistence() const
{
	return _forcePersistence;
}


inline void SetAudioOutputConfiguration::setConfiguration(const ONVIF::AudioOutputConfiguration& val)
{
	_configuration = val;
}


inline void SetAudioOutputConfiguration::setConfiguration(ONVIF::AudioOutputConfiguration&& val)
{
	_configuration = std::move(val);
}


inline void SetAudioOutputConfiguration::setForcePersistence(bool val)
{
	_forcePersistence = val;
}


} } // ONVIF::Media


#endif // ONVIF_Media_SetAudioOutputConfiguration_INCLUDED
