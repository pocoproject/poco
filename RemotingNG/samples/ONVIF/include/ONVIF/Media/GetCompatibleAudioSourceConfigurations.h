// 
// GetCompatibleAudioSourceConfigurations.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Media_GetCompatibleAudioSourceConfigurations_INCLUDED
#define ONVIF_Media_GetCompatibleAudioSourceConfigurations_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Media {


//@ name="#GetCompatibleAudioSourceConfigurations"
//@ namespace="http://www.onvif.org/ver10/media/wsdl"
//@ serialize
class ONVIF_API GetCompatibleAudioSourceConfigurations
{
public:
	GetCompatibleAudioSourceConfigurations();

	GetCompatibleAudioSourceConfigurations(const std::string& profileToken);

	virtual ~GetCompatibleAudioSourceConfigurations();

	const std::string& getProfileToken() const;

	void setProfileToken(const std::string& val);

	void setProfileToken(std::string&& val);

private:
	//@ name=ProfileToken
	//@ order=0
	std::string _profileToken;

};


inline const std::string& GetCompatibleAudioSourceConfigurations::getProfileToken() const
{
	return _profileToken;
}


inline void GetCompatibleAudioSourceConfigurations::setProfileToken(const std::string& val)
{
	_profileToken = val;
}


inline void GetCompatibleAudioSourceConfigurations::setProfileToken(std::string&& val)
{
	_profileToken = std::move(val);
}


} } // ONVIF::Media


#endif // ONVIF_Media_GetCompatibleAudioSourceConfigurations_INCLUDED
