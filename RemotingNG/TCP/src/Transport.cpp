//
// Transport.cpp
//
// Library: RemotingNG/TCP
// Package: TCP
// Module:  Transport
//
// Copyright (c) 2006-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/RemotingNG/TCP/Transport.h"
#include "Poco/RemotingNG/TCP/ConnectionManager.h"
#include "Poco/RemotingNG/TCP/ChannelStream.h"
#include "Poco/RemotingNG/TCP/Frame.h"
#include "Poco/RemotingNG/TCP/FrameHandler.h"
#include "Poco/RemotingNG/Authenticator.h"
#include "Poco/RemotingNG/RemotingException.h"
#include "Poco/InflatingStream.h"
#include "Poco/DeflatingStream.h"
#include "Poco/MemoryStream.h"
#include "Poco/CountingStream.h"
#include "Poco/BinaryWriter.h"
#include "Poco/BinaryReader.h"
#include "Poco/StreamCopier.h"
#include "Poco/NumberFormatter.h"


using namespace std::string_literals;


namespace Poco {
namespace RemotingNG {
namespace TCP {


class AuthResponseFrameHandler: public FrameHandler
{
public:
	typedef Poco::AutoPtr<AuthResponseFrameHandler> Ptr;

	explicit AuthResponseFrameHandler(Poco::UInt32 channel):
		_channel(channel),
		_authToken(0)
	{
	}

	bool handleFrame(Connection::Ptr pConnection, Frame::Ptr pFrame)
	{
		if (pFrame->channel() == _channel && pFrame->type() == Frame::FRAME_TYPE_AUTR)
		{
			Poco::MemoryInputStream istr(pFrame->payloadBegin(), pFrame->getPayloadSize());
			Poco::BinaryReader reader(istr, Poco::BinaryReader::NETWORK_BYTE_ORDER);
			Poco::UInt8 state;
			reader >> state;
			Poco::UInt32 conversationID;
			reader >> conversationID;
			Poco::UInt8 size;
			reader >> size;
			Credentials creds;
			for (Poco::UInt8 i = 0; i < size; i++)
			{
				std::string key;
				std::string value;
				reader >> key >> value;
				creds.setAttribute(key, value);
			}
			_authResult = AuthenticateResult(static_cast<AuthenticateResult::State>(state), creds, conversationID);

			if (state == AuthenticateResult::AUTH_DONE)
			{
				reader >> _authToken;
			}

			_received.set();
			return true;
		}
		else return false;
	}

	Poco::UInt64 token() const
	{
		return _authToken;
	}

	const AuthenticateResult& result() const
	{
		return _authResult;
	}

	void wait(long timeout)
	{
		_received.wait(timeout);
	}

private:
	Poco::UInt32 _channel;
	Poco::Event _received;

	AuthenticateResult _authResult;
	Poco::UInt64 _authToken;
};


const std::string Transport::PROTOCOL("tcp");


Transport::Transport(ConnectionManager& connectionManager):
	_connectionManager(connectionManager),
	_timeout(DEFAULT_TIMEOUT, 0),
	_compression(false),
	_channel(0),
	_authToken(0),
	_authConnectionId(0),
	_logger(Poco::Logger::get("RemotingNG.TCP.Transport"s))
{
}


Transport::~Transport()
{
	try
	{
		if (_pConnection && _channel != 0)
		{
			_pConnection->releaseChannel(_channel);
		}
	}
	catch (...)
	{
		poco_unexpected();
	}
}


Poco::Timespan Transport::getTimeout() const
{
	return _timeout;
}


void Transport::setTimeout(const Poco::Timespan& timeout)
{
	_timeout = timeout;
}


bool Transport::isCompressionEnabled() const
{
	return _compression;
}


void Transport::enableCompression(bool enable)
{
	_compression = enable;
}


const std::string& Transport::endPoint() const
{
	return _endPoint;
}


void Transport::setAuthenticator(ClientAuthenticator::Ptr pAuthenticator)
{
	_pClientAuthenticator = pAuthenticator;
}


ClientAuthenticator::Ptr Transport::getAuthenticator() const
{
	return _pClientAuthenticator;
}


void Transport::setCredentials(const Credentials& credentials)
{
	_credentials = credentials;
	// force re-authentication with next request
	_authToken = 0;
	_authConnectionId = 0;
}


const Credentials& Transport::getCredentials() const
{
	return _credentials;
}


void Transport::connect(const std::string& endPoint)
{
	_endPoint = endPoint;
	_endPointURI = endPoint;
}


void Transport::disconnect()
{
	_endPoint.clear();
	_endPointURI.clear();
}


bool Transport::connected() const
{
	return !_endPoint.empty();
}


Poco::RemotingNG::Serializer& Transport::beginMessage(const Poco::RemotingNG::Identifiable::ObjectId& oid, const Poco::RemotingNG::Identifiable::TypeId& tid, const std::string& messageName, Poco::RemotingNG::SerializerBase::MessageType messageType)
{
	if (_logger.debug())
	{
		_logger.debug("Beginning one-way request '%s' to %s."s, messageName, _endPoint);
	}
	_pConnection = _connectionManager.getConnection(_endPointURI);
	_channel = _pConnection->allocChannel();
	authenticate();
	setupSerializer(oid, tid, messageType, Frame::FRAME_FLAG_ONEWAY);
	return _serializer;
}


void Transport::sendMessage(const Poco::RemotingNG::Identifiable::ObjectId& /*oid*/, const Poco::RemotingNG::Identifiable::TypeId& /*tid*/, const std::string& messageName, Poco::RemotingNG::SerializerBase::MessageType /*messageType*/)
{
	if (_logger.debug()) _logger.debug("Completing one-way request."s);

	if (_pDeflatingStream)
	{
		_pDeflatingStream->close();
		_pDeflatingStream = 0;
	}
	_pRequestStream->close();
	_pRequestStream = 0;
	_pConnection->releaseChannel(_channel);
	_channel = 0;
}


Poco::RemotingNG::Serializer& Transport::beginRequest(const Poco::RemotingNG::Identifiable::ObjectId& oid, const Poco::RemotingNG::Identifiable::TypeId& tid, const std::string& messageName, Poco::RemotingNG::SerializerBase::MessageType messageType)
{
	if (_logger.debug())
	{
		_logger.debug("Beginning request '%s' to %s."s, messageName, _endPoint);
	}
	_pConnection = _connectionManager.getConnection(_endPointURI);
	_channel = _pConnection->allocChannel();
	authenticate();
	setupSerializer(oid, tid, messageType, 0);
	return _serializer;
}


Poco::RemotingNG::Deserializer& Transport::sendRequest(const Poco::RemotingNG::Identifiable::ObjectId& /*oid*/, const Poco::RemotingNG::Identifiable::TypeId& /*tid*/, const std::string& messageName, Poco::RemotingNG::SerializerBase::MessageType messageType)
{
	if (_logger.debug()) _logger.debug("Sending request..."s);

	// Set up reply stream before closing request to avoid
	// race condition (no reply handler registered) with fast
	// connections.

	Poco::UInt32 frameType = 0;
	switch (messageType)
	{
	case Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST:
		frameType = Frame::FRAME_TYPE_REPL;
		break;

	case Poco::RemotingNG::SerializerBase::MESSAGE_EVENT:
		frameType = Frame::FRAME_TYPE_EVNR;
		break;

	default:
		poco_bugcheck();
	}

	_pReplyStream = new ChannelInputStream(_pConnection, frameType, _channel, _timeout);

	if (_pDeflatingStream)
	{
		_pDeflatingStream->close();
		_pDeflatingStream = 0;
	}
	_pRequestStream->close();
	_pRequestStream = 0;

	Poco::Timestamp requestTime;
	if (_pReplyStream->peek() < 0)
	{
		if (_timeout != 0 && requestTime.isElapsed(_timeout.totalMicroseconds()))
		{
			throw Poco::TimeoutException(Poco::format("request %s to object %s"s, messageName, _endPoint));
		}
		else
		{
			throw TransportException(Poco::format("no response received for request %s to object %s (server may be gone)"s, messageName, _endPoint));
		}
	}

	if (_compression)
	{
		_pInflatingStream = new Poco::InflatingInputStream(*_pReplyStream);
		_deserializer.setup(*_pInflatingStream);
	}
	else
	{
		_deserializer.setup(*_pReplyStream);
	}
	return _deserializer;
}


void Transport::endRequest()
{
	if (_logger.debug()) _logger.debug("Request done."s);

	_serializer.reset();
	_deserializer.reset();
	_pReplyStream = 0;
	_pInflatingStream = 0;
	_pConnection->releaseChannel(_channel);
	_channel = 0;
	_pConnection = 0;
}


void Transport::setupSerializer(const Poco::RemotingNG::Identifiable::ObjectId& oid, const Poco::RemotingNG::Identifiable::TypeId& tid, Poco::RemotingNG::SerializerBase::MessageType messageType, Poco::UInt16 frameFlags)
{
	Poco::UInt32 frameType = 0;
	switch (messageType)
	{
	case Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST:
		frameType = Frame::FRAME_TYPE_REQU;
		break;

	case Poco::RemotingNG::SerializerBase::MESSAGE_EVENT:
		frameType = Frame::FRAME_TYPE_EVNT;
		break;

	default:
		poco_bugcheck();
	}

	if (_compression) frameFlags |= Frame::FRAME_FLAG_DEFLATE;
	if (_authToken) frameFlags |= Frame::FRAME_FLAG_AUTH;

	_pRequestStream = new ChannelOutputStream(_pConnection, frameType, _channel, frameFlags);
	if (_compression)
	{
		_pDeflatingStream = new Poco::DeflatingOutputStream(*_pRequestStream);
		_serializer.setup(*_pDeflatingStream);
	}
	else
	{
		_serializer.setup(*_pRequestStream);
	}
	if (_authToken)
	{
		_serializer.serializeToken(_authToken);
	}
	if (messageType == Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST)
	{
		_serializer.serializeEndPoint(oid, tid);
	}
	else
	{
		std::string eoid(oid);
		eoid += '#';
		eoid += _endPointURI.getFragment();
		_serializer.serializeEndPoint(eoid, tid);
	}
}


namespace
{
	void serializeCredentials(const Credentials& creds, Poco::BinaryWriter& writer)
	{
		poco_assert (creds.countAttributes() < 256);

		writer << static_cast<Poco::UInt8>(creds.countAttributes());
		std::vector<std::string> keys = creds.enumerateAttributes();
		for (std::vector<std::string>::const_iterator it = keys.begin(); it != keys.end(); ++it)
		{
			writer << *it << creds.getAttribute(*it);
		}
	}
}


void Transport::authenticate()
{
	if (_credentials.countAttributes() > 0 && _authConnectionId != _pConnection->id() && _pClientAuthenticator)
	{
		if (verifyCredentials(_credentials))
		{
			if (_logger.debug())
			{
				_logger.debug("Authenticating transport..."s);
			}
			AuthResponseFrameHandler::Ptr pAuthResponseFrameHandler = new AuthResponseFrameHandler(_channel);
			_pConnection->pushFrameHandler(pAuthResponseFrameHandler);
			try
			{
				Credentials clientCreds(_credentials);
				std::string mechanism = _pClientAuthenticator->startAuthentication(clientCreds);

				Frame::Ptr pFrame = new Frame(Frame::FRAME_TYPE_AUTH, _channel, Frame::FRAME_FLAG_EOM, Frame::FRAME_MAX_SIZE);

				Poco::MemoryOutputStream ostr(pFrame->payloadBegin(), pFrame->maxPayloadSize());
				Poco::BinaryWriter writer(ostr, Poco::BinaryWriter::NETWORK_BYTE_ORDER);

				writer << mechanism;
				serializeCredentials(clientCreds, writer);
				pFrame->setPayloadSize(static_cast<Poco::UInt16>(ostr.charsWritten()));
				_pConnection->sendFrame(pFrame);

				if (_logger.debug())
				{
					_logger.debug("Waiting for authentication response..."s);
				}

				bool cont = true;
				while (cont)
				{
					cont = false;
					pAuthResponseFrameHandler->wait(static_cast<long>(_timeout.totalMilliseconds()));
					if (_logger.debug())
					{
						_logger.debug("Authentication response received."s);
					}

					if (pAuthResponseFrameHandler->result().cont())
					{
						clientCreds.clearAttributes();
						if (_pClientAuthenticator->continueAuthentication(pAuthResponseFrameHandler->result().credentials(), clientCreds))
						{
							if (_logger.debug())
							{
								_logger.debug("Continuing authentication..."s);
							}

							Frame::Ptr pFrame = new Frame(Frame::FRAME_TYPE_AUTC, _channel, Frame::FRAME_FLAG_EOM, Frame::FRAME_MAX_SIZE);

							Poco::MemoryOutputStream ostr(pFrame->payloadBegin(), pFrame->maxPayloadSize());
							Poco::BinaryWriter writer(ostr, Poco::BinaryWriter::NETWORK_BYTE_ORDER);

							writer << pAuthResponseFrameHandler->result().conversationID();
							serializeCredentials(clientCreds, writer);
							pFrame->setPayloadSize(static_cast<Poco::UInt16>(ostr.charsWritten()));
							_pConnection->sendFrame(pFrame);

							cont = true;
						}
					}
				}

				if (pAuthResponseFrameHandler->result().done())
				{
					_authToken = pAuthResponseFrameHandler->token();
					_authConnectionId = _pConnection->id();
					if (_logger.debug())
					{
						_logger.debug("Authentication done."s);
					}
				}
				else throw AuthenticationFailedException("The server refused the provided credentials");
			}
			catch (Poco::Exception&)
			{
				_authToken = 0;
				_authConnectionId = 0;
				_pConnection->popFrameHandler(pAuthResponseFrameHandler);
				throw;
			}
			_pConnection->popFrameHandler(pAuthResponseFrameHandler);
		}
		else throw InvalidCredentialsException("Credentials too large");
	}
}


bool Transport::verifyCredentials(const Credentials& creds)
{
	if (creds.countAttributes() < 256)
	{
		Poco::CountingOutputStream ostr;
		Poco::BinaryWriter writer(ostr, Poco::BinaryWriter::NETWORK_BYTE_ORDER);
		serializeCredentials(creds, writer);
		return ostr.chars() <= Frame::FRAME_MAX_PAYLOAD_SIZE;
	}
	else return false;
}


} } } // namespace Poco::RemotingNG::TCP
