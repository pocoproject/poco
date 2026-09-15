// 
// SecurityCapabilities.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_SecurityCapabilities_INCLUDED
#define ONVIF_SecurityCapabilities_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
class SecurityCapabilitiesExtension;
} 


namespace ONVIF {


//@ name=SecurityCapabilities
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API SecurityCapabilities
{
public:
	SecurityCapabilities();

	SecurityCapabilities(
		bool tLS1_1, 
		bool tLS1_2, 
		bool onboardKeyGeneration, 
		bool accessPolicyConfig, 
		bool x_509Token, 
		bool sAMLToken, 
		bool kerberosToken, 
		bool rELToken, 
		const Poco::SharedPtr<SecurityCapabilitiesExtension>& extension);

	virtual ~SecurityCapabilities();

	bool getAccessPolicyConfig() const;

	const Poco::SharedPtr<SecurityCapabilitiesExtension>& getExtension() const;

	bool getKerberosToken() const;

	bool getOnboardKeyGeneration() const;

	bool getRELToken() const;

	bool getSAMLToken() const;

	bool getTLS1_1() const;

	bool getTLS1_2() const;

	bool getX_509Token() const;

	void setAccessPolicyConfig(bool val);

	void setExtension(const Poco::SharedPtr<SecurityCapabilitiesExtension>& val);

	void setExtension(Poco::SharedPtr<SecurityCapabilitiesExtension>&& val);

	void setKerberosToken(bool val);

	void setOnboardKeyGeneration(bool val);

	void setRELToken(bool val);

	void setSAMLToken(bool val);

	void setTLS1_1(bool val);

	void setTLS1_2(bool val);

	void setX_509Token(bool val);

private:
	//@ name="TLS1.1"
	//@ order=0
	bool _tLS1_1;

	//@ name="TLS1.2"
	//@ order=1
	bool _tLS1_2;

	//@ name=OnboardKeyGeneration
	//@ order=2
	bool _onboardKeyGeneration;

	//@ name=AccessPolicyConfig
	//@ order=3
	bool _accessPolicyConfig;

	//@ name="X.509Token"
	//@ order=4
	bool _x_509Token;

	//@ name=SAMLToken
	//@ order=5
	bool _sAMLToken;

	//@ name=KerberosToken
	//@ order=6
	bool _kerberosToken;

	//@ name=RELToken
	//@ order=7
	bool _rELToken;

	//@ mandatory=false
	//@ name=Extension
	//@ order=8
	Poco::SharedPtr<SecurityCapabilitiesExtension> _extension;

};


inline bool SecurityCapabilities::getAccessPolicyConfig() const
{
	return _accessPolicyConfig;
}


inline const Poco::SharedPtr<SecurityCapabilitiesExtension>& SecurityCapabilities::getExtension() const
{
	return _extension;
}


inline bool SecurityCapabilities::getKerberosToken() const
{
	return _kerberosToken;
}


inline bool SecurityCapabilities::getOnboardKeyGeneration() const
{
	return _onboardKeyGeneration;
}


inline bool SecurityCapabilities::getRELToken() const
{
	return _rELToken;
}


inline bool SecurityCapabilities::getSAMLToken() const
{
	return _sAMLToken;
}


inline bool SecurityCapabilities::getTLS1_1() const
{
	return _tLS1_1;
}


inline bool SecurityCapabilities::getTLS1_2() const
{
	return _tLS1_2;
}


inline bool SecurityCapabilities::getX_509Token() const
{
	return _x_509Token;
}


inline void SecurityCapabilities::setAccessPolicyConfig(bool val)
{
	_accessPolicyConfig = val;
}


inline void SecurityCapabilities::setExtension(const Poco::SharedPtr<SecurityCapabilitiesExtension>& val)
{
	_extension = val;
}


inline void SecurityCapabilities::setExtension(Poco::SharedPtr<SecurityCapabilitiesExtension>&& val)
{
	_extension = std::move(val);
}


inline void SecurityCapabilities::setKerberosToken(bool val)
{
	_kerberosToken = val;
}


inline void SecurityCapabilities::setOnboardKeyGeneration(bool val)
{
	_onboardKeyGeneration = val;
}


inline void SecurityCapabilities::setRELToken(bool val)
{
	_rELToken = val;
}


inline void SecurityCapabilities::setSAMLToken(bool val)
{
	_sAMLToken = val;
}


inline void SecurityCapabilities::setTLS1_1(bool val)
{
	_tLS1_1 = val;
}


inline void SecurityCapabilities::setTLS1_2(bool val)
{
	_tLS1_2 = val;
}


inline void SecurityCapabilities::setX_509Token(bool val)
{
	_x_509Token = val;
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/SecurityCapabilitiesExtension.h"


#endif // ONVIF_SecurityCapabilities_INCLUDED
