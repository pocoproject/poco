// 
// GetCompatibleAudioSourceConfigurationsResponse.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Media_GetCompatibleAudioSourceConfigurationsResponse_INCLUDED
#define ONVIF_Media_GetCompatibleAudioSourceConfigurationsResponse_INCLUDED


#include "ONVIF/AudioSourceConfiguration.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Media {


//@ name="#GetCompatibleAudioSourceConfigurationsResponse"
//@ namespace="http://www.onvif.org/ver10/media/wsdl"
//@ serialize
class ONVIF_API GetCompatibleAudioSourceConfigurationsResponse
{
public:
	GetCompatibleAudioSourceConfigurationsResponse();

	GetCompatibleAudioSourceConfigurationsResponse(const std::vector<ONVIF::AudioSourceConfiguration>& configurations);

	virtual ~GetCompatibleAudioSourceConfigurationsResponse();

	const std::vector<ONVIF::AudioSourceConfiguration>& getConfigurations() const;

	std::vector<ONVIF::AudioSourceConfiguration>& getConfigurations();

	void setConfigurations(const std::vector<ONVIF::AudioSourceConfiguration>& val);

	void setConfigurations(std::vector<ONVIF::AudioSourceConfiguration>&& val);

private:
	//@ mandatory=false
	//@ name=Configurations
	//@ order=0
	std::vector<ONVIF::AudioSourceConfiguration> _configurations;

};


inline const std::vector<ONVIF::AudioSourceConfiguration>& GetCompatibleAudioSourceConfigurationsResponse::getConfigurations() const
{
	return _configurations;
}


inline std::vector<ONVIF::AudioSourceConfiguration>& GetCompatibleAudioSourceConfigurationsResponse::getConfigurations()
{
	return _configurations;
}


inline void GetCompatibleAudioSourceConfigurationsResponse::setConfigurations(const std::vector<ONVIF::AudioSourceConfiguration>& val)
{
	_configurations = val;
}


inline void GetCompatibleAudioSourceConfigurationsResponse::setConfigurations(std::vector<ONVIF::AudioSourceConfiguration>&& val)
{
	_configurations = std::move(val);
}


} } // ONVIF::Media


#endif // ONVIF_Media_GetCompatibleAudioSourceConfigurationsResponse_INCLUDED
