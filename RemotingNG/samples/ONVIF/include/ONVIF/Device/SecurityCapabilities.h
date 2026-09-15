// 
// SecurityCapabilities.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Device_SecurityCapabilities_INCLUDED
#define ONVIF_Device_SecurityCapabilities_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Device {


//@ name=SecurityCapabilities
//@ namespace="http://www.onvif.org/ver10/device/wsdl"
//@ serialize
class ONVIF_API SecurityCapabilities
{
public:
	SecurityCapabilities();

	SecurityCapabilities(
		Poco::Optional<bool> tLS1_0, 
		Poco::Optional<bool> tLS1_1, 
		Poco::Optional<bool> tLS1_2, 
		Poco::Optional<bool> onboardKeyGeneration, 
		Poco::Optional<bool> accessPolicyConfig, 
		Poco::Optional<bool> defaultAccessPolicy, 
		Poco::Optional<bool> dot1X, 
		Poco::Optional<bool> remoteUserHandling, 
		Poco::Optional<bool> x_509Token, 
		Poco::Optional<bool> sAMLToken, 
		Poco::Optional<bool> kerberosToken, 
		Poco::Optional<bool> usernameToken, 
		Poco::Optional<bool> httpDigest, 
		Poco::Optional<bool> rELToken, 
		const std::vector<std::vector<int>>& supportedEAPMethods, 
		Poco::Optional<int> maxUsers, 
		Poco::Optional<int> maxUserNameLength, 
		Poco::Optional<int> maxPasswordLength, 
		const std::vector<std::vector<std::string>>& securityPolicies, 
		Poco::Optional<int> maxPasswordHistory, 
		const std::vector<std::vector<std::string>>& hashingAlgorithms);

	virtual ~SecurityCapabilities();

	Poco::Optional<bool> getAccessPolicyConfig() const;

	Poco::Optional<bool> getDefaultAccessPolicy() const;

	Poco::Optional<bool> getDot1X() const;

	const std::vector<std::vector<std::string>>& getHashingAlgorithms() const;

	std::vector<std::vector<std::string>>& getHashingAlgorithms();

	Poco::Optional<bool> getHttpDigest() const;

	Poco::Optional<bool> getKerberosToken() const;

	Poco::Optional<int> getMaxPasswordHistory() const;

	Poco::Optional<int> getMaxPasswordLength() const;

	Poco::Optional<int> getMaxUserNameLength() const;

	Poco::Optional<int> getMaxUsers() const;

	Poco::Optional<bool> getOnboardKeyGeneration() const;

	Poco::Optional<bool> getRELToken() const;

	Poco::Optional<bool> getRemoteUserHandling() const;

	Poco::Optional<bool> getSAMLToken() const;

	const std::vector<std::vector<std::string>>& getSecurityPolicies() const;

	std::vector<std::vector<std::string>>& getSecurityPolicies();

	const std::vector<std::vector<int>>& getSupportedEAPMethods() const;

	std::vector<std::vector<int>>& getSupportedEAPMethods();

	Poco::Optional<bool> getTLS1_0() const;

	Poco::Optional<bool> getTLS1_1() const;

	Poco::Optional<bool> getTLS1_2() const;

	Poco::Optional<bool> getUsernameToken() const;

	Poco::Optional<bool> getX_509Token() const;

	void setAccessPolicyConfig(Poco::Optional<bool> val);

	void setDefaultAccessPolicy(Poco::Optional<bool> val);

	void setDot1X(Poco::Optional<bool> val);

	void setHashingAlgorithms(const std::vector<std::vector<std::string>>& val);

	void setHashingAlgorithms(std::vector<std::vector<std::string>>&& val);

	void setHttpDigest(Poco::Optional<bool> val);

	void setKerberosToken(Poco::Optional<bool> val);

	void setMaxPasswordHistory(Poco::Optional<int> val);

	void setMaxPasswordLength(Poco::Optional<int> val);

	void setMaxUserNameLength(Poco::Optional<int> val);

	void setMaxUsers(Poco::Optional<int> val);

	void setOnboardKeyGeneration(Poco::Optional<bool> val);

	void setRELToken(Poco::Optional<bool> val);

	void setRemoteUserHandling(Poco::Optional<bool> val);

	void setSAMLToken(Poco::Optional<bool> val);

	void setSecurityPolicies(const std::vector<std::vector<std::string>>& val);

	void setSecurityPolicies(std::vector<std::vector<std::string>>&& val);

	void setSupportedEAPMethods(const std::vector<std::vector<int>>& val);

	void setSupportedEAPMethods(std::vector<std::vector<int>>&& val);

	void setTLS1_0(Poco::Optional<bool> val);

	void setTLS1_1(Poco::Optional<bool> val);

	void setTLS1_2(Poco::Optional<bool> val);

	void setUsernameToken(Poco::Optional<bool> val);

	void setX_509Token(Poco::Optional<bool> val);

private:
	//@ mandatory=false
	//@ name="TLS1.0"
	//@ order=0
	//@ type=attr
	Poco::Optional<bool> _tLS1_0;

	//@ mandatory=false
	//@ name="TLS1.1"
	//@ order=1
	//@ type=attr
	Poco::Optional<bool> _tLS1_1;

	//@ mandatory=false
	//@ name="TLS1.2"
	//@ order=2
	//@ type=attr
	Poco::Optional<bool> _tLS1_2;

	//@ mandatory=false
	//@ name=OnboardKeyGeneration
	//@ order=3
	//@ type=attr
	Poco::Optional<bool> _onboardKeyGeneration;

	//@ mandatory=false
	//@ name=AccessPolicyConfig
	//@ order=4
	//@ type=attr
	Poco::Optional<bool> _accessPolicyConfig;

	//@ mandatory=false
	//@ name=DefaultAccessPolicy
	//@ order=5
	//@ type=attr
	Poco::Optional<bool> _defaultAccessPolicy;

	//@ mandatory=false
	//@ name=Dot1X
	//@ order=6
	//@ type=attr
	Poco::Optional<bool> _dot1X;

	//@ mandatory=false
	//@ name=RemoteUserHandling
	//@ order=7
	//@ type=attr
	Poco::Optional<bool> _remoteUserHandling;

	//@ mandatory=false
	//@ name="X.509Token"
	//@ order=8
	//@ type=attr
	Poco::Optional<bool> _x_509Token;

	//@ mandatory=false
	//@ name=SAMLToken
	//@ order=9
	//@ type=attr
	Poco::Optional<bool> _sAMLToken;

	//@ mandatory=false
	//@ name=KerberosToken
	//@ order=10
	//@ type=attr
	Poco::Optional<bool> _kerberosToken;

	//@ mandatory=false
	//@ name=UsernameToken
	//@ order=11
	//@ type=attr
	Poco::Optional<bool> _usernameToken;

	//@ mandatory=false
	//@ name=HttpDigest
	//@ order=12
	//@ type=attr
	Poco::Optional<bool> _httpDigest;

	//@ mandatory=false
	//@ name=RELToken
	//@ order=13
	//@ type=attr
	Poco::Optional<bool> _rELToken;

	//@ mandatory=false
	//@ name=SupportedEAPMethods
	//@ order=14
	//@ type=attr
	std::vector<std::vector<int>> _supportedEAPMethods;

	//@ mandatory=false
	//@ name=MaxUsers
	//@ order=15
	//@ type=attr
	Poco::Optional<int> _maxUsers;

	//@ mandatory=false
	//@ name=MaxUserNameLength
	//@ order=16
	//@ type=attr
	Poco::Optional<int> _maxUserNameLength;

	//@ mandatory=false
	//@ name=MaxPasswordLength
	//@ order=17
	//@ type=attr
	Poco::Optional<int> _maxPasswordLength;

	//@ mandatory=false
	//@ name=SecurityPolicies
	//@ order=18
	//@ type=attr
	std::vector<std::vector<std::string>> _securityPolicies;

	//@ mandatory=false
	//@ name=MaxPasswordHistory
	//@ order=19
	//@ type=attr
	Poco::Optional<int> _maxPasswordHistory;

	//@ mandatory=false
	//@ name=HashingAlgorithms
	//@ order=20
	//@ type=attr
	std::vector<std::vector<std::string>> _hashingAlgorithms;

};


inline Poco::Optional<bool> SecurityCapabilities::getAccessPolicyConfig() const
{
	return _accessPolicyConfig;
}


inline Poco::Optional<bool> SecurityCapabilities::getDefaultAccessPolicy() const
{
	return _defaultAccessPolicy;
}


inline Poco::Optional<bool> SecurityCapabilities::getDot1X() const
{
	return _dot1X;
}


inline const std::vector<std::vector<std::string>>& SecurityCapabilities::getHashingAlgorithms() const
{
	return _hashingAlgorithms;
}


inline std::vector<std::vector<std::string>>& SecurityCapabilities::getHashingAlgorithms()
{
	return _hashingAlgorithms;
}


inline Poco::Optional<bool> SecurityCapabilities::getHttpDigest() const
{
	return _httpDigest;
}


inline Poco::Optional<bool> SecurityCapabilities::getKerberosToken() const
{
	return _kerberosToken;
}


inline Poco::Optional<int> SecurityCapabilities::getMaxPasswordHistory() const
{
	return _maxPasswordHistory;
}


inline Poco::Optional<int> SecurityCapabilities::getMaxPasswordLength() const
{
	return _maxPasswordLength;
}


inline Poco::Optional<int> SecurityCapabilities::getMaxUserNameLength() const
{
	return _maxUserNameLength;
}


inline Poco::Optional<int> SecurityCapabilities::getMaxUsers() const
{
	return _maxUsers;
}


inline Poco::Optional<bool> SecurityCapabilities::getOnboardKeyGeneration() const
{
	return _onboardKeyGeneration;
}


inline Poco::Optional<bool> SecurityCapabilities::getRELToken() const
{
	return _rELToken;
}


inline Poco::Optional<bool> SecurityCapabilities::getRemoteUserHandling() const
{
	return _remoteUserHandling;
}


inline Poco::Optional<bool> SecurityCapabilities::getSAMLToken() const
{
	return _sAMLToken;
}


inline const std::vector<std::vector<std::string>>& SecurityCapabilities::getSecurityPolicies() const
{
	return _securityPolicies;
}


inline std::vector<std::vector<std::string>>& SecurityCapabilities::getSecurityPolicies()
{
	return _securityPolicies;
}


inline const std::vector<std::vector<int>>& SecurityCapabilities::getSupportedEAPMethods() const
{
	return _supportedEAPMethods;
}


inline std::vector<std::vector<int>>& SecurityCapabilities::getSupportedEAPMethods()
{
	return _supportedEAPMethods;
}


inline Poco::Optional<bool> SecurityCapabilities::getTLS1_0() const
{
	return _tLS1_0;
}


inline Poco::Optional<bool> SecurityCapabilities::getTLS1_1() const
{
	return _tLS1_1;
}


inline Poco::Optional<bool> SecurityCapabilities::getTLS1_2() const
{
	return _tLS1_2;
}


inline Poco::Optional<bool> SecurityCapabilities::getUsernameToken() const
{
	return _usernameToken;
}


inline Poco::Optional<bool> SecurityCapabilities::getX_509Token() const
{
	return _x_509Token;
}


inline void SecurityCapabilities::setAccessPolicyConfig(Poco::Optional<bool> val)
{
	_accessPolicyConfig = val;
}


inline void SecurityCapabilities::setDefaultAccessPolicy(Poco::Optional<bool> val)
{
	_defaultAccessPolicy = val;
}


inline void SecurityCapabilities::setDot1X(Poco::Optional<bool> val)
{
	_dot1X = val;
}


inline void SecurityCapabilities::setHashingAlgorithms(const std::vector<std::vector<std::string>>& val)
{
	_hashingAlgorithms = val;
}


inline void SecurityCapabilities::setHashingAlgorithms(std::vector<std::vector<std::string>>&& val)
{
	_hashingAlgorithms = std::move(val);
}


inline void SecurityCapabilities::setHttpDigest(Poco::Optional<bool> val)
{
	_httpDigest = val;
}


inline void SecurityCapabilities::setKerberosToken(Poco::Optional<bool> val)
{
	_kerberosToken = val;
}


inline void SecurityCapabilities::setMaxPasswordHistory(Poco::Optional<int> val)
{
	_maxPasswordHistory = val;
}


inline void SecurityCapabilities::setMaxPasswordLength(Poco::Optional<int> val)
{
	_maxPasswordLength = val;
}


inline void SecurityCapabilities::setMaxUserNameLength(Poco::Optional<int> val)
{
	_maxUserNameLength = val;
}


inline void SecurityCapabilities::setMaxUsers(Poco::Optional<int> val)
{
	_maxUsers = val;
}


inline void SecurityCapabilities::setOnboardKeyGeneration(Poco::Optional<bool> val)
{
	_onboardKeyGeneration = val;
}


inline void SecurityCapabilities::setRELToken(Poco::Optional<bool> val)
{
	_rELToken = val;
}


inline void SecurityCapabilities::setRemoteUserHandling(Poco::Optional<bool> val)
{
	_remoteUserHandling = val;
}


inline void SecurityCapabilities::setSAMLToken(Poco::Optional<bool> val)
{
	_sAMLToken = val;
}


inline void SecurityCapabilities::setSecurityPolicies(const std::vector<std::vector<std::string>>& val)
{
	_securityPolicies = val;
}


inline void SecurityCapabilities::setSecurityPolicies(std::vector<std::vector<std::string>>&& val)
{
	_securityPolicies = std::move(val);
}


inline void SecurityCapabilities::setSupportedEAPMethods(const std::vector<std::vector<int>>& val)
{
	_supportedEAPMethods = val;
}


inline void SecurityCapabilities::setSupportedEAPMethods(std::vector<std::vector<int>>&& val)
{
	_supportedEAPMethods = std::move(val);
}


inline void SecurityCapabilities::setTLS1_0(Poco::Optional<bool> val)
{
	_tLS1_0 = val;
}


inline void SecurityCapabilities::setTLS1_1(Poco::Optional<bool> val)
{
	_tLS1_1 = val;
}


inline void SecurityCapabilities::setTLS1_2(Poco::Optional<bool> val)
{
	_tLS1_2 = val;
}


inline void SecurityCapabilities::setUsernameToken(Poco::Optional<bool> val)
{
	_usernameToken = val;
}


inline void SecurityCapabilities::setX_509Token(Poco::Optional<bool> val)
{
	_x_509Token = val;
}


} } // ONVIF::Device


#endif // ONVIF_Device_SecurityCapabilities_INCLUDED
