// 
// GetAudioDecoderConfigurationsResponse.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Media_GetAudioDecoderConfigurationsResponse_INCLUDED
#define ONVIF_Media_GetAudioDecoderConfigurationsResponse_INCLUDED


#include "ONVIF/AudioDecoderConfiguration.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Media {


//@ name="#GetAudioDecoderConfigurationsResponse"
//@ namespace="http://www.onvif.org/ver10/media/wsdl"
//@ serialize
class ONVIF_API GetAudioDecoderConfigurationsResponse
{
public:
	GetAudioDecoderConfigurationsResponse();

	GetAudioDecoderConfigurationsResponse(const std::vector<ONVIF::AudioDecoderConfiguration>& configurations);

	virtual ~GetAudioDecoderConfigurationsResponse();

	const std::vector<ONVIF::AudioDecoderConfiguration>& getConfigurations() const;

	std::vector<ONVIF::AudioDecoderConfiguration>& getConfigurations();

	void setConfigurations(const std::vector<ONVIF::AudioDecoderConfiguration>& val);

	void setConfigurations(std::vector<ONVIF::AudioDecoderConfiguration>&& val);

private:
	//@ mandatory=false
	//@ name=Configurations
	//@ order=0
	std::vector<ONVIF::AudioDecoderConfiguration> _configurations;

};


inline const std::vector<ONVIF::AudioDecoderConfiguration>& GetAudioDecoderConfigurationsResponse::getConfigurations() const
{
	return _configurations;
}


inline std::vector<ONVIF::AudioDecoderConfiguration>& GetAudioDecoderConfigurationsResponse::getConfigurations()
{
	return _configurations;
}


inline void GetAudioDecoderConfigurationsResponse::setConfigurations(const std::vector<ONVIF::AudioDecoderConfiguration>& val)
{
	_configurations = val;
}


inline void GetAudioDecoderConfigurationsResponse::setConfigurations(std::vector<ONVIF::AudioDecoderConfiguration>&& val)
{
	_configurations = std::move(val);
}


} } // ONVIF::Media


#endif // ONVIF_Media_GetAudioDecoderConfigurationsResponse_INCLUDED
