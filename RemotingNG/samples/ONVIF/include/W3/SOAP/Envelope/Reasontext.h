// 
// Reasontext.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef W3_SOAP_Envelope_Reasontext_INCLUDED
#define W3_SOAP_Envelope_Reasontext_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace W3 {
namespace SOAP {
namespace Envelope {


//@ name=reasontext
//@ namespace="http://www.w3.org/2003/05/soap-envelope"
//@ serialize
class ONVIF_API Reasontext
{
public:
	Reasontext();

	Reasontext(const Poco::Optional<std::string>& value);

	virtual ~Reasontext();

	const Poco::Optional<std::string>& getValue() const;

	void setValue(const Poco::Optional<std::string>& val);

	void setValue(Poco::Optional<std::string>&& val);

private:
	//@ inline=true
	//@ mandatory=false
	//@ name=value
	//@ order=0
	Poco::Optional<std::string> _value;

};


inline const Poco::Optional<std::string>& Reasontext::getValue() const
{
	return _value;
}


inline void Reasontext::setValue(const Poco::Optional<std::string>& val)
{
	_value = val;
}


inline void Reasontext::setValue(Poco::Optional<std::string>&& val)
{
	_value = std::move(val);
}


} } } // W3::SOAP::Envelope


#endif // W3_SOAP_Envelope_Reasontext_INCLUDED
