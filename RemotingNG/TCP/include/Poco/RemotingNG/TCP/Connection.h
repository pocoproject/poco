//
// Connection.h
//
// Library: RemotingNG/TCP
// Package: TCP
// Module:  Connection
//
// Definition of the Connection class.
//
// Copyright (c) 2006-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// License: Applied Informatics Software and Source Code License Agreement
//


#ifndef RemotingNG_TCP_Connection_INCLUDED
#define RemotingNG_TCP_Connection_INCLUDED


#include "Poco/RemotingNG/TCP/TCP.h"
#include "Poco/RemotingNG/TCP/FrameFactory.h"
#include "Poco/RemotingNG/AttributedObject.h"
#include "Poco/Net/StreamSocket.h"
#include "Poco/Runnable.h"
#include "Poco/RefCountedObject.h"
#include "Poco/AutoPtr.h"
#include "Poco/Clock.h"
#include "Poco/ObjectPool.h"
#include "Poco/BasicEvent.h"
#include "Poco/Event.h"
#include "Poco/Mutex.h"
#include "Poco/Logger.h"
#include "Poco/AtomicCounter.h"
#include <atomic>
#include <vector>
#include <set>


namespace Poco {
namespace RemotingNG {
namespace TCP {


class FrameHandler;


class RemotingNGTCP_API Connection: public Poco::Runnable, public Poco::RemotingNG::AttributedObject, public Poco::RefCountedObject
	/// This class represents a TCP connection between a client
	/// and a server.
	///
	/// A Connection is used by multiple proxies simultaneously.
	/// It is also used to send event messages from a server
	/// to a client.
	///
	/// A Connection is split up into multiple channels, with
	/// each channel belonging to exactly one Proxy or EventSubscriber.
	///
	/// Messages sent between the client and the server are split
	/// up into frames, with each frame being transmitted
	/// on a specific channel.
	///
	/// Each connection is handled by a thread. Incoming requests
	/// or event messages will typically be handled by a separate thread
	/// obtained from a ThreadPool, but this is outside the scope of
	/// the Connection object.
{
public:
	using Ptr = Poco::AutoPtr<Connection>;

	enum ConnectionMode
	{
		MODE_CLIENT,
			/// A connection created by a client.

		MODE_SERVER
			/// A connection created by a server.
	};

	enum ConnectionState
	{
		STATE_PRE_HANDSHAKE,
			/// The connection has been established, but no handshake
			///(HELO) frames have been exchanged.

		STATE_HANDSHAKE,
			/// The first handshake frame has been sent, and the
			/// connection is waiting for the peer's handshake frame.

		STATE_ESTABLISHED,
			/// The connection is ready.

		STATE_CLOSING_ACTIVE,
			/// The connection is being actively closed and a
			/// BYE frame has been sent to the peer. Waiting
			/// for the confirming BYE frame from the peer.

		STATE_CLOSING_PASSIVE,
			/// A BYE frame has been received and a confirming
			/// BYE frame will be sent.

		STATE_ABORTED,
			/// The connection has been forcefully closed, without
			/// a closing handshake, due to a protocol violation.

		STATE_CLOSED
			/// The connection has been orderly closed.
	};

	Poco::BasicEvent<Connection::Ptr> connectionEstablished;
		/// Fired after the handshake is complete and the
		/// connection has been established.

	Poco::BasicEvent<Connection::Ptr> connectionClosing;
		/// Fired when the connection is about to be closed.

	Poco::BasicEvent<Connection::Ptr> connectionClosed;
		/// Fired after the connection has been closed.

	Poco::BasicEvent<Connection::Ptr> connectionAborted;
		/// Fired after the connection has been aborted.

	Connection(const Poco::Net::StreamSocket& socket, ConnectionMode mode);
		/// Creates the Connection for the given socket and endpoint mode.

	~Connection();
		/// Destroys the Connection.

	Poco::UInt32 id() const;
		/// Returns the ID of the connection. This is unique within the
		/// server or client process among all connections.

	ConnectionMode mode() const;
		/// Returns the connection mode.

	ConnectionState state() const;
		/// Returns the connection state.

	bool secure() const;
		/// Returns true iff the connection uses a secure socket.

	void sendFrame(Frame::Ptr pFrame);
		/// Send a single frame.
		/// The connection must be in STATE_ESTABLISHED.

	void addCapability(Poco::UInt32 capability);
		/// Adds the given capability to the connection.
		///
		/// This must be done before the connection handshake takes place.

	bool hasCapability(Poco::UInt32 capability);
		/// Returns true if the connection has the given capability.

	bool peerHasCapability(Poco::UInt32 capability);
		/// Returns true if the peer has the given capability.
		///
		/// Peer capabilities will be available after the connection handshake.

	void setIdleTimeout(Poco::Timespan timeout);
		/// Sets the timeout after an idle connection is closed.

	Poco::Timespan getIdleTimeout() const;
		/// Returns the idle connection timeout.

	void setHandshakeTimeout(Poco::Timespan timeout);
		/// Sets the timeout for the HELO handshake.

	Poco::Timespan getHandshakeTimeout() const;
		/// Returns the timeout for the HELO handshake.

	Poco::Net::SocketAddress remoteAddress() const;
		/// Returns the remote peer's socket address.

	Poco::Net::SocketAddress localAddress() const;
		/// Returns the socket's local address.

	Poco::Net::StreamSocket socket() const;
		/// Returns the underlying socket.

	void close();
		/// Orderly closes the connection.

	void abort();
		/// Aborts the connection.

	Poco::UInt32 allocChannel();
		/// Allocates a channel number.

	void releaseChannel(Poco::UInt32 channel);
		/// Releases the given channel number.

	void pushFrameHandler(Poco::AutoPtr<FrameHandler> pHandler);
		/// Pushes the FrameHandler to the internal stack.

	void popFrameHandler(Poco::AutoPtr<FrameHandler> pHandler);
		/// Removes the FrameHandler from the internal stack.

	bool waitReady(Poco::Timespan timeout = 2*TIMEOUT_HELO);
		/// Waits until the connection is established, or the
		/// given timeout expires.

	void returnFrame(Frame::Ptr pFrame);
		/// Returns the frame to the pool.

protected:
	void run();
		/// Handle incoming frames.

	void runImpl();
		/// Handle incoming frames.

	void processFrame(Frame::Ptr pFrame);
		/// Process a single frame.

	void receiveHELO();
		/// Waits for and receives the initial HELO frame.

	void sendHELO();
		/// Sends the initial HELO frame.

	void receiveBYE();
		/// Waits for and receives the final BYE frame.

	void sendBYE();
		/// Sends the final BYE frame.

	void sendProtocolFrame(Frame::Ptr pFrame);
		/// Send a single protocol frame.

	Frame::Ptr receiveFrame();
		/// Receives the next frame from the connection,
		/// or returns null if no frame has been received
		/// within a certain time.

	int receiveNBytes(char* buffer, int bytes);
		/// Receive exactly the given number of bytes.

private:
	Connection();
	Connection(const Connection&);
	Connection& operator = (const Connection&);

	enum Timeouts // microseconds
	{
		TIMEOUT_POLL  =  200000,
		TIMEOUT_HELO  = 8000000,
		TIMEOUT_BYE   = 2000000,
		TIMEOUT_FRAME =  500000
	};

	using FramePool = Poco::ObjectPool<Frame, Frame::Ptr, FrameFactory>;
	using FrameHandlerVec = std::vector<Poco::AutoPtr<FrameHandler>>;

	Poco::Net::StreamSocket _socket;
	Poco::UInt32 _id;
	Poco::Timespan _idleTimeout;
	Poco::Timespan _handshakeTimeout;
	ConnectionMode _mode;
	std::atomic<ConnectionState> _state;
	FramePool _framePool;
	FrameHandlerVec _frameHandlers;
	FrameHandlerVec _tmpFrameHandlers;
	std::set<Poco::UInt32> _capabilities;
	std::set<Poco::UInt32> _peerCapabilities;
	std::set<Poco::UInt32> _allocatedChannels;
	Poco::UInt32 _nextChannel;
	Poco::Clock _lastFrame;
	Poco::Event _ready;
	Poco::Logger& _logger;
	Poco::FastMutex _mutex;

	static Poco::AtomicCounter _idCounter;

	friend class ServerConnection;
};


//
// inlines
//
inline Poco::UInt32 Connection::id() const
{
	return _id;
}


inline Poco::Timespan Connection::getIdleTimeout() const
{
	return _idleTimeout;
}


inline Poco::Timespan Connection::getHandshakeTimeout() const
{
	return _handshakeTimeout;
}


inline Connection::ConnectionMode Connection::mode() const
{
	return _mode;
}


inline Connection::ConnectionState Connection::state() const
{
	return _state.load(std::memory_order_relaxed);
}


inline void Connection::returnFrame(Frame::Ptr pFrame)
{
	_framePool.returnObject(pFrame);
}


} } } // namespace Poco::RemotingNG::TCP


#endif // RemotingNG_TCP_Connection_INCLUDED
