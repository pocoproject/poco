//
// EventLoop.cpp
//
// $Id: //poco/1.7/DNSSD/Bonjour/src/EventLoop.cpp#1 $
//
// Library: DNSSD/Bonjour
// Package: Implementation
// Module:  EventLoop
//
// Copyright (c) 2006-2024, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/DNSSD/Bonjour/EventLoop.h"
#include "Poco/Net/PollSet.h"
#include "Poco/Net/StreamSocket.h"
#include "Poco/Net/StreamSocketImpl.h"
#include <dns_sd.h>


namespace Poco {
namespace DNSSD {
namespace Bonjour {


EventLoop::EventLoop():
	_stop(false)
{
}


EventLoop::~EventLoop()
{
	try
	{
		shutdown();
	}
	catch (...)
	{
		poco_unexpected();
	}
}


void EventLoop::shutdown()
{
	for (auto& it: _refs)
	{
		DNSServiceRefDeallocate(it.first);
	}
	_refs.clear();
	_sockets.clear();
	_invalidatedRefs.clear();
}


void EventLoop::add(DNSServiceRef sdRef)
{
	int sockfd = DNSServiceRefSockFD(sdRef);
	Poco::Net::StreamSocket sock(new Poco::Net::StreamSocketImpl(sockfd));

	{
		Poco::Mutex::ScopedLock lock(_mutex);
		_sockets[sock] = sdRef;
		_refs[sdRef]   = sock;
	}

	_refAdded.set();
}


void EventLoop::remove(DNSServiceRef sdRef)
{
	Poco::Mutex::ScopedLock lock(_mutex);

	_invalidatedRefs.insert(sdRef);
}


void EventLoop::removeImpl(DNSServiceRef sdRef)
{
	auto it = _refs.find(sdRef);
	if (it != _refs.end())
	{
		_sockets.erase(it->second);
		_refs.erase(it);
	}
	DNSServiceRefDeallocate(sdRef);
}


void EventLoop::run()
{
	Poco::Net::PollSet pollSet;

	while (!_stop)
	{
		if (!_refs.empty() || _refAdded.tryWait(EVENTLOOP_TIMEOUT))
		{
			Poco::Mutex::ScopedLock lock(_mutex);			
			for (const auto& r: _refs)
			{
				pollSet.add(r.second, Net::Socket::SELECT_READ);
			}
		}

		if (!pollSet.empty())
		{
			Poco::Timespan timeout(1000LL * EVENTLOOP_TIMEOUT);
			const auto sm = pollSet.poll(timeout);
			if (!sm.empty())
			{
				for (const auto& it: sm)
				{
					Poco::Mutex::ScopedLock lock(_mutex);
					if (it.second & Net::PollSet::POLL_READ)
					{
						auto& socket = it.first;
						const auto itSock = _sockets.find(socket);
						poco_assert_dbg (itSock != _sockets.end());
						auto itSet = _invalidatedRefs.find(itSock->second);
						if (itSet != _invalidatedRefs.end())
						{
							removeImpl(itSock->second);
							_invalidatedRefs.erase(itSet);
						}
						else
						{
							DNSServiceProcessResult(itSock->second);
						}
					}
				}
			}
		}

		Poco::Mutex::ScopedLock lock(_mutex);		
		for (auto& ir: _invalidatedRefs)
		{
			removeImpl(ir);
		}
		_invalidatedRefs.clear();
	}
}


} } } // namespace Poco::DNSSD::Bonjour
