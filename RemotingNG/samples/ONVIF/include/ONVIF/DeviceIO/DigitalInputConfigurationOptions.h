// 
// DigitalInputConfigurationOptions.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_DeviceIO_DigitalInputConfigurationOptions_INCLUDED
#define ONVIF_DeviceIO_DigitalInputConfigurationOptions_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace DeviceIO {


//@ name=DigitalInputConfigurationOptions
//@ namespace="http://www.onvif.org/ver10/deviceIO/wsdl"
//@ serialize
class ONVIF_API DigitalInputConfigurationOptions
{
public:
	DigitalInputConfigurationOptions();

	DigitalInputConfigurationOptions(const std::vector<std::string>& idleState);

	virtual ~DigitalInputConfigurationOptions();

	const std::vector<std::string>& getIdleState() const;

	std::vector<std::string>& getIdleState();

	void setIdleState(const std::vector<std::string>& val);

	void setIdleState(std::vector<std::string>&& val);

private:
	//@ name=IdleState
	//@ order=0
	std::vector<std::string> _idleState;

};


inline const std::vector<std::string>& DigitalInputConfigurationOptions::getIdleState() const
{
	return _idleState;
}


inline std::vector<std::string>& DigitalInputConfigurationOptions::getIdleState()
{
	return _idleState;
}


inline void DigitalInputConfigurationOptions::setIdleState(const std::vector<std::string>& val)
{
	_idleState = val;
}


inline void DigitalInputConfigurationOptions::setIdleState(std::vector<std::string>&& val)
{
	_idleState = std::move(val);
}


} } // ONVIF::DeviceIO


#endif // ONVIF_DeviceIO_DigitalInputConfigurationOptions_INCLUDED
