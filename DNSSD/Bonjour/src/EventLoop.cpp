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
	RefToSock::iterator it = _refs.begin();
	RefToSock::iterator itEnd = _refs.end();
	for (; it != itEnd; ++it)
	{
		DNSServiceRefDeallocate(it->first);
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
	RefToSock::iterator it = _refs.find(sdRef);
	if (it != _refs.end())
	{
		_sockets.erase(it->second);
		_refs.erase(it);
	}
	DNSServiceRefDeallocate(sdRef);
}


void EventLoop::run()
{
	Poco::Net::Socket::SocketList readList;
	Poco::Net::Socket::SocketList writeList;
	Poco::Net::Socket::SocketList errList;

	while (!_stop)
	{
		readList.clear();
		if (!_refs.empty() || _refAdded.tryWait(EVENTLOOP_TIMEOUT))
		{
			Poco::Mutex::ScopedLock lock(_mutex);

			RefToSock::const_iterator it = _refs.begin();
			RefToSock::const_iterator itEnd = _refs.end();
			for (; it != itEnd; ++it)
			{
				readList.push_back(it->second);
			}
		}

		if (!readList.empty())
		{
			Poco::Timespan timeout(1000*EVENTLOOP_TIMEOUT);
			int ready = Poco::Net::Socket::select(readList, writeList, errList, timeout);
			if (ready > 0)
			{
				Poco::Net::Socket::SocketList::iterator it = readList.begin();
				Poco::Net::Socket::SocketList::iterator itEnd = readList.end();
				for (; it != itEnd; ++it)
				{
					Poco::Mutex::ScopedLock lock(_mutex);

					SockToRef::iterator itSock = _sockets.find(*it);
					poco_assert_dbg (itSock != _sockets.end());
					RefSet::iterator itSet = _invalidatedRefs.find(itSock->second);
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

		Poco::Mutex::ScopedLock lock(_mutex);
		RefSet::iterator itSet =_invalidatedRefs.begin();
		RefSet::iterator itSetEnd = _invalidatedRefs.end();
		for (; itSet != itSetEnd; ++itSet)
		{
			removeImpl(*itSet);
		}
		_invalidatedRefs.clear();
	}
}


} } } // namespace Poco::DNSSD::Bonjour
