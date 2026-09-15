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


class RequestFrameHandler: public FrameHandler
{
public:
	typedef Poco::AutoPtr<RequestFrameHandler> Ptr;

	RequestFrameHandler(Listener::Ptr pListener, CredentialsStore::Ptr pCredentialsStore):
		_pListener(pListener),
		_pCredentialsStore(pCredentialsStore)
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
	CredentialsStore::Ptr _pCredentialsStore;
};


class EventSubscriptionFrameHandler: public FrameHandler
{
public:
	typedef Poco::AutoPtr<EventSubscriptionFrameHandler> Ptr;

	EventSubscriptionFrameHandler(Listener::Ptr pListener):
		_pListener(pListener)
	{
	}

	bool handleFrame(Connection::Ptr pConnection, Frame::Ptr pFrame)
	{
		if (pFrame->type() == Frame::FRAME_TYPE_EVSU || pFrame->type() == Frame::FRAME_TYPE_EVUN)
		{
			Poco::RemotingNG::ORB& orb = Poco::RemotingNG::ORB::instance();
			std::string suri(pFrame->payloadBegin(), pFrame->getPayloadSize());
			Poco::URI dispURI(suri);
			dispURI.setAuthority(_pListener->endPoint());
			dispURI.setFragment("");
			Poco::RemotingNG::EventDispatcher::Ptr pEventDispatcher = orb.findEventDispatcher(dispURI.toString(), Transport::PROTOCOL);
			if (pFrame->type() == Frame::FRAME_TYPE_EVSU)
			{
				Poco::Clock expire;
				expire += _pListener->getEventSubscriptionTimeout().totalMicroseconds();
				pEventDispatcher->subscribe(suri, suri, expire);
			}
			else
			{
				pEventDispatcher->unsubscribe(suri);
			}
			pConnection->returnFrame(pFrame);
			return true;
		}
		return false;
	}

private:
	Listener::Ptr _pListener;
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
	EventSubscriptionFrameHandler::Ptr pEventSubFrameHandler = new EventSubscriptionFrameHandler(_pListener);
	RequestFrameHandler::Ptr pRequestFrameHandler = new RequestFrameHandler(_pListener, _pCredentialsStore);
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
