// 
// GetAudioSourceConfigurationOptions.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_DeviceIO_GetAudioSourceConfigurationOptions_INCLUDED
#define ONVIF_DeviceIO_GetAudioSourceConfigurationOptions_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace DeviceIO {


//@ name="#GetAudioSourceConfigurationOptions"
//@ namespace="http://www.onvif.org/ver10/deviceIO/wsdl"
//@ serialize
class ONVIF_API GetAudioSourceConfigurationOptions
{
public:
	GetAudioSourceConfigurationOptions();

	GetAudioSourceConfigurationOptions(const std::string& audioSourceToken);

	virtual ~GetAudioSourceConfigurationOptions();

	const std::string& getAudioSourceToken() const;

	void setAudioSourceToken(const std::string& val);

	void setAudioSourceToken(std::string&& val);

private:
	//@ name=AudioSourceToken
	//@ order=0
	std::string _audioSourceToken;

};


inline const std::string& GetAudioSourceConfigurationOptions::getAudioSourceToken() const
{
	return _audioSourceToken;
}


inline void GetAudioSourceConfigurationOptions::setAudioSourceToken(const std::string& val)
{
	_audioSourceToken = val;
}


inline void GetAudioSourceConfigurationOptions::setAudioSourceToken(std::string&& val)
{
	_audioSourceToken = std::move(val);
}


} } // ONVIF::DeviceIO


#endif // ONVIF_DeviceIO_GetAudioSourceConfigurationOptions_INCLUDED
