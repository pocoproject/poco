// 
// AddVideoEncoderConfiguration.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Media_AddVideoEncoderConfiguration_INCLUDED
#define ONVIF_Media_AddVideoEncoderConfiguration_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Media {


//@ name="#AddVideoEncoderConfiguration"
//@ namespace="http://www.onvif.org/ver10/media/wsdl"
//@ serialize
class ONVIF_API AddVideoEncoderConfiguration
{
public:
	AddVideoEncoderConfiguration();

	AddVideoEncoderConfiguration(
		const std::string& profileToken, 
		const std::string& configurationToken);

	virtual ~AddVideoEncoderConfiguration();

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


inline const std::string& AddVideoEncoderConfiguration::getConfigurationToken() const
{
	return _configurationToken;
}


inline const std::string& AddVideoEncoderConfiguration::getProfileToken() const
{
	return _profileToken;
}


inline void AddVideoEncoderConfiguration::setConfigurationToken(const std::string& val)
{
	_configurationToken = val;
}


inline void AddVideoEncoderConfiguration::setConfigurationToken(std::string&& val)
{
	_configurationToken = std::move(val);
}


inline void AddVideoEncoderConfiguration::setProfileToken(const std::string& val)
{
	_profileToken = val;
}


inline void AddVideoEncoderConfiguration::setProfileToken(std::string&& val)
{
	_profileToken = std::move(val);
}


} } // ONVIF::Media


#endif // ONVIF_Media_AddVideoEncoderConfiguration_INCLUDED
