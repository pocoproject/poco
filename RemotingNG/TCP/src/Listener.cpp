//
// Listener.cpp
//
// Library: RemotingNG/TCP
// Package: TCP
// Module:  Listener
//
// Copyright (c) 2006-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/RemotingNG/TCP/Listener.h"
#include "Poco/RemotingNG/TCP/Transport.h"
#include "Poco/RemotingNG/TCP/ServerTransport.h"
#include "Poco/RemotingNG/TCP/ServerConnectionFactory.h"
#include "Poco/RemotingNG/TCP/FrameHandler.h"
#include "Poco/RemotingNG/TCP/ChannelStream.h"
#include "Poco/NumberFormatter.h"


namespace Poco {
namespace RemotingNG {
namespace TCP {


class EventFrameHandler: public FrameHandler
{
public:
	typedef Poco::AutoPtr<EventFrameHandler> Ptr;

	EventFrameHandler(Listener::Ptr pListener):
		_pListener(pListener)
	{
	}

	bool handleFrame(Connection::Ptr pConnection, Frame::Ptr pFrame)
	{
		if (pFrame->type() == Frame::FRAME_TYPE_EVNT && (pFrame->flags() & Frame::FRAME_FLAG_CONT) == 0)
		{
			Poco::SharedPtr<ChannelInputStream> pRequestStream = new ChannelInputStream(pConnection, pFrame->type(), pFrame->channel(), _pListener->getTimeout());
			Poco::SharedPtr<ChannelOutputStream> pReplyStream;
			if ((pFrame->flags() & Frame::FRAME_FLAG_ONEWAY) == 0)
			{
				Poco::UInt16 flags(0);
				if (pFrame->flags() & Frame::FRAME_FLAG_DEFLATE)
					flags |= Frame::FRAME_FLAG_DEFLATE;
				pReplyStream = new ChannelOutputStream(pConnection, Frame::FRAME_TYPE_EVNR, pFrame->channel(), flags);
			}
			ServerTransport::Ptr pServerTransport = new ServerTransport(
				*_pListener, 0, pRequestStream, pReplyStream,
				(pFrame->flags() & Frame::FRAME_FLAG_DEFLATE) != 0,
				false);
			_pListener->connectionManager().threadPool().start(*pServerTransport);
			Poco::Thread::yield();
			pServerTransport->waitReady();
			pRequestStream->rdbuf()->queue()->handleFrame(pConnection, pFrame);
			return true;
		}
		else return false;
	}

private:
	Listener::Ptr _pListener;
};


Poco::FastMutex Listener::_staticMutex;
Poco::UInt32 Listener::_nextSubscriberId(1);
Listener::Ptr Listener::_pDefaultListener;


Listener::Listener(ConnectionManager& connectionManager):
	Poco::RemotingNG::EventListener("0.0.0.0:0"),
	_connectionManager(connectionManager),
	_timeout(DEFAULT_TIMEOUT, 0),
	_handshakeTimeout(DEFAULT_HANDSHAKE_TIMEOUT, 0),
	_eventSubscriptionTimeout(DEFAULT_EVENT_SUBSCR_TIMEOUT, 0),
	_secure(false)
{
}


Listener::Listener(const std::string& endPoint, ConnectionManager& connectionManager):
	Poco::RemotingNG::EventListener(encodeEndPoint(endPoint)),
	_connectionManager(connectionManager),
	_timeout(DEFAULT_TIMEOUT, 0),
	_handshakeTimeout(DEFAULT_HANDSHAKE_TIMEOUT, 0),
	_eventSubscriptionTimeout(DEFAULT_EVENT_SUBSCR_TIMEOUT, 0),
	_secure(false)
{
	Poco::Net::SocketAddress addr(endPoint);
	Poco::Net::ServerSocket socket(addr);
	_pTCPServer = new Poco::Net::TCPServer(new ServerConnectionFactory(Ptr(this, true)), socket);
}


Listener::Listener(const std::string& endPoint, Poco::Net::TCPServerParams::Ptr pParams, ConnectionManager& connectionManager):
	Poco::RemotingNG::EventListener(encodeEndPoint(endPoint)),
	_connectionManager(connectionManager),
	_timeout(DEFAULT_TIMEOUT, 0),
	_handshakeTimeout(DEFAULT_HANDSHAKE_TIMEOUT, 0),
	_eventSubscriptionTimeout(DEFAULT_EVENT_SUBSCR_TIMEOUT, 0),
	_secure(false)
{
	Poco::Net::SocketAddress addr(endPoint);
	Poco::Net::ServerSocket socket(addr);
	_pTCPServer = new Poco::Net::TCPServer(new ServerConnectionFactory(Ptr(this, true)), socket, pParams);
}


Listener::Listener(const std::string& endPoint, const Poco::Net::ServerSocket& socket, Poco::Net::TCPServerParams::Ptr pParams, ConnectionManager& connectionManager):
	Poco::RemotingNG::EventListener(encodeEndPoint(endPoint)),
	_connectionManager(connectionManager),
	_timeout(DEFAULT_TIMEOUT, 0),
	_handshakeTimeout(DEFAULT_HANDSHAKE_TIMEOUT, 0),
	_eventSubscriptionTimeout(DEFAULT_EVENT_SUBSCR_TIMEOUT, 0),
	_secure(socket.secure())
{
	_pTCPServer = new Poco::Net::TCPServer(new ServerConnectionFactory(Ptr(this, true)), socket, pParams);
}


Listener::~Listener()
{
}


void Listener::setTimeout(Poco::Timespan timeout)
{
	_timeout = timeout;
}


Poco::Timespan Listener::getTimeout() const
{
	return _timeout;
}


void Listener::setHandshakeTimeout(Poco::Timespan timeout)
{
	_handshakeTimeout = timeout;
}


Poco::Timespan Listener::getHandshakeTimeout() const
{
	return _handshakeTimeout;
}


void Listener::setEventSubscriptionTimeout(Poco::Timespan timeout)
{
	_eventSubscriptionTimeout = timeout;
}


Poco::Timespan Listener::getEventSubscriptionTimeout() const
{
	return _eventSubscriptionTimeout;
}


ConnectionManager& Listener::connectionManager()
{
	return _connectionManager;
}


std::string Listener::subscribeToEvents(Poco::RemotingNG::EventSubscriber::Ptr pEventSubscriber)
{
	Poco::FastMutex::ScopedLock lock(_mutex);

	EventSubscription::Ptr pEventSubscription = new EventSubscription(*this, pEventSubscriber->uri(), nextSubscriberId());
	EventSubscriptionsMap::iterator it = _eventSubscriptions.find(pEventSubscriber);
	if (it == _eventSubscriptions.end())
	{
		_eventSubscriptions[pEventSubscriber] = pEventSubscription;
	}
	else
	{
		it->second->cancel();
		it->second = pEventSubscription;
	}
	long interval = static_cast<long>(_eventSubscriptionTimeout.totalMilliseconds()/2);
	_timer.scheduleAtFixedRate(pEventSubscription, interval, interval);
	pEventSubscription->run();
	return pEventSubscription->uri();
}


void Listener::unsubscribeFromEvents(Poco::RemotingNG::EventSubscriber::Ptr pEventSubscriber)
{
	Poco::FastMutex::ScopedLock lock(_mutex);

	EventSubscriptionsMap::iterator it = _eventSubscriptions.find(pEventSubscriber);
	if (it != _eventSubscriptions.end())
	{
		it->second->cancel();
		it->second->run(); // sends unsubscribe message
		_eventSubscriptions.erase(it);
	}
}


Poco::RemotingNG::EventSubscriber::Ptr Listener::findEventSubscriber(const std::string& path) const
{
	Poco::FastMutex::ScopedLock lock(_mutex);

	for (EventSubscriptionsMap::const_iterator it = _eventSubscriptions.begin(); it != _eventSubscriptions.end(); ++it)
	{
		if (it->second->path() == path)
		{
			return it->first;
		}
	}
	return Poco::RemotingNG::EventSubscriber::Ptr();
}


void Listener::start()
{
	if (_pTCPServer) _pTCPServer->start();
}


void Listener::stop()
{
	if (_pTCPServer) _pTCPServer->stop();
	_pTCPServer = 0;
}


const std::string& Listener::protocol() const
{
	return Transport::PROTOCOL;
}


std::string Listener::createURI(const Poco::RemotingNG::Identifiable::TypeId& typeId, const Poco::RemotingNG::Identifiable::ObjectId& objectId)
{
	std::string uri;
	std::string endp = endPoint();

	if (endp.compare(0, 3, "%2f") == 0 || endp.compare(0, 3, "%2F") == 0)
		uri = "remoting.unix://";
	else if (_secure)
		uri = "remoting.tcps://";
	else
		uri = "remoting.tcp://";
	uri += endp;
	uri += '/';
	uri += protocol();
	uri += '/';
	uri += typeId;
	uri += '/';
	uri += objectId;
	return uri;
}


bool Listener::handlesURI(const std::string& /*uri*/)
{
	return false;
}


void Listener::registerObject(Poco::RemotingNG::RemoteObject::Ptr /*pRemoteObject*/, Poco::RemotingNG::Skeleton::Ptr /*pSkeleton*/)
{
}


void Listener::unregisterObject(Poco::RemotingNG::RemoteObject::Ptr /*pRemoteObject*/)
{
}


Listener::EventSubscription::EventSubscription(Listener& listener, const std::string& uri, Poco::UInt32 id):
	_listener(listener),
	_uri(uri)
{
	Poco::URI suri(uri);
	Connection::Ptr pConnection = _listener.connectionManager().getConnection(suri);
	if (_listener.endPoint() != "0.0.0.0:0")
		suri.setAuthority(_listener.endPoint());
	else
		suri.setAuthority(pConnection->localAddress().toString());

	suri.setFragment(Poco::NumberFormatter::format(id));
	_suri = suri.toString();
	_path = suri.getPathEtc().substr(1);
}


const std::string& Listener::EventSubscription::uri() const
{
	return _suri;
}


const std::string& Listener::EventSubscription::path() const
{
	return _path;
}


void Listener::EventSubscription::run()
{
	Connection::Ptr pConnection = _listener.connectionManager().getConnection(_uri);
	if (!isCancelled())
	{
		_listener.registerEventFrameHandler(pConnection);
	}
	Frame::Ptr pFrame = new Frame(isCancelled() ? Frame::FRAME_TYPE_EVUN : Frame::FRAME_TYPE_EVSU, 0, Frame::FRAME_FLAG_EOM, static_cast<Poco::UInt16>(Frame::FRAME_HEADER_SIZE + _suri.size()));
	_suri.copy(pFrame->payloadBegin(), _suri.size());
	pFrame->setPayloadSize(static_cast<Poco::UInt16>(_suri.size()));
	pConnection->sendFrame(pFrame);
}


void Listener::registerEventFrameHandler(Connection::Ptr pConnection)
{
	if (!pConnection->hasAttribute("EventFrameHandler"))
	{
		pConnection->pushFrameHandler(new EventFrameHandler(Listener::Ptr(this, true)));
		pConnection->setAttribute("EventFrameHandler", "");
	}
}


Poco::UInt32 Listener::nextSubscriberId()
{
	Poco::FastMutex::ScopedLock lock(_staticMutex);
	return _nextSubscriberId++;
}


std::string Listener::encodeEndPoint(const std::string& endPoint)
{
	if (!endPoint.empty() && endPoint[0] == '/')
	{
		std::string encodedEndPoint;
		Poco::URI::encode(endPoint, "/", encodedEndPoint);
		return encodedEndPoint;
	}
	return endPoint;
}


Listener::Ptr Listener::defaultListener()
{
	Poco::FastMutex::ScopedLock lock(_staticMutex);
	if (!_pDefaultListener)
	{
		_pDefaultListener = new Listener;
	}
	return _pDefaultListener;
}


Listener::Ptr Listener::defaultListener(ConnectionManager& cm)
{
	Poco::FastMutex::ScopedLock lock(_staticMutex);
	if (!_pDefaultListener)
	{
		_pDefaultListener = new Listener(cm);
	}
	else if (&_pDefaultListener->connectionManager() != &cm)
	{
		throw Poco::IllegalStateException("A default Listener using a different ConnectionManager already exists");
	}
	return _pDefaultListener;
}


void Listener::makeDefaultListener()
{
	Poco::FastMutex::ScopedLock lock(_staticMutex);
	if (!_pDefaultListener)
	{
		_pDefaultListener.assign(this, true);
	}
	else throw Poco::IllegalStateException("A default Listener already exists");
}


} } } // namespace Poco::RemotingNG::TCP
