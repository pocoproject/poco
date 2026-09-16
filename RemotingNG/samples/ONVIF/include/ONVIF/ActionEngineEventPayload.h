// 
// ActionEngineEventPayload.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_ActionEngineEventPayload_INCLUDED
#define ONVIF_ActionEngineEventPayload_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
class ActionEngineEventPayloadExtension;
} 
namespace W3 {
namespace SOAP {
namespace Envelope {
class Envelope;
class Fault;
} } } 


namespace ONVIF {


//@ name=ActionEngineEventPayload
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API ActionEngineEventPayload
{
public:
	ActionEngineEventPayload();

	ActionEngineEventPayload(
		const Poco::SharedPtr<W3::SOAP::Envelope::Envelope>& requestInfo, 
		const Poco::SharedPtr<W3::SOAP::Envelope::Envelope>& responseInfo, 
		const Poco::SharedPtr<W3::SOAP::Envelope::Fault>& fault, 
		const Poco::SharedPtr<ActionEngineEventPayloadExtension>& extension);

	virtual ~ActionEngineEventPayload();

	const Poco::SharedPtr<ActionEngineEventPayloadExtension>& getExtension() const;

	const Poco::SharedPtr<W3::SOAP::Envelope::Fault>& getFault() const;

	const Poco::SharedPtr<W3::SOAP::Envelope::Envelope>& getRequestInfo() const;

	const Poco::SharedPtr<W3::SOAP::Envelope::Envelope>& getResponseInfo() const;

	void setExtension(const Poco::SharedPtr<ActionEngineEventPayloadExtension>& val);

	void setExtension(Poco::SharedPtr<ActionEngineEventPayloadExtension>&& val);

	void setFault(const Poco::SharedPtr<W3::SOAP::Envelope::Fault>& val);

	void setFault(Poco::SharedPtr<W3::SOAP::Envelope::Fault>&& val);

	void setRequestInfo(const Poco::SharedPtr<W3::SOAP::Envelope::Envelope>& val);

	void setRequestInfo(Poco::SharedPtr<W3::SOAP::Envelope::Envelope>&& val);

	void setResponseInfo(const Poco::SharedPtr<W3::SOAP::Envelope::Envelope>& val);

	void setResponseInfo(Poco::SharedPtr<W3::SOAP::Envelope::Envelope>&& val);

private:
	//@ mandatory=false
	//@ name=RequestInfo
	//@ order=0
	Poco::SharedPtr<W3::SOAP::Envelope::Envelope> _requestInfo;

	//@ mandatory=false
	//@ name=ResponseInfo
	//@ order=1
	Poco::SharedPtr<W3::SOAP::Envelope::Envelope> _responseInfo;

	//@ mandatory=false
	//@ name=Fault
	//@ order=2
	Poco::SharedPtr<W3::SOAP::Envelope::Fault> _fault;

	//@ mandatory=false
	//@ name=Extension
	//@ order=3
	Poco::SharedPtr<ActionEngineEventPayloadExtension> _extension;

};


inline const Poco::SharedPtr<ActionEngineEventPayloadExtension>& ActionEngineEventPayload::getExtension() const
{
	return _extension;
}


inline const Poco::SharedPtr<W3::SOAP::Envelope::Fault>& ActionEngineEventPayload::getFault() const
{
	return _fault;
}


inline const Poco::SharedPtr<W3::SOAP::Envelope::Envelope>& ActionEngineEventPayload::getRequestInfo() const
{
	return _requestInfo;
}


inline const Poco::SharedPtr<W3::SOAP::Envelope::Envelope>& ActionEngineEventPayload::getResponseInfo() const
{
	return _responseInfo;
}


inline void ActionEngineEventPayload::setExtension(const Poco::SharedPtr<ActionEngineEventPayloadExtension>& val)
{
	_extension = val;
}


inline void ActionEngineEventPayload::setExtension(Poco::SharedPtr<ActionEngineEventPayloadExtension>&& val)
{
	_extension = std::move(val);
}


inline void ActionEngineEventPayload::setFault(const Poco::SharedPtr<W3::SOAP::Envelope::Fault>& val)
{
	_fault = val;
}


inline void ActionEngineEventPayload::setFault(Poco::SharedPtr<W3::SOAP::Envelope::Fault>&& val)
{
	_fault = std::move(val);
}


inline void ActionEngineEventPayload::setRequestInfo(const Poco::SharedPtr<W3::SOAP::Envelope::Envelope>& val)
{
	_requestInfo = val;
}


inline void ActionEngineEventPayload::setRequestInfo(Poco::SharedPtr<W3::SOAP::Envelope::Envelope>&& val)
{
	_requestInfo = std::move(val);
}


inline void ActionEngineEventPayload::setResponseInfo(const Poco::SharedPtr<W3::SOAP::Envelope::Envelope>& val)
{
	_responseInfo = val;
}


inline void ActionEngineEventPayload::setResponseInfo(Poco::SharedPtr<W3::SOAP::Envelope::Envelope>&& val)
{
	_responseInfo = std::move(val);
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/ActionEngineEventPayloadExtension.h"
#include "W3/SOAP/Envelope/Envelope.h"
#include "W3/SOAP/Envelope/Fault.h"


#endif // ONVIF_ActionEngineEventPayload_INCLUDED
