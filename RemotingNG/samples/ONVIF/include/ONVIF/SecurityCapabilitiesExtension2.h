// 
// SecurityCapabilitiesExtension2.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_SecurityCapabilitiesExtension2_INCLUDED
#define ONVIF_SecurityCapabilitiesExtension2_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=SecurityCapabilitiesExtension2
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API SecurityCapabilitiesExtension2
{
public:
	SecurityCapabilitiesExtension2();

	SecurityCapabilitiesExtension2(
		bool dot1X, 
		const std::vector<int>& supportedEAPMethod, 
		bool remoteUserHandling);

	virtual ~SecurityCapabilitiesExtension2();

	bool getDot1X() const;

	bool getRemoteUserHandling() const;

	const std::vector<int>& getSupportedEAPMethod() const;

	std::vector<int>& getSupportedEAPMethod();

	void setDot1X(bool val);

	void setRemoteUserHandling(bool val);

	void setSupportedEAPMethod(const std::vector<int>& val);

private:
	//@ name=Dot1X
	//@ order=0
	bool _dot1X;

	//@ mandatory=false
	//@ name=SupportedEAPMethod
	//@ order=1
	std::vector<int> _supportedEAPMethod;

	//@ name=RemoteUserHandling
	//@ order=2
	bool _remoteUserHandling;

};


inline bool SecurityCapabilitiesExtension2::getDot1X() const
{
	return _dot1X;
}


inline bool SecurityCapabilitiesExtension2::getRemoteUserHandling() const
{
	return _remoteUserHandling;
}


inline const std::vector<int>& SecurityCapabilitiesExtension2::getSupportedEAPMethod() const
{
	return _supportedEAPMethod;
}


inline std::vector<int>& SecurityCapabilitiesExtension2::getSupportedEAPMethod()
{
	return _supportedEAPMethod;
}


inline void SecurityCapabilitiesExtension2::setDot1X(bool val)
{
	_dot1X = val;
}


inline void SecurityCapabilitiesExtension2::setRemoteUserHandling(bool val)
{
	_remoteUserHandling = val;
}


inline void SecurityCapabilitiesExtension2::setSupportedEAPMethod(const std::vector<int>& val)
{
	_supportedEAPMethod = val;
}


} // ONVIF


#endif // ONVIF_SecurityCapabilitiesExtension2_INCLUDED
