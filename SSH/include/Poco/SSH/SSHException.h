//
// SSHException.h
//
// Library: SSH
// Package: SSH
// Module:  SSHException
//
// Definition of SSHException.
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC
// and Contributors.
//
// SPDX-License-Identifier: BSL-1.0
//


#ifndef SSH_SSHException_INCLUDED
#define SSH_SSHException_INCLUDED


#include "Poco/SSH/SSH.h"
#include "Poco/Exception.h"


namespace Poco {
namespace SSH {


POCO_DECLARE_EXCEPTION(SSH_API, SSHException, Poco::IOException)
POCO_DECLARE_EXCEPTION(SSH_API, SSHKeyExchangeException, SSHException)
POCO_DECLARE_EXCEPTION(SSH_API, SSHAuthenticationException, SSHException)
POCO_DECLARE_EXCEPTION(SSH_API, SSHChannelException, SSHException)
POCO_DECLARE_EXCEPTION(SSH_API, SSHConnectionException, SSHException)


} } // namespace Poco::SSH


#endif // SSH_SSHException_INCLUDED
