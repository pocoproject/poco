// 
// Fault.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef W3_SOAP_Envelope_Fault_INCLUDED
#define W3_SOAP_Envelope_Fault_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include "Poco/URI.h"
#include "W3/SOAP/Envelope/Faultcode.h"
#include "W3/SOAP/Envelope/Faultreason.h"
#include <string>
#include <vector>


namespace W3 {
namespace SOAP {
namespace Envelope {
class Detail;
} } } 


namespace W3 {
namespace SOAP {
namespace Envelope {


//@ name=Fault
//@ namespace="http://www.w3.org/2003/05/soap-envelope"
//@ serialize
class ONVIF_API Fault
{
public:
	Fault();

	Fault(
		const Faultcode& code, 
		const Faultreason& reason, 
		const Poco::Optional<Poco::URI>& node, 
		const Poco::Optional<Poco::URI>& role, 
		const Poco::SharedPtr<Detail>& detail);

	virtual ~Fault();

	const Faultcode& getCode() const;

	const Poco::SharedPtr<Detail>& getDetail() const;

	const Poco::Optional<Poco::URI>& getNode() const;

	const Faultreason& getReason() const;

	const Poco::Optional<Poco::URI>& getRole() const;

	void setCode(const Faultcode& val);

	void setCode(Faultcode&& val);

	void setDetail(const Poco::SharedPtr<Detail>& val);

	void setDetail(Poco::SharedPtr<Detail>&& val);

	void setNode(const Poco::Optional<Poco::URI>& val);

	void setNode(Poco::Optional<Poco::URI>&& val);

	void setReason(const Faultreason& val);

	void setReason(Faultreason&& val);

	void setRole(const Poco::Optional<Poco::URI>& val);

	void setRole(Poco::Optional<Poco::URI>&& val);

private:
	//@ name=Code
	//@ order=0
	Faultcode _code;

	//@ name=Reason
	//@ order=1
	Faultreason _reason;

	//@ mandatory=false
	//@ name=Node
	//@ order=2
	Poco::Optional<Poco::URI> _node;

	//@ mandatory=false
	//@ name=Role
	//@ order=3
	Poco::Optional<Poco::URI> _role;

	//@ mandatory=false
	//@ name=Detail
	//@ order=4
	Poco::SharedPtr<Detail> _detail;

};


inline const Faultcode& Fault::getCode() const
{
	return _code;
}


inline const Poco::SharedPtr<Detail>& Fault::getDetail() const
{
	return _detail;
}


inline const Poco::Optional<Poco::URI>& Fault::getNode() const
{
	return _node;
}


inline const Faultreason& Fault::getReason() const
{
	return _reason;
}


inline const Poco::Optional<Poco::URI>& Fault::getRole() const
{
	return _role;
}


inline void Fault::setCode(const Faultcode& val)
{
	_code = val;
}


inline void Fault::setCode(Faultcode&& val)
{
	_code = std::move(val);
}


inline void Fault::setDetail(const Poco::SharedPtr<Detail>& val)
{
	_detail = val;
}


inline void Fault::setDetail(Poco::SharedPtr<Detail>&& val)
{
	_detail = std::move(val);
}


inline void Fault::setNode(const Poco::Optional<Poco::URI>& val)
{
	_node = val;
}


inline void Fault::setNode(Poco::Optional<Poco::URI>&& val)
{
	_node = std::move(val);
}


inline void Fault::setReason(const Faultreason& val)
{
	_reason = val;
}


inline void Fault::setReason(Faultreason&& val)
{
	_reason = std::move(val);
}


inline void Fault::setRole(const Poco::Optional<Poco::URI>& val)
{
	_role = val;
}


inline void Fault::setRole(Poco::Optional<Poco::URI>&& val)
{
	_role = std::move(val);
}


} } } // W3::SOAP::Envelope


// The following headers are required for template instantiation.
#include "W3/SOAP/Envelope/Detail.h"


#endif // W3_SOAP_Envelope_Fault_INCLUDED
