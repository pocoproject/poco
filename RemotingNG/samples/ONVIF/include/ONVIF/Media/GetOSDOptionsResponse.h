// 
// GetOSDOptionsResponse.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Media_GetOSDOptionsResponse_INCLUDED
#define ONVIF_Media_GetOSDOptionsResponse_INCLUDED


#include "ONVIF/ONVIF.h"
#include "ONVIF/OSDConfigurationOptions.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Media {


//@ name="#GetOSDOptionsResponse"
//@ namespace="http://www.onvif.org/ver10/media/wsdl"
//@ serialize
class ONVIF_API GetOSDOptionsResponse
{
public:
	GetOSDOptionsResponse();

	GetOSDOptionsResponse(const ONVIF::OSDConfigurationOptions& oSDOptions);

	virtual ~GetOSDOptionsResponse();

	const ONVIF::OSDConfigurationOptions& getOSDOptions() const;

	void setOSDOptions(const ONVIF::OSDConfigurationOptions& val);

	void setOSDOptions(ONVIF::OSDConfigurationOptions&& val);

private:
	//@ name=OSDOptions
	//@ order=0
	ONVIF::OSDConfigurationOptions _oSDOptions;

};


inline const ONVIF::OSDConfigurationOptions& GetOSDOptionsResponse::getOSDOptions() const
{
	return _oSDOptions;
}


inline void GetOSDOptionsResponse::setOSDOptions(const ONVIF::OSDConfigurationOptions& val)
{
	_oSDOptions = val;
}


inline void GetOSDOptionsResponse::setOSDOptions(ONVIF::OSDConfigurationOptions&& val)
{
	_oSDOptions = std::move(val);
}


} } // ONVIF::Media


#endif // ONVIF_Media_GetOSDOptionsResponse_INCLUDED
