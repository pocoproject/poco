// 
// GetVideoEncoderConfigurationResponse.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Media_GetVideoEncoderConfigurationResponse_INCLUDED
#define ONVIF_Media_GetVideoEncoderConfigurationResponse_INCLUDED


#include "ONVIF/ONVIF.h"
#include "ONVIF/VideoEncoderConfiguration.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Media {


//@ name="#GetVideoEncoderConfigurationResponse"
//@ namespace="http://www.onvif.org/ver10/media/wsdl"
//@ serialize
class ONVIF_API GetVideoEncoderConfigurationResponse
{
public:
	GetVideoEncoderConfigurationResponse();

	GetVideoEncoderConfigurationResponse(const ONVIF::VideoEncoderConfiguration& configuration);

	virtual ~GetVideoEncoderConfigurationResponse();

	const ONVIF::VideoEncoderConfiguration& getConfiguration() const;

	void setConfiguration(const ONVIF::VideoEncoderConfiguration& val);

	void setConfiguration(ONVIF::VideoEncoderConfiguration&& val);

private:
	//@ name=Configuration
	//@ order=0
	ONVIF::VideoEncoderConfiguration _configuration;

};


inline const ONVIF::VideoEncoderConfiguration& GetVideoEncoderConfigurationResponse::getConfiguration() const
{
	return _configuration;
}


inline void GetVideoEncoderConfigurationResponse::setConfiguration(const ONVIF::VideoEncoderConfiguration& val)
{
	_configuration = val;
}


inline void GetVideoEncoderConfigurationResponse::setConfiguration(ONVIF::VideoEncoderConfiguration&& val)
{
	_configuration = std::move(val);
}


} } // ONVIF::Media


#endif // ONVIF_Media_GetVideoEncoderConfigurationResponse_INCLUDED
