// 
// Base64Binary.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef W3_XMLMIME_Base64Binary_INCLUDED
#define W3_XMLMIME_Base64Binary_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace W3 {
namespace XMLMIME {


//@ name=base64Binary
//@ namespace="http://www.w3.org/2005/05/xmlmime"
//@ serialize
class ONVIF_API Base64Binary
{
public:
	Base64Binary();

	Base64Binary(const Poco::Optional<std::vector<char>>& value);

	virtual ~Base64Binary();

	const Poco::Optional<std::vector<char>>& getValue() const;

	void setValue(const Poco::Optional<std::vector<char>>& val);

	void setValue(Poco::Optional<std::vector<char>>&& val);

private:
	//@ inline=true
	//@ mandatory=false
	//@ name=value
	//@ order=0
	Poco::Optional<std::vector<char>> _value;

};


inline const Poco::Optional<std::vector<char>>& Base64Binary::getValue() const
{
	return _value;
}


inline void Base64Binary::setValue(const Poco::Optional<std::vector<char>>& val)
{
	_value = val;
}


inline void Base64Binary::setValue(Poco::Optional<std::vector<char>>&& val)
{
	_value = std::move(val);
}


} } // W3::XMLMIME


#endif // W3_XMLMIME_Base64Binary_INCLUDED
