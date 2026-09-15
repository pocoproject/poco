//
// ServerHelper.h
//
// Library: RemotingNG
// Package: ORB
// Module:  ServerHelper
//
// Definition of the ServerHelper class template.
//
// Copyright (c) 2006-2014, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef RemotingNG_ServerHelper_INCLUDED
#define RemotingNG_ServerHelper_INCLUDED


#include "Poco/RemotingNG/Identifiable.h"
#include "Poco/RemotingNG/ORB.h"


namespace Poco {
namespace RemotingNG {


template <class C>
class ServerHelper
	/// This class template provides a generic interface to the generated
	/// ServerHelper class. Actual members are defined by the specialization,
	/// which is generated using the REMOTING_SPECIALIZE_SERVER_HELPER macro.
	///
	/// A specialization of ServerHelper looks like this:
	///
	///     template <>
	///     class ServerHelper<SomeService>
	///     {
	///     public:
	///         using Helper = SomeServiceServerHelper;
	///         using Service = SomeService;
	///         using ServicePtr = Poco::SharedPtr<Service>;
	///         using Interface = ISomeService;
	///         using InterfacePtr = Poco::AutoPtr<ISomeService>;
	///         using RemoteObject = SomeServiceRemoteObject;
	///         using RemoteObjectPtr = Poco::AutoPtr<RemoteObject>;
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
	class ServerHelper<namespc::clazz##ServerHelper::Service> \
	{ \
	public: \
		using Helper = namespc::clazz##ServerHelper; \
		using Service = Helper::Service; \
		using ServicePtr = Poco::SharedPtr<Service>; \
		using Interface = namespc::I##clazz; \
		using InterfacePtr = Poco::AutoPtr<Interface>; \
		using RemoteObject = namespc::clazz##RemoteObject; \
		using RemoteObjectPtr = Poco::AutoPtr<RemoteObject>; \
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
		static void shutdown() \
		{ \
			return Helper::shutdown(); \
		} \
	}; \
	} }


#endif // RemotingNG_ServerHelper_INCLUDED
