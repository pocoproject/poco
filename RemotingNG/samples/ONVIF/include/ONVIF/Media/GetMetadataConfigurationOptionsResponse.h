// 
// GetMetadataConfigurationOptionsResponse.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Media_GetMetadataConfigurationOptionsResponse_INCLUDED
#define ONVIF_Media_GetMetadataConfigurationOptionsResponse_INCLUDED


#include "ONVIF/MetadataConfigurationOptions.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Media {


//@ name="#GetMetadataConfigurationOptionsResponse"
//@ namespace="http://www.onvif.org/ver10/media/wsdl"
//@ serialize
class ONVIF_API GetMetadataConfigurationOptionsResponse
{
public:
	GetMetadataConfigurationOptionsResponse();

	GetMetadataConfigurationOptionsResponse(const ONVIF::MetadataConfigurationOptions& options);

	virtual ~GetMetadataConfigurationOptionsResponse();

	const ONVIF::MetadataConfigurationOptions& getOptions() const;

	void setOptions(const ONVIF::MetadataConfigurationOptions& val);

	void setOptions(ONVIF::MetadataConfigurationOptions&& val);

private:
	//@ name=Options
	//@ order=0
	ONVIF::MetadataConfigurationOptions _options;

};


inline const ONVIF::MetadataConfigurationOptions& GetMetadataConfigurationOptionsResponse::getOptions() const
{
	return _options;
}


inline void GetMetadataConfigurationOptionsResponse::setOptions(const ONVIF::MetadataConfigurationOptions& val)
{
	_options = val;
}


inline void GetMetadataConfigurationOptionsResponse::setOptions(ONVIF::MetadataConfigurationOptions&& val)
{
	_options = std::move(val);
}


} } // ONVIF::Media


#endif // ONVIF_Media_GetMetadataConfigurationOptionsResponse_INCLUDED
