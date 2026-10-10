// 
// SetAudioSourceConfiguration.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Media_SetAudioSourceConfiguration_INCLUDED
#define ONVIF_Media_SetAudioSourceConfiguration_INCLUDED


#include "ONVIF/AudioSourceConfiguration.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Media {


//@ name="#SetAudioSourceConfiguration"
//@ namespace="http://www.onvif.org/ver10/media/wsdl"
//@ serialize
class ONVIF_API SetAudioSourceConfiguration
{
public:
	SetAudioSourceConfiguration();

	SetAudioSourceConfiguration(
		const ONVIF::AudioSourceConfiguration& configuration, 
		bool forcePersistence);

	virtual ~SetAudioSourceConfiguration();

	const ONVIF::AudioSourceConfiguration& getConfiguration() const;

	bool getForcePersistence() const;

	void setConfiguration(const ONVIF::AudioSourceConfiguration& val);

	void setConfiguration(ONVIF::AudioSourceConfiguration&& val);

	void setForcePersistence(bool val);

private:
	//@ name=Configuration
	//@ order=0
	ONVIF::AudioSourceConfiguration _configuration;

	//@ name=ForcePersistence
	//@ order=1
	bool _forcePersistence;

};


inline const ONVIF::AudioSourceConfiguration& SetAudioSourceConfiguration::getConfiguration() const
{
	return _configuration;
}


inline bool SetAudioSourceConfiguration::getForcePersistence() const
{
	return _forcePersistence;
}


inline void SetAudioSourceConfiguration::setConfiguration(const ONVIF::AudioSourceConfiguration& val)
{
	_configuration = val;
}


inline void SetAudioSourceConfiguration::setConfiguration(ONVIF::AudioSourceConfiguration&& val)
{
	_configuration = std::move(val);
}


inline void SetAudioSourceConfiguration::setForcePersistence(bool val)
{
	_forcePersistence = val;
}


} } // ONVIF::Media


#endif // ONVIF_Media_SetAudioSourceConfiguration_INCLUDED
