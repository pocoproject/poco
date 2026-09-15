//
// RemoteObject.h
//
// Library: RemotingNG
// Package: ORB
// Module:  RemoteObject
//
// Definition of the RemoteObject class.
//
// Copyright (c) 2006-2014, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// License: Applied Informatics Software and Source Code License Agreement
//


#ifndef RemotingNG_RemoteObject_INCLUDED
#define RemotingNG_RemoteObject_INCLUDED


#include "Poco/RemotingNG/Identifiable.h"
#include "Poco/Mutex.h"


namespace Poco {
namespace RemotingNG {
	
	
class EventListener;


class RemotingNG_API RemoteObject: public Identifiable
	/// A RemoteObject forwards method calls to the
	/// local service object.
	///
	/// For every service object, the RemotingNG code generator
	/// generates a RemoteObject that is a subclass of the
	/// service object's interface class.
{
public:
	using Ptr = Poco::AutoPtr<RemoteObject>;

	RemoteObject(const Poco::RemotingNG::Identifiable::ObjectId& oid);
		/// Creates a RemoteObject.

	virtual ~RemoteObject();
		/// Destroys the RemoteObject.

	virtual bool remoting__hasEvents() const;
		/// Returns true if the service object has events, false otherwise.
		///
		/// Must be overridden by subclasses if the service object has events.
		/// The default implementation always returns false.
		
	virtual void remoting__enableRemoteEvents(const std::string& protocol);
		/// Enables remote events for the RemoteObject identified by the given URI.
		///
		/// Events will be delivered using the Transport for the given protocol.
		/// Can be called multiple times for the same URI with different protocols.
		///
		/// Must be overridden by subclasses if the service object has events.
		/// The default implementation does nothing.

protected:
	Poco::FastMutex& mutex() const;
	
private:
	mutable Poco::FastMutex _mutex;
};


//
// inlines
//
inline Poco::FastMutex& RemoteObject::mutex() const
{
	return _mutex;
}


} } // namespace Poco::RemotingNG


#endif // RemotingNG_RemoteObject_INCLUDED
