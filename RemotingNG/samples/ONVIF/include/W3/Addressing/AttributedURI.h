// 
// AttributedURI.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef W3_Addressing_AttributedURI_INCLUDED
#define W3_Addressing_AttributedURI_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include "Poco/URI.h"
#include <string>
#include <vector>


namespace W3 {
namespace Addressing {


//@ name=AttributedURIType
//@ namespace="http://www.w3.org/2005/08/addressing"
//@ serialize
class ONVIF_API AttributedURI
{
public:
	AttributedURI();

	AttributedURI(const Poco::Optional<Poco::URI>& value);

	virtual ~AttributedURI();

	const Poco::Optional<Poco::URI>& getValue() const;

	void setValue(const Poco::Optional<Poco::URI>& val);

	void setValue(Poco::Optional<Poco::URI>&& val);

private:
	//@ inline=true
	//@ mandatory=false
	//@ name=value
	//@ order=0
	Poco::Optional<Poco::URI> _value;

};


inline const Poco::Optional<Poco::URI>& AttributedURI::getValue() const
{
	return _value;
}


inline void AttributedURI::setValue(const Poco::Optional<Poco::URI>& val)
{
	_value = val;
}


inline void AttributedURI::setValue(Poco::Optional<Poco::URI>&& val)
{
	_value = std::move(val);
}


} } // W3::Addressing


#endif // W3_Addressing_AttributedURI_INCLUDED
