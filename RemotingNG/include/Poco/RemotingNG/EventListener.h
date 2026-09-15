//
// EventListener.h
//
// Library: RemotingNG
// Package: Transport
// Module:  EventListener
//
// Definition of the EventListener class.
//
// Copyright (c) 2006-2014, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef RemotingNG_EventListener_INCLUDED
#define RemotingNG_EventListener_INCLUDED


#include "Poco/RemotingNG/Listener.h"
#include "Poco/RemotingNG/EventSubscriber.h"


namespace Poco {
namespace RemotingNG {


class RemotingNG_API EventListener: public Listener
	/// The EventListener class extends the Listener interface 
	/// with methods for managing event subscriptions on the
	/// client side.
{
public:
	using Ptr = Poco::AutoPtr<EventListener>;

	EventListener(const std::string& endPoint);
		/// Creates an EventListener. 

	virtual ~EventListener();
		/// Destroys the EventListener.

	virtual std::string subscribeToEvents(EventSubscriber::Ptr pEventSubscriber) = 0;
		/// Register an EventSubscriber to the EventListener.
		///
		/// The EventListener is responsible for sending a protocol-specific
		/// subscription request to the server so that events will be
		/// delivered from the server to the EventListener.
		///
		/// If necessary for the event delivery protocol, the EventListener
		/// is also responsible for assigning a unique ID (or URI) to
		/// the registered EventSubscriber instance.
		///
		/// Returns the unique URI that identifies the EventListener if the
		/// underlying transport provides one. Otherwise returns an empty string.

	virtual void unsubscribeFromEvents(EventSubscriber::Ptr pEventSubscriber) = 0;
		/// Unregister an EventSubscriber from the EventListener.
		///
		/// The EventListener is responsible for sending a protocol-specific
		/// unsubscription request to the server so that the server
		/// will no longer deliver events for this EventSubscriber.
};


} } // namespace Poco::RemotingNG


#endif // RemotingNG_EventListener_INCLUDED
