//
// TransportFactoryManager.h
//
// Library: RemotingNG
// Package: Transport
// Module:  TransportFactoryManager
//
// Definition of the TransportFactoryManager class.
//
// Copyright (c) 2006-2014, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// License: Applied Informatics Software and Source Code License Agreement
//


#ifndef RemotingNG_TransportFactoryManager_INCLUDED
#define RemotingNG_TransportFactoryManager_INCLUDED


#include "Poco/RemotingNG/RemotingNG.h"
#include "Poco/RemotingNG/TransportFactory.h"
#include "Poco/SingletonHolder.h"
#include "Poco/Mutex.h"
#include <map>


namespace Poco {
namespace RemotingNG {


class Transport;
class TransportFactory;


class RemotingNG_API TransportFactoryManager
	/// A TransportFactoryManager manages TransportFactory objects.
{
public:
	Transport* createTransport(const std::string& protocol);
		/// Creates a new Transport for the given protocol. 
		///
		/// The newly created Transport is unconnected.
		///
		/// Throws an exception if the protocol is unknown

	Transport* createTransport(const std::string& protocol, const std::string& uri);
		/// Creates a new Transport for the given protocol and
		/// connects it to the endpoint specified by the
		/// given URI.
		///
		/// Throws an exception if the protocol is unknown.

	void registerFactory(const std::string& protocol, TransportFactory::Ptr pFactory);
		/// Registers the factory if it doesn't exist yet.

	void unregisterFactory(const std::string& protocol);
		/// Removes the factory from the manager. 
		/// If for the given protocol no factory is found,
		/// the error is silently ignored.

	bool hasFactory(const std::string& protocol) const;
		/// Returns true if for the protocol a factory was registered.

	static TransportFactoryManager& instance();
		/// Returns a reference to the one and only instance of TransportFactoryManager.

private:
	TransportFactoryManager();
		/// Creates the TransportFactoryManager.

	~TransportFactoryManager();
		/// Destroys the TransportFactoryManager.

	TransportFactoryManager& operator = (const TransportFactoryManager&);
	TransportFactoryManager(const TransportFactoryManager&);

private:
	using Factories = std::map<std::string, TransportFactory::Ptr>;

	Factories _factories;
	mutable Poco::FastMutex _mutex;

	friend class Poco::SingletonHolder<TransportFactoryManager>;
};


} } // namespace Poco::RemotingNG


#endif // RemotingNG_TransportFactoryManager_INCLUDED
