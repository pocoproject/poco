//
// PollSet.cpp
//
// Library: Net
// Package: Sockets
// Module:  PollSet
//
// Copyright (c) 2016, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Net/PollSet.h"
#include "Poco/Net/SocketImpl.h"
#include "Poco/Mutex.h"
#include "Poco/Format.h"
#include "Poco/Thread.h"
#include "Poco/Stopwatch.h"
#include <set>


#if defined(_WIN32) && _WIN32_WINNT >= 0x0600
#include "Poco/OrderedMap.h"
#ifndef POCO_HAVE_WSAEventSelect
#define POCO_HAVE_WSAEventSelect 1
#endif
#elif defined(POCO_OS_FAMILY_BSD)
#ifndef POCO_HAVE_FD_POLL
#define POCO_HAVE_FD_POLL 1
#endif
#endif


#if defined(POCO_HAVE_FD_EPOLL)
#include <sys/epoll.h>
#elif defined(POCO_HAVE_FD_POLL)
#ifndef _WIN32
#include <poll.h>
#endif
#endif


namespace Poco {
namespace Net {


#if defined(POCO_HAVE_WSAEventSelect)


//
// implementation using WSAEventSelect
//
class PollSetImpl
{
public:
	void add(const Socket& socket, int mode)
	{
		poco_socket_t fd = socket.impl()->sockfd();
		_sockets.add(socket, mode);
	}

	void remove(const Socket& socket)
	{
		poco_socket_t fd = socket.impl()->sockfd();
		_sockets.remove(fd);
	}

	bool has(const Socket& socket) const
	{
		return _sockets.has(socket);
	}

	bool empty() const
	{
		return _sockets.empty();
	}

	void update(const Socket& socket, int mode)
	{
		_sockets.update(socket, mode);
	}

	void clear()
	{
		_sockets.clear();
	}

	PollSet::SocketModeMap poll(const Poco::Timespan& timeout)
	{
		Poco::FastMutex::ScopedLock lock(_sockets.mutex());
		PollSet::SocketModeMap result;

		if (_sockets.empty()) return result;

		EventList& events = _sockets.events();
		DWORD sz = static_cast<DWORD>(events.size());

		if (timeout.totalMilliseconds() > 10)
			Poco::Thread::sleep(static_cast<long>(timeout.totalMilliseconds())-10);

		int ret = WSAWaitForMultipleEvents(sz, &(events[0]), FALSE, 0, FALSE);
		
		if (ret == WSA_WAIT_FAILED) SocketImpl::error();
		else if (ret == WSA_WAIT_TIMEOUT) return result;
		else if (ret == WSA_WAIT_IO_COMPLETION)
		{
			throw Poco::IllegalStateException("PollSet::poll(): thread alertable state enabled");
		}
		else
		{
			int firstEvent = ret - WSA_WAIT_EVENT_0;
			if (firstEvent < 0 || firstEvent >= events.size() || firstEvent >= _sockets.map().size())
			{
				throw Poco::IllegalStateException(
					Poco::format("PollSet::poll(): firstEvent=%d", firstEvent));
			}
			WSANETWORKEVENTS netEvents;
			auto it = _sockets.getFDIterator(firstEvent);
			for (int i = firstEvent; i < events.size(); ++i, ++it)
			{
				if (it == _sockets.map().end())
					throw Poco::IllegalStateException("PollSet::poll(): end of socket map reached");
				std::memset(&netEvents, 0, sizeof(netEvents));
				if (WSAEnumNetworkEvents(it->first, _sockets.events()[i], &netEvents) == SOCKET_ERROR)
					SocketImpl::error();
				if ((netEvents.iErrorCode[FD_CONNECT_BIT] != 0) ||
					(netEvents.iErrorCode[FD_READ_BIT] != 0) ||
					(netEvents.iErrorCode[FD_WRITE_BIT] != 0) ||
					(netEvents.iErrorCode[FD_CLOSE_BIT] != 0))
				{
					result[it->second] |= PollSet::POLL_ERROR;
				}
				if (netEvents.lNetworkEvents & FD_CONNECT)
				{
					//result[it->second] |= PollSet::POLL_WRITE;
				}
				if (netEvents.lNetworkEvents & FD_READ)
				{
					result[it->second] |= PollSet::POLL_READ;
				}
				if (netEvents.lNetworkEvents & FD_WRITE)
				{
					result[it->second] |= PollSet::POLL_WRITE;
				}
				if (netEvents.lNetworkEvents & FD_CLOSE)
				{
					result[it->second] |= PollSet::POLL_READ;
				}
			}
		}

		return result;
	}

private:
	using EventList = std::vector<HANDLE>;

	class Sockets
	{
	public:
		using SocketMap = Poco::OrderedMap<poco_socket_t, Socket>;

		Sockets() = default;

		~Sockets()
		{
			clear();
		}

		void add(const Socket& sock, int mode)
		{
			Poco::FastMutex::ScopedLock lock(_mutex);
			if (_events.size() >= WSA_MAXIMUM_WAIT_EVENTS)
			{
				throw Poco::IllegalStateException(
					Poco::format("PollSet::Sockets::add(): max wait event limit (%d) reached",
						static_cast<int>(WSA_MAXIMUM_WAIT_EVENTS)));
			}
			poco_socket_t fd = getFD(sock);
			auto ins = _socketMap.insert({ fd, sock});
			HANDLE* pEvent = 0;
			if (ins.second)
			{
				_events.push_back(WSACreateEvent());
				if (_events.back() == WSA_INVALID_EVENT)
					throw Poco::IllegalStateException("PollSet::Sockets::add(): invalid event");
				pEvent = std::addressof(_events.back());
			}
			else
			{
				ptrdiff_t pos = std::distance(_socketMap.begin(), ins.first);
				pEvent = std::addressof(_events[pos]);
			}
			update(fd, mode, *pEvent);
			poco_assert_dbg(_events.size() == _socketMap.size());
		}

		void update(poco_socket_t fd, int mode, HANDLE& evt)
		{
			if (!has(fd))
				throw Poco::NotFoundException("PollSet::Sockets::update()");
			long netEvents = 0;
			setMode(netEvents, mode);
			if (WSAEventSelect(fd, evt, netEvents) == SOCKET_ERROR)
				Socket::error();
		}

		void update(const Socket& sock, int mode)
		{
			Poco::FastMutex::ScopedLock lock(_mutex);
			poco_socket_t fd = getFD(sock);
			update(fd, mode, getEvent(fd));
		}

		void remove(poco_socket_t fd)
		{
			Poco::FastMutex::ScopedLock lock(_mutex);
			auto it = _socketMap.find(fd);
			if (it != _socketMap.end())
			{
				ptrdiff_t pos = std::distance(_socketMap.begin(), it);
				if (pos >= _events.size() || pos < 0)
					throw Poco::IllegalStateException(
						Poco::format("PollSet::Sockets::remove(): event position (%d) out of bounds(0-%d)",
							static_cast<int>(pos), static_cast<int>(_events.size())));
				_socketMap.erase(fd);
				WSACloseEvent(_events[pos]);
				_events.erase(_events.begin() + pos);
			}
			poco_assert_dbg(_events.size() == _socketMap.size());
		}

		bool has(poco_socket_t fd) const
		{
			Poco::FastMutex::ScopedLock lock(_mutex);
			return _socketMap.find(fd) != _socketMap.end();
		}

		bool has(const Socket& sock) const
		{
			return has(getFD(sock));
		}

		bool empty() const
		{
			Poco::FastMutex::ScopedLock lock(_mutex);
			return _socketMap.empty();
		}

		void clear()
		{
			Poco::FastMutex::ScopedLock lock(_mutex);
			for (auto& ev : _events) WSACloseEvent(ev);
			_socketMap.clear();
			_events.clear();
		}

		EventList& events()
		{
			return _events;
		}

		const Socket& socket(poco_socket_t fd)
		{
			Poco::FastMutex::ScopedLock lock(_mutex);
			auto it = _socketMap.find(fd);
			if (it == _socketMap.end())
				throw Poco::NotFoundException("PollSet::Sockets::socket(): descriptor not found");
			return it->second;
		}

		SocketMap::iterator getFDIterator(int idx)
		{
			Poco::FastMutex::ScopedLock lock(_mutex);
			if (idx >= _socketMap.size())
			{
				throw Poco::InvalidAccessException(
					Poco::format("PollSet::Sockets::getFDIterator(%d); "
						"socket map size=%d", idx, static_cast<int>(_socketMap.size())));
			}
			return _socketMap.begin() + idx;
		}

		const SocketMap& map() const
		{
			return _socketMap;
		}

		Poco::FastMutex& mutex()
		{
			return _mutex;
		}

	private:
		HANDLE& getEvent(poco_socket_t fd)
		{
			auto it = _socketMap.find(fd);
			if (it != _socketMap.end())
			{
				ptrdiff_t pos = std::distance(_socketMap.begin(), it);
				if (pos >= _events.size() || pos < 0)
				{
					throw Poco::IllegalStateException(
						Poco::format("PollSet::Sockets::getEventPos(): event position (%d) out of bounds(0-%d)",
							static_cast<int>(pos), static_cast<int>(_events.size())));
				}
				return _events[pos];
			}
			throw Poco::NotFoundException("PollSet::Sockets::getEvent()");
		}

		static void setMode(long& target, int mode)
		{
			target |= FD_CONNECT;
			if (mode & PollSet::POLL_READ) target |= FD_READ;
			if (mode & PollSet::POLL_WRITE) target |= FD_WRITE;
			target |= FD_CLOSE;
		}

		static poco_socket_t getFD(const Socket& sock)
		{
			Poco::Net::SocketImpl* pImpl = sock.impl();
			if (!pImpl)
				throw Poco::NullPointerException("PollSet::getFD(): impl is null");
			return pImpl->sockfd();
		}

		mutable Poco::FastMutex _mutex;
		SocketMap               _socketMap;
		EventList               _events;
	};

	Sockets                      _sockets;
	
};


#elif defined(POCO_HAVE_FD_EPOLL)


//
// Linux implementation using epoll
//
class PollSetImpl
{
public:
	PollSetImpl():
		_epollfd(-1),
		_events(1024)
	{
		_epollfd = epoll_create(1);
		if (_epollfd < 0)
		{
			SocketImpl::error();
		}
	}

	~PollSetImpl()
	{
		if (_epollfd >= 0)
			::close(_epollfd);
	}

	void add(const Socket& socket, int mode)
	{
		Poco::FastMutex::ScopedLock lock(_mutex);

		SocketImpl* sockImpl = socket.impl();
		poco_socket_t fd = sockImpl->sockfd();
		struct epoll_event ev;
		ev.events = 0;
		if (mode & PollSet::POLL_READ)
			ev.events |= EPOLLIN;
		if (mode & PollSet::POLL_WRITE)
			ev.events |= EPOLLOUT;
		if (mode & PollSet::POLL_ERROR)
			ev.events |= EPOLLERR;
		ev.data.ptr = socket.impl();
		int err = epoll_ctl(_epollfd, EPOLL_CTL_ADD, fd, &ev);

		if (err)
		{
			if (errno == EEXIST) update(socket, mode);
			else SocketImpl::error();
		}

		if (_socketMap.find(sockImpl) == _socketMap.end())
			_socketMap[sockImpl] = socket;
	}

	void remove(const Socket& socket)
	{
		Poco::FastMutex::ScopedLock lock(_mutex);

		poco_socket_t fd = socket.impl()->sockfd();
		struct epoll_event ev;
		ev.events = 0;
		ev.data.ptr = 0;
		int err = epoll_ctl(_epollfd, EPOLL_CTL_DEL, fd, &ev);
		if (err) SocketImpl::error();

		_socketMap.erase(socket.impl());
	}

	bool has(const Socket& socket) const
	{
		Poco::FastMutex::ScopedLock lock(_mutex);
		SocketImpl* sockImpl = socket.impl();
		return sockImpl &&
			(_socketMap.find(sockImpl) != _socketMap.end());
	}

	bool empty() const
	{
		Poco::FastMutex::ScopedLock lock(_mutex);
		return _socketMap.empty();
	}

	void update(const Socket& socket, int mode)
	{
		poco_socket_t fd = socket.impl()->sockfd();
		struct epoll_event ev;
		ev.events = 0;
		if (mode & PollSet::POLL_READ)
			ev.events |= EPOLLIN;
		if (mode & PollSet::POLL_WRITE)
			ev.events |= EPOLLOUT;
		if (mode & PollSet::POLL_ERROR)
			ev.events |= EPOLLERR;
		ev.data.ptr = socket.impl();
		int err = epoll_ctl(_epollfd, EPOLL_CTL_MOD, fd, &ev);
		if (err)
		{
			SocketImpl::error();
		}
	}

	void clear()
	{
		Poco::FastMutex::ScopedLock lock(_mutex);

		::close(_epollfd);
		_socketMap.clear();
		_epollfd = epoll_create(1);
		if (_epollfd < 0)
		{
			SocketImpl::error();
		}
	}

	PollSet::SocketModeMap poll(const Poco::Timespan& timeout)
	{
		PollSet::SocketModeMap result;

		{
			Poco::FastMutex::ScopedLock lock(_mutex);
			if(_socketMap.empty()) return result;
		}

		Poco::Timespan remainingTime(timeout);
		int rc;
		do
		{
			Poco::Timestamp start;
			rc = epoll_wait(_epollfd, &_events[0], _events.size(), remainingTime.totalMilliseconds());
			if (rc < 0 && SocketImpl::lastError() == POCO_EINTR)
			{
				Poco::Timestamp end;
				Poco::Timespan waited = end - start;
				if (waited < remainingTime)
					remainingTime -= waited;
				else
					remainingTime = 0;
			}
		}
		while (rc < 0 && SocketImpl::lastError() == POCO_EINTR);
		if (rc < 0) SocketImpl::error();

		Poco::FastMutex::ScopedLock lock(_mutex);

		for (int i = 0; i < rc; i++)
		{
			std::map<void*, Socket>::iterator it = _socketMap.find(_events[i].data.ptr);
			if (it != _socketMap.end())
			{
				if (_events[i].events & EPOLLIN)
					result[it->second] |= PollSet::POLL_READ;
				if (_events[i].events & EPOLLOUT)
					result[it->second] |= PollSet::POLL_WRITE;
				if (_events[i].events & EPOLLERR)
					result[it->second] |= PollSet::POLL_ERROR;
			}
		}

		return result;
	}

private:
	mutable Poco::FastMutex         _mutex;
	int                             _epollfd;
	std::map<void*, Socket>         _socketMap;
	std::vector<struct epoll_event> _events;
};


#elif defined(POCO_HAVE_FD_POLL)


//
// implementation using poll
//
class PollSetImpl
{
public:
	void add(const Socket& socket, int mode)
	{
		Poco::FastMutex::ScopedLock lock(_mutex);

		poco_socket_t fd = socket.impl()->sockfd();
		_addMap[fd] = mode;
		_removeSet.erase(fd);
		_socketMap[fd] = socket;
	}

	void remove(const Socket& socket)
	{
		Poco::FastMutex::ScopedLock lock(_mutex);

		poco_socket_t fd = socket.impl()->sockfd();
		_removeSet.insert(fd);
		_addMap.erase(fd);
		_socketMap.erase(fd);
	}

	bool has(const Socket& socket) const
	{
		Poco::FastMutex::ScopedLock lock(_mutex);
		SocketImpl* sockImpl = socket.impl();
		return sockImpl &&
			(_socketMap.find(sockImpl->sockfd()) != _socketMap.end());
	}

	bool empty() const
	{
		Poco::FastMutex::ScopedLock lock(_mutex);
		return _socketMap.empty();
	}

	void update(const Socket& socket, int mode)
	{
		Poco::FastMutex::ScopedLock lock(_mutex);

		poco_socket_t fd = socket.impl()->sockfd();
		for (auto it = _pollfds.begin(); it != _pollfds.end(); ++it)
		{
			if (it->fd == fd)
			{
				it->events = 0;
				it->revents = 0;
				setMode(it->fd, it->events, mode);
			}
		}
	}

	void clear()
	{
		Poco::FastMutex::ScopedLock lock(_mutex);

		_socketMap.clear();
		_addMap.clear();
		_removeSet.clear();
		_pollfds.clear();
	}

	PollSet::SocketModeMap poll(const Poco::Timespan& timeout)
	{
		PollSet::SocketModeMap result;
		{
			Poco::FastMutex::ScopedLock lock(_mutex);

			if (!_removeSet.empty())
			{
				for (auto it = _pollfds.begin(); it != _pollfds.end();)
				{
					if (_removeSet.find(it->fd) != _removeSet.end())
					{
						it = _pollfds.erase(it);
					}
					else ++it;
				}
				_removeSet.clear();
			}

			_pollfds.reserve(_pollfds.size() + _addMap.size());
			for (auto it = _addMap.begin(); it != _addMap.end(); ++it)
			{
				pollfd pfd;
				pfd.fd = it->first;
				pfd.events = 0;
				pfd.revents = 0;
				setMode(pfd.fd, pfd.events, it->second);
				_pollfds.push_back(pfd);
			}
			_addMap.clear();
		}

		if (_pollfds.empty()) return result;

		Poco::Timespan remainingTime(timeout);
		int rc;
		do
		{
			Poco::Timestamp start;
#ifdef _WIN32
			rc = WSAPoll(&_pollfds[0], static_cast<ULONG>(_pollfds.size()), static_cast<INT>(remainingTime.totalMilliseconds()));
			// see https://github.com/pocoproject/poco/issues/3248
			if ((remainingTime > 0) && (rc > 0) && !hasSignaledFDs())
			{
				rc = -1;
				WSASetLastError(WSAEINTR);
			}
#else
			rc = ::poll(&_pollfds[0], _pollfds.size(), remainingTime.totalMilliseconds());
#endif
			if (rc < 0 && SocketImpl::lastError() == POCO_EINTR)
			{
				Poco::Timestamp end;
				Poco::Timespan waited = end - start;
				if (waited < remainingTime)
					remainingTime -= waited;
				else
					remainingTime = 0;
			}
		}
		while (rc < 0 && SocketImpl::lastError() == POCO_EINTR);
		if (rc < 0) SocketImpl::error();

		{
			Poco::FastMutex::ScopedLock lock(_mutex);

			if (!_socketMap.empty())
			{
				for (auto it = _pollfds.begin(); it != _pollfds.end(); ++it)
				{
					std::map<poco_socket_t, Socket>::const_iterator its = _socketMap.find(it->fd);
					if (its != _socketMap.end())
					{
						if ((it->revents & POLLIN)
#ifdef _WIN32
						|| (it->revents & POLLHUP)
#endif
							)
							result[its->second] |= PollSet::POLL_READ;
						if ((it->revents & POLLOUT)
#ifdef _WIN32
							&& (_wantPOLLOUT.find(it->fd) != _wantPOLLOUT.end())
#endif
							)
							result[its->second] |= PollSet::POLL_WRITE;
						if (it->revents & POLLERR)
							result[its->second] |= PollSet::POLL_ERROR;
					}
					it->revents = 0;
				}
			}
		}

		return result;
	}

private:

#ifdef _WIN32

	void setMode(poco_socket_t fd, short& target, int mode)
	{
		if (mode & PollSet::POLL_READ)
			target |= POLLIN;

		if (mode & PollSet::POLL_WRITE)
			_wantPOLLOUT.insert(fd);
		else
			_wantPOLLOUT.erase(fd);
		target |= POLLOUT;
	}

	bool hasSignaledFDs()
	{
		for (const auto& pollfd : _pollfds)
		{
			if ((pollfd.revents | POLLOUT) &&
				(_wantPOLLOUT.find(pollfd.fd) != _wantPOLLOUT.end()))
			{
				return true;
			}
		}
		return false;
	}

#else

	void setMode(poco_socket_t fd, short& target, int mode)
	{
		if (mode & PollSet::POLL_READ)
			target |= POLLIN;

		if (mode & PollSet::POLL_WRITE)
			target |= POLLOUT;
	}

#endif

	mutable Poco::FastMutex         _mutex;
	std::map<poco_socket_t, Socket> _socketMap;
#ifdef _WIN32
	std::set<poco_socket_t>         _wantPOLLOUT;
#endif
	std::map<poco_socket_t, int>    _addMap;
	std::set<poco_socket_t>         _removeSet;
	std::vector<pollfd>             _pollfds;
};


#else


//
// Fallback implementation using select()
//
class PollSetImpl
{
public:
	void add(const Socket& socket, int mode)
	{
		Poco::FastMutex::ScopedLock lock(_mutex);
		_map[socket] = mode;
	}

	void remove(const Socket& socket)
	{
		Poco::FastMutex::ScopedLock lock(_mutex);
		_map.erase(socket);
	}

	bool has(const Socket& socket) const
	{
		Poco::FastMutex::ScopedLock lock(_mutex);
		return _map.find(socket) != _map.end();
	}

	bool empty() const
	{
		Poco::FastMutex::ScopedLock lock(_mutex);
		return _map.empty();
	}

	void update(const Socket& socket, int mode)
	{
		Poco::FastMutex::ScopedLock lock(_mutex);
		_map[socket] = mode;
	}

	void clear()
	{
		Poco::FastMutex::ScopedLock lock(_mutex);
		_map.clear();
	}

	PollSet::SocketModeMap poll(const Poco::Timespan& timeout)
	{
		fd_set fdRead;
		fd_set fdWrite;
		fd_set fdExcept;
		int nfd = 0;

		FD_ZERO(&fdRead);
		FD_ZERO(&fdWrite);
		FD_ZERO(&fdExcept);

		{
			Poco::FastMutex::ScopedLock lock(_mutex);

			for (auto it = _map.begin(); it != _map.end(); ++it)
			{
				poco_socket_t fd = it->first.impl()->sockfd();
				if (fd != POCO_INVALID_SOCKET && it->second)
				{
					if (int(fd) > nfd) nfd = int(fd);

					if (it->second & PollSet::POLL_READ)
					{
						FD_SET(fd, &fdRead);
					}
					if (it->second & PollSet::POLL_WRITE)
					{
						FD_SET(fd, &fdWrite);
					}
					if (it->second & PollSet::POLL_ERROR)
					{
						FD_SET(fd, &fdExcept);
					}
				}
			}
		}

		PollSet::SocketModeMap result;
		if (nfd == 0) return result;

		Poco::Timespan remainingTime(timeout);
		int rc;
		do
		{
			struct timeval tv;
			tv.tv_sec  = (long) remainingTime.totalSeconds();
			tv.tv_usec = (long) remainingTime.useconds();
			Poco::Timestamp start;
			rc = ::select(nfd + 1, &fdRead, &fdWrite, &fdExcept, &tv);
			if (rc < 0 && SocketImpl::lastError() == POCO_EINTR)
			{
				Poco::Timestamp end;
				Poco::Timespan waited = end - start;
				if (waited < remainingTime)
					remainingTime -= waited;
				else
					remainingTime = 0;
			}
		}
		while (rc < 0 && SocketImpl::lastError() == POCO_EINTR);
		if (rc < 0) SocketImpl::error();

		{
			Poco::FastMutex::ScopedLock lock(_mutex);

			for (auto it = _map.begin(); it != _map.end(); ++it)
			{
				poco_socket_t fd = it->first.impl()->sockfd();
				if (fd != POCO_INVALID_SOCKET)
				{
					if (FD_ISSET(fd, &fdRead))
					{
						result[it->first] |= PollSet::POLL_READ;
					}
					if (FD_ISSET(fd, &fdWrite))
					{
						result[it->first] |= PollSet::POLL_WRITE;
					}
					if (FD_ISSET(fd, &fdExcept))
					{
						result[it->first] |= PollSet::POLL_ERROR;
					}
				}
			}
		}

		return result;
	}

private:
	mutable Poco::FastMutex _mutex;
	PollSet::SocketModeMap  _map;
};


#endif


PollSet::PollSet():
	_pImpl(new PollSetImpl)
{
}


PollSet::~PollSet()
{
	delete _pImpl;
}


void PollSet::add(const Socket& socket, int mode)
{
	_pImpl->add(socket, mode);
}


void PollSet::remove(const Socket& socket)
{
	_pImpl->remove(socket);
}


void PollSet::update(const Socket& socket, int mode)
{
	_pImpl->update(socket, mode);
}


bool PollSet::has(const Socket& socket) const
{
	return _pImpl->has(socket);
}


bool PollSet::empty() const
{
	return _pImpl->empty();
}


void PollSet::clear()
{
	_pImpl->clear();
}


PollSet::SocketModeMap PollSet::poll(const Poco::Timespan& timeout)
{
	return _pImpl->poll(timeout);
}


} } // namespace Poco::Net
