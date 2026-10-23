//
// RemotingNG.cppm
//
// C++ module file
//
// Copyright (c) 2026, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//

module;

#ifdef ENABLE_REMOTINGNG
#include "Poco/RemotingNG/AttributedObject.h"
#include "Poco/RemotingNG/Authenticator.h"
#include "Poco/RemotingNG/Authorizer.h"
#include "Poco/RemotingNG/BinaryDeserializer.h"
#include "Poco/RemotingNG/BinarySerializer.h"
#include "Poco/RemotingNG/ClientHelper.h"
#include "Poco/RemotingNG/Context.h"
#include "Poco/RemotingNG/Credentials.h"
#include "Poco/RemotingNG/Deserializer.h"
#include "Poco/RemotingNG/EventDispatcher.h"
#include "Poco/RemotingNG/EventFilter.h"
#include "Poco/RemotingNG/EventListener.h"
#include "Poco/RemotingNG/EventSubscriber.h"
#include "Poco/RemotingNG/Identifiable.h"
#include "Poco/RemotingNG/Listener.h"
#include "Poco/RemotingNG/MethodHandler.h"
#include "Poco/RemotingNG/ORB.h"
#include "Poco/RemotingNG/ProxyFactory.h"
#include "Poco/RemotingNG/ProxyFactoryManager.h"
#include "Poco/RemotingNG/Proxy.h"
#include "Poco/RemotingNG/RemoteObject.h"
#include "Poco/RemotingNG/RemotingException.h"
#include "Poco/RemotingNG/RemotingNG.h"
#include "Poco/RemotingNG/SerializerBase.h"
#include "Poco/RemotingNG/Serializer.h"
#include "Poco/RemotingNG/ServerHelper.h"
#include "Poco/RemotingNG/ServerTransport.h"
#include "Poco/RemotingNG/Skeleton.h"
#include "Poco/RemotingNG/TransportFactory.h"
#include "Poco/RemotingNG/TransportFactoryManager.h"
#include "Poco/RemotingNG/Transport.h"
#include "Poco/RemotingNG/TypeDeserializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"
#include "Poco/RemotingNG/URIUtility.h"
#endif

export module Poco:RemotingNG;

#ifdef ENABLE_REMOTINGNG_HTTP
export import :RemotingNG.HTTP;
#endif
#ifdef ENABLE_REMOTINGNG_JSONRPC
export import :RemotingNG.JSONRPC;
#endif
#ifdef ENABLE_REMOTINGNG_REST
export import :RemotingNG.REST;
#endif
#ifdef ENABLE_REMOTINGNG_SOAP
export import :RemotingNG.SOAP;
#endif
#ifdef ENABLE_REMOTINGNG_TCP
export import :RemotingNG.TCP;
#endif

export namespace Poco::RemotingNG {
	#ifdef ENABLE_REMOTINGNG
	using Poco::RemotingNG::AttributedObject;
	using Poco::RemotingNG::AuthenticateResult;
	using Poco::RemotingNG::AuthenticationFailedException;
	using Poco::RemotingNG::Authenticator;
	using Poco::RemotingNG::Authorizer;
	using Poco::RemotingNG::BinaryDeserializer;
	using Poco::RemotingNG::BinarySerializer;
	using Poco::RemotingNG::ClientHelper;
	using Poco::RemotingNG::Context;
	using Poco::RemotingNG::Credentials;
	using Poco::RemotingNG::Deserializer;
	using Poco::RemotingNG::DeserializerException;
	using Poco::RemotingNG::EventDispatcher;
	using Poco::RemotingNG::EventFilter;
	using Poco::RemotingNG::EventListener;
	using Poco::RemotingNG::EventSubscriber;
	using Poco::RemotingNG::HysteresisFilter;
	using Poco::RemotingNG::Identifiable;
	using Poco::RemotingNG::InvalidCredentialsException;
	using Poco::RemotingNG::IsGreaterThanFilter;
	using Poco::RemotingNG::IsGreaterThanOrEqualToFilter;
	using Poco::RemotingNG::IsLessThanFilter;
	using Poco::RemotingNG::IsLessThanOrEqualToFilter;
	using Poco::RemotingNG::Listener;
	using Poco::RemotingNG::LogicalAndFilter;
	using Poco::RemotingNG::LogicalOrFilter;
	using Poco::RemotingNG::LogicalXorFilter;
	using Poco::RemotingNG::MethodHandler;
	using Poco::RemotingNG::MethodNotFoundException;
	using Poco::RemotingNG::MinimumDeltaFilter;
	using Poco::RemotingNG::MinimumIntervalAndDeltaFilter;
	using Poco::RemotingNG::MinimumIntervalFilter;
	using Poco::RemotingNG::MinimumIntervalOrDeltaFilter;
	using Poco::RemotingNG::MissingParamException;
	using Poco::RemotingNG::NoPermissionException;
	using Poco::RemotingNG::ORB;
	using Poco::RemotingNG::ORBDisabledException;
	using Poco::RemotingNG::ProtocolException;
	using Poco::RemotingNG::Proxy;
	using Poco::RemotingNG::ProxyFactory;
	using Poco::RemotingNG::ProxyFactoryManager;
	using Poco::RemotingNG::RemoteException;
	using Poco::RemotingNG::RemoteObject;
	using Poco::RemotingNG::RemotingException;
	using Poco::RemotingNG::ScopedContext;
	using Poco::RemotingNG::SecurityException;
	using Poco::RemotingNG::Serializer;
	using Poco::RemotingNG::SerializerBase;
	using Poco::RemotingNG::SerializerException;
	using Poco::RemotingNG::ServerHelper;
	using Poco::RemotingNG::ServerTransport;
	using Poco::RemotingNG::Skeleton;
	using Poco::RemotingNG::Transport;
	using Poco::RemotingNG::TransportException;
	using Poco::RemotingNG::TransportFactory;
	using Poco::RemotingNG::TransportFactoryManager;
	using Poco::RemotingNG::TypeDeserializer;
	using Poco::RemotingNG::TypeSerializer;
	using Poco::RemotingNG::URIUtility;
	using Poco::RemotingNG::UnexpectedMessageException;
	using Poco::RemotingNG::UnknownEventSubscriberException;
	using Poco::RemotingNG::UnknownObjectException;
	using Poco::RemotingNG::UnknownTransportException;
	using Poco::RemotingNG::UnknownTypeException;
	using Poco::RemotingNG::WrongReplyException;
	#endif
}
