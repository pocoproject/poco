//
// TCPServerConnectionFactory.h
//
// Library: Net
// Package: TCPServer
// Module:  TCPServerConnectionFactory
//
// Definition of the TCPServerConnectionFactory class.
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Net_TCPServerConnectionFactory_INCLUDED
#define Net_TCPServerConnectionFactory_INCLUDED


#include "Poco/Net/Net.h"
#include "Poco/Net/TCPServerConnection.h"
#include "Poco/SharedPtr.h"


namespace Poco {
namespace Net {


class Net_API TCPServerConnectionFactory
	/// A factory for TCPServerConnection objects.
	///
	/// The TCPServer class uses a TCPServerConnectionFactory
	/// to create a connection object for each new connection
	/// it accepts.
	///
	/// Subclasses must override the createConnection()
	/// method, which can refuse connections by returning nullptr.
	/// Some examples when an implementation may refuse new connections:
	///
	///  - number of connections exceeded a limit
	///  - a connection from unwanted client attempted
	///  - too many connection attempts in a short timespan
	///  - etc.
	///
	/// The TCPServerConnectionFactoryImpl template class
	/// can be used to automatically instantiate a
	/// TCPServerConnectionFactory for a given subclass
	/// of TCPServerConnection.
{
public:
	using Ptr = Poco::SharedPtr<TCPServerConnectionFactory>;

	TCPServerConnectionFactory(const TCPServerConnectionFactory&) = delete;
	TCPServerConnectionFactory& operator = (const TCPServerConnectionFactory&) = delete;
	TCPServerConnectionFactory(TCPServerConnectionFactory&&) = delete;
	TCPServerConnectionFactory& operator = (TCPServerConnectionFactory&&) = delete;

	virtual ~TCPServerConnectionFactory();
		/// Destroys the TCPServerConnectionFactory.

	virtual TCPServerConnection* createConnection(const StreamSocket& socket) = 0;
		/// Creates an instance of a subclass of TCPServerConnection,
		/// using the given StreamSocket.
		/// This function is allowed to return nullptr, in which case an accepted
		/// socket will be destroyed by the TCPServerDispatcher.

	void stop();
		/// Stops the factory.
		/// Normally, this function is called by TCPServerDispatcher
		/// to indicate that the server is shutting down; the expected
		/// implementation behavior after this call is to return nullptr
		/// on all subsequent connection creation attempts.

	bool isStopped() const;
		/// Returns true if the factory was stopped, false otherwise.

protected:
	TCPServerConnectionFactory();
		/// Creates the TCPServerConnectionFactory.

private:
	std::atomic<bool> _stopped;
};


//
// inlines
//

inline void TCPServerConnectionFactory::stop()
{
	_stopped = true;
}


inline bool TCPServerConnectionFactory::isStopped() const
{
	return _stopped;
}


template <class S>
class TCPServerConnectionFactoryImpl: public TCPServerConnectionFactory
	/// This template provides a basic implementation of
	/// TCPServerConnectionFactory.
{
public:
	TCPServerConnectionFactoryImpl()
	{
	}

	~TCPServerConnectionFactoryImpl()
	{
	}

	TCPServerConnection* createConnection(const StreamSocket& socket)
	{
		if (!isStopped())
			return new S(socket);

		return nullptr;
	}
};


} } // namespace Poco::Net


#endif // Net_TCPServerConnectionFactory_INCLUDED
