//
// ServerConnection.cpp
//
// Library: RemotingNG/TCP
// Package: TCP
// Module:  ServerConnection
//
// Copyright (c) 2006-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/RemotingNG/TCP/ServerConnection.h"
#include "Poco/RemotingNG/TCP/ConnectionManager.h"
#include "Poco/RemotingNG/TCP/Connection.h"
#include "Poco/RemotingNG/TCP/FrameHandler.h"
#include "Poco/RemotingNG/TCP/ServerTransport.h"
#include "Poco/RemotingNG/TCP/Transport.h"
#include "Poco/RemotingNG/TCP/ChannelStream.h"
#include "Poco/RemotingNG/EventDispatcher.h"
#include "Poco/RemotingNG/Context.h"
#include "Poco/RemotingNG/ORB.h"
#include "Poco/BinaryReader.h"
#include "Poco/MemoryStream.h"
#include "Poco/RefCountedObject.h"
#include "Poco/Runnable.h"
#include "Poco/Thread.h"
#include "Poco/Clock.h"
#include "Poco/URI.h"
#include <deque>


using namespace std::string_literals;


namespace Poco {
namespace RemotingNG {
namespace TCP {


class AuthFrameHandler: public FrameHandler
{
public:
	typedef Poco::AutoPtr<AuthFrameHandler> Ptr;

	AuthFrameHandler(Listener::Ptr pListener, CredentialsStore::Ptr pCredentialsStore, Poco::Logger& logger):
		_pListener(pListener),
		_pCredentialsStore(pCredentialsStore),
		_logger(logger)
	{
	}

	bool handleFrame(Connection::Ptr pConnection, Frame::Ptr pFrame)
	{
		if (pFrame->type() == Frame::FRAME_TYPE_AUTH || pFrame->type() == Frame::FRAME_TYPE_AUTC)
		{
			Poco::UInt64 authToken = 0;
			Authenticator::Ptr pAuthenticator = _pListener->getAuthenticator();
			AuthenticateResult authResult;
			if (pAuthenticator)
			{
				try
				{
					Poco::MemoryInputStream istr(pFrame->payloadBegin(), pFrame->getPayloadSize());
					Poco::BinaryReader reader(istr, Poco::BinaryReader::NETWORK_BYTE_ORDER);

					std::string mechanism;
					Poco::UInt32 conversationID = 0;
					if (pFrame->type() == Frame::FRAME_TYPE_AUTH)
					{
						reader >> mechanism;
					}
					else
					{
						reader >> conversationID;
					}

					Poco::UInt8 size;
					reader >> size;
					Credentials creds;
					if (!mechanism.empty())
					{
						creds.setAttribute(Credentials::ATTR_MECHANISM, mechanism);
					}
					for (Poco::UInt8 i = 0; i < size; i++)
					{
						std::string key;
						std::string value;
						reader >> key >> value;
						creds.setAttribute(key, value);
					}

					Poco::RemotingNG::ScopedContext scopedContext;
					Context::Ptr pContext = scopedContext.context();
					pContext->setValue("transport"s, Transport::PROTOCOL);
					pContext->setValue("remoteAddress"s, pConnection->remoteAddress());
					pContext->setValue("localAddress"s, pConnection->localAddress());
					pContext->setValue("id"s, pConnection->id());
					pContext->setValue("connection"s, pConnection.get());
					pContext->clearCredentials();

					authResult = pAuthenticator->authenticate(creds, conversationID);
					if (authResult.done())
					{
						authToken = _pCredentialsStore->addCredentials(authResult.credentials());
					}
				}
				catch (Poco::Exception& exc)
				{
					_logger.log(exc);
				}
			}
			sendAUTR(pConnection, pFrame->channel(), authResult, authToken);
			return true;
		}
		else return false;
	}

	void sendAUTR(Connection::Ptr pConnection, Poco::UInt32 channel, const AuthenticateResult& authResult, Poco::UInt64 token)
	{
		Frame::Ptr pFrame = new Frame(Frame::FRAME_TYPE_AUTR, channel, Frame::FRAME_FLAG_EOM, 256);
		Poco::MemoryOutputStream ostr(pFrame->payloadBegin(), pFrame->maxPayloadSize());
		Poco::BinaryWriter writer(ostr, Poco::BinaryWriter::NETWORK_BYTE_ORDER);

		writer << static_cast<Poco::UInt8>(authResult.state());
		writer << authResult.conversationID();

		// Write credentials only if state = continue.
		if (authResult.cont())
		{
			writer << static_cast<Poco::UInt8>(authResult.credentials().countAttributes());

			std::vector<std::string> keys = authResult.credentials().enumerateAttributes();
			for (std::vector<std::string>::const_iterator it = keys.begin(); it != keys.end(); ++it)
			{
				writer << *it << authResult.credentials().getAttribute(*it);
			}
		}
		else
		{
			writer << static_cast<Poco::UInt8>(0);
		}

		if (authResult.done())
		{
			writer << token;
		}

		pFrame->setPayloadSize(static_cast<Poco::UInt16>(ostr.charsWritten()));
		pConnection->sendFrame(pFrame);
	}

private:
	Listener::Ptr _pListener;
	CredentialsStore::Ptr _pCredentialsStore;
	Poco::Logger& _logger;
};


class SubscriptionChanges: public Poco::Runnable, public Poco::RefCountedObject
	/// The changes to event subscriptions that a connection asks for.
	///
	/// A change is made by the thread of the connection, at once, if the
	/// EventDispatcher is free. That thread does not wait for an
	/// EventDispatcher that is busy: it is busy while it delivers an
	/// event, and if that event goes to this connection and wants a
	/// reply, the reply has to pass the very thread that would be waiting.
	/// The change is then made by a thread of the listener's thread pool
	/// as soon as the EventDispatcher is free. Until it has been made, the
	/// changes and the requests that arrive on the connection are kept
	/// and taken up in the order of their arrival, so that none of them
	/// overtakes a change that was asked for before it.
{
public:
	using Ptr = Poco::AutoPtr<SubscriptionChanges>;

	SubscriptionChanges(Listener::Ptr pListener, Poco::Logger& logger):
		_pListener(pListener),
		_logger(logger)
	{
	}

	void change(bool subscribe, const std::string& subscriberURI)
		/// Subscribes or unsubscribes the subscriber, now if nothing
		/// is pending and its EventDispatcher is free, otherwise after
		/// what is pending.
	{
		Item item;
		item.type = subscribe ? Item::SUBSCRIBE : Item::UNSUBSCRIBE;
		item.subscriberURI = subscriberURI;
		if (subscribe) item.expireTime += _pListener->getEventSubscriptionTimeout().totalMicroseconds();

		{
			Poco::FastMutex::ScopedLock lock(_mutex);

			if (!_working && make(item, false)) return;
			_pending.push_back(item);
			if (_working) return;
			_working = true;
			// The thread that is asked for may find the connection gone,
			// and with it everyone else who holds this object.
			_pSelf.assign(this, true);
		}
		try
		{
			_pListener->connectionManager().threadPool().start(*this);
		}
		catch (Poco::Exception&)
		{
			// There is no thread to be had: what is pending is done here.
			run();
		}
	}

	bool defer(ServerTransport::Ptr pServerTransport)
		/// Takes the request over if changes are pending: it is begun
		/// when they have been made. Returns false if nothing is
		/// pending, and the request is the caller's to begin.
	{
		Poco::FastMutex::ScopedLock lock(_mutex);

		if (!_working) return false;
		Item item;
		item.type = Item::REQUEST;
		item.pServerTransport = pServerTransport;
		_pending.push_back(item);
		return true;
	}

	void run()
	{
		Ptr pSelf;
		{
			Poco::FastMutex::ScopedLock lock(_mutex);
			pSelf.swap(_pSelf);
		}
		for (;;)
		{
			Item item;
			{
				Poco::FastMutex::ScopedLock lock(_mutex);

				if (_pending.empty())
				{
					_working = false;
					break;
				}
				item = _pending.front();
				_pending.pop_front();
			}
			const std::string what(item.type == Item::REQUEST ? "begin a request that waited for an event subscription change"s : "change an event subscription"s);
			try
			{
				if (item.type == Item::REQUEST)
					begin(*_pListener, item.pServerTransport);
				else
					make(item, true);
			}
			catch (Poco::Exception& exc)
			{
				_logger.warning("Failed to %s: %s"s, what, exc.displayText());
			}
			catch (...)
			{
				_logger.warning("Failed to %s."s, what);
			}
		}
	}

	static void begin(Listener& listener, ServerTransport::Ptr pServerTransport)
		/// Has the request served by a thread of the listener's thread pool.
	{
		listener.connectionManager().threadPool().start(*pServerTransport);
		Poco::Thread::yield();
		pServerTransport->waitReady();
	}

private:
	struct Item
	{
		enum Type
		{
			SUBSCRIBE,
			UNSUBSCRIBE,
			REQUEST
		};

		Type type = SUBSCRIBE;
		std::string subscriberURI;
		Poco::Clock expireTime;
		ServerTransport::Ptr pServerTransport;
	};

	bool make(const Item& item, bool wait)
		/// Makes the change. If the EventDispatcher is busy, waits for
		/// it, or returns false with nothing changed.
	{
		Poco::URI dispatcherURI(item.subscriberURI);
		dispatcherURI.setAuthority(_pListener->endPoint());
		dispatcherURI.setFragment("");
		Poco::RemotingNG::EventDispatcher::Ptr pEventDispatcher = Poco::RemotingNG::ORB::instance().findEventDispatcher(dispatcherURI.toString(), Transport::PROTOCOL);
		if (item.type == Item::SUBSCRIBE)
		{
			if (!wait) return pEventDispatcher->trySubscribe(item.subscriberURI, item.subscriberURI, item.expireTime);
			pEventDispatcher->subscribe(item.subscriberURI, item.subscriberURI, item.expireTime);
		}
		else
		{
			if (!wait) return pEventDispatcher->tryUnsubscribe(item.subscriberURI);
			pEventDispatcher->unsubscribe(item.subscriberURI);
		}
		return true;
	}

	Listener::Ptr _pListener;
	Poco::Logger& _logger;
	std::deque<Item> _pending;
	bool _working = false;
	Ptr _pSelf;
	Poco::FastMutex _mutex;
};


class RequestFrameHandler: public FrameHandler
{
public:
	typedef Poco::AutoPtr<RequestFrameHandler> Ptr;

	RequestFrameHandler(Listener::Ptr pListener, CredentialsStore::Ptr pCredentialsStore, SubscriptionChanges::Ptr pSubscriptionChanges):
		_pListener(pListener),
		_pCredentialsStore(pCredentialsStore),
		_pSubscriptionChanges(pSubscriptionChanges)
	{
	}

	bool handleFrame(Connection::Ptr pConnection, Frame::Ptr pFrame)
	{
		if (pFrame->type() == Frame::FRAME_TYPE_REQU && (pFrame->flags() & Frame::FRAME_FLAG_CONT) == 0)
		{
			Poco::SharedPtr<ChannelInputStream> pRequestStream = new ChannelInputStream(pConnection, pFrame->type(), pFrame->channel(), _pListener->getTimeout());
			Poco::SharedPtr<ChannelOutputStream> pReplyStream;
			if ((pFrame->flags() & Frame::FRAME_FLAG_ONEWAY) == 0)
			{
				Poco::UInt16 flags(0);
				if (pFrame->flags() & Frame::FRAME_FLAG_DEFLATE)
					flags |= Frame::FRAME_FLAG_DEFLATE;
				pReplyStream = new ChannelOutputStream(pConnection, Frame::FRAME_TYPE_REPL, pFrame->channel(), flags);
			}
			ServerTransport::Ptr pServerTransport = new ServerTransport(
				*_pListener, _pCredentialsStore, pRequestStream, pReplyStream,
				(pFrame->flags() & Frame::FRAME_FLAG_DEFLATE) != 0,
				(pFrame->flags() & Frame::FRAME_FLAG_AUTH) != 0);
			// A request waits for the subscription changes that were asked
			// for before it and have not been made yet.
			if (!_pSubscriptionChanges->defer(pServerTransport))
			{
				SubscriptionChanges::begin(*_pListener, pServerTransport);
			}
			bool queued = pRequestStream->rdbuf()->queue()->handleFrame(pConnection, pFrame);
			poco_assert (queued);
			return true;
		}
		else return false;
	}

private:
	Listener::Ptr _pListener;
	CredentialsStore::Ptr _pCredentialsStore;
	SubscriptionChanges::Ptr _pSubscriptionChanges;
};


class EventSubscriptionFrameHandler: public FrameHandler
{
public:
	typedef Poco::AutoPtr<EventSubscriptionFrameHandler> Ptr;

	explicit EventSubscriptionFrameHandler(SubscriptionChanges::Ptr pSubscriptionChanges):
		_pSubscriptionChanges(pSubscriptionChanges)
	{
	}

	bool handleFrame(Connection::Ptr pConnection, Frame::Ptr pFrame)
	{
		if (pFrame->type() == Frame::FRAME_TYPE_EVSU || pFrame->type() == Frame::FRAME_TYPE_EVUN)
		{
			std::string suri(pFrame->payloadBegin(), pFrame->getPayloadSize());
			_pSubscriptionChanges->change(pFrame->type() == Frame::FRAME_TYPE_EVSU, suri);
			pConnection->returnFrame(pFrame);
			return true;
		}
		return false;
	}

private:
	SubscriptionChanges::Ptr _pSubscriptionChanges;
};


ServerConnection::ServerConnection(Listener::Ptr pListener, const Poco::Net::StreamSocket& socket):
	Poco::Net::TCPServerConnection(socket),
	_pListener(pListener),
	_pCredentialsStore(new CredentialsStore),
	_logger(Poco::Logger::get("RemotingNG.TCP.ServerConnection"s))
{
}


ServerConnection::~ServerConnection()
{
}


void ServerConnection::run()
{
	if (_logger.debug()) _logger.debug("ServerConnection started."s);
	Connection::Ptr pConnection = new Connection(socket(), Connection::MODE_SERVER);
	AuthFrameHandler::Ptr pAuthFrameHandler = new AuthFrameHandler(_pListener, _pCredentialsStore, _logger);
	SubscriptionChanges::Ptr pSubscriptionChanges = new SubscriptionChanges(_pListener, _logger);
	EventSubscriptionFrameHandler::Ptr pEventSubFrameHandler = new EventSubscriptionFrameHandler(pSubscriptionChanges);
	RequestFrameHandler::Ptr pRequestFrameHandler = new RequestFrameHandler(_pListener, _pCredentialsStore, pSubscriptionChanges);
	pConnection->setHandshakeTimeout(_pListener->getHandshakeTimeout());
	pConnection->pushFrameHandler(pAuthFrameHandler);
	pConnection->pushFrameHandler(pEventSubFrameHandler);
	pConnection->pushFrameHandler(pRequestFrameHandler);
	_pListener->connectionManager().registerConnection(pConnection);
	_pListener->registerEventFrameHandler(pConnection);
	try
	{
		_pListener->connectionAccepted(pConnection);
	}
	catch (Poco::Exception& exc)
	{
		_logger.error("connectionAccepted event handler threw exception: %s"s, exc.displayText());
	}
	catch (...)
	{
		_logger.error("connectionAccepted event handler threw unknown exception"s);
	}
	try
	{
		pConnection->run();
	}
	catch (Poco::Exception& exc)
	{
		_logger.log(exc);
	}
	_pListener->connectionManager().unregisterConnection(pConnection);
	pConnection->popFrameHandler(pRequestFrameHandler);
	pConnection->popFrameHandler(pEventSubFrameHandler);
	pConnection->popFrameHandler(pAuthFrameHandler);
	if (_logger.debug()) _logger.debug("ServerConnection done."s);
}


} } } // namespace Poco::RemotingNG::TCP
