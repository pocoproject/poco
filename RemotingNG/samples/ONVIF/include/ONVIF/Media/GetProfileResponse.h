// 
// GetProfileResponse.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Media_GetProfileResponse_INCLUDED
#define ONVIF_Media_GetProfileResponse_INCLUDED


#include "ONVIF/ONVIF.h"
#include "ONVIF/Profile.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Media {


//@ name="#GetProfileResponse"
//@ namespace="http://www.onvif.org/ver10/media/wsdl"
//@ serialize
class ONVIF_API GetProfileResponse
{
public:
	GetProfileResponse();

	GetProfileResponse(const ONVIF::Profile& profile);

	virtual ~GetProfileResponse();

	const ONVIF::Profile& getProfile() const;

	void setProfile(const ONVIF::Profile& val);

	void setProfile(ONVIF::Profile&& val);

private:
	//@ name=Profile
	//@ order=0
	ONVIF::Profile _profile;

};


inline const ONVIF::Profile& GetProfileResponse::getProfile() const
{
	return _profile;
}


inline void GetProfileResponse::setProfile(const ONVIF::Profile& val)
{
	_profile = val;
}


inline void GetProfileResponse::setProfile(ONVIF::Profile&& val)
{
	_profile = std::move(val);
}


} } // ONVIF::Media


#endif // ONVIF_Media_GetProfileResponse_INCLUDED
