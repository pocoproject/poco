// 
// PaneConfiguration.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_PaneConfiguration_INCLUDED
#define ONVIF_PaneConfiguration_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
class AudioEncoderConfiguration;
} 


namespace ONVIF {


//@ name=PaneConfiguration
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API PaneConfiguration
{
public:
	PaneConfiguration();

	PaneConfiguration(
		const Poco::Optional<std::string>& paneName, 
		const Poco::Optional<std::string>& audioOutputToken, 
		const Poco::Optional<std::string>& audioSourceToken, 
		const Poco::SharedPtr<AudioEncoderConfiguration>& audioEncoderConfiguration, 
		const Poco::Optional<std::string>& receiverToken, 
		const std::string& token);

	virtual ~PaneConfiguration();

	const Poco::SharedPtr<AudioEncoderConfiguration>& getAudioEncoderConfiguration() const;

	const Poco::Optional<std::string>& getAudioOutputToken() const;

	const Poco::Optional<std::string>& getAudioSourceToken() const;

	const Poco::Optional<std::string>& getPaneName() const;

	const Poco::Optional<std::string>& getReceiverToken() const;

	const std::string& getToken() const;

	void setAudioEncoderConfiguration(const Poco::SharedPtr<AudioEncoderConfiguration>& val);

	void setAudioEncoderConfiguration(Poco::SharedPtr<AudioEncoderConfiguration>&& val);

	void setAudioOutputToken(const Poco::Optional<std::string>& val);

	void setAudioOutputToken(Poco::Optional<std::string>&& val);

	void setAudioSourceToken(const Poco::Optional<std::string>& val);

	void setAudioSourceToken(Poco::Optional<std::string>&& val);

	void setPaneName(const Poco::Optional<std::string>& val);

	void setPaneName(Poco::Optional<std::string>&& val);

	void setReceiverToken(const Poco::Optional<std::string>& val);

	void setReceiverToken(Poco::Optional<std::string>&& val);

	void setToken(const std::string& val);

	void setToken(std::string&& val);

private:
	//@ mandatory=false
	//@ name=PaneName
	//@ order=0
	Poco::Optional<std::string> _paneName;

	//@ mandatory=false
	//@ name=AudioOutputToken
	//@ order=1
	Poco::Optional<std::string> _audioOutputToken;

	//@ mandatory=false
	//@ name=AudioSourceToken
	//@ order=2
	Poco::Optional<std::string> _audioSourceToken;

	//@ mandatory=false
	//@ name=AudioEncoderConfiguration
	//@ order=3
	Poco::SharedPtr<AudioEncoderConfiguration> _audioEncoderConfiguration;

	//@ mandatory=false
	//@ name=ReceiverToken
	//@ order=4
	Poco::Optional<std::string> _receiverToken;

	//@ name=Token
	//@ order=5
	std::string _token;

};


inline const Poco::SharedPtr<AudioEncoderConfiguration>& PaneConfiguration::getAudioEncoderConfiguration() const
{
	return _audioEncoderConfiguration;
}


inline const Poco::Optional<std::string>& PaneConfiguration::getAudioOutputToken() const
{
	return _audioOutputToken;
}


inline const Poco::Optional<std::string>& PaneConfiguration::getAudioSourceToken() const
{
	return _audioSourceToken;
}


inline const Poco::Optional<std::string>& PaneConfiguration::getPaneName() const
{
	return _paneName;
}


inline const Poco::Optional<std::string>& PaneConfiguration::getReceiverToken() const
{
	return _receiverToken;
}


inline const std::string& PaneConfiguration::getToken() const
{
	return _token;
}


inline void PaneConfiguration::setAudioEncoderConfiguration(const Poco::SharedPtr<AudioEncoderConfiguration>& val)
{
	_audioEncoderConfiguration = val;
}


inline void PaneConfiguration::setAudioEncoderConfiguration(Poco::SharedPtr<AudioEncoderConfiguration>&& val)
{
	_audioEncoderConfiguration = std::move(val);
}


inline void PaneConfiguration::setAudioOutputToken(const Poco::Optional<std::string>& val)
{
	_audioOutputToken = val;
}


inline void PaneConfiguration::setAudioOutputToken(Poco::Optional<std::string>&& val)
{
	_audioOutputToken = std::move(val);
}


inline void PaneConfiguration::setAudioSourceToken(const Poco::Optional<std::string>& val)
{
	_audioSourceToken = val;
}


inline void PaneConfiguration::setAudioSourceToken(Poco::Optional<std::string>&& val)
{
	_audioSourceToken = std::move(val);
}


inline void PaneConfiguration::setPaneName(const Poco::Optional<std::string>& val)
{
	_paneName = val;
}


inline void PaneConfiguration::setPaneName(Poco::Optional<std::string>&& val)
{
	_paneName = std::move(val);
}


inline void PaneConfiguration::setReceiverToken(const Poco::Optional<std::string>& val)
{
	_receiverToken = val;
}


inline void PaneConfiguration::setReceiverToken(Poco::Optional<std::string>&& val)
{
	_receiverToken = std::move(val);
}


inline void PaneConfiguration::setToken(const std::string& val)
{
	_token = val;
}


inline void PaneConfiguration::setToken(std::string&& val)
{
	_token = std::move(val);
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/AudioEncoderConfiguration.h"


#endif // ONVIF_PaneConfiguration_INCLUDED
