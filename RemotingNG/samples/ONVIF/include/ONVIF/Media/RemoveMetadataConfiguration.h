// 
// RemoveMetadataConfiguration.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Media_RemoveMetadataConfiguration_INCLUDED
#define ONVIF_Media_RemoveMetadataConfiguration_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Media {


//@ name="#RemoveMetadataConfiguration"
//@ namespace="http://www.onvif.org/ver10/media/wsdl"
//@ serialize
class ONVIF_API RemoveMetadataConfiguration
{
public:
	RemoveMetadataConfiguration();

	RemoveMetadataConfiguration(const std::string& profileToken);

	virtual ~RemoveMetadataConfiguration();

	const std::string& getProfileToken() const;

	void setProfileToken(const std::string& val);

	void setProfileToken(std::string&& val);

private:
	//@ name=ProfileToken
	//@ order=0
	std::string _profileToken;

};


inline const std::string& RemoveMetadataConfiguration::getProfileToken() const
{
	return _profileToken;
}


inline void RemoveMetadataConfiguration::setProfileToken(const std::string& val)
{
	_profileToken = val;
}


inline void RemoveMetadataConfiguration::setProfileToken(std::string&& val)
{
	_profileToken = std::move(val);
}


} } // ONVIF::Media


#endif // ONVIF_Media_RemoveMetadataConfiguration_INCLUDED
