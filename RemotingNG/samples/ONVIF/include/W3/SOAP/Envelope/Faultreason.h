// 
// Faultreason.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef W3_SOAP_Envelope_Faultreason_INCLUDED
#define W3_SOAP_Envelope_Faultreason_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include "W3/SOAP/Envelope/Reasontext.h"
#include <string>
#include <vector>


namespace W3 {
namespace SOAP {
namespace Envelope {


//@ name=faultreason
//@ namespace="http://www.w3.org/2003/05/soap-envelope"
//@ serialize
class ONVIF_API Faultreason
{
public:
	Faultreason();

	Faultreason(const std::vector<Reasontext>& text);

	virtual ~Faultreason();

	const std::vector<Reasontext>& getText() const;

	std::vector<Reasontext>& getText();

	void setText(const std::vector<Reasontext>& val);

	void setText(std::vector<Reasontext>&& val);

private:
	//@ name=Text
	//@ order=0
	std::vector<Reasontext> _text;

};


inline const std::vector<Reasontext>& Faultreason::getText() const
{
	return _text;
}


inline std::vector<Reasontext>& Faultreason::getText()
{
	return _text;
}


inline void Faultreason::setText(const std::vector<Reasontext>& val)
{
	_text = val;
}


inline void Faultreason::setText(std::vector<Reasontext>&& val)
{
	_text = std::move(val);
}


} } } // W3::SOAP::Envelope


#endif // W3_SOAP_Envelope_Faultreason_INCLUDED
