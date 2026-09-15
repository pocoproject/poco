// 
// AudioOutput.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_AudioOutput_INCLUDED
#define ONVIF_AudioOutput_INCLUDED


#include "ONVIF/DeviceEntity.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=AudioOutput
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API AudioOutput: public DeviceEntity
{
public:
	AudioOutput();

	AudioOutput(const std::string& token);

	virtual ~AudioOutput();

};


} // ONVIF


#endif // ONVIF_AudioOutput_INCLUDED
