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
	#include <sys/epoll.h>
	#include <sys/eventfd.h>
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
// Linux implementation using epoll
//
class PollSetImpl
{
public:
	using Mutex = Poco::SpinlockMutex;
	using ScopedLock = Mutex::ScopedLock;

	PollSetImpl(): _epollfd(epoll_create(1)),
		_events(1024),
		_eventfd(eventfd(0, 0))
	{
		int err = addImpl(_eventfd, PollSet::POLL_READ, 0);
		if ((err) || (_epollfd < 0))
		{
			SocketImpl::error();
		}
	}

	~PollSetImpl()
	{
		::close(_eventfd.exchange(0));
		ScopedLock l(_mutex);
		if (_epollfd >= 0) ::close(_epollfd);
	}

	void add(const Socket& socket, int mode)
	{
		SocketImpl* sockImpl = socket.impl();

		int err = addImpl(sockImpl->sockfd(), mode, sockImpl);

		if (err)
		{
			if (errno == EEXIST) update(socket, mode);
			else SocketImpl::error();
		}

		ScopedLock lock(_mutex);
		if (_socketMap.find(sockImpl) == _socketMap.end())
			_socketMap[sockImpl] = socket;
	}

	void remove(const Socket& socket)
	{
		poco_socket_t fd = socket.impl()->sockfd();
		struct epoll_event ev;
		ev.events = 0;
		ev.data.ptr = 0;

		ScopedLock lock(_mutex);
		int err = epoll_ctl(_epollfd, EPOLL_CTL_DEL, fd, &ev);
		if (err) SocketImpl::error();
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

		int err = 0;
		{
			ScopedLock lock(_mutex);
			err = epoll_ctl(_epollfd, EPOLL_CTL_MOD, fd, &ev);
		}
		if (err) SocketImpl::error();
	}

	void clear()
	{
		{
			ScopedLock lock(_mutex);

			::close(_epollfd);
			_socketMap.clear();
			_epollfd = epoll_create(1);
			if (_epollfd < 0) SocketImpl::error();
		}
		::close(_eventfd.exchange(0));
		_eventfd = eventfd(0, 0);
		addImpl(_eventfd, PollSet::POLL_READ, 0);
	}

	PollSet::SocketModeMap poll(const Poco::Timespan& timeout)
	{
		PollSet::SocketModeMap result;
		Poco::Timespan remainingTime(timeout);
		int rc;
		ScopedLock lock(_mutex);
		do
		{
			Poco::Timestamp start;
			rc = epoll_wait(_epollfd, &_events[0], _events.size(), remainingTime.totalMilliseconds());
			if (rc == 0) return result;
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

		for (int i = 0; i < rc; i++)
		{
			if (_events[i].data.ptr) // skip eventfd
			{
				std::map<void *, Socket>::iterator it = _socketMap.find(_events[i].data.ptr);
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
		}

		return result;
	}

	void wakeUp()
	{
		uint64_t val = 1;
		// This is guaranteed to write into a valid fd,
		// or 0 (meaning PollSet is being destroyed).
		// Errors are ignored.
		::write(_eventfd, &val, sizeof(val));
	}

	int count() const
	{
		ScopedLock lock(_mutex);
		return static_cast<int>(_socketMap.size());
	}

private:
	int addImpl(int fd, int mode, void* ptr)
	{
		struct epoll_event ev;
		ev.events = 0;
		if (mode & PollSet::POLL_READ)
			ev.events |= EPOLLIN;
		if (mode & PollSet::POLL_WRITE)
			ev.events |= EPOLLOUT;
		if (mode & PollSet::POLL_ERROR)
			ev.events |= EPOLLERR;
		ev.data.ptr = ptr;
		ScopedLock lock(_mutex);
		return epoll_ctl(_epollfd, EPOLL_CTL_ADD, fd, &ev);
	}

	mutable Mutex _mutex;
	int _epollfd;
	std::map<void*, Socket> _socketMap;
	std::vector<struct epoll_event> _events;
	std::atomic<int> _eventfd;
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
#ifdef _WIN32
			rc = WSAPoll(&_pollfds[0], static_cast<ULONG>(_pollfds.size()), static_cast<INT>(remainingTime.totalMilliseconds()));
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
						if ((it->revents & POLLIN)
#ifdef _WIN32
							|| (it->revents & POLLHUP)
#endif
							)
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
	Poco::Pipe _pipe;
		/// Add _pipe to head of _pollfds used to wake up poll blocking
};


#else


#ifdef POCO_OS_FAMILY_WINDOWS


//
// Windows-specific implementation using select()
// The size of select set is determined at compile
// time (see FD_SETSIZE in SocketDefs.h).
//
// This implementation works around that limit by
// having multiple socket descriptor sets and,
// when needed, calling select() multiple times.
// To avoid multiple sets situtation, the FD_SETSIZE
// can be increased, however then Poco::Net library
// must be recompiled in order for the new setting
// to be in effect.
//


class PollSetImpl
{
public:
	PollSetImpl() : _fdRead(1, {0, {0}}),
		_fdWrite(1, {0, {0}}),
		_fdExcept(1, {0, {0}}),
		_pFDRead(std::make_unique<fd_set>()),
		_pFDWrite(std::make_unique<fd_set>()),
		_pFDExcept(std::make_unique<fd_set>()),
		_nfd(0)
	{
	}

	void add(const Socket& socket, int mode)
	{
		Poco::Net::SocketImpl* pImpl = socket.impl();
		poco_check_ptr(pImpl);
		Poco::FastMutex::ScopedLock lock(_mutex);
		_map[socket] = mode;
		setMode(pImpl->sockfd(), mode);
	}

	void remove(const Socket& socket)
	{
		Poco::Net::SocketImpl* pImpl = socket.impl();
		poco_check_ptr(pImpl);
		Poco::FastMutex::ScopedLock lock(_mutex);
		remove(pImpl->sockfd());
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
		Poco::Net::SocketImpl* pImpl = socket.impl();
		poco_check_ptr(pImpl);
		SOCKET fd = pImpl->sockfd();
		Poco::FastMutex::ScopedLock lock(_mutex);
		_map[socket] = mode;
		setMode(fd, mode);
		if (!(mode & PollSet::POLL_READ)) remove(fd, _fdRead);
		if (!(mode & PollSet::POLL_WRITE)) remove(fd, _fdWrite);
		if (!(mode & PollSet::POLL_ERROR)) remove(fd, _fdExcept);
	}

	void clear()
	{
		Poco::FastMutex::ScopedLock lock(_mutex);
		_map.clear();
		for (auto& fd : _fdRead) std::memset(&fd, 0, sizeof(fd));
		for (auto& fd : _fdWrite) std::memset(&fd, 0, sizeof(fd));
		for (auto& fd : _fdExcept) std::memset(&fd, 0, sizeof(fd));
		_nfd = 0;
	}

	PollSet::SocketModeMap poll(const Poco::Timespan& timeout)
	{
		Poco::Timestamp start;
		poco_assert_dbg(_fdRead.size() == _fdWrite.size());
		poco_assert_dbg(_fdWrite.size() == _fdExcept.size());

		PollSet::SocketModeMap result;
		if (_nfd == 0) return result;

		Poco::Timespan remainingTime(timeout);
		struct timeval tv {0, 1000};

		Poco::FastMutex::ScopedLock lock(_mutex);

		auto readIt = _fdRead.begin();
		auto writeIt = _fdWrite.begin();
		auto exceptIt = _fdExcept.begin();
		do
		{
			std::memcpy(_pFDRead.get(), &*readIt, sizeof(fd_set));
			std::memcpy(_pFDWrite.get(), &*writeIt, sizeof(fd_set));
			std::memcpy(_pFDExcept.get(), &*exceptIt, sizeof(fd_set));

			int rc;
			do
			{
				rc = ::select((int)_nfd + 1, _pFDRead.get(), _pFDWrite.get(), _pFDExcept.get(), &tv);
			} while (rc < 0 && SocketImpl::lastError() == POCO_EINTR);
			if (rc < 0) SocketImpl::error();
			else if (rc > 0)
			{
				for (auto it = _map.begin(); it != _map.end(); ++it)
				{
					poco_socket_t fd = it->first.impl()->sockfd();
					if (fd != POCO_INVALID_SOCKET)
					{
						if (FD_ISSET(fd, _pFDRead.get()))
						{
							result[it->first] |= PollSet::POLL_READ;
						}
						if (FD_ISSET(fd, _pFDWrite.get()))
						{
							result[it->first] |= PollSet::POLL_WRITE;
						}
						if (FD_ISSET(fd, _pFDExcept.get()))
						{
							result[it->first] |= PollSet::POLL_ERROR;
						}
					}
				}
			}

			Timespan elapsed = Timestamp() - start;
			if (++readIt == _fdRead.end())
			{
				if ((rc > 0) || (elapsed.totalMilliseconds() > timeout.totalMilliseconds()))
					break;
				readIt = _fdRead.begin();
				writeIt = _fdWrite.begin();
				exceptIt = _fdExcept.begin();
			}
			else
			{
				++writeIt;
				++exceptIt;
			}

			Poco::UInt64 tOut = (((Poco::UInt64)tv.tv_sec * 1000000) + tv.tv_usec) * 2;
			Poco::Timespan left = timeout - elapsed;
			if (tOut > left.totalMicroseconds())
				tOut = left.totalMicroseconds();

			tv.tv_sec = static_cast<long>(tOut / 1000000);
			tv.tv_usec = tOut % 1000000;
		} while (true);

		return result;
	}

	int count() const
	{
		Poco::FastMutex::ScopedLock lock(_mutex);
		return static_cast<int>(_map.size());
	}

	void wakeUp()
	{
		// TODO
	}

private:

	void setMode(std::vector<fd_set>& fdSet, SOCKET fd)
	{
		SOCKET* pFD = 0;
		for (auto& fdr : fdSet)
		{
			SOCKET* begin = fdr.fd_array;
			SOCKET* end = fdr.fd_array + fdr.fd_count;
			pFD = std::find(begin, end, fd);
			if (end != pFD)
			{
				FD_SET(fd, &fdr);
				if (fd > _nfd) _nfd = fd;
				return;
			}
		}
		// not found, insert at first free location
		for (auto& fdr : fdSet)
		{
			if (fdr.fd_count < FD_SETSIZE)
			{
				fdr.fd_count++;
				fdr.fd_array[fdr.fd_count-1] = fd;
				if (fd > _nfd) _nfd = fd;
				return;
			}
		}
		// all fd sets are full; insert another one
		fdSet.push_back({0, {0}});
		fd_set& fds = fdSet.back();
		fds.fd_count = 1;
		fds.fd_array[0] = fd;
		if (fd > _nfd) _nfd = fd;
	}

	void setMode(SOCKET fd, int mode)
	{
		if (mode & PollSet::POLL_READ) setMode(_fdRead, fd);
		if (mode & PollSet::POLL_WRITE) setMode(_fdWrite, fd);
		if (mode & PollSet::POLL_ERROR) setMode(_fdExcept, fd);
	}

	void remove(SOCKET fd, std::vector<fd_set>& fdSets)
	{
		bool newNFD = false;
		for (auto& fdSet : fdSets)
		{
			if (fdSet.fd_count)
			{
				newNFD = (fd == _nfd);
				int i = 0;
				for (; i < fdSet.fd_count; ++i)
				{
					if (fdSet.fd_array[i] == fd)
					{
						if (i == (fdSet.fd_count-1))
						{
							fdSet.fd_array[i] = 0;
						}
						else
						{
							for (; i < fdSet.fd_count-1; ++i)
							{
								fdSet.fd_array[i] = fdSet.fd_array[i+1];
								if (newNFD && fdSet.fd_array[i] > _nfd)
									_nfd = fdSet.fd_array[i];
							}
						}
						fdSet.fd_array[fdSet.fd_count-1] = 0;
						fdSet.fd_count--;
						break;
					}
					if (newNFD && fdSet.fd_array[i] > _nfd)
						_nfd = fdSet.fd_array[i];
				}
			}
		}
		if (newNFD)
		{
			findNFD(_fdRead);
			findNFD(_fdWrite);
			findNFD(_fdExcept);
		}
	}

	void findNFD(std::vector<fd_set>& fdSets)
	{
		for (auto& fdSet : fdSets)
		{
			for (int i = 0; i < fdSet.fd_count; ++i)
			{
				if (fdSet.fd_array[i] > _nfd)
					_nfd = fdSet.fd_array[i];
			}
		}
	}

	void remove(SOCKET fd)
	{
		remove(fd, _fdRead);
		remove(fd, _fdWrite);
		remove(fd, _fdExcept);
	}

	mutable Poco::FastMutex _mutex;
	PollSet::SocketModeMap  _map;
	SOCKET                  _nfd;
	std::vector<fd_set>     _fdRead;
	std::vector<fd_set>     _fdWrite;
	std::vector<fd_set>     _fdExcept;
	std::unique_ptr<fd_set> _pFDRead;
	std::unique_ptr<fd_set> _pFDWrite;
	std::unique_ptr<fd_set> _pFDExcept;
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


#endif // POCO_OS_FAMILY_WINDOWS


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
