//
// Connection.cpp
//
// Library: RemotingNG/TCP
// Package: TCP
// Module:  Connection
//
// Copyright (c) 2006-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// License: Applied Informatics Software and Source Code License Agreement
//


#include "Poco/RemotingNG/TCP/Connection.h"
#include "Poco/RemotingNG/TCP/FrameHandler.h"
#include "Poco/RemotingNG/RemotingException.h"
#include "Poco/BinaryReader.h"
#include "Poco/BinaryWriter.h"
#include "Poco/MemoryStream.h"
#include "Poco/Format.h"
#include <cstring>
#include <set>


using namespace std::string_literals;


namespace Poco {
namespace RemotingNG {
namespace TCP {


Poco::AtomicCounter Connection::_idCounter;


Connection::Connection(const Poco::Net::StreamSocket& socket, ConnectionMode mode):
	_socket(socket),
	_id(++_idCounter),
	_idleTimeout(0),
	_handshakeTimeout(TIMEOUT_HELO),
	_mode(mode),
	_state(STATE_PRE_HANDSHAKE),
	_framePool(256, 4096),
	_nextChannel(1),
	_ready(false),
	_logger(Poco::Logger::get("RemotingNG.TCP.Connection"s))
{
	_socket.setReceiveTimeout(TIMEOUT_FRAME);
	_socket.setSendTimeout(TIMEOUT_FRAME);
	try
	{
		_socket.setNoDelay(true);
	}
	catch (...)
	{
	}

	_frameHandlers.reserve(64);
	_tmpFrameHandlers.reserve(64);
}


Connection::~Connection()
{
}


bool Connection::secure() const
{
	return _socket.secure();
}


void Connection::setIdleTimeout(Poco::Timespan timeout)
{
	_idleTimeout = timeout;
}


void Connection::setHandshakeTimeout(Poco::Timespan timeout)
{
	_handshakeTimeout = timeout;
}


void Connection::sendFrame(Frame::Ptr pFrame)
{
	Poco::FastMutex::ScopedLock lock(_mutex);

	if (_state == STATE_ESTABLISHED)
	{
		if (_logger.debug())
		{
			std::string msg(Poco::format("Sending Frame, type=%08x, channel=%08x, flags=%04hx, psize=%hu"s,
				pFrame->type(),
				pFrame->channel(),
				pFrame->flags(),
				pFrame->getPayloadSize()));
			_logger.dump(msg, pFrame->payloadBegin(), pFrame->getPayloadSize());
		}
		_socket.sendBytes(pFrame->bufferBegin(), pFrame->frameSize());
		_lastFrame.update();
	}
	else throw Poco::IllegalStateException("Connection not in established state");
}


void Connection::sendProtocolFrame(Frame::Ptr pFrame)
{
	Poco::FastMutex::ScopedLock lock(_mutex);

	if (_logger.debug())
	{
		std::string msg(Poco::format("Sending Protocol Frame, type=%08x, channel=%08x, flags=%04hx, psize=%hu"s,
			pFrame->type(),
			pFrame->channel(),
			pFrame->flags(),
			pFrame->getPayloadSize()));
		_logger.dump(msg, pFrame->payloadBegin(), pFrame->getPayloadSize());
	}
	_socket.sendBytes(pFrame->bufferBegin(), pFrame->frameSize());
	_lastFrame.update();
}


void Connection::addCapability(Poco::UInt32 capability)
{
	Poco::FastMutex::ScopedLock lock(_mutex);
	_capabilities.insert(capability);
}


bool Connection::hasCapability(Poco::UInt32 capability)
{
	Poco::FastMutex::ScopedLock lock(_mutex);
	return _capabilities.find(capability) != _capabilities.end();
}


bool Connection::peerHasCapability(Poco::UInt32 capability)
{
	Poco::FastMutex::ScopedLock lock(_mutex);
	return _peerCapabilities.find(capability) != _peerCapabilities.end();
}


Poco::Net::SocketAddress Connection::remoteAddress() const
{
	return _socket.peerAddress();
}


Poco::Net::SocketAddress Connection::localAddress() const
{
	return _socket.address();
}


Poco::Net::StreamSocket Connection::socket() const
{
	return _socket;
}


void Connection::close()
{
	Poco::FastMutex::ScopedLock lock(_mutex);

	if (_state == STATE_ESTABLISHED)
	{
		_state = STATE_CLOSING_ACTIVE;
	}
}


void Connection::abort()
{
	Poco::FastMutex::ScopedLock lock(_mutex);

	if (_state == STATE_ESTABLISHED)
	{
		_state = STATE_ABORTED;
	}
}


Poco::UInt32 Connection::allocChannel()
{
	Poco::FastMutex::ScopedLock lock(_mutex);

	while (_nextChannel == 0 || _allocatedChannels.find(_nextChannel) != _allocatedChannels.end())
	{
		++_nextChannel;
	}
	_allocatedChannels.insert(_nextChannel);
	return _nextChannel++;
}


void Connection::releaseChannel(Poco::UInt32 channel)
{
	poco_assert (channel != 0);

	Poco::FastMutex::ScopedLock lock(_mutex);

	_allocatedChannels.erase(channel);
}


void Connection::pushFrameHandler(Poco::AutoPtr<FrameHandler> pHandler)
{
	Poco::FastMutex::ScopedLock lock(_mutex);

	_frameHandlers.push_back(pHandler);
}


void Connection::popFrameHandler(Poco::AutoPtr<FrameHandler> pHandler)
{
	Poco::FastMutex::ScopedLock lock(_mutex);

	for (FrameHandlerVec::iterator it = _frameHandlers.begin(); it != _frameHandlers.end(); ++it)
	{
		if (*it == pHandler)
		{
			_frameHandlers.erase(it);
			break;
		}
	}
}


bool Connection::waitReady(Poco::Timespan timeout)
{
	return _ready.tryWait(static_cast<long>(timeout.totalMilliseconds()));
}


void Connection::run()
{
	try
	{
		runImpl();
	}
	catch (Poco::Exception& exc)
	{
		_logger.log(exc);
	}
	catch (...)
	{
		_logger.fatal("Unknown exception while handling connection.");
	}
}


void Connection::runImpl()
{
	Connection::Ptr pThis(this, true);

	if (_logger.debug())
	{
		_logger.debug("Starting handshake with %s"s, remoteAddress().toString());
	}
	_state = STATE_HANDSHAKE;
	if (_mode == MODE_CLIENT)
	{
		sendHELO();
	}
	receiveHELO();
	if (_logger.debug())
	{
		_logger.debug("Peer HELO received from %s"s, remoteAddress().toString());
	}
	if (_mode == MODE_SERVER)
	{
		sendHELO();
	}
	if (_logger.debug())
	{
		_logger.debug("Connection established with %s"s, remoteAddress().toString());
	}

	_state = STATE_ESTABLISHED;
	connectionEstablished(pThis);
	_ready.set();

	while (_state == STATE_ESTABLISHED)
	{
		Frame::Ptr pFrame;
		try
		{
			pFrame = receiveFrame();
			if (pFrame)
			{
				processFrame(pFrame);
			}
			else if (_idleTimeout.totalMicroseconds() > 0)
			{
				Poco::FastMutex::ScopedLock lock(_mutex);
				if (_lastFrame.isElapsed(_idleTimeout.totalMicroseconds()))
				{
					_state = STATE_CLOSING_ACTIVE;
					_logger.debug("Closing connection due to idle timeout."s);
				}
			}
		}
		catch (Poco::Exception& exc)
		{
			if (pFrame) _framePool.returnObject(pFrame);
			_state = STATE_ABORTED;
			_logger.error("Aborting connection due to exception: %s"s, exc.displayText());
		}
		catch (...)
		{
			if (pFrame) _framePool.returnObject(pFrame);
			_state = STATE_ABORTED;
			_logger.error("Aborting connection due to unknown exception."s);
		}
	}

	if (_state == STATE_CLOSING_ACTIVE || _state == STATE_CLOSING_PASSIVE)
	{
		_logger.debug("Closing connection to %s"s, remoteAddress().toString());
		sendBYE();
		connectionClosing(pThis);
	}

	if (_state == STATE_CLOSING_ACTIVE)
	{
		receiveBYE();
		_state = STATE_CLOSED;
	}
	else if (_state == STATE_CLOSING_PASSIVE)
	{
		// wait for peer to process our BYE and close the connection
		_socket.poll(Connection::TIMEOUT_BYE, Poco::Net::Socket::SELECT_READ);
		_state = STATE_CLOSED;
	}

	_socket.close();
	if (_state == STATE_CLOSED)
	{
		connectionClosed(pThis);
		_logger.debug("Connection closed."s);
	}
	else if (_state == STATE_ABORTED)
	{
		connectionAborted(pThis);
		_logger.debug("Connection ABORTED."s);
	}
}


void Connection::processFrame(Frame::Ptr pFrame)
{
	if (pFrame->type() == Frame::FRAME_TYPE_BYE)
	{
		Poco::FastMutex::ScopedLock lock(_mutex);
		if (_state == STATE_ESTABLISHED)
		{
			_state = STATE_CLOSING_PASSIVE;
		}
	}
	else
	{
		Connection::Ptr pThis(this, true);
		{
			Poco::FastMutex::ScopedLock lock(_mutex);
			_tmpFrameHandlers = _frameHandlers;
		}
		bool handled = false;
		for (FrameHandlerVec::reverse_iterator it = _tmpFrameHandlers.rbegin(); !handled && it != _tmpFrameHandlers.rend(); ++it)
		{
			try
			{
				handled = (*it)->handleFrame(pThis, pFrame);
			}
			catch (Poco::Exception& exc)
			{
				_logger.warning("Frame handler exception: %s"s, exc.displayText());
			}
			catch (...)
			{
				_logger.warning("Frame handler exception."s);
			}
		}
		_tmpFrameHandlers.clear();
		if (handled)
		{
			return;
		}
		else
		{
			_logger.warning("No handler for frame with type=%08x, channel=%08x."s, pFrame->type(), pFrame->channel());
		}
	}
	_framePool.returnObject(pFrame);
}


void Connection::receiveHELO()
{
	if (_socket.poll(_handshakeTimeout, Poco::Net::Socket::SELECT_READ))
	{
		Frame::Ptr pFrame = receiveFrame();
		if (pFrame)
		{
			try
			{
				if (pFrame->type() == Frame::FRAME_TYPE_HELO)
				{
					if (pFrame->flags() != Frame::FRAME_FLAG_EOM)
						throw Poco::RemotingNG::ProtocolException("HELO frame must have EOM flag set");
					Poco::MemoryInputStream istr(pFrame->payloadBegin(), pFrame->getPayloadSize());
					Poco::BinaryReader reader(istr, Poco::BinaryReader::NETWORK_BYTE_ORDER);
					Poco::UInt8 majorVersion(0);
					Poco::UInt8 minorVersion(0);
					Poco::UInt8 flags(0);
					Poco::UInt8 nCaps(0);
					reader >> majorVersion >> minorVersion >> flags >> nCaps;
					if (majorVersion != Frame::PROTO_MAJOR_VERSION)
						throw Poco::RemotingNG::ProtocolException(Poco::format("Unsupported protocol version: %u.%u"s, static_cast<unsigned>(majorVersion), static_cast<unsigned>(minorVersion)));
					if (pFrame->getPayloadSize() != 4 + nCaps*4)
						throw Poco::RemotingNG::ProtocolException("Invalid HELO frame received");
					Poco::FastMutex::ScopedLock lock(_mutex);
					for (Poco::UInt8 i = 0; i < nCaps; i++)
					{
						Poco::UInt32 cap(0);
						reader >> cap;
						_peerCapabilities.insert(cap);
					}
					_framePool.returnObject(pFrame);
					return;
				}
				else throw Poco::RemotingNG::ProtocolException(Poco::format("Unexpected frame received: %08x"s, pFrame->type()));
			}
			catch (...)
			{
				_framePool.returnObject(pFrame);
				throw;
			}
		}
	}
	throw Poco::TimeoutException("Timeout waiting for HELO frame");
}


void Connection::sendHELO()
{
	Frame::Ptr pFrame = new Frame(Frame::FRAME_TYPE_HELO, 0, Frame::FRAME_FLAG_EOM, 64);
	Poco::MemoryOutputStream ostr(pFrame->payloadBegin(), pFrame->maxPayloadSize());
	Poco::BinaryWriter writer(ostr, Poco::BinaryWriter::NETWORK_BYTE_ORDER);
	Poco::UInt8 majorVersion(Frame::PROTO_MAJOR_VERSION);
	Poco::UInt8 minorVersion(Frame::PROTO_MINOR_VERSION);
	Poco::UInt8 flags(0);
	Poco::UInt8 nCaps(static_cast<Poco::UInt8>(_capabilities.size()));
	writer << majorVersion << minorVersion << flags << nCaps;
	for (std::set<Poco::UInt32>::const_iterator it = _capabilities.begin(); it != _capabilities.end(); ++it)
	{
		writer << *it;
	}
	pFrame->setPayloadSize(static_cast<Poco::UInt16>(ostr.charsWritten()));
	sendProtocolFrame(pFrame);
}


void Connection::receiveBYE()
{
	if (_socket.poll(TIMEOUT_BYE, Poco::Net::Socket::SELECT_READ))
	{
		try
		{
			Frame::Ptr pFrame = receiveFrame();
			if (pFrame)
			{
				try
				{
					if (pFrame->type() != Frame::FRAME_TYPE_BYE)
					{
						_logger.notice("Unexpected frame (type=%08x) received while closing connection."s, pFrame->type());
					}
				}
				catch (...)
				{
					_framePool.returnObject(pFrame);
					throw;
				}
				_framePool.returnObject(pFrame);
			}
		}
		catch (Poco::Exception&)
		{
		}
	}
}


void Connection::sendBYE()
{
	Frame::Ptr pFrame = new Frame(Frame::FRAME_TYPE_BYE, 0, Frame::FRAME_FLAG_EOM, Frame::FRAME_HEADER_SIZE);
	sendProtocolFrame(pFrame);
}


Frame::Ptr Connection::receiveFrame()
{
	Frame::Ptr pFrame;
	if (_socket.poll(TIMEOUT_POLL, Poco::Net::Socket::SELECT_READ))
	{
		Poco::FastMutex::ScopedLock lock(_mutex);

		char header[Frame::FRAME_HEADER_SIZE];
		int n = _socket.receiveBytes(header, Frame::FRAME_HEADER_SIZE);
		if (n == 0)
		{
			throw Poco::RemotingNG::ProtocolException("Connection unexpectedly closed");
		}
		else if (n < Frame::FRAME_HEADER_SIZE)
		{
			receiveNBytes(header + n, Frame::FRAME_HEADER_SIZE - n);
		}
		_lastFrame.update();
		pFrame = _framePool.borrowObject();
		if (pFrame)
		{
			std::memcpy(pFrame->bufferBegin(), header, Frame::FRAME_HEADER_SIZE);
			if (pFrame->getPayloadSize() > 0)
			{
				try
				{
					if (pFrame->getPayloadSize() <= pFrame->maxPayloadSize())
					{
						receiveNBytes(pFrame->payloadBegin(), pFrame->getPayloadSize());
					}
					else throw Poco::RemotingNG::ProtocolException("Invalid frame payload size");
				}
				catch (...)
				{
					_framePool.returnObject(pFrame);
					throw;
				}
			}
			if (_logger.debug())
			{
				std::string msg(Poco::format("Received Frame, type=%08x, channel=%08x, flags=%04hx, psize=%hu"s,
					pFrame->type(),
					pFrame->channel(),
					pFrame->flags(),
					pFrame->getPayloadSize()));
				_logger.dump(msg, pFrame->payloadBegin(), pFrame->getPayloadSize());
			}
			return pFrame;
		}
		else throw Poco::OutOfMemoryException("No more frames available");
	}
	return pFrame;
}


int Connection::receiveNBytes(char* buffer, int bytes)
{
	int received = _socket.receiveBytes(buffer, bytes);
	while (received < bytes)
	{
		int n = _socket.receiveBytes(buffer + received, bytes - received);
		if (n > 0)
			received += n;
		else
			throw Poco::RemotingNG::ProtocolException("Incomplete frame received");
	}
	return received;
}


} } } // namespace Poco::RemotingNG::TCP
