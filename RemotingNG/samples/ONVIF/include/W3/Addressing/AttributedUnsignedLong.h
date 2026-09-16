// 
// AttributedUnsignedLong.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef W3_Addressing_AttributedUnsignedLong_INCLUDED
#define W3_Addressing_AttributedUnsignedLong_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include "Poco/Types.h"
#include <string>
#include <vector>


namespace W3 {
namespace Addressing {


//@ name=AttributedUnsignedLongType
//@ namespace="http://www.w3.org/2005/08/addressing"
//@ serialize
class ONVIF_API AttributedUnsignedLong
{
public:
	AttributedUnsignedLong();

	AttributedUnsignedLong(Poco::Optional<Poco::UInt64> value);

	virtual ~AttributedUnsignedLong();

	Poco::Optional<Poco::UInt64> getValue() const;

	void setValue(Poco::Optional<Poco::UInt64> val);

private:
	//@ inline=true
	//@ mandatory=false
	//@ name=value
	//@ order=0
	Poco::Optional<Poco::UInt64> _value;

};


inline Poco::Optional<Poco::UInt64> AttributedUnsignedLong::getValue() const
{
	return _value;
}


inline void AttributedUnsignedLong::setValue(Poco::Optional<Poco::UInt64> val)
{
	_value = val;
}


} } // W3::Addressing


#endif // W3_Addressing_AttributedUnsignedLong_INCLUDED
