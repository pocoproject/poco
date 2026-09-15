// 
// GetCompatibleMetadataConfigurations.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Media_GetCompatibleMetadataConfigurations_INCLUDED
#define ONVIF_Media_GetCompatibleMetadataConfigurations_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Media {


//@ name="#GetCompatibleMetadataConfigurations"
//@ namespace="http://www.onvif.org/ver10/media/wsdl"
//@ serialize
class ONVIF_API GetCompatibleMetadataConfigurations
{
public:
	GetCompatibleMetadataConfigurations();

	GetCompatibleMetadataConfigurations(const std::string& profileToken);

	virtual ~GetCompatibleMetadataConfigurations();

	const std::string& getProfileToken() const;

	void setProfileToken(const std::string& val);

	void setProfileToken(std::string&& val);

private:
	//@ name=ProfileToken
	//@ order=0
	std::string _profileToken;

};


inline const std::string& GetCompatibleMetadataConfigurations::getProfileToken() const
{
	return _profileToken;
}


inline void GetCompatibleMetadataConfigurations::setProfileToken(const std::string& val)
{
	_profileToken = val;
}


inline void GetCompatibleMetadataConfigurations::setProfileToken(std::string&& val)
{
	_profileToken = std::move(val);
}


} } // ONVIF::Media


#endif // ONVIF_Media_GetCompatibleMetadataConfigurations_INCLUDED
