// 
// AttributedQName.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef W3_Addressing_AttributedQName_INCLUDED
#define W3_Addressing_AttributedQName_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace W3 {
namespace Addressing {


//@ name=AttributedQNameType
//@ namespace="http://www.w3.org/2005/08/addressing"
//@ serialize
class ONVIF_API AttributedQName
{
public:
	AttributedQName();

	AttributedQName(const Poco::Optional<std::string>& value);

	virtual ~AttributedQName();

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


inline const Poco::Optional<std::string>& AttributedQName::getValue() const
{
	return _value;
}


inline void AttributedQName::setValue(const Poco::Optional<std::string>& val)
{
	_value = val;
}


inline void AttributedQName::setValue(Poco::Optional<std::string>&& val)
{
	_value = std::move(val);
}


} } // W3::Addressing


#endif // W3_Addressing_AttributedQName_INCLUDED
