// 
// GetCompatibleVideoEncoderConfigurationsResponse.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Media_GetCompatibleVideoEncoderConfigurationsResponse_INCLUDED
#define ONVIF_Media_GetCompatibleVideoEncoderConfigurationsResponse_INCLUDED


#include "ONVIF/ONVIF.h"
#include "ONVIF/VideoEncoderConfiguration.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Media {


//@ name="#GetCompatibleVideoEncoderConfigurationsResponse"
//@ namespace="http://www.onvif.org/ver10/media/wsdl"
//@ serialize
class ONVIF_API GetCompatibleVideoEncoderConfigurationsResponse
{
public:
	GetCompatibleVideoEncoderConfigurationsResponse();

	GetCompatibleVideoEncoderConfigurationsResponse(const std::vector<ONVIF::VideoEncoderConfiguration>& configurations);

	virtual ~GetCompatibleVideoEncoderConfigurationsResponse();

	const std::vector<ONVIF::VideoEncoderConfiguration>& getConfigurations() const;

	std::vector<ONVIF::VideoEncoderConfiguration>& getConfigurations();

	void setConfigurations(const std::vector<ONVIF::VideoEncoderConfiguration>& val);

	void setConfigurations(std::vector<ONVIF::VideoEncoderConfiguration>&& val);

private:
	//@ mandatory=false
	//@ name=Configurations
	//@ order=0
	std::vector<ONVIF::VideoEncoderConfiguration> _configurations;

};


inline const std::vector<ONVIF::VideoEncoderConfiguration>& GetCompatibleVideoEncoderConfigurationsResponse::getConfigurations() const
{
	return _configurations;
}


inline std::vector<ONVIF::VideoEncoderConfiguration>& GetCompatibleVideoEncoderConfigurationsResponse::getConfigurations()
{
	return _configurations;
}


inline void GetCompatibleVideoEncoderConfigurationsResponse::setConfigurations(const std::vector<ONVIF::VideoEncoderConfiguration>& val)
{
	_configurations = val;
}


inline void GetCompatibleVideoEncoderConfigurationsResponse::setConfigurations(std::vector<ONVIF::VideoEncoderConfiguration>&& val)
{
	_configurations = std::move(val);
}


} } // ONVIF::Media


#endif // ONVIF_Media_GetCompatibleVideoEncoderConfigurationsResponse_INCLUDED
