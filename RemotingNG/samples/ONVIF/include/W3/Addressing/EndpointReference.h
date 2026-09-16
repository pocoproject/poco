// 
// EndpointReference.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef W3_Addressing_EndpointReference_INCLUDED
#define W3_Addressing_EndpointReference_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include "W3/Addressing/AttributedURI.h"
#include <string>
#include <vector>


namespace W3 {
namespace Addressing {
class Metadata;
class ReferenceParameters;
} } 


namespace W3 {
namespace Addressing {


//@ name=EndpointReferenceType
//@ namespace="http://www.w3.org/2005/08/addressing"
//@ serialize
class ONVIF_API EndpointReference
{
public:
	EndpointReference();

	EndpointReference(
		const AttributedURI& address, 
		const Poco::SharedPtr<ReferenceParameters>& referenceParameters, 
		const Poco::SharedPtr<Metadata>& metadata);

	virtual ~EndpointReference();

	const AttributedURI& getAddress() const;

	const Poco::SharedPtr<Metadata>& getMetadata() const;

	const Poco::SharedPtr<ReferenceParameters>& getReferenceParameters() const;

	void setAddress(const AttributedURI& val);

	void setAddress(AttributedURI&& val);

	void setMetadata(const Poco::SharedPtr<Metadata>& val);

	void setMetadata(Poco::SharedPtr<Metadata>&& val);

	void setReferenceParameters(const Poco::SharedPtr<ReferenceParameters>& val);

	void setReferenceParameters(Poco::SharedPtr<ReferenceParameters>&& val);

private:
	//@ name=Address
	//@ order=0
	AttributedURI _address;

	//@ mandatory=false
	//@ name=ReferenceParameters
	//@ order=1
	Poco::SharedPtr<ReferenceParameters> _referenceParameters;

	//@ mandatory=false
	//@ name=Metadata
	//@ order=2
	Poco::SharedPtr<Metadata> _metadata;

};


inline const AttributedURI& EndpointReference::getAddress() const
{
	return _address;
}


inline const Poco::SharedPtr<Metadata>& EndpointReference::getMetadata() const
{
	return _metadata;
}


inline const Poco::SharedPtr<ReferenceParameters>& EndpointReference::getReferenceParameters() const
{
	return _referenceParameters;
}


inline void EndpointReference::setAddress(const AttributedURI& val)
{
	_address = val;
}


inline void EndpointReference::setAddress(AttributedURI&& val)
{
	_address = std::move(val);
}


inline void EndpointReference::setMetadata(const Poco::SharedPtr<Metadata>& val)
{
	_metadata = val;
}


inline void EndpointReference::setMetadata(Poco::SharedPtr<Metadata>&& val)
{
	_metadata = std::move(val);
}


inline void EndpointReference::setReferenceParameters(const Poco::SharedPtr<ReferenceParameters>& val)
{
	_referenceParameters = val;
}


inline void EndpointReference::setReferenceParameters(Poco::SharedPtr<ReferenceParameters>&& val)
{
	_referenceParameters = std::move(val);
}


} } // W3::Addressing


// The following headers are required for template instantiation.
#include "W3/Addressing/Metadata.h"
#include "W3/Addressing/ReferenceParameters.h"


#endif // W3_Addressing_EndpointReference_INCLUDED
