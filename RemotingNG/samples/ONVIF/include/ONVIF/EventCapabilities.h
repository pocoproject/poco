// 
// EventCapabilities.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_EventCapabilities_INCLUDED
#define ONVIF_EventCapabilities_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include "Poco/URI.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=EventCapabilities
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API EventCapabilities
{
public:
	EventCapabilities();

	EventCapabilities(
		const Poco::URI& xAddr, 
		bool wSSubscriptionPolicySupport, 
		bool wSPullPointSupport, 
		bool wSPausableSubscriptionManagerInterfaceSupport);

	virtual ~EventCapabilities();

	bool getWSPausableSubscriptionManagerInterfaceSupport() const;

	bool getWSPullPointSupport() const;

	bool getWSSubscriptionPolicySupport() const;

	const Poco::URI& getXAddr() const;

	void setWSPausableSubscriptionManagerInterfaceSupport(bool val);

	void setWSPullPointSupport(bool val);

	void setWSSubscriptionPolicySupport(bool val);

	void setXAddr(const Poco::URI& val);

	void setXAddr(Poco::URI&& val);

private:
	//@ name=XAddr
	//@ order=0
	Poco::URI _xAddr;

	//@ name=WSSubscriptionPolicySupport
	//@ order=1
	bool _wSSubscriptionPolicySupport;

	//@ name=WSPullPointSupport
	//@ order=2
	bool _wSPullPointSupport;

	//@ name=WSPausableSubscriptionManagerInterfaceSupport
	//@ order=3
	bool _wSPausableSubscriptionManagerInterfaceSupport;

};


inline bool EventCapabilities::getWSPausableSubscriptionManagerInterfaceSupport() const
{
	return _wSPausableSubscriptionManagerInterfaceSupport;
}


inline bool EventCapabilities::getWSPullPointSupport() const
{
	return _wSPullPointSupport;
}


inline bool EventCapabilities::getWSSubscriptionPolicySupport() const
{
	return _wSSubscriptionPolicySupport;
}


inline const Poco::URI& EventCapabilities::getXAddr() const
{
	return _xAddr;
}


inline void EventCapabilities::setWSPausableSubscriptionManagerInterfaceSupport(bool val)
{
	_wSPausableSubscriptionManagerInterfaceSupport = val;
}


inline void EventCapabilities::setWSPullPointSupport(bool val)
{
	_wSPullPointSupport = val;
}


inline void EventCapabilities::setWSSubscriptionPolicySupport(bool val)
{
	_wSSubscriptionPolicySupport = val;
}


inline void EventCapabilities::setXAddr(const Poco::URI& val)
{
	_xAddr = val;
}


inline void EventCapabilities::setXAddr(Poco::URI&& val)
{
	_xAddr = std::move(val);
}


} // ONVIF


#endif // ONVIF_EventCapabilities_INCLUDED
