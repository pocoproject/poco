// 
// RemoveAudioSourceConfiguration.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Media_RemoveAudioSourceConfiguration_INCLUDED
#define ONVIF_Media_RemoveAudioSourceConfiguration_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Media {


//@ name="#RemoveAudioSourceConfiguration"
//@ namespace="http://www.onvif.org/ver10/media/wsdl"
//@ serialize
class ONVIF_API RemoveAudioSourceConfiguration
{
public:
	RemoveAudioSourceConfiguration();

	RemoveAudioSourceConfiguration(const std::string& profileToken);

	virtual ~RemoveAudioSourceConfiguration();

	const std::string& getProfileToken() const;

	void setProfileToken(const std::string& val);

	void setProfileToken(std::string&& val);

private:
	//@ name=ProfileToken
	//@ order=0
	std::string _profileToken;

};


inline const std::string& RemoveAudioSourceConfiguration::getProfileToken() const
{
	return _profileToken;
}


inline void RemoveAudioSourceConfiguration::setProfileToken(const std::string& val)
{
	_profileToken = val;
}


inline void RemoveAudioSourceConfiguration::setProfileToken(std::string&& val)
{
	_profileToken = std::move(val);
}


} } // ONVIF::Media


#endif // ONVIF_Media_RemoveAudioSourceConfiguration_INCLUDED
