// 
// GetProfilesResponse.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Media_GetProfilesResponse_INCLUDED
#define ONVIF_Media_GetProfilesResponse_INCLUDED


#include "ONVIF/ONVIF.h"
#include "ONVIF/Profile.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Media {


//@ name="#GetProfilesResponse"
//@ namespace="http://www.onvif.org/ver10/media/wsdl"
//@ serialize
class ONVIF_API GetProfilesResponse
{
public:
	GetProfilesResponse();

	GetProfilesResponse(const std::vector<ONVIF::Profile>& profiles);

	virtual ~GetProfilesResponse();

	const std::vector<ONVIF::Profile>& getProfiles() const;

	std::vector<ONVIF::Profile>& getProfiles();

	void setProfiles(const std::vector<ONVIF::Profile>& val);

	void setProfiles(std::vector<ONVIF::Profile>&& val);

private:
	//@ mandatory=false
	//@ name=Profiles
	//@ order=0
	std::vector<ONVIF::Profile> _profiles;

};


inline const std::vector<ONVIF::Profile>& GetProfilesResponse::getProfiles() const
{
	return _profiles;
}


inline std::vector<ONVIF::Profile>& GetProfilesResponse::getProfiles()
{
	return _profiles;
}


inline void GetProfilesResponse::setProfiles(const std::vector<ONVIF::Profile>& val)
{
	_profiles = val;
}


inline void GetProfilesResponse::setProfiles(std::vector<ONVIF::Profile>&& val)
{
	_profiles = std::move(val);
}


} } // ONVIF::Media


#endif // ONVIF_Media_GetProfilesResponse_INCLUDED
