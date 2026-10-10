// 
// MediaCapabilitiesExtension.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_MediaCapabilitiesExtension_INCLUDED
#define ONVIF_MediaCapabilitiesExtension_INCLUDED


#include "ONVIF/ONVIF.h"
#include "ONVIF/ProfileCapabilities.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=MediaCapabilitiesExtension
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API MediaCapabilitiesExtension
{
public:
	MediaCapabilitiesExtension();

	MediaCapabilitiesExtension(const ProfileCapabilities& profileCapabilities);

	virtual ~MediaCapabilitiesExtension();

	const ProfileCapabilities& getProfileCapabilities() const;

	void setProfileCapabilities(const ProfileCapabilities& val);

	void setProfileCapabilities(ProfileCapabilities&& val);

private:
	//@ name=ProfileCapabilities
	//@ order=0
	ProfileCapabilities _profileCapabilities;

};


inline const ProfileCapabilities& MediaCapabilitiesExtension::getProfileCapabilities() const
{
	return _profileCapabilities;
}


inline void MediaCapabilitiesExtension::setProfileCapabilities(const ProfileCapabilities& val)
{
	_profileCapabilities = val;
}


inline void MediaCapabilitiesExtension::setProfileCapabilities(ProfileCapabilities&& val)
{
	_profileCapabilities = std::move(val);
}


} // ONVIF


#endif // ONVIF_MediaCapabilitiesExtension_INCLUDED
