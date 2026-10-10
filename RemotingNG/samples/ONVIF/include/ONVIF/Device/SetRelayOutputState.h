// 
// SetRelayOutputState.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Device_SetRelayOutputState_INCLUDED
#define ONVIF_Device_SetRelayOutputState_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Device {


//@ name="#SetRelayOutputState"
//@ namespace="http://www.onvif.org/ver10/device/wsdl"
//@ serialize
class ONVIF_API SetRelayOutputState
{
public:
	SetRelayOutputState();

	SetRelayOutputState(
		const std::string& relayOutputToken, 
		const std::string& logicalState);

	virtual ~SetRelayOutputState();

	const std::string& getLogicalState() const;

	const std::string& getRelayOutputToken() const;

	void setLogicalState(const std::string& val);

	void setLogicalState(std::string&& val);

	void setRelayOutputToken(const std::string& val);

	void setRelayOutputToken(std::string&& val);

private:
	//@ name=RelayOutputToken
	//@ order=0
	std::string _relayOutputToken;

	//@ name=LogicalState
	//@ order=1
	std::string _logicalState;

};


inline const std::string& SetRelayOutputState::getLogicalState() const
{
	return _logicalState;
}


inline const std::string& SetRelayOutputState::getRelayOutputToken() const
{
	return _relayOutputToken;
}


inline void SetRelayOutputState::setLogicalState(const std::string& val)
{
	_logicalState = val;
}


inline void SetRelayOutputState::setLogicalState(std::string&& val)
{
	_logicalState = std::move(val);
}


inline void SetRelayOutputState::setRelayOutputToken(const std::string& val)
{
	_relayOutputToken = val;
}


inline void SetRelayOutputState::setRelayOutputToken(std::string&& val)
{
	_relayOutputToken = std::move(val);
}


} } // ONVIF::Device


#endif // ONVIF_Device_SetRelayOutputState_INCLUDED
