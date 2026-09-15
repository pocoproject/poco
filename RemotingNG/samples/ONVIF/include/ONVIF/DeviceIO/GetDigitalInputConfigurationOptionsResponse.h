// 
// GetDigitalInputConfigurationOptionsResponse.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_DeviceIO_GetDigitalInputConfigurationOptionsResponse_INCLUDED
#define ONVIF_DeviceIO_GetDigitalInputConfigurationOptionsResponse_INCLUDED


#include "ONVIF/DeviceIO/DigitalInputConfigurationOptions.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace DeviceIO {


//@ name="#GetDigitalInputConfigurationOptionsResponse"
//@ namespace="http://www.onvif.org/ver10/deviceIO/wsdl"
//@ serialize
class ONVIF_API GetDigitalInputConfigurationOptionsResponse
{
public:
	GetDigitalInputConfigurationOptionsResponse();

	GetDigitalInputConfigurationOptionsResponse(const DigitalInputConfigurationOptions& digitalInputOptions);

	virtual ~GetDigitalInputConfigurationOptionsResponse();

	const DigitalInputConfigurationOptions& getDigitalInputOptions() const;

	void setDigitalInputOptions(const DigitalInputConfigurationOptions& val);

	void setDigitalInputOptions(DigitalInputConfigurationOptions&& val);

private:
	//@ name=DigitalInputOptions
	//@ order=0
	DigitalInputConfigurationOptions _digitalInputOptions;

};


inline const DigitalInputConfigurationOptions& GetDigitalInputConfigurationOptionsResponse::getDigitalInputOptions() const
{
	return _digitalInputOptions;
}


inline void GetDigitalInputConfigurationOptionsResponse::setDigitalInputOptions(const DigitalInputConfigurationOptions& val)
{
	_digitalInputOptions = val;
}


inline void GetDigitalInputConfigurationOptionsResponse::setDigitalInputOptions(DigitalInputConfigurationOptions&& val)
{
	_digitalInputOptions = std::move(val);
}


} } // ONVIF::DeviceIO


#endif // ONVIF_DeviceIO_GetDigitalInputConfigurationOptionsResponse_INCLUDED
