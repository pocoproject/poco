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
#include <set>


#if defined(POCO_HAVE_FD_EPOLL)
	#ifdef POCO_OS_FAMILY_WINDOWS
		#include "Poco/Net/ServerSocket.h"
		#include "Poco/Net/SocketAddress.h"
		#include "wepoll.h"
	#else
		#include <sys/epoll.h>
		#include <sys/eventfd.h>
	#endif
#elif defined(POCO_HAVE_FD_POLL)
	#ifndef _WIN32
		#include <poll.h>
		#include "Poco/Pipe.h"
	#endif
#endif


namespace Poco {
namespace Net {


#if defined(POCO_HAVE_FD_EPOLL)

//
// Implementation using epoll (Linux) or wepoll (Windows)
//


#ifdef WEPOLL_H_

namespace {

int close(HANDLE h)
{
	return epoll_close(h);
}

}

#endif // WEPOLL_H_


class PollSetImpl
{
public:
	using Mutex = Poco::FastMutex;
	using ScopedLock = Mutex::ScopedLock;
	using SocketMode = std::pair<Socket, int>;
	using SocketMap = std::map<void*, SocketMode>;

	PollSetImpl(): _events(1024),
		_port(0),
		_eventfd(eventfd(_port, 0)),
		_epollfd(epoll_create(1))
	{
		int err = addFD(_eventfd, PollSet::POLL_READ, EPOLL_CTL_ADD);
#ifdef WEPOLL_H_
		if ((err) || !_epollfd)
#else
		if ((err) || (_epollfd < 0))
#endif
		{
			SocketImpl::error();
		}
	}

	~PollSetImpl()
	{
#ifdef WEPOLL_H_
		if (_eventfd >= 0) eventfd(_port, _eventfd);
		if (_epollfd) close(_epollfd);
#else
		if (_eventfd > 0) close(_eventfd.exchange(0));
		if (_epollfd >= 0) close(_epollfd);
#endif
	}

	void add(const Socket& socket, int mode)
	{
		int newMode = getNewMode(socket.impl(), mode);
		int err = addImpl(socket, newMode);
		if (err)
		{
			if (errno == EEXIST) update(socket, newMode);
			else SocketImpl::error();
		}
	}

	void update(const Socket& socket, int mode)
	{
		int err = updateImpl(socket, mode);
		if (err) SocketImpl::error();
	}

	void remove(const Socket& socket)
	{
		poco_socket_t fd = socket.impl()->sockfd();
		struct epoll_event ev;
		ev.events = 0;
		ev.data.ptr = 0;

		int err = epoll_ctl(_epollfd, EPOLL_CTL_DEL, fd, &ev);
		if (err) SocketImpl::error();
		ScopedLock lock(_mutex);
		_socketMap.erase(socket.impl());
	}

	bool has(const Socket& socket) const
	{
		SocketImpl* sockImpl = socket.impl();
		ScopedLock lock(_mutex);
		return sockImpl &&
			(_socketMap.find(sockImpl) != _socketMap.end());
	}

	bool empty() const
	{
		ScopedLock lock(_mutex);
		return _socketMap.empty();
	}

	void clear()
	{
		{
			ScopedLock lock(_mutex);

			close(_epollfd);
			_socketMap.clear();
			_epollfd = epoll_create(1);
#ifdef WEPOLL_H_
			if (!_epollfd) SocketImpl::error();
#else
			if (_epollfd < 0) SocketImpl::error();
#endif
		}
#ifdef WEPOLL_H_
		eventfd(_port, _eventfd);
		_eventfd = eventfd(_port);
#else
		close(_eventfd.exchange(0));
		_eventfd = eventfd(0, 0);
#endif
		addFD(_eventfd, PollSet::POLL_READ, EPOLL_CTL_ADD);
	}

	PollSet::SocketModeMap poll(const Poco::Timespan& timeout)
	{
		PollSet::SocketModeMap result;
		Poco::Timespan remainingTime(timeout);
		int rc;

		do
		{
			Poco::Timestamp start;
			rc = epoll_wait(_epollfd, &_events[0],
				static_cast<int>(_events.size()), static_cast<int>(remainingTime.totalMilliseconds()));
			if (rc == 0) return result;

			// if we are hitting the events limit, resize it; even without resizing, the subseqent
			// calls would round-robin through the remaining ready sockets, but it's better to give
			// the call enough room once we start hitting the boundary
			if (rc >= _events.size()) _events.resize(_events.size()*2);
			else if (rc < 0)
			{
				// if interrupted and there's still time left, keep waiting
				if (SocketImpl::lastError() == POCO_EINTR)
				{
					Poco::Timestamp end;
					Poco::Timespan waited = end - start;
					if (waited < remainingTime)
					{
						remainingTime -= waited;
						continue;
					}
				}
				else SocketImpl::error();
			}
		}
		while (false);

		ScopedLock lock(_mutex);

		for (int i = 0; i < rc; i++)
		{
			if (_events[i].data.ptr) // skip eventfd
			{
				SocketMap::iterator it = _socketMap.find(_events[i].data.ptr);
				if (it != _socketMap.end())
				{
					if (_events[i].events & EPOLLIN)
						result[it->second.first] |= PollSet::POLL_READ;
					if (_events[i].events & EPOLLOUT)
						result[it->second.first] |= PollSet::POLL_WRITE;
					if (_events[i].events & EPOLLERR)
						result[it->second.first] |= PollSet::POLL_ERROR;
				}
			}
			else if (_events[i].events & EPOLLIN) // eventfd signaled
			{
				uint64_t val;
#ifdef WEPOLL_H_
				if (_pSocket && _pSocket->available())
					_pSocket->impl()->receiveBytes(&val, sizeof(val));
#else
				read(_eventfd, &val, sizeof(val));
#endif
			}
		}
		return result;
	}

	void wakeUp()
	{
#ifdef WEPOLL_H_
		StreamSocket ss(SocketAddress("127.0.0.1", _port));
#else
		uint64_t val = 1;
		// This is guaranteed to write into a valid fd,
		// or 0 (meaning PollSet is being destroyed).
		// Errors are ignored.
		write(_eventfd, &val, sizeof(val));
#endif
	}

	int count() const
	{
		ScopedLock lock(_mutex);
		return static_cast<int>(_socketMap.size());
	}

private:
	int getNewMode(SocketImpl* sockImpl, int mode)
	{
		ScopedLock lock(_mutex);
		auto it = _socketMap.find(sockImpl);
		if (it != _socketMap.end())
			mode |= it->second.second;
		return mode;
	}

	void socketMapUpdate(const Socket& socket, int mode)
	{
		SocketImpl* sockImpl = socket.impl();
		ScopedLock lock(_mutex);
		_socketMap[sockImpl] = {socket, mode};
	}

	int updateImpl(const Socket& socket, int mode)
	{
		SocketImpl* sockImpl = socket.impl();
		int ret = addFD(static_cast<int>(sockImpl->sockfd()), mode, EPOLL_CTL_MOD, sockImpl);
		if (ret == 0) socketMapUpdate(socket, mode);
		return ret;
	}

	int addImpl(const Socket& socket, int mode)
	{
		SocketImpl* sockImpl = socket.impl();
		int newMode = getNewMode(sockImpl, mode);
		int ret = addFD(static_cast<int>(sockImpl->sockfd()), newMode, EPOLL_CTL_ADD, sockImpl);
		if (ret == 0) socketMapUpdate(socket, newMode);
		return ret;
	}

	int addFD(int fd, int mode, int op, void* ptr = 0)
	{
		struct epoll_event ev{};
		ev.events = 0;
		if (mode & PollSet::POLL_READ)
			ev.events |= EPOLLIN;
		if (mode & PollSet::POLL_WRITE)
			ev.events |= EPOLLOUT;
		if (mode & PollSet::POLL_ERROR)
			ev.events |= EPOLLERR;
		ev.data.ptr = ptr;
		return epoll_ctl(_epollfd, op, fd, &ev);
	}

#ifdef WEPOLL_H_

	int eventfd(int& port, int rmFD = 0)
	{
		if (rmFD == 0)
		{
			_pSocket = new ServerSocket(SocketAddress("127.0.0.1", 0));
			_pSocket->setBlocking(false);
			port = _pSocket->address().port();
			return static_cast<int>(_pSocket->impl()->sockfd());
		}
		else
		{
			delete _pSocket;
			_pSocket = 0;
			port = 0;
		}
		return 0;
	}

#endif // WEPOLL_H_

	mutable Mutex _mutex;
	SocketMap _socketMap;
	std::vector<struct epoll_event> _events;
	int _port;
	std::atomic<int> _eventfd;
#ifdef WEPOLL_H_
	std::atomic <HANDLE> _epollfd;
	ServerSocket* _pSocket;
#else
	std::atomic<int> _epollfd;
#endif
};


#elif defined(POCO_HAVE_FD_POLL)


//
// BSD/Windows implementation using poll/WSAPoll
//
class PollSetImpl
{
public:
	PollSetImpl()
	{
		pollfd fd{_pipe.readHandle(), POLLIN, 0};
		_pollfds.push_back(fd);
	}

	~PollSetImpl()
	{
		_pipe.close();
	}

	void add(const Socket& socket, int mode)
	{
		Poco::FastMutex::ScopedLock lock(_mutex);
		poco_socket_t fd = socket.impl()->sockfd();
		_addMap[fd] |= mode;
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
				setMode(it->events, mode);
			}
		}
	}

	void clear()
	{
		Poco::FastMutex::ScopedLock lock(_mutex);

		_socketMap.clear();
		_addMap.clear();
		_removeSet.clear();
		_pollfds.reserve(1);
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
				setMode(pfd.events, it->second);
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
			rc = ::poll(&_pollfds[0], _pollfds.size(), remainingTime.totalMilliseconds());
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
			if (_pollfds[0].revents & POLLIN)
			{
				char c;
				_pipe.readBytes(&c, 1);
			}

			Poco::FastMutex::ScopedLock lock(_mutex);

			if (!_socketMap.empty())
			{
				for (auto it = _pollfds.begin() + 1; it != _pollfds.end(); ++it)
				{
					std::map<poco_socket_t, Socket>::const_iterator its = _socketMap.find(it->fd);
					if (its != _socketMap.end())
					{
						if (it->revents & POLLIN)
							result[its->second] |= PollSet::POLL_READ;
						if (it->revents & POLLOUT)
							result[its->second] |= PollSet::POLL_WRITE;
						if (it->revents & POLLERR || (it->revents & POLLHUP))
							result[its->second] |= PollSet::POLL_ERROR;
					}
					it->revents = 0;
				}
			}
		}

		return result;
	}

	void wakeUp()
	{
		char c = 1;
		_pipe.writeBytes(&c, 1);
	}

	int count() const
	{
		Poco::FastMutex::ScopedLock lock(_mutex);
		return static_cast<int>(_socketMap.size());
	}

private:

	void setMode(short& target, int mode)
	{
		if (mode & PollSet::POLL_READ)
			target |= POLLIN;

		if (mode & PollSet::POLL_WRITE)
			target |= POLLOUT;
	}

	mutable Poco::FastMutex         _mutex;
	std::map<poco_socket_t, Socket> _socketMap;
	std::map<poco_socket_t, int>    _addMap;
	std::set<poco_socket_t>         _removeSet;
	std::vector<pollfd>             _pollfds;
	Poco::Pipe                      _pipe;
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

	void wakeUp()
	{
		// TODO
	}

	int count() const
	{
		Poco::FastMutex::ScopedLock lock(_mutex);
		return static_cast<int>(_map.size());
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


int PollSet::count() const
{
	return _pImpl->count();
}


void PollSet::wakeUp()
{
	_pImpl->wakeUp();
}


} } // namespace Poco::Net
