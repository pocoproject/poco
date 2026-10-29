//
// TCP.cppm
//
// C++ module file
//
// Copyright (c) 2026, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//

module;

#ifdef ENABLE_REMOTINGNG_TCP
#include "Poco/RemotingNG/TCP/ChannelStream.h"
#include "Poco/RemotingNG/TCP/ClientAuthenticator.h"
#include "Poco/RemotingNG/TCP/Connection.h"
#include "Poco/RemotingNG/TCP/ConnectionManager.h"
#include "Poco/RemotingNG/TCP/CredentialsStore.h"
#include "Poco/RemotingNG/TCP/FrameFactory.h"
#include "Poco/RemotingNG/TCP/Frame.h"
#include "Poco/RemotingNG/TCP/FrameHandler.h"
#include "Poco/RemotingNG/TCP/FrameQueue.h"
#include "Poco/RemotingNG/TCP/Listener.h"
#include "Poco/RemotingNG/TCP/PlainClientAuthenticator.h"
#include "Poco/RemotingNG/TCP/SCRAMAuthenticator.h"
#include "Poco/RemotingNG/TCP/SCRAMClientAuthenticator.h"
#include "Poco/RemotingNG/TCP/ServerConnectionFactory.h"
#include "Poco/RemotingNG/TCP/ServerConnection.h"
#include "Poco/RemotingNG/TCP/ServerTransport.h"
#include "Poco/RemotingNG/TCP/SocketFactory.h"
#include "Poco/RemotingNG/TCP/TCP.h"
#include "Poco/RemotingNG/TCP/Timer.h"
#include "Poco/RemotingNG/TCP/TransportFactory.h"
#include "Poco/RemotingNG/TCP/Transport.h"
#endif

export module Poco:RemotingNG.TCP;

export namespace Poco::RemotingNG::TCP {
	#ifdef ENABLE_REMOTINGNG_TCP
	using Poco::RemotingNG::TCP::ChannelIOS;
	using Poco::RemotingNG::TCP::ChannelInputStream;
	using Poco::RemotingNG::TCP::ChannelOutputStream;
	using Poco::RemotingNG::TCP::ChannelStreamBuf;
	using Poco::RemotingNG::TCP::ClientAuthenticator;
	using Poco::RemotingNG::TCP::Connection;
	using Poco::RemotingNG::TCP::ConnectionManager;
	using Poco::RemotingNG::TCP::CredentialsStore;
	using Poco::RemotingNG::TCP::Frame;
	using Poco::RemotingNG::TCP::FrameFactory;
	using Poco::RemotingNG::TCP::FrameHandler;
	using Poco::RemotingNG::TCP::FrameQueue;
	using Poco::RemotingNG::TCP::Listener;
	using Poco::RemotingNG::TCP::PlainClientAuthenticator;
	using Poco::RemotingNG::TCP::SCRAMAuthenticator;
	using Poco::RemotingNG::TCP::SCRAMClientAuthenticator;
	using Poco::RemotingNG::TCP::ServerConnection;
	using Poco::RemotingNG::TCP::ServerConnectionFactory;
	using Poco::RemotingNG::TCP::ServerTransport;
	using Poco::RemotingNG::TCP::SocketFactory;
	using Poco::RemotingNG::TCP::Timer;
	using Poco::RemotingNG::TCP::TimerTask;
	using Poco::RemotingNG::TCP::Transport;
	using Poco::RemotingNG::TCP::TransportFactory;
	#endif
}
