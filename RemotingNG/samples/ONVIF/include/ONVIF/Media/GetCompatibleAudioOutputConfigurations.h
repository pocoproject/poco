// 
// GetCompatibleAudioOutputConfigurations.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Media_GetCompatibleAudioOutputConfigurations_INCLUDED
#define ONVIF_Media_GetCompatibleAudioOutputConfigurations_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Media {


//@ name="#GetCompatibleAudioOutputConfigurations"
//@ namespace="http://www.onvif.org/ver10/media/wsdl"
//@ serialize
class ONVIF_API GetCompatibleAudioOutputConfigurations
{
public:
	GetCompatibleAudioOutputConfigurations();

	GetCompatibleAudioOutputConfigurations(const std::string& profileToken);

	virtual ~GetCompatibleAudioOutputConfigurations();

	const std::string& getProfileToken() const;

	void setProfileToken(const std::string& val);

	void setProfileToken(std::string&& val);

private:
	//@ name=ProfileToken
	//@ order=0
	std::string _profileToken;

};


inline const std::string& GetCompatibleAudioOutputConfigurations::getProfileToken() const
{
	return _profileToken;
}


inline void GetCompatibleAudioOutputConfigurations::setProfileToken(const std::string& val)
{
	_profileToken = val;
}


inline void GetCompatibleAudioOutputConfigurations::setProfileToken(std::string&& val)
{
	_profileToken = std::move(val);
}


} } // ONVIF::Media


#endif // ONVIF_Media_GetCompatibleAudioOutputConfigurations_INCLUDED
