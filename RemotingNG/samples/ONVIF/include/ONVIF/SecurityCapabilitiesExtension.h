// 
// SecurityCapabilitiesExtension.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_SecurityCapabilitiesExtension_INCLUDED
#define ONVIF_SecurityCapabilitiesExtension_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
class SecurityCapabilitiesExtension2;
} 


namespace ONVIF {


//@ name=SecurityCapabilitiesExtension
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API SecurityCapabilitiesExtension
{
public:
	SecurityCapabilitiesExtension();

	SecurityCapabilitiesExtension(
		bool tLS1_0, 
		const Poco::SharedPtr<SecurityCapabilitiesExtension2>& extension);

	virtual ~SecurityCapabilitiesExtension();

	const Poco::SharedPtr<SecurityCapabilitiesExtension2>& getExtension() const;

	bool getTLS1_0() const;

	void setExtension(const Poco::SharedPtr<SecurityCapabilitiesExtension2>& val);

	void setExtension(Poco::SharedPtr<SecurityCapabilitiesExtension2>&& val);

	void setTLS1_0(bool val);

private:
	//@ name="TLS1.0"
	//@ order=0
	bool _tLS1_0;

	//@ mandatory=false
	//@ name=Extension
	//@ order=1
	Poco::SharedPtr<SecurityCapabilitiesExtension2> _extension;

};


inline const Poco::SharedPtr<SecurityCapabilitiesExtension2>& SecurityCapabilitiesExtension::getExtension() const
{
	return _extension;
}


inline bool SecurityCapabilitiesExtension::getTLS1_0() const
{
	return _tLS1_0;
}


inline void SecurityCapabilitiesExtension::setExtension(const Poco::SharedPtr<SecurityCapabilitiesExtension2>& val)
{
	_extension = val;
}


inline void SecurityCapabilitiesExtension::setExtension(Poco::SharedPtr<SecurityCapabilitiesExtension2>&& val)
{
	_extension = std::move(val);
}


inline void SecurityCapabilitiesExtension::setTLS1_0(bool val)
{
	_tLS1_0 = val;
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/SecurityCapabilitiesExtension2.h"


#endif // ONVIF_SecurityCapabilitiesExtension_INCLUDED
