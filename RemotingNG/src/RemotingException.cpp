//
// RemotingException.cpp
//
// Library: RemotingNG
// Package: RemotingNG
// Module:  RemotingException
//
// Copyright (c) 2006-2014, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// License: Applied Informatics Software and Source Code License Agreement
//


#include "Poco/RemotingNG/RemotingException.h"
#include <typeinfo>


namespace Poco {
namespace RemotingNG {


POCO_IMPLEMENT_EXCEPTION(RemotingException, Poco::LogicException, "RemotingNG Exception")
POCO_IMPLEMENT_EXCEPTION(TransportException, RemotingException, "Transport error")
POCO_IMPLEMENT_EXCEPTION(ProtocolException, RemotingException, "Protocol error")
POCO_IMPLEMENT_EXCEPTION(SecurityException, RemotingException, "Security Exception")
POCO_IMPLEMENT_EXCEPTION(AuthenticationFailedException, SecurityException, "Authentication failed")
POCO_IMPLEMENT_EXCEPTION(InvalidCredentialsException, AuthenticationFailedException, "Invalid credentials")
POCO_IMPLEMENT_EXCEPTION(NoPermissionException, SecurityException, "No permission")
POCO_IMPLEMENT_EXCEPTION(RemoteException, RemotingException, "Remote Exception")
POCO_IMPLEMENT_EXCEPTION(SerializerException, RemotingException, "Failed to serialize data")
POCO_IMPLEMENT_EXCEPTION(DeserializerException, RemotingException, "Failed to deserialize data")
POCO_IMPLEMENT_EXCEPTION(UnknownTransportException, RemotingException, "Unknown transport identifier")
POCO_IMPLEMENT_EXCEPTION(UnknownTypeException, RemotingException, "Unknown type identifier")
POCO_IMPLEMENT_EXCEPTION(UnknownObjectException, RemotingException, "Unknown object identifier")
POCO_IMPLEMENT_EXCEPTION(UnknownEventSubscriberException, RemotingException, "Unknown event subscriber")
POCO_IMPLEMENT_EXCEPTION(ORBDisabledException, RemotingException, "ORB is disabled")
POCO_IMPLEMENT_EXCEPTION(MissingParamException, DeserializerException, "Failed to deserialize data: missing mandatory parameter")
POCO_IMPLEMENT_EXCEPTION(WrongReplyException, DeserializerException, "Failed to deserialize data: reply name doesn't match request name")
POCO_IMPLEMENT_EXCEPTION(UnexpectedMessageException, DeserializerException, "Unexpected message found")
POCO_IMPLEMENT_EXCEPTION(MethodNotFoundException, DeserializerException, "Method not found")


} } // namespace Poco::RemotingNG
