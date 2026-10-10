// 
// Faultcode.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef W3_SOAP_Envelope_Faultcode_INCLUDED
#define W3_SOAP_Envelope_Faultcode_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace W3 {
namespace SOAP {
namespace Envelope {
class Subcode;
} } } 


namespace W3 {
namespace SOAP {
namespace Envelope {


//@ name=faultcode
//@ namespace="http://www.w3.org/2003/05/soap-envelope"
//@ serialize
class ONVIF_API Faultcode
{
public:
	Faultcode();

	Faultcode(
		const std::string& value, 
		const Poco::SharedPtr<Subcode>& subcode);

	virtual ~Faultcode();

	const Poco::SharedPtr<Subcode>& getSubcode() const;

	const std::string& getValue() const;

	void setSubcode(const Poco::SharedPtr<Subcode>& val);

	void setSubcode(Poco::SharedPtr<Subcode>&& val);

	void setValue(const std::string& val);

	void setValue(std::string&& val);

private:
	//@ name=Value
	//@ order=0
	std::string _value;

	//@ mandatory=false
	//@ name=Subcode
	//@ order=1
	Poco::SharedPtr<Subcode> _subcode;

};


inline const Poco::SharedPtr<Subcode>& Faultcode::getSubcode() const
{
	return _subcode;
}


inline const std::string& Faultcode::getValue() const
{
	return _value;
}


inline void Faultcode::setSubcode(const Poco::SharedPtr<Subcode>& val)
{
	_subcode = val;
}


inline void Faultcode::setSubcode(Poco::SharedPtr<Subcode>&& val)
{
	_subcode = std::move(val);
}


inline void Faultcode::setValue(const std::string& val)
{
	_value = val;
}


inline void Faultcode::setValue(std::string&& val)
{
	_value = std::move(val);
}


} } } // W3::SOAP::Envelope


// The following headers are required for template instantiation.
#include "W3/SOAP/Envelope/Subcode.h"


#endif // W3_SOAP_Envelope_Faultcode_INCLUDED
