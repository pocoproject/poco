//
// TesterServerHelper.h
//
// Package: Generated
// Module:  TesterServerHelper
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TesterServerHelper_INCLUDED
#define TesterServerHelper_INCLUDED


#include "ITester.h"
#include "Poco/RemotingNG/Identifiable.h"
#include "Poco/RemotingNG/ORB.h"
#include "Poco/RemotingNG/ServerHelper.h"
#include "Tester.h"
#include "TesterRemoteObject.h"


class TesterServerHelper
{
public:
	using Service = Tester;

	TesterServerHelper();
		/// Creates a TesterServerHelper.

	~TesterServerHelper();
		/// Destroys the TesterServerHelper.

	static Poco::AutoPtr<TesterRemoteObject> createRemoteObject(Poco::SharedPtr<Tester> pServiceObject, const Poco::RemotingNG::Identifiable::ObjectId& oid);
		/// Creates and returns a RemoteObject wrapper for the given Tester instance.

	static void enableEvents(const std::string& uri, const std::string& protocol);
		/// Enables remote events for the RemoteObject identified by the given URI.
		///
		/// Events will be delivered using the Transport for the given protocol.
		/// Can be called multiple times for the same URI with different protocols.

	static std::string registerObject(Poco::SharedPtr<Tester> pServiceObject, const Poco::RemotingNG::Identifiable::ObjectId& oid, const std::string& listenerId);
		/// Creates a RemoteObject wrapper for the given Tester instance
		/// and registers it with the ORB and the Listener instance
		/// uniquely identified by the Listener's ID.
		/// 
		///	Returns the URI created for the object.

	static std::string registerRemoteObject(Poco::AutoPtr<TesterRemoteObject> pRemoteObject, const std::string& listenerId);
		/// Registers the given RemoteObject with the ORB and the Listener instance
		/// uniquely identified by the Listener's ID.
		/// 
		///	Returns the URI created for the object.

	static void shutdown();
		/// Removes the Skeleton for Tester from the ORB.

	static void unregisterObject(const std::string& uri);
		/// Unregisters a service object identified by URI from the ORB.

private:
	static Poco::AutoPtr<TesterRemoteObject> createRemoteObjectImpl(Poco::SharedPtr<Tester> pServiceObject, const Poco::RemotingNG::Identifiable::ObjectId& oid);

	void enableEventsImpl(const std::string& uri, const std::string& protocol);

	static TesterServerHelper& instance();
		/// Returns a static instance of the helper class.

	std::string registerObjectImpl(Poco::AutoPtr<TesterRemoteObject> pRemoteObject, const std::string& listenerId);

	void registerSkeleton();

	void unregisterObjectImpl(const std::string& uri);

	void unregisterSkeleton();

	Poco::RemotingNG::ORB* _pORB;
};


inline Poco::AutoPtr<TesterRemoteObject> TesterServerHelper::createRemoteObject(Poco::SharedPtr<Tester> pServiceObject, const Poco::RemotingNG::Identifiable::ObjectId& oid)
{
	return TesterServerHelper::instance().createRemoteObjectImpl(pServiceObject, oid);
}


inline void TesterServerHelper::enableEvents(const std::string& uri, const std::string& protocol)
{
	TesterServerHelper::instance().enableEventsImpl(uri, protocol);
}


inline std::string TesterServerHelper::registerObject(Poco::SharedPtr<Tester> pServiceObject, const Poco::RemotingNG::Identifiable::ObjectId& oid, const std::string& listenerId)
{
	return TesterServerHelper::instance().registerObjectImpl(createRemoteObject(pServiceObject, oid), listenerId);
}


inline std::string TesterServerHelper::registerRemoteObject(Poco::AutoPtr<TesterRemoteObject> pRemoteObject, const std::string& listenerId)
{
	return TesterServerHelper::instance().registerObjectImpl(pRemoteObject, listenerId);
}


inline void TesterServerHelper::unregisterObject(const std::string& uri)
{
	TesterServerHelper::instance().unregisterObjectImpl(uri);
}




REMOTING_SPECIALIZE_SERVER_HELPER(, Tester)


#endif // TesterServerHelper_INCLUDED

