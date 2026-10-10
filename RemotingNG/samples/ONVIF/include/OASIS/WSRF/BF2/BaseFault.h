// 
// BaseFault.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef OASIS_WSRF_BF2_BaseFault_INCLUDED
#define OASIS_WSRF_BF2_BaseFault_INCLUDED


#include "OASIS/WSRF/BF2/Description.h"
#include "ONVIF/ONVIF.h"
#include "Poco/DateTime.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace OASIS {
namespace WSRF {
namespace BF2 {
class ErrorCode;
class FaultCause;
} } } 
namespace W3 {
namespace Addressing {
class EndpointReference;
} } 


namespace OASIS {
namespace WSRF {
namespace BF2 {


//@ name=BaseFaultType
//@ namespace="http://docs.oasis-open.org/wsrf/bf-2"
//@ serialize
class ONVIF_API BaseFault
{
public:
	BaseFault();

	BaseFault(
		const Poco::DateTime& timestamp, 
		const Poco::SharedPtr<W3::Addressing::EndpointReference>& originator, 
		const Poco::SharedPtr<ErrorCode>& errorCode, 
		const std::vector<Description>& description, 
		const Poco::SharedPtr<FaultCause>& faultCause);

	virtual ~BaseFault();

	const std::vector<Description>& getDescription() const;

	std::vector<Description>& getDescription();

	const Poco::SharedPtr<ErrorCode>& getErrorCode() const;

	const Poco::SharedPtr<FaultCause>& getFaultCause() const;

	const Poco::SharedPtr<W3::Addressing::EndpointReference>& getOriginator() const;

	//@ return={xsdType=dateTime}
	const Poco::DateTime& getTimestamp() const;

	void setDescription(const std::vector<Description>& val);

	void setDescription(std::vector<Description>&& val);

	void setErrorCode(const Poco::SharedPtr<ErrorCode>& val);

	void setErrorCode(Poco::SharedPtr<ErrorCode>&& val);

	void setFaultCause(const Poco::SharedPtr<FaultCause>& val);

	void setFaultCause(Poco::SharedPtr<FaultCause>&& val);

	void setOriginator(const Poco::SharedPtr<W3::Addressing::EndpointReference>& val);

	void setOriginator(Poco::SharedPtr<W3::Addressing::EndpointReference>&& val);

	//@ $val={xsdType=dateTime}
	void setTimestamp(const Poco::DateTime& val);

	//@ $val={xsdType=dateTime}
	void setTimestamp(Poco::DateTime&& val);

private:
	//@ name=Timestamp
	//@ order=0
	//@ xsdType=dateTime
	Poco::DateTime _timestamp;

	//@ mandatory=false
	//@ name=Originator
	//@ order=1
	Poco::SharedPtr<W3::Addressing::EndpointReference> _originator;

	//@ mandatory=false
	//@ name=ErrorCode
	//@ order=2
	Poco::SharedPtr<ErrorCode> _errorCode;

	//@ mandatory=false
	//@ name=Description
	//@ order=3
	std::vector<Description> _description;

	//@ mandatory=false
	//@ name=FaultCause
	//@ order=4
	Poco::SharedPtr<FaultCause> _faultCause;

};


inline const std::vector<Description>& BaseFault::getDescription() const
{
	return _description;
}


inline std::vector<Description>& BaseFault::getDescription()
{
	return _description;
}


inline const Poco::SharedPtr<ErrorCode>& BaseFault::getErrorCode() const
{
	return _errorCode;
}


inline const Poco::SharedPtr<FaultCause>& BaseFault::getFaultCause() const
{
	return _faultCause;
}


inline const Poco::SharedPtr<W3::Addressing::EndpointReference>& BaseFault::getOriginator() const
{
	return _originator;
}


inline const Poco::DateTime& BaseFault::getTimestamp() const
{
	return _timestamp;
}


inline void BaseFault::setDescription(const std::vector<Description>& val)
{
	_description = val;
}


inline void BaseFault::setDescription(std::vector<Description>&& val)
{
	_description = std::move(val);
}


inline void BaseFault::setErrorCode(const Poco::SharedPtr<ErrorCode>& val)
{
	_errorCode = val;
}


inline void BaseFault::setErrorCode(Poco::SharedPtr<ErrorCode>&& val)
{
	_errorCode = std::move(val);
}


inline void BaseFault::setFaultCause(const Poco::SharedPtr<FaultCause>& val)
{
	_faultCause = val;
}


inline void BaseFault::setFaultCause(Poco::SharedPtr<FaultCause>&& val)
{
	_faultCause = std::move(val);
}


inline void BaseFault::setOriginator(const Poco::SharedPtr<W3::Addressing::EndpointReference>& val)
{
	_originator = val;
}


inline void BaseFault::setOriginator(Poco::SharedPtr<W3::Addressing::EndpointReference>&& val)
{
	_originator = std::move(val);
}


inline void BaseFault::setTimestamp(const Poco::DateTime& val)
{
	_timestamp = val;
}


inline void BaseFault::setTimestamp(Poco::DateTime&& val)
{
	_timestamp = std::move(val);
}


} } } // OASIS::WSRF::BF2


// The following headers are required for template instantiation.
#include "OASIS/WSRF/BF2/ErrorCode.h"
#include "OASIS/WSRF/BF2/FaultCause.h"
#include "W3/Addressing/EndpointReference.h"


#endif // OASIS_WSRF_BF2_BaseFault_INCLUDED
