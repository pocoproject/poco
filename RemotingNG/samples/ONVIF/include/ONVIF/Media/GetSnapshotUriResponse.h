// 
// GetSnapshotUriResponse.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Media_GetSnapshotUriResponse_INCLUDED
#define ONVIF_Media_GetSnapshotUriResponse_INCLUDED


#include "ONVIF/MediaUri.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Media {


//@ name="#GetSnapshotUriResponse"
//@ namespace="http://www.onvif.org/ver10/media/wsdl"
//@ serialize
class ONVIF_API GetSnapshotUriResponse
{
public:
	GetSnapshotUriResponse();

	GetSnapshotUriResponse(const ONVIF::MediaUri& mediaUri);

	virtual ~GetSnapshotUriResponse();

	const ONVIF::MediaUri& getMediaUri() const;

	void setMediaUri(const ONVIF::MediaUri& val);

	void setMediaUri(ONVIF::MediaUri&& val);

private:
	//@ name=MediaUri
	//@ order=0
	ONVIF::MediaUri _mediaUri;

};


inline const ONVIF::MediaUri& GetSnapshotUriResponse::getMediaUri() const
{
	return _mediaUri;
}


inline void GetSnapshotUriResponse::setMediaUri(const ONVIF::MediaUri& val)
{
	_mediaUri = val;
}


inline void GetSnapshotUriResponse::setMediaUri(ONVIF::MediaUri&& val)
{
	_mediaUri = std::move(val);
}


} } // ONVIF::Media


#endif // ONVIF_Media_GetSnapshotUriResponse_INCLUDED
