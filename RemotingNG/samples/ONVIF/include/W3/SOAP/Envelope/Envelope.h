// 
// Envelope.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef W3_SOAP_Envelope_Envelope_INCLUDED
#define W3_SOAP_Envelope_Envelope_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include "W3/SOAP/Envelope/Body.h"
#include <string>
#include <vector>


namespace W3 {
namespace SOAP {
namespace Envelope {
class Header;
} } } 


namespace W3 {
namespace SOAP {
namespace Envelope {


//@ name=Envelope
//@ namespace="http://www.w3.org/2003/05/soap-envelope"
//@ serialize
class ONVIF_API Envelope
{
public:
	Envelope();

	Envelope(
		const Poco::SharedPtr<Header>& header, 
		const Body& body);

	virtual ~Envelope();

	const Body& getBody() const;

	const Poco::SharedPtr<Header>& getHeader() const;

	void setBody(const Body& val);

	void setBody(Body&& val);

	void setHeader(const Poco::SharedPtr<Header>& val);

	void setHeader(Poco::SharedPtr<Header>&& val);

private:
	//@ mandatory=false
	//@ name=Header
	//@ order=0
	Poco::SharedPtr<Header> _header;

	//@ name=Body
	//@ order=1
	Body _body;

};


inline const Body& Envelope::getBody() const
{
	return _body;
}


inline const Poco::SharedPtr<Header>& Envelope::getHeader() const
{
	return _header;
}


inline void Envelope::setBody(const Body& val)
{
	_body = val;
}


inline void Envelope::setBody(Body&& val)
{
	_body = std::move(val);
}


inline void Envelope::setHeader(const Poco::SharedPtr<Header>& val)
{
	_header = val;
}


inline void Envelope::setHeader(Poco::SharedPtr<Header>&& val)
{
	_header = std::move(val);
}


} } } // W3::SOAP::Envelope


// The following headers are required for template instantiation.
#include "W3/SOAP/Envelope/Header.h"


#endif // W3_SOAP_Envelope_Envelope_INCLUDED
