// 
// GetVideoSourceConfigurationOptionsResponse.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Media_GetVideoSourceConfigurationOptionsResponse_INCLUDED
#define ONVIF_Media_GetVideoSourceConfigurationOptionsResponse_INCLUDED


#include "ONVIF/ONVIF.h"
#include "ONVIF/VideoSourceConfigurationOptions.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Media {


//@ name="#GetVideoSourceConfigurationOptionsResponse"
//@ namespace="http://www.onvif.org/ver10/media/wsdl"
//@ serialize
class ONVIF_API GetVideoSourceConfigurationOptionsResponse
{
public:
	GetVideoSourceConfigurationOptionsResponse();

	GetVideoSourceConfigurationOptionsResponse(const ONVIF::VideoSourceConfigurationOptions& options);

	virtual ~GetVideoSourceConfigurationOptionsResponse();

	const ONVIF::VideoSourceConfigurationOptions& getOptions() const;

	void setOptions(const ONVIF::VideoSourceConfigurationOptions& val);

	void setOptions(ONVIF::VideoSourceConfigurationOptions&& val);

private:
	//@ name=Options
	//@ order=0
	ONVIF::VideoSourceConfigurationOptions _options;

};


inline const ONVIF::VideoSourceConfigurationOptions& GetVideoSourceConfigurationOptionsResponse::getOptions() const
{
	return _options;
}


inline void GetVideoSourceConfigurationOptionsResponse::setOptions(const ONVIF::VideoSourceConfigurationOptions& val)
{
	_options = val;
}


inline void GetVideoSourceConfigurationOptionsResponse::setOptions(ONVIF::VideoSourceConfigurationOptions&& val)
{
	_options = std::move(val);
}


} } // ONVIF::Media


#endif // ONVIF_Media_GetVideoSourceConfigurationOptionsResponse_INCLUDED
