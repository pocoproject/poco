// 
// GetAudioDecoderConfigurationOptions.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Media_GetAudioDecoderConfigurationOptions_INCLUDED
#define ONVIF_Media_GetAudioDecoderConfigurationOptions_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Media {


//@ name="#GetAudioDecoderConfigurationOptions"
//@ namespace="http://www.onvif.org/ver10/media/wsdl"
//@ serialize
class ONVIF_API GetAudioDecoderConfigurationOptions
{
public:
	GetAudioDecoderConfigurationOptions();

	GetAudioDecoderConfigurationOptions(
		const Poco::Optional<std::string>& configurationToken, 
		const Poco::Optional<std::string>& profileToken);

	virtual ~GetAudioDecoderConfigurationOptions();

	const Poco::Optional<std::string>& getConfigurationToken() const;

	const Poco::Optional<std::string>& getProfileToken() const;

	void setConfigurationToken(const Poco::Optional<std::string>& val);

	void setConfigurationToken(Poco::Optional<std::string>&& val);

	void setProfileToken(const Poco::Optional<std::string>& val);

	void setProfileToken(Poco::Optional<std::string>&& val);

private:
	//@ mandatory=false
	//@ name=ConfigurationToken
	//@ order=0
	Poco::Optional<std::string> _configurationToken;

	//@ mandatory=false
	//@ name=ProfileToken
	//@ order=1
	Poco::Optional<std::string> _profileToken;

};


inline const Poco::Optional<std::string>& GetAudioDecoderConfigurationOptions::getConfigurationToken() const
{
	return _configurationToken;
}


inline const Poco::Optional<std::string>& GetAudioDecoderConfigurationOptions::getProfileToken() const
{
	return _profileToken;
}


inline void GetAudioDecoderConfigurationOptions::setConfigurationToken(const Poco::Optional<std::string>& val)
{
	_configurationToken = val;
}


inline void GetAudioDecoderConfigurationOptions::setConfigurationToken(Poco::Optional<std::string>&& val)
{
	_configurationToken = std::move(val);
}


inline void GetAudioDecoderConfigurationOptions::setProfileToken(const Poco::Optional<std::string>& val)
{
	_profileToken = val;
}


inline void GetAudioDecoderConfigurationOptions::setProfileToken(Poco::Optional<std::string>&& val)
{
	_profileToken = std::move(val);
}


} } // ONVIF::Media


#endif // ONVIF_Media_GetAudioDecoderConfigurationOptions_INCLUDED
