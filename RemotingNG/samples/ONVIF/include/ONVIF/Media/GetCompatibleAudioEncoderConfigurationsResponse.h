// 
// GetCompatibleAudioEncoderConfigurationsResponse.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Media_GetCompatibleAudioEncoderConfigurationsResponse_INCLUDED
#define ONVIF_Media_GetCompatibleAudioEncoderConfigurationsResponse_INCLUDED


#include "ONVIF/AudioEncoderConfiguration.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Media {


//@ name="#GetCompatibleAudioEncoderConfigurationsResponse"
//@ namespace="http://www.onvif.org/ver10/media/wsdl"
//@ serialize
class ONVIF_API GetCompatibleAudioEncoderConfigurationsResponse
{
public:
	GetCompatibleAudioEncoderConfigurationsResponse();

	GetCompatibleAudioEncoderConfigurationsResponse(const std::vector<ONVIF::AudioEncoderConfiguration>& configurations);

	virtual ~GetCompatibleAudioEncoderConfigurationsResponse();

	const std::vector<ONVIF::AudioEncoderConfiguration>& getConfigurations() const;

	std::vector<ONVIF::AudioEncoderConfiguration>& getConfigurations();

	void setConfigurations(const std::vector<ONVIF::AudioEncoderConfiguration>& val);

	void setConfigurations(std::vector<ONVIF::AudioEncoderConfiguration>&& val);

private:
	//@ mandatory=false
	//@ name=Configurations
	//@ order=0
	std::vector<ONVIF::AudioEncoderConfiguration> _configurations;

};


inline const std::vector<ONVIF::AudioEncoderConfiguration>& GetCompatibleAudioEncoderConfigurationsResponse::getConfigurations() const
{
	return _configurations;
}


inline std::vector<ONVIF::AudioEncoderConfiguration>& GetCompatibleAudioEncoderConfigurationsResponse::getConfigurations()
{
	return _configurations;
}


inline void GetCompatibleAudioEncoderConfigurationsResponse::setConfigurations(const std::vector<ONVIF::AudioEncoderConfiguration>& val)
{
	_configurations = val;
}


inline void GetCompatibleAudioEncoderConfigurationsResponse::setConfigurations(std::vector<ONVIF::AudioEncoderConfiguration>&& val)
{
	_configurations = std::move(val);
}


} } // ONVIF::Media


#endif // ONVIF_Media_GetCompatibleAudioEncoderConfigurationsResponse_INCLUDED
