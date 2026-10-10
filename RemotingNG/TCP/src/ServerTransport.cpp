//
// ServerTransport.cpp
//
// Library: RemotingNG/TCP
// Package: TCP
// Module:  ServerTransport
//
// Copyright (c) 2006-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/RemotingNG/TCP/ServerTransport.h"
#include "Poco/RemotingNG/TCP/Listener.h"
#include "Poco/RemotingNG/TCP/Transport.h"
#include "Poco/RemotingNG/ORB.h"
#include "Poco/RemotingNG/Context.h"
#include "Poco/RemotingNG/Authorizer.h"


using namespace std::string_literals;


namespace Poco {
namespace RemotingNG {
namespace TCP {


ServerTransport::ServerTransport(Listener& listener, CredentialsStore::Ptr pCredentialsStore, const Poco::SharedPtr<ChannelInputStream>& pRequestStream, const Poco::SharedPtr<ChannelOutputStream>& pReplyStream, bool compressed, bool authenticated):
	_listener(listener),
	_pCredentialsStore(pCredentialsStore),
	_pRequestStream(pRequestStream),
	_pReplyStream(pReplyStream),
	_authenticated(authenticated),
	_pInflater(0),
	_pDeflater(0),
	_logger(Poco::Logger::get("RemotingNG.TCP.ServerTransport"s))
{
	if (compressed)
	{
		_pInflater = new Poco::InflatingInputStream(*_pRequestStream);
		if (_pReplyStream)
		{
			_pDeflater = new Poco::DeflatingOutputStream(*_pReplyStream);
		}
	}
}


ServerTransport::~ServerTransport()
{
	delete _pInflater;
	delete _pDeflater;
}


void ServerTransport::waitReady()
{
	_ready.wait();
}


bool ServerTransport::authenticate(const std::string& /*method*/)
{
	return _authenticated;
}


bool ServerTransport::authorize(const std::string& method, const std::string& permission)
{
	Authorizer::Ptr pAuth = _listener.getAuthorizer();
	if (pAuth)
	{
		return pAuth->authorize(method, permission);
	}
	return false;
}


Deserializer& ServerTransport::beginRequest()
{
	if (_logger.debug()) _logger.debug("Beginning request..."s);

	return _deserializer;
}


Serializer& ServerTransport::sendReply(SerializerBase::MessageType messageType)
{
	if (_logger.debug()) _logger.debug("Sending reply..."s);

	if (_pDeflater)
		_serializer.setup(*_pDeflater);
	else
		_serializer.setup(*_pReplyStream);
	return _serializer;
}


void ServerTransport::endRequest()
{
	if (_logger.debug()) _logger.debug("Request done."s);

	if (_pDeflater)
		_pDeflater->close();
	if (_pReplyStream)
		_pReplyStream->close();
}


void ServerTransport::run()
{
	Ptr pThis(this, true);
	_ready.set();

	if (_pInflater)
		_deserializer.setup(*_pInflater);
	else
		_deserializer.setup(*_pRequestStream);

	Poco::UInt64 authToken(0);
	if (_authenticated)
	{
		authToken = _deserializer.deserializeToken<Poco::UInt64>();
	}

	std::string oid;
	std::string tid;
	_deserializer.deserializeEndPoint(oid, tid);
	std::string path = Transport::PROTOCOL;
	path += '/';
	path += tid;
	path += '/';
	path += oid;

	Poco::RemotingNG::ScopedContext scopedContext;
	Context::Ptr pContext = scopedContext.context();
	pContext->setValue("transport", Transport::PROTOCOL);
	pContext->setValue("remoteAddress", _pRequestStream->rdbuf()->connection()->remoteAddress());
	pContext->setValue("localAddress", _pRequestStream->rdbuf()->connection()->localAddress());
	pContext->setValue("id", _pRequestStream->rdbuf()->connection()->id());
	pContext->setValue("connection", _pRequestStream->rdbuf()->connection().get());
	pContext->setValue("uri", path);
	pContext->clearCredentials();

	if (_authenticated)
	{
		try
		{
			scopedContext.context()->setCredentials(_pCredentialsStore->getCredentials(authToken));
		}
		catch (Poco::NotFoundException&)
		{
			_logger.error("Invalid authentication token"s);
			InvalidCredentialsException exc("Authentication token");
			sendReply(Poco::RemotingNG::SerializerBase::MESSAGE_FAULT).serializeFaultMessage("fault", exc);
			return;
		}
	}

	Poco::RemotingNG::ORB& orb = Poco::RemotingNG::ORB::instance();
	if (oid.find('#') != std::string::npos)
	{
		if (_logger.debug())
		{
			_logger.debug("Dispatching event to subscriber: %s"s, path);
		}
		Poco::RemotingNG::EventSubscriber::Ptr pEventSubscriber = _listener.findEventSubscriber(path);
		if (pEventSubscriber)
		{
			pEventSubscriber->invoke(*this, 0);
		}
		else
		{
			_logger.error("Unknown event subscriber: %s"s, path);
			UnknownEventSubscriberException exc(path);
			sendReply(Poco::RemotingNG::SerializerBase::MESSAGE_FAULT).serializeFaultMessage("fault", exc);
			endRequest();
		}
	}
	else
	{
		if (_logger.debug())
		{
			_logger.debug("Dispatching request to service object: %s"s, path);
		}

		bool serviceFound = orb.invoke(_listener, path, *this);
		if (!serviceFound)
		{
			_logger.error("Unknown service object: %s"s, path);
			UnknownObjectException exc(path);
			sendReply(Poco::RemotingNG::SerializerBase::MESSAGE_FAULT).serializeFaultMessage("fault", exc);
			endRequest();
		}
	}
}


} } } // namespace Poco::RemotingNG::TCP
