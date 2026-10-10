// 
// NotUnderstood.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef W3_SOAP_Envelope_NotUnderstood_INCLUDED
#define W3_SOAP_Envelope_NotUnderstood_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace W3 {
namespace SOAP {
namespace Envelope {


//@ name=NotUnderstoodType
//@ namespace="http://www.w3.org/2003/05/soap-envelope"
//@ serialize
class ONVIF_API NotUnderstood
{
public:
	NotUnderstood();

	NotUnderstood(const std::string& qname);

	virtual ~NotUnderstood();

	const std::string& getQname() const;

	void setQname(const std::string& val);

	void setQname(std::string&& val);

private:
	//@ name=qname
	//@ order=0
	//@ type=attr
	std::string _qname;

};


inline const std::string& NotUnderstood::getQname() const
{
	return _qname;
}


inline void NotUnderstood::setQname(const std::string& val)
{
	_qname = val;
}


inline void NotUnderstood::setQname(std::string&& val)
{
	_qname = std::move(val);
}


} } } // W3::SOAP::Envelope


#endif // W3_SOAP_Envelope_NotUnderstood_INCLUDED
