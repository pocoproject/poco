// 
// SetAudioEncoderConfiguration.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Media_SetAudioEncoderConfiguration_INCLUDED
#define ONVIF_Media_SetAudioEncoderConfiguration_INCLUDED


#include "ONVIF/AudioEncoderConfiguration.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Media {


//@ name="#SetAudioEncoderConfiguration"
//@ namespace="http://www.onvif.org/ver10/media/wsdl"
//@ serialize
class ONVIF_API SetAudioEncoderConfiguration
{
public:
	SetAudioEncoderConfiguration();

	SetAudioEncoderConfiguration(
		const ONVIF::AudioEncoderConfiguration& configuration, 
		bool forcePersistence);

	virtual ~SetAudioEncoderConfiguration();

	const ONVIF::AudioEncoderConfiguration& getConfiguration() const;

	bool getForcePersistence() const;

	void setConfiguration(const ONVIF::AudioEncoderConfiguration& val);

	void setConfiguration(ONVIF::AudioEncoderConfiguration&& val);

	void setForcePersistence(bool val);

private:
	//@ name=Configuration
	//@ order=0
	ONVIF::AudioEncoderConfiguration _configuration;

	//@ name=ForcePersistence
	//@ order=1
	bool _forcePersistence;

};


inline const ONVIF::AudioEncoderConfiguration& SetAudioEncoderConfiguration::getConfiguration() const
{
	return _configuration;
}


inline bool SetAudioEncoderConfiguration::getForcePersistence() const
{
	return _forcePersistence;
}


inline void SetAudioEncoderConfiguration::setConfiguration(const ONVIF::AudioEncoderConfiguration& val)
{
	_configuration = val;
}


inline void SetAudioEncoderConfiguration::setConfiguration(ONVIF::AudioEncoderConfiguration&& val)
{
	_configuration = std::move(val);
}


inline void SetAudioEncoderConfiguration::setForcePersistence(bool val)
{
	_forcePersistence = val;
}


} } // ONVIF::Media


#endif // ONVIF_Media_SetAudioEncoderConfiguration_INCLUDED
