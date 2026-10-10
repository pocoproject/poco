// 
// SetRelayOutputSettings.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_DeviceIO_SetRelayOutputSettings_INCLUDED
#define ONVIF_DeviceIO_SetRelayOutputSettings_INCLUDED


#include "ONVIF/ONVIF.h"
#include "ONVIF/RelayOutput.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace DeviceIO {


//@ name="#SetRelayOutputSettings"
//@ namespace="http://www.onvif.org/ver10/deviceIO/wsdl"
//@ serialize
class ONVIF_API SetRelayOutputSettings
{
public:
	SetRelayOutputSettings();

	SetRelayOutputSettings(const ONVIF::RelayOutput& relayOutput);

	virtual ~SetRelayOutputSettings();

	const ONVIF::RelayOutput& getRelayOutput() const;

	void setRelayOutput(const ONVIF::RelayOutput& val);

	void setRelayOutput(ONVIF::RelayOutput&& val);

private:
	//@ name=RelayOutput
	//@ order=0
	ONVIF::RelayOutput _relayOutput;

};


inline const ONVIF::RelayOutput& SetRelayOutputSettings::getRelayOutput() const
{
	return _relayOutput;
}


inline void SetRelayOutputSettings::setRelayOutput(const ONVIF::RelayOutput& val)
{
	_relayOutput = val;
}


inline void SetRelayOutputSettings::setRelayOutput(ONVIF::RelayOutput&& val)
{
	_relayOutput = std::move(val);
}


} } // ONVIF::DeviceIO


#endif // ONVIF_DeviceIO_SetRelayOutputSettings_INCLUDED
