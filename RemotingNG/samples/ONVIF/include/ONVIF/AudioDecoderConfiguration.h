// 
// AudioDecoderConfiguration.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_AudioDecoderConfiguration_INCLUDED
#define ONVIF_AudioDecoderConfiguration_INCLUDED


#include "ONVIF/ConfigurationEntity.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=AudioDecoderConfiguration
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API AudioDecoderConfiguration: public ConfigurationEntity
{
public:
	AudioDecoderConfiguration();

	AudioDecoderConfiguration(
		const std::string& token, 
		const std::string& name, 
		int useCount);

	virtual ~AudioDecoderConfiguration();

};


} // ONVIF


#endif // ONVIF_AudioDecoderConfiguration_INCLUDED
