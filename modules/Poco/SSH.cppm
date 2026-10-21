//
// SSH.cppm
//
// C++ module file
//
// Copyright (c) 2026, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//

module;

#ifdef ENABLE_SSH
#include "Poco/SSH/SSHChannelStream.h"
#include "Poco/SSH/SSHClient.h"
#include "Poco/SSH/SSHException.h"
#include "Poco/SSH/SSH.h"
#include "Poco/SSH/SSHHostKeyManager.h"
#include "Poco/SSH/SSHServerConfig.h"
#include "Poco/SSH/SSHServer.h"
#include "Poco/SSH/SSHSession.h"
#include "Poco/SSH/SSHTypes.h"
#endif

export module Poco:SSH;

export namespace Poco::SSH {
	#ifdef ENABLE_SSH
	using Poco::SSH::SSHAuthenticationException;
	using Poco::SSH::SSHChannelException;
	using Poco::SSH::SSHChannelStream;
	using Poco::SSH::SSHChannelStreamBuf;
	using Poco::SSH::SSHClient;
	using Poco::SSH::SSHConnectionException;
	using Poco::SSH::SSHException;
	using Poco::SSH::SSHHostKeyManager;
	using Poco::SSH::SSHKeyExchangeException;
	using Poco::SSH::SSHServer;
	using Poco::SSH::SSHServerConfig;
	using Poco::SSH::SSHSession;

	using Poco::SSH::sshReadLine;
	#endif
}

#ifdef ENABLE_SSH
export using ::ssh_bind;
export using ::ssh_channel;
export using ::ssh_key;
export using ::ssh_session;
#endif
