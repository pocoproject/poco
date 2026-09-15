//
// EventDispatcher.h
//
// Library: RemotingNG
// Package: ORB
// Module:  EventDispatcher
//
// Definition of the EventDispatcher class.
//
// Copyright (c) 2006-2014, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// License: Applied Informatics Software and Source Code License Agreement
//


#ifndef RemotingNG_EventDispatcher_INCLUDED
#define RemotingNG_EventDispatcher_INCLUDED


#include "Poco/RemotingNG/RemotingNG.h"
#include "Poco/RemotingNG/Transport.h"
#include "Poco/RemotingNG/EventFilter.h"
#include "Poco/Clock.h"
#include "Poco/SharedPtr.h"
#include "Poco/AutoPtr.h"
#include "Poco/RefCountedObject.h"
#include "Poco/Mutex.h"
#include "Poco/Any.h"
#include <map>


namespace Poco {
namespace RemotingNG {


class ORB;


class RemotingNG_API EventDispatcher: public Poco::RefCountedObject
	/// The EventDispatcher is responsible for delivering
	/// events fired by service objects to remote subscribers.
	///
	/// Event dispatching and delivery in RemotingNG works as follows:
	/// If a service object has events, the corresponding RemoteObject class 
	/// will have a delegate member function for every service object
	/// event, as well as providing all the events itself (inherited from the
	/// interface class). During construction of the RemoteObject, its delegates
	/// will be bound to the service objects' events. The delegates will then
	/// simply fire the RemoteObject's event, when invoked.
	/// 
	/// The EventDispatcher subclass for a specific service will have
	/// delegate functions for all service object events, which will
	/// be bound to the RemoteObject's events. The EventDispatcher class
	/// maintains a list of event subscribers. When an event is fired,
	/// the EventDispatcher's delegate will dispatch the event to
	/// all registered event subscribers, using a Transport. This works
	/// in a very similar way to a Proxy.
{
public:
	using Ptr = Poco::AutoPtr<EventDispatcher>;
	using NameValueMap = Transport::NameValueMap;

	EventDispatcher(const std::string& protocol);
		/// Creates an EventDispatcher. 

	virtual ~EventDispatcher();
		/// Destroys the EventDispatcher.

	void subscribe(const std::string& subscriberURI, const std::string& endpointURI, Poco::Clock expireTime = 0);
		/// Registers a remote EventSubscriber identified by
		/// the given subscriberURI. Events will be sent to the
		/// specified endpoint, which must be a proper RemotingNG URI.
		/// Depending on the actual transport-specific events implementation,
		/// subscriberURI and endpointURI may be the same or different.
		///
		/// If a non-null expireTime is given, the subscription will automatically
		/// expire at the time given in expireTime.
		///
		/// If a subscription with the given subscriberURI already exists,
		/// the expireTime of the existing subscription will be updated.

	void unsubscribe(const std::string& subscriberURI);
		/// Unregisters the remote EventSubscriber identified
		/// by the given URI.
		///
		/// Throws a Poco::NotFoundException if no subscription exists.
		
	template <typename T>
	void setEventFilter(const std::string& subscriberURI, const std::string& event, typename EventFilter<T>::Ptr pFilter)
		/// Sets an event filter for the given subscriber and event.
		///
		/// If pFilter is null, removes the filter.
	{
		if (pFilter)
			setEventFilterImpl(subscriberURI, event, pFilter);
		else
			removeEventFilter(subscriberURI, event);
	}
	
	void removeEventFilter(const std::string& subscriberURI, const std::string& event);
		/// Removes the event filter for the given subscriber and event.

	const std::string& protocol() const;
		/// Returns the protocol to be used for delivering
		/// event messages to subscribers.

	AttributedObject& attributes(const std::string& subscriberURI);
		/// Returns the attributes for the subscriber identified
		/// by the given URI. The attributes are stored in the
		/// Transport object.
		
protected:	
	Transport& transportForSubscriber(const std::string& subscriberURI);
		/// Returns the Transport object for communicating
		/// with the remote EventSubscriber identified by the 
		/// given URI.
		///
		/// Throws a Poco::NotFoundException if the subscriber is not known.
		
	void setEventFilterImpl(const std::string& subscriberURI, const std::string& event, const Poco::Any& filter);
		/// Sets the event filter for the given subscriber and event.
		///
		/// If the given filter is empty, removes the filter.
		
	using FilterMap = std::map<std::string, Poco::Any>;
	
	template <typename T>
	bool accept(const FilterMap& filters, const std::string& event, const T& value)
	{
		FilterMap::const_iterator it = filters.find(event);
		if (it != filters.end())
		{
			typename EventFilter<T>::Ptr pFilter = Poco::AnyCast<typename EventFilter<T>::Ptr>(it->second);
			return pFilter->accept(value);
		}
		return true;
	}

	struct SubscriberInfo: public Poco::RefCountedObject
	{
		using Ptr = Poco::AutoPtr<SubscriberInfo>;
		
		std::string     endpoint;
		Transport::Ptr  pTransport;
		Poco::Clock     expireTime;
		FilterMap       filters;
	};
	using SubscriberMap = std::map<std::string, SubscriberInfo::Ptr>;

	SubscriberMap   _subscribers;
	Poco::FastMutex _mutex;

private:
	EventDispatcher(const EventDispatcher&);
	EventDispatcher& operator = (const EventDispatcher&);

	void setOwner(const Poco::AutoPtr<Poco::RefCountedObject>& pOwner);
		/// Keeps the object whose events this dispatcher forwards alive for the
		/// lifetime of the dispatcher. The ORB calls this when the dispatcher is
		/// registered: generated dispatchers hold the remote object by plain pointer
		/// and unsubscribe from its events in their destructor, so the object must
		/// not be destroyed while a dispatcher reference is held elsewhere.

	std::string _protocol;
	Poco::AutoPtr<Poco::RefCountedObject> _pOwner;

	friend class ORB;
};


//
// inlines
//
inline const std::string& EventDispatcher::protocol() const
{
	return _protocol;
}


inline AttributedObject& EventDispatcher::attributes(const std::string& subscriberURI)
{
	Poco::FastMutex::ScopedLock lock(_mutex);

	return transportForSubscriber(subscriberURI);
}


} } // namespace Poco::RemotingNG


#endif // RemotingNG_EventDispatcher_INCLUDED
