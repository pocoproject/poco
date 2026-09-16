// 
// OSDReference.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_OSDReference_INCLUDED
#define ONVIF_OSDReference_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=OSDReference
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API OSDReference
{
public:
	OSDReference();

	OSDReference(const Poco::Optional<std::string>& value);

	virtual ~OSDReference();

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


inline const Poco::Optional<std::string>& OSDReference::getValue() const
{
	return _value;
}


inline void OSDReference::setValue(const Poco::Optional<std::string>& val)
{
	_value = val;
}


inline void OSDReference::setValue(Poco::Optional<std::string>&& val)
{
	_value = std::move(val);
}


} // ONVIF


#endif // ONVIF_OSDReference_INCLUDED
