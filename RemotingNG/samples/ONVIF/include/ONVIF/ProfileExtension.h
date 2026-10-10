// 
// ProfileExtension.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_ProfileExtension_INCLUDED
#define ONVIF_ProfileExtension_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
class AudioDecoderConfiguration;
class AudioOutputConfiguration;
class ProfileExtension2;
} 


namespace ONVIF {


//@ name=ProfileExtension
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API ProfileExtension
{
public:
	ProfileExtension();

	ProfileExtension(
		const Poco::SharedPtr<AudioOutputConfiguration>& audioOutputConfiguration, 
		const Poco::SharedPtr<AudioDecoderConfiguration>& audioDecoderConfiguration, 
		const Poco::SharedPtr<ProfileExtension2>& extension);

	virtual ~ProfileExtension();

	const Poco::SharedPtr<AudioDecoderConfiguration>& getAudioDecoderConfiguration() const;

	const Poco::SharedPtr<AudioOutputConfiguration>& getAudioOutputConfiguration() const;

	const Poco::SharedPtr<ProfileExtension2>& getExtension() const;

	void setAudioDecoderConfiguration(const Poco::SharedPtr<AudioDecoderConfiguration>& val);

	void setAudioDecoderConfiguration(Poco::SharedPtr<AudioDecoderConfiguration>&& val);

	void setAudioOutputConfiguration(const Poco::SharedPtr<AudioOutputConfiguration>& val);

	void setAudioOutputConfiguration(Poco::SharedPtr<AudioOutputConfiguration>&& val);

	void setExtension(const Poco::SharedPtr<ProfileExtension2>& val);

	void setExtension(Poco::SharedPtr<ProfileExtension2>&& val);

private:
	//@ mandatory=false
	//@ name=AudioOutputConfiguration
	//@ order=0
	Poco::SharedPtr<AudioOutputConfiguration> _audioOutputConfiguration;

	//@ mandatory=false
	//@ name=AudioDecoderConfiguration
	//@ order=1
	Poco::SharedPtr<AudioDecoderConfiguration> _audioDecoderConfiguration;

	//@ mandatory=false
	//@ name=Extension
	//@ order=2
	Poco::SharedPtr<ProfileExtension2> _extension;

};


inline const Poco::SharedPtr<AudioDecoderConfiguration>& ProfileExtension::getAudioDecoderConfiguration() const
{
	return _audioDecoderConfiguration;
}


inline const Poco::SharedPtr<AudioOutputConfiguration>& ProfileExtension::getAudioOutputConfiguration() const
{
	return _audioOutputConfiguration;
}


inline const Poco::SharedPtr<ProfileExtension2>& ProfileExtension::getExtension() const
{
	return _extension;
}


inline void ProfileExtension::setAudioDecoderConfiguration(const Poco::SharedPtr<AudioDecoderConfiguration>& val)
{
	_audioDecoderConfiguration = val;
}


inline void ProfileExtension::setAudioDecoderConfiguration(Poco::SharedPtr<AudioDecoderConfiguration>&& val)
{
	_audioDecoderConfiguration = std::move(val);
}


inline void ProfileExtension::setAudioOutputConfiguration(const Poco::SharedPtr<AudioOutputConfiguration>& val)
{
	_audioOutputConfiguration = val;
}


inline void ProfileExtension::setAudioOutputConfiguration(Poco::SharedPtr<AudioOutputConfiguration>&& val)
{
	_audioOutputConfiguration = std::move(val);
}


inline void ProfileExtension::setExtension(const Poco::SharedPtr<ProfileExtension2>& val)
{
	_extension = val;
}


inline void ProfileExtension::setExtension(Poco::SharedPtr<ProfileExtension2>&& val)
{
	_extension = std::move(val);
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/AudioDecoderConfiguration.h"
#include "ONVIF/AudioOutputConfiguration.h"
#include "ONVIF/ProfileExtension2.h"


#endif // ONVIF_ProfileExtension_INCLUDED
