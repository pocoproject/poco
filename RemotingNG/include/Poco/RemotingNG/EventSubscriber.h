//
// EventSubscriber.h
//
// Library: RemotingNG
// Package: ORB
// Module:  EventSubscriber
//
// Definition of the EventSubscriber class.
//
// Copyright (c) 2006-2014, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// License: Applied Informatics Software and Source Code License Agreement
//


#ifndef RemotingNG_EventSubscriber_INCLUDED
#define RemotingNG_EventSubscriber_INCLUDED


#include "Poco/RefCountedObject.h"
#include "Poco/AutoPtr.h"
#include "Poco/RemotingNG/RemotingNG.h"
#include "Poco/RemotingNG/Transport.h"
#include "Poco/RemotingNG/Skeleton.h"
#include "Poco/Mutex.h"
#include "Poco/SharedPtr.h"
#include <set>
#include <map>


namespace Poco {
namespace RemotingNG {


class RemotingNG_API EventSubscriber: public Skeleton
	/// The EventSubscriber is responsible for deserializing
	/// and dispatching event messages received from a
	/// server via an EventListener to a Proxy object.
{
public:
	using Ptr = Poco::AutoPtr<EventSubscriber>;

	EventSubscriber(const std::string& uri);
		/// Creates an EventSubscriber, using the given URI.
		///
		/// The given URI is used for subscribing to the event.
		/// Usually, this will be the URI of the remote object
		/// providing the events for this EventSubscriber.

	virtual ~EventSubscriber();
		/// Destroys the EventSubscriber.

	const std::string& uri() const;
		/// Returns the URI of the remote object providing the
		/// events for this EventSubscriber.

private:
	EventSubscriber(const EventSubscriber&);
	EventSubscriber& operator = (const EventSubscriber&);

	std::string _uri;
};


//
// inlines
//
inline const std::string& EventSubscriber::uri() const
{
	return _uri;
}


} } // namespace Poco::RemotingNG


#endif // RemotingNG_EventSubscriber_INCLUDED
