//
// ClientHelper.h
//
// Library: RemotingNG
// Package: ORB
// Module:  ClientHelper
//
// Definition of the ClientHelper class template.
//
// Copyright (c) 2006-2015, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// License: Applied Informatics Software and Source Code License Agreement
//


#ifndef RemotingNG_ClientHelper_INCLUDED
#define RemotingNG_ClientHelper_INCLUDED


#include "Poco/RemotingNG/Identifiable.h"
#include "Poco/RemotingNG/ORB.h"


namespace Poco {
namespace RemotingNG {


template <class C>
class ClientHelper
	/// This class template provides a generic interface to the generated
	/// ClientHelper class. Actual members are defined by the specialization,
	/// which is generated using the REMOTING_SPECIALIZE_CLIENT_HELPER macro.
	///
	/// A specialization of ClientHelper looks like this:
	///
	///     template <>
	///     class ClientHelper<SomeService>
	///     {
	///     public:
	///         typedef SomeServiceClientHelper Helper;
	///         typedef SomeService Service;
	///         typedef Poco::SharedPtr<Service> ServicePtr;
	///         typedef ISomeService Interface;
	///         typedef Poco::AutoPtr<ISomeService> InterfacePtr;
	///         typedef SomeServiceRemoteObject RemoteObject;
	///         typedef Poco::AutoPtr<RemoteObject> RemoteObjectPtr;
	///
	///         static std::string registerObject(ServicePtr pServiceObject, const Poco::RemotingNG::Identifiable::ObjectId& oid, const std::string& listenerId);
	///         static std::string registerObject(RemoteObjectPtr pRemoteObject, const std::string listenerId);
	///         static void unregisterObject(const std::string& uri);
	///         static RemoteObjectPtr createRemoteObject(const Poco::RemotingNG::Identifiable::ObjectId& oid, ServicePtr pService);
	///    };
{
};


} } // namespace Poco::RemotingNG


#define REMOTING_SPECIALIZE_SERVER_HELPER(namespc, clazz) \
	namespace Poco { \
	namespace RemotingNG { \
	template <> \
	class ClientHelper<namespc::clazz##ClientHelper::Service> \
	{ \
	public: \
		typedef namespc::clazz##ClientHelper Helper; \
		typedef Helper::Service Service; \
		typedef Poco::SharedPtr<Service> ServicePtr; \
		typedef namespc::I##clazz Interface; \
		typedef Poco::AutoPtr<Interface> InterfacePtr; \
		typedef namespc::clazz##RemoteObject RemoteObject; \
		typedef Poco::AutoPtr<RemoteObject> RemoteObjectPtr; \
		\
		static std::string registerObject(ServicePtr pServiceObject, const Poco::RemotingNG::Identifiable::ObjectId& oid, const std::string& listenerId) \
		{ \
			return Helper::registerObject(pServiceObject, oid, listenerId); \
		} \
		\
		static std::string registerObject(RemoteObjectPtr pRemoteObject, const std::string listenerId) \
		{ \
			return Helper::registerRemoteObject(pRemoteObject, listenerId); \
		} \
		\
		static void unregisterObject(const std::string& uri) \
		{ \
			Helper::unregisterObject(uri); \
		} \
		\
		static RemoteObjectPtr createRemoteObject(ServicePtr pService, const Poco::RemotingNG::Identifiable::ObjectId& oid) \
		{ \
			return Helper::createRemoteObject(pService, oid); \
		} \
	}; \
	} }


#endif // RemotingNG_ClientHelper_INCLUDED
