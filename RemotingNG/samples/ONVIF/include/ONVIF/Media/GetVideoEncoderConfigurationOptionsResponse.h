// 
// GetVideoEncoderConfigurationOptionsResponse.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Media_GetVideoEncoderConfigurationOptionsResponse_INCLUDED
#define ONVIF_Media_GetVideoEncoderConfigurationOptionsResponse_INCLUDED


#include "ONVIF/ONVIF.h"
#include "ONVIF/VideoEncoderConfigurationOptions.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Media {


//@ name="#GetVideoEncoderConfigurationOptionsResponse"
//@ namespace="http://www.onvif.org/ver10/media/wsdl"
//@ serialize
class ONVIF_API GetVideoEncoderConfigurationOptionsResponse
{
public:
	GetVideoEncoderConfigurationOptionsResponse();

	GetVideoEncoderConfigurationOptionsResponse(const ONVIF::VideoEncoderConfigurationOptions& options);

	virtual ~GetVideoEncoderConfigurationOptionsResponse();

	const ONVIF::VideoEncoderConfigurationOptions& getOptions() const;

	void setOptions(const ONVIF::VideoEncoderConfigurationOptions& val);

	void setOptions(ONVIF::VideoEncoderConfigurationOptions&& val);

private:
	//@ name=Options
	//@ order=0
	ONVIF::VideoEncoderConfigurationOptions _options;

};


inline const ONVIF::VideoEncoderConfigurationOptions& GetVideoEncoderConfigurationOptionsResponse::getOptions() const
{
	return _options;
}


inline void GetVideoEncoderConfigurationOptionsResponse::setOptions(const ONVIF::VideoEncoderConfigurationOptions& val)
{
	_options = val;
}


inline void GetVideoEncoderConfigurationOptionsResponse::setOptions(ONVIF::VideoEncoderConfigurationOptions&& val)
{
	_options = std::move(val);
}


} } // ONVIF::Media


#endif // ONVIF_Media_GetVideoEncoderConfigurationOptionsResponse_INCLUDED
