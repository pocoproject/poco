//
// ProxyFactory.h
//
// Library: RemotingNG
// Package: ORB
// Module:  ProxyFactory
//
// Definition of the ProxyFactory class.
//
// Copyright (c) 2006-2014, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef RemotingNG_ProxyFactory_INCLUDED
#define RemotingNG_ProxyFactory_INCLUDED


#include "Poco/RemotingNG/RemotingNG.h"
#include "Poco/RemotingNG/Identifiable.h"
#include "Poco/RefCountedObject.h"
#include "Poco/AutoPtr.h"


namespace Poco {
namespace RemotingNG {


class Proxy;


class RemotingNG_API ProxyFactory: public Poco::RefCountedObject
	/// A ProxyFactory is used to create instances of a given Proxy class.
{
public:
	using Ptr = Poco::AutoPtr<ProxyFactory>;
	
	ProxyFactory();
		/// Creates a ProxyFactory.

	virtual ~ProxyFactory();
		/// Destroys the ProxyFactory.

	virtual Proxy* createProxy(const RemotingNG::Identifiable::ObjectId& oid) const = 0;
		/// Returns a new unconnected Proxy object for the remote 
		/// service object with the given ObjectId.
};


} } // namespace Poco::RemotingNG


#endif // RemotingNG_ProxyFactory_INCLUDED
