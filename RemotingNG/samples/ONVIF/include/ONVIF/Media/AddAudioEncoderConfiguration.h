// 
// AddAudioEncoderConfiguration.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Media_AddAudioEncoderConfiguration_INCLUDED
#define ONVIF_Media_AddAudioEncoderConfiguration_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Media {


//@ name="#AddAudioEncoderConfiguration"
//@ namespace="http://www.onvif.org/ver10/media/wsdl"
//@ serialize
class ONVIF_API AddAudioEncoderConfiguration
{
public:
	AddAudioEncoderConfiguration();

	AddAudioEncoderConfiguration(
		const std::string& profileToken, 
		const std::string& configurationToken);

	virtual ~AddAudioEncoderConfiguration();

	const std::string& getConfigurationToken() const;

	const std::string& getProfileToken() const;

	void setConfigurationToken(const std::string& val);

	void setConfigurationToken(std::string&& val);

	void setProfileToken(const std::string& val);

	void setProfileToken(std::string&& val);

private:
	//@ name=ProfileToken
	//@ order=0
	std::string _profileToken;

	//@ name=ConfigurationToken
	//@ order=1
	std::string _configurationToken;

};


inline const std::string& AddAudioEncoderConfiguration::getConfigurationToken() const
{
	return _configurationToken;
}


inline const std::string& AddAudioEncoderConfiguration::getProfileToken() const
{
	return _profileToken;
}


inline void AddAudioEncoderConfiguration::setConfigurationToken(const std::string& val)
{
	_configurationToken = val;
}


inline void AddAudioEncoderConfiguration::setConfigurationToken(std::string&& val)
{
	_configurationToken = std::move(val);
}


inline void AddAudioEncoderConfiguration::setProfileToken(const std::string& val)
{
	_profileToken = val;
}


inline void AddAudioEncoderConfiguration::setProfileToken(std::string&& val)
{
	_profileToken = std::move(val);
}


} } // ONVIF::Media


#endif // ONVIF_Media_AddAudioEncoderConfiguration_INCLUDED
