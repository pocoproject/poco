// 
// GetStreamUriResponse.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Media_GetStreamUriResponse_INCLUDED
#define ONVIF_Media_GetStreamUriResponse_INCLUDED


#include "ONVIF/MediaUri.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Media {


//@ name="#GetStreamUriResponse"
//@ namespace="http://www.onvif.org/ver10/media/wsdl"
//@ serialize
class ONVIF_API GetStreamUriResponse
{
public:
	GetStreamUriResponse();

	GetStreamUriResponse(const ONVIF::MediaUri& mediaUri);

	virtual ~GetStreamUriResponse();

	const ONVIF::MediaUri& getMediaUri() const;

	void setMediaUri(const ONVIF::MediaUri& val);

	void setMediaUri(ONVIF::MediaUri&& val);

private:
	//@ name=MediaUri
	//@ order=0
	ONVIF::MediaUri _mediaUri;

};


inline const ONVIF::MediaUri& GetStreamUriResponse::getMediaUri() const
{
	return _mediaUri;
}


inline void GetStreamUriResponse::setMediaUri(const ONVIF::MediaUri& val)
{
	_mediaUri = val;
}


inline void GetStreamUriResponse::setMediaUri(ONVIF::MediaUri&& val)
{
	_mediaUri = std::move(val);
}


} } // ONVIF::Media


#endif // ONVIF_Media_GetStreamUriResponse_INCLUDED
