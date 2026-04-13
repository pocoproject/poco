//
// SSHException.cpp
//
// Library: SSH
// Package: SSH
// Module:  SSHException
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC
// and Contributors.
//
// SPDX-License-Identifier: BSL-1.0
//


#include "Poco/SSH/SSHException.h"


namespace Poco {
namespace SSH {


POCO_IMPLEMENT_EXCEPTION(SSHException, Poco::IOException, "SSH Exception")
POCO_IMPLEMENT_EXCEPTION(SSHKeyExchangeException, SSHException, "SSH Key Exchange Failed")
POCO_IMPLEMENT_EXCEPTION(SSHAuthenticationException, SSHException, "SSH Authentication Failed")
POCO_IMPLEMENT_EXCEPTION(SSHChannelException, SSHException, "SSH Channel Error")
POCO_IMPLEMENT_EXCEPTION(SSHConnectionException, SSHException, "SSH Connection Error")


} } // namespace Poco::SSH
