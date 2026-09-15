// 
// GetCompatibleAudioOutputConfigurationsResponse.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Media_GetCompatibleAudioOutputConfigurationsResponse_INCLUDED
#define ONVIF_Media_GetCompatibleAudioOutputConfigurationsResponse_INCLUDED


#include "ONVIF/AudioOutputConfiguration.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Media {


//@ name="#GetCompatibleAudioOutputConfigurationsResponse"
//@ namespace="http://www.onvif.org/ver10/media/wsdl"
//@ serialize
class ONVIF_API GetCompatibleAudioOutputConfigurationsResponse
{
public:
	GetCompatibleAudioOutputConfigurationsResponse();

	GetCompatibleAudioOutputConfigurationsResponse(const std::vector<ONVIF::AudioOutputConfiguration>& configurations);

	virtual ~GetCompatibleAudioOutputConfigurationsResponse();

	const std::vector<ONVIF::AudioOutputConfiguration>& getConfigurations() const;

	std::vector<ONVIF::AudioOutputConfiguration>& getConfigurations();

	void setConfigurations(const std::vector<ONVIF::AudioOutputConfiguration>& val);

	void setConfigurations(std::vector<ONVIF::AudioOutputConfiguration>&& val);

private:
	//@ mandatory=false
	//@ name=Configurations
	//@ order=0
	std::vector<ONVIF::AudioOutputConfiguration> _configurations;

};


inline const std::vector<ONVIF::AudioOutputConfiguration>& GetCompatibleAudioOutputConfigurationsResponse::getConfigurations() const
{
	return _configurations;
}


inline std::vector<ONVIF::AudioOutputConfiguration>& GetCompatibleAudioOutputConfigurationsResponse::getConfigurations()
{
	return _configurations;
}


inline void GetCompatibleAudioOutputConfigurationsResponse::setConfigurations(const std::vector<ONVIF::AudioOutputConfiguration>& val)
{
	_configurations = val;
}


inline void GetCompatibleAudioOutputConfigurationsResponse::setConfigurations(std::vector<ONVIF::AudioOutputConfiguration>&& val)
{
	_configurations = std::move(val);
}


} } // ONVIF::Media


#endif // ONVIF_Media_GetCompatibleAudioOutputConfigurationsResponse_INCLUDED
