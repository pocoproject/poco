//
// AuthTesterServerHelper.h
//
// Package: Generated
// Module:  AuthTesterServerHelper
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef AuthTesterServerHelper_INCLUDED
#define AuthTesterServerHelper_INCLUDED


#include "AuthTesterRemoteObject.h"
#include "IAuthTester.h"
#include "Poco/RemotingNG/Identifiable.h"
#include "Poco/RemotingNG/ORB.h"
#include "Poco/RemotingNG/ServerHelper.h"
#include "Tester.h"


class AuthTesterServerHelper
{
public:
	using Service = AuthTester;

	AuthTesterServerHelper();
		/// Creates a AuthTesterServerHelper.

	~AuthTesterServerHelper();
		/// Destroys the AuthTesterServerHelper.

	static Poco::AutoPtr<AuthTesterRemoteObject> createRemoteObject(Poco::SharedPtr<AuthTester> pServiceObject, const Poco::RemotingNG::Identifiable::ObjectId& oid);
		/// Creates and returns a RemoteObject wrapper for the given AuthTester instance.

	static std::string registerObject(Poco::SharedPtr<AuthTester> pServiceObject, const Poco::RemotingNG::Identifiable::ObjectId& oid, const std::string& listenerId);
		/// Creates a RemoteObject wrapper for the given AuthTester instance
		/// and registers it with the ORB and the Listener instance
		/// uniquely identified by the Listener's ID.
		/// 
		///	Returns the URI created for the object.

	static std::string registerRemoteObject(Poco::AutoPtr<AuthTesterRemoteObject> pRemoteObject, const std::string& listenerId);
		/// Registers the given RemoteObject with the ORB and the Listener instance
		/// uniquely identified by the Listener's ID.
		/// 
		///	Returns the URI created for the object.

	static void shutdown();
		/// Removes the Skeleton for AuthTester from the ORB.

	static void unregisterObject(const std::string& uri);
		/// Unregisters a service object identified by URI from the ORB.

private:
	static Poco::AutoPtr<AuthTesterRemoteObject> createRemoteObjectImpl(Poco::SharedPtr<AuthTester> pServiceObject, const Poco::RemotingNG::Identifiable::ObjectId& oid);

	static AuthTesterServerHelper& instance();
		/// Returns a static instance of the helper class.

	std::string registerObjectImpl(Poco::AutoPtr<AuthTesterRemoteObject> pRemoteObject, const std::string& listenerId);

	void registerSkeleton();

	void unregisterObjectImpl(const std::string& uri);

	void unregisterSkeleton();

	Poco::RemotingNG::ORB* _pORB;
};


inline Poco::AutoPtr<AuthTesterRemoteObject> AuthTesterServerHelper::createRemoteObject(Poco::SharedPtr<AuthTester> pServiceObject, const Poco::RemotingNG::Identifiable::ObjectId& oid)
{
	return AuthTesterServerHelper::instance().createRemoteObjectImpl(pServiceObject, oid);
}


inline std::string AuthTesterServerHelper::registerObject(Poco::SharedPtr<AuthTester> pServiceObject, const Poco::RemotingNG::Identifiable::ObjectId& oid, const std::string& listenerId)
{
	return AuthTesterServerHelper::instance().registerObjectImpl(createRemoteObject(pServiceObject, oid), listenerId);
}


inline std::string AuthTesterServerHelper::registerRemoteObject(Poco::AutoPtr<AuthTesterRemoteObject> pRemoteObject, const std::string& listenerId)
{
	return AuthTesterServerHelper::instance().registerObjectImpl(pRemoteObject, listenerId);
}


inline void AuthTesterServerHelper::unregisterObject(const std::string& uri)
{
	AuthTesterServerHelper::instance().unregisterObjectImpl(uri);
}




REMOTING_SPECIALIZE_SERVER_HELPER(, AuthTester)


#endif // AuthTesterServerHelper_INCLUDED

