//
// TransportFactory.h
//
// Library: RemotingNG
// Package: Transport
// Module:  TransportFactory
//
// Definition of the TransportFactory class.
//
// Copyright (c) 2006-2014, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// License: Applied Informatics Software and Source Code License Agreement
//


#ifndef RemotingNG_TransportFactory_INCLUDED
#define RemotingNG_TransportFactory_INCLUDED


#include "Poco/RemotingNG/RemotingNG.h"
#include "Poco/RefCountedObject.h"
#include "Poco/AutoPtr.h"


namespace Poco {
namespace RemotingNG {


class Transport;


class RemotingNG_API TransportFactory: public Poco::RefCountedObject
	/// A TransportFactory creates Transport objects.
{
public:
	using Ptr = Poco::AutoPtr<TransportFactory>;

	TransportFactory();
		/// Creates a TransportFactory.

	virtual ~TransportFactory();
		/// Destroys the TransportFactory.

	virtual Transport* createTransport() = 0;
		/// Returns an unconnected Transport.
};


} } // namespace Poco::RemotingNG


#endif // RemotingNG_TransportFactory_INCLUDED
