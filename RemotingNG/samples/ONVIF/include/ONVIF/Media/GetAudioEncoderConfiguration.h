// 
// GetAudioEncoderConfiguration.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Media_GetAudioEncoderConfiguration_INCLUDED
#define ONVIF_Media_GetAudioEncoderConfiguration_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Media {


//@ name="#GetAudioEncoderConfiguration"
//@ namespace="http://www.onvif.org/ver10/media/wsdl"
//@ serialize
class ONVIF_API GetAudioEncoderConfiguration
{
public:
	GetAudioEncoderConfiguration();

	GetAudioEncoderConfiguration(const std::string& configurationToken);

	virtual ~GetAudioEncoderConfiguration();

	const std::string& getConfigurationToken() const;

	void setConfigurationToken(const std::string& val);

	void setConfigurationToken(std::string&& val);

private:
	//@ name=ConfigurationToken
	//@ order=0
	std::string _configurationToken;

};


inline const std::string& GetAudioEncoderConfiguration::getConfigurationToken() const
{
	return _configurationToken;
}


inline void GetAudioEncoderConfiguration::setConfigurationToken(const std::string& val)
{
	_configurationToken = val;
}


inline void GetAudioEncoderConfiguration::setConfigurationToken(std::string&& val)
{
	_configurationToken = std::move(val);
}


} } // ONVIF::Media


#endif // ONVIF_Media_GetAudioEncoderConfiguration_INCLUDED
