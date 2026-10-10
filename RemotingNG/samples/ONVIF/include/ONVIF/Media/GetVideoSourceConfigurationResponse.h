// 
// GetVideoSourceConfigurationResponse.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Media_GetVideoSourceConfigurationResponse_INCLUDED
#define ONVIF_Media_GetVideoSourceConfigurationResponse_INCLUDED


#include "ONVIF/ONVIF.h"
#include "ONVIF/VideoSourceConfiguration.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Media {


//@ name="#GetVideoSourceConfigurationResponse"
//@ namespace="http://www.onvif.org/ver10/media/wsdl"
//@ serialize
class ONVIF_API GetVideoSourceConfigurationResponse
{
public:
	GetVideoSourceConfigurationResponse();

	GetVideoSourceConfigurationResponse(const ONVIF::VideoSourceConfiguration& configuration);

	virtual ~GetVideoSourceConfigurationResponse();

	const ONVIF::VideoSourceConfiguration& getConfiguration() const;

	void setConfiguration(const ONVIF::VideoSourceConfiguration& val);

	void setConfiguration(ONVIF::VideoSourceConfiguration&& val);

private:
	//@ name=Configuration
	//@ order=0
	ONVIF::VideoSourceConfiguration _configuration;

};


inline const ONVIF::VideoSourceConfiguration& GetVideoSourceConfigurationResponse::getConfiguration() const
{
	return _configuration;
}


inline void GetVideoSourceConfigurationResponse::setConfiguration(const ONVIF::VideoSourceConfiguration& val)
{
	_configuration = val;
}


inline void GetVideoSourceConfigurationResponse::setConfiguration(ONVIF::VideoSourceConfiguration&& val)
{
	_configuration = std::move(val);
}


} } // ONVIF::Media


#endif // ONVIF_Media_GetVideoSourceConfigurationResponse_INCLUDED
