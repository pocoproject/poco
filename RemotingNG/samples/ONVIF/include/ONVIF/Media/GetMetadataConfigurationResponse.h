// 
// GetMetadataConfigurationResponse.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Media_GetMetadataConfigurationResponse_INCLUDED
#define ONVIF_Media_GetMetadataConfigurationResponse_INCLUDED


#include "ONVIF/MetadataConfiguration.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Media {


//@ name="#GetMetadataConfigurationResponse"
//@ namespace="http://www.onvif.org/ver10/media/wsdl"
//@ serialize
class ONVIF_API GetMetadataConfigurationResponse
{
public:
	GetMetadataConfigurationResponse();

	GetMetadataConfigurationResponse(const ONVIF::MetadataConfiguration& configuration);

	virtual ~GetMetadataConfigurationResponse();

	const ONVIF::MetadataConfiguration& getConfiguration() const;

	void setConfiguration(const ONVIF::MetadataConfiguration& val);

	void setConfiguration(ONVIF::MetadataConfiguration&& val);

private:
	//@ name=Configuration
	//@ order=0
	ONVIF::MetadataConfiguration _configuration;

};


inline const ONVIF::MetadataConfiguration& GetMetadataConfigurationResponse::getConfiguration() const
{
	return _configuration;
}


inline void GetMetadataConfigurationResponse::setConfiguration(const ONVIF::MetadataConfiguration& val)
{
	_configuration = val;
}


inline void GetMetadataConfigurationResponse::setConfiguration(ONVIF::MetadataConfiguration&& val)
{
	_configuration = std::move(val);
}


} } // ONVIF::Media


#endif // ONVIF_Media_GetMetadataConfigurationResponse_INCLUDED
