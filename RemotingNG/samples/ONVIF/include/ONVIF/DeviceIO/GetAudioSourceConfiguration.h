// 
// GetAudioSourceConfiguration.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_DeviceIO_GetAudioSourceConfiguration_INCLUDED
#define ONVIF_DeviceIO_GetAudioSourceConfiguration_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace DeviceIO {


//@ name="#GetAudioSourceConfiguration"
//@ namespace="http://www.onvif.org/ver10/deviceIO/wsdl"
//@ serialize
class ONVIF_API GetAudioSourceConfiguration
{
public:
	GetAudioSourceConfiguration();

	GetAudioSourceConfiguration(const std::string& audioSourceToken);

	virtual ~GetAudioSourceConfiguration();

	const std::string& getAudioSourceToken() const;

	void setAudioSourceToken(const std::string& val);

	void setAudioSourceToken(std::string&& val);

private:
	//@ name=AudioSourceToken
	//@ order=0
	std::string _audioSourceToken;

};


inline const std::string& GetAudioSourceConfiguration::getAudioSourceToken() const
{
	return _audioSourceToken;
}


inline void GetAudioSourceConfiguration::setAudioSourceToken(const std::string& val)
{
	_audioSourceToken = val;
}


inline void GetAudioSourceConfiguration::setAudioSourceToken(std::string&& val)
{
	_audioSourceToken = std::move(val);
}


} } // ONVIF::DeviceIO


#endif // ONVIF_DeviceIO_GetAudioSourceConfiguration_INCLUDED
