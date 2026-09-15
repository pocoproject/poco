// 
// SetAudioDecoderConfiguration.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Media_SetAudioDecoderConfiguration_INCLUDED
#define ONVIF_Media_SetAudioDecoderConfiguration_INCLUDED


#include "ONVIF/AudioDecoderConfiguration.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Media {


//@ name="#SetAudioDecoderConfiguration"
//@ namespace="http://www.onvif.org/ver10/media/wsdl"
//@ serialize
class ONVIF_API SetAudioDecoderConfiguration
{
public:
	SetAudioDecoderConfiguration();

	SetAudioDecoderConfiguration(
		const ONVIF::AudioDecoderConfiguration& configuration, 
		bool forcePersistence);

	virtual ~SetAudioDecoderConfiguration();

	const ONVIF::AudioDecoderConfiguration& getConfiguration() const;

	bool getForcePersistence() const;

	void setConfiguration(const ONVIF::AudioDecoderConfiguration& val);

	void setConfiguration(ONVIF::AudioDecoderConfiguration&& val);

	void setForcePersistence(bool val);

private:
	//@ name=Configuration
	//@ order=0
	ONVIF::AudioDecoderConfiguration _configuration;

	//@ name=ForcePersistence
	//@ order=1
	bool _forcePersistence;

};


inline const ONVIF::AudioDecoderConfiguration& SetAudioDecoderConfiguration::getConfiguration() const
{
	return _configuration;
}


inline bool SetAudioDecoderConfiguration::getForcePersistence() const
{
	return _forcePersistence;
}


inline void SetAudioDecoderConfiguration::setConfiguration(const ONVIF::AudioDecoderConfiguration& val)
{
	_configuration = val;
}


inline void SetAudioDecoderConfiguration::setConfiguration(ONVIF::AudioDecoderConfiguration&& val)
{
	_configuration = std::move(val);
}


inline void SetAudioDecoderConfiguration::setForcePersistence(bool val)
{
	_forcePersistence = val;
}


} } // ONVIF::Media


#endif // ONVIF_Media_SetAudioDecoderConfiguration_INCLUDED
