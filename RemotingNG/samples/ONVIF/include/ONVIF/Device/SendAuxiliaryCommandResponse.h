// 
// SendAuxiliaryCommandResponse.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Device_SendAuxiliaryCommandResponse_INCLUDED
#define ONVIF_Device_SendAuxiliaryCommandResponse_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Device {


//@ name="#SendAuxiliaryCommandResponse"
//@ namespace="http://www.onvif.org/ver10/device/wsdl"
//@ serialize
class ONVIF_API SendAuxiliaryCommandResponse
{
public:
	SendAuxiliaryCommandResponse();

	SendAuxiliaryCommandResponse(const Poco::Optional<std::string>& auxiliaryCommandResponse);

	virtual ~SendAuxiliaryCommandResponse();

	const Poco::Optional<std::string>& getAuxiliaryCommandResponse() const;

	void setAuxiliaryCommandResponse(const Poco::Optional<std::string>& val);

	void setAuxiliaryCommandResponse(Poco::Optional<std::string>&& val);

private:
	//@ mandatory=false
	//@ name=AuxiliaryCommandResponse
	//@ order=0
	Poco::Optional<std::string> _auxiliaryCommandResponse;

};


inline const Poco::Optional<std::string>& SendAuxiliaryCommandResponse::getAuxiliaryCommandResponse() const
{
	return _auxiliaryCommandResponse;
}


inline void SendAuxiliaryCommandResponse::setAuxiliaryCommandResponse(const Poco::Optional<std::string>& val)
{
	_auxiliaryCommandResponse = val;
}


inline void SendAuxiliaryCommandResponse::setAuxiliaryCommandResponse(Poco::Optional<std::string>&& val)
{
	_auxiliaryCommandResponse = std::move(val);
}


} } // ONVIF::Device


#endif // ONVIF_Device_SendAuxiliaryCommandResponse_INCLUDED
