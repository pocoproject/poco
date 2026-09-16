// 
// SetDigitalInputConfigurations.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_DeviceIO_SetDigitalInputConfigurations_INCLUDED
#define ONVIF_DeviceIO_SetDigitalInputConfigurations_INCLUDED


#include "ONVIF/DigitalInput.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace DeviceIO {


//@ name="#SetDigitalInputConfigurations"
//@ namespace="http://www.onvif.org/ver10/deviceIO/wsdl"
//@ serialize
class ONVIF_API SetDigitalInputConfigurations
{
public:
	SetDigitalInputConfigurations();

	SetDigitalInputConfigurations(const std::vector<ONVIF::DigitalInput>& digitalInputs);

	virtual ~SetDigitalInputConfigurations();

	const std::vector<ONVIF::DigitalInput>& getDigitalInputs() const;

	std::vector<ONVIF::DigitalInput>& getDigitalInputs();

	void setDigitalInputs(const std::vector<ONVIF::DigitalInput>& val);

	void setDigitalInputs(std::vector<ONVIF::DigitalInput>&& val);

private:
	//@ name=DigitalInputs
	//@ order=0
	std::vector<ONVIF::DigitalInput> _digitalInputs;

};


inline const std::vector<ONVIF::DigitalInput>& SetDigitalInputConfigurations::getDigitalInputs() const
{
	return _digitalInputs;
}


inline std::vector<ONVIF::DigitalInput>& SetDigitalInputConfigurations::getDigitalInputs()
{
	return _digitalInputs;
}


inline void SetDigitalInputConfigurations::setDigitalInputs(const std::vector<ONVIF::DigitalInput>& val)
{
	_digitalInputs = val;
}


inline void SetDigitalInputConfigurations::setDigitalInputs(std::vector<ONVIF::DigitalInput>&& val)
{
	_digitalInputs = std::move(val);
}


} } // ONVIF::DeviceIO


#endif // ONVIF_DeviceIO_SetDigitalInputConfigurations_INCLUDED
