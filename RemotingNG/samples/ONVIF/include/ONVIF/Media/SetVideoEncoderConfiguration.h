// 
// SetVideoEncoderConfiguration.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Media_SetVideoEncoderConfiguration_INCLUDED
#define ONVIF_Media_SetVideoEncoderConfiguration_INCLUDED


#include "ONVIF/ONVIF.h"
#include "ONVIF/VideoEncoderConfiguration.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Media {


//@ name="#SetVideoEncoderConfiguration"
//@ namespace="http://www.onvif.org/ver10/media/wsdl"
//@ serialize
class ONVIF_API SetVideoEncoderConfiguration
{
public:
	SetVideoEncoderConfiguration();

	SetVideoEncoderConfiguration(
		const ONVIF::VideoEncoderConfiguration& configuration, 
		bool forcePersistence);

	virtual ~SetVideoEncoderConfiguration();

	const ONVIF::VideoEncoderConfiguration& getConfiguration() const;

	bool getForcePersistence() const;

	void setConfiguration(const ONVIF::VideoEncoderConfiguration& val);

	void setConfiguration(ONVIF::VideoEncoderConfiguration&& val);

	void setForcePersistence(bool val);

private:
	//@ name=Configuration
	//@ order=0
	ONVIF::VideoEncoderConfiguration _configuration;

	//@ name=ForcePersistence
	//@ order=1
	bool _forcePersistence;

};


inline const ONVIF::VideoEncoderConfiguration& SetVideoEncoderConfiguration::getConfiguration() const
{
	return _configuration;
}


inline bool SetVideoEncoderConfiguration::getForcePersistence() const
{
	return _forcePersistence;
}


inline void SetVideoEncoderConfiguration::setConfiguration(const ONVIF::VideoEncoderConfiguration& val)
{
	_configuration = val;
}


inline void SetVideoEncoderConfiguration::setConfiguration(ONVIF::VideoEncoderConfiguration&& val)
{
	_configuration = std::move(val);
}


inline void SetVideoEncoderConfiguration::setForcePersistence(bool val)
{
	_forcePersistence = val;
}


} } // ONVIF::Media


#endif // ONVIF_Media_SetVideoEncoderConfiguration_INCLUDED
