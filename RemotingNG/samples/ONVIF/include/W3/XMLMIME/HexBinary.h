// 
// HexBinary.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef W3_XMLMIME_HexBinary_INCLUDED
#define W3_XMLMIME_HexBinary_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace W3 {
namespace XMLMIME {


//@ name=hexBinary
//@ namespace="http://www.w3.org/2005/05/xmlmime"
//@ serialize
class ONVIF_API HexBinary
{
public:
	HexBinary();

	HexBinary(const Poco::Optional<std::string>& value);

	virtual ~HexBinary();

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


inline const Poco::Optional<std::string>& HexBinary::getValue() const
{
	return _value;
}


inline void HexBinary::setValue(const Poco::Optional<std::string>& val)
{
	_value = val;
}


inline void HexBinary::setValue(Poco::Optional<std::string>&& val)
{
	_value = std::move(val);
}


} } // W3::XMLMIME


#endif // W3_XMLMIME_HexBinary_INCLUDED
