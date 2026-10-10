// 
// GetAudioOutputConfigurationsResponse.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Media_GetAudioOutputConfigurationsResponse_INCLUDED
#define ONVIF_Media_GetAudioOutputConfigurationsResponse_INCLUDED


#include "ONVIF/AudioOutputConfiguration.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Media {


//@ name="#GetAudioOutputConfigurationsResponse"
//@ namespace="http://www.onvif.org/ver10/media/wsdl"
//@ serialize
class ONVIF_API GetAudioOutputConfigurationsResponse
{
public:
	GetAudioOutputConfigurationsResponse();

	GetAudioOutputConfigurationsResponse(const std::vector<ONVIF::AudioOutputConfiguration>& configurations);

	virtual ~GetAudioOutputConfigurationsResponse();

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


inline const std::vector<ONVIF::AudioOutputConfiguration>& GetAudioOutputConfigurationsResponse::getConfigurations() const
{
	return _configurations;
}


inline std::vector<ONVIF::AudioOutputConfiguration>& GetAudioOutputConfigurationsResponse::getConfigurations()
{
	return _configurations;
}


inline void GetAudioOutputConfigurationsResponse::setConfigurations(const std::vector<ONVIF::AudioOutputConfiguration>& val)
{
	_configurations = val;
}


inline void GetAudioOutputConfigurationsResponse::setConfigurations(std::vector<ONVIF::AudioOutputConfiguration>&& val)
{
	_configurations = std::move(val);
}


} } // ONVIF::Media


#endif // ONVIF_Media_GetAudioOutputConfigurationsResponse_INCLUDED
