// 
// GetOSDResponse.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Media_GetOSDResponse_INCLUDED
#define ONVIF_Media_GetOSDResponse_INCLUDED


#include "ONVIF/ONVIF.h"
#include "ONVIF/OSDConfiguration.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Media {


//@ name="#GetOSDResponse"
//@ namespace="http://www.onvif.org/ver10/media/wsdl"
//@ serialize
class ONVIF_API GetOSDResponse
{
public:
	GetOSDResponse();

	GetOSDResponse(const ONVIF::OSDConfiguration& oSD);

	virtual ~GetOSDResponse();

	const ONVIF::OSDConfiguration& getOSD() const;

	void setOSD(const ONVIF::OSDConfiguration& val);

	void setOSD(ONVIF::OSDConfiguration&& val);

private:
	//@ name=OSD
	//@ order=0
	ONVIF::OSDConfiguration _oSD;

};


inline const ONVIF::OSDConfiguration& GetOSDResponse::getOSD() const
{
	return _oSD;
}


inline void GetOSDResponse::setOSD(const ONVIF::OSDConfiguration& val)
{
	_oSD = val;
}


inline void GetOSDResponse::setOSD(ONVIF::OSDConfiguration&& val)
{
	_oSD = std::move(val);
}


} } // ONVIF::Media


#endif // ONVIF_Media_GetOSDResponse_INCLUDED
