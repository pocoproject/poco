//
// Socket.cpp
//
// Library: Net
// Package: Sockets
// Module:  Socket
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Net/Socket.h"
#include "Poco/Net/StreamSocketImpl.h"
#include "Poco/Net/PollSet.h"
#include "Poco/Timestamp.h"
#include "Poco/Error.h"
#include <algorithm>
#include <string.h> // FD_SET needs memset on some platforms, so we can't use <cstring>


#if defined(POCO_HAVE_FD_POLL)
	#include "Poco/SharedPtr.h"
	#ifndef _WIN32
		#include <poll.h>
	#endif
#endif


namespace Poco {
namespace Net {


Socket::Socket():
	_pImpl(new StreamSocketImpl)
{
}


Socket::Socket(SocketImpl* pImpl):
	_pImpl(pImpl)
{
	poco_check_ptr (_pImpl);
}


Socket::Socket(const Socket& socket):
	_pImpl(socket._pImpl)
{
	poco_check_ptr (_pImpl);

	_pImpl->duplicate();
}


Socket Socket::fromFileDescriptor(poco_socket_t fd)
{
	Socket s;
	s.impl()->useFileDescriptor(fd);
	return s;
}


#if POCO_NEW_STATE_ON_MOVE

Socket::Socket(Socket&& socket):
	_pImpl(socket._pImpl)
{
	poco_check_ptr (_pImpl);

	socket._pImpl = nullptr;
}


Socket& Socket::operator = (Socket&& socket)
{
	if (&socket != this)
	{
		if (_pImpl) _pImpl->release();
		_pImpl = socket._pImpl;
		socket._pImpl = nullptr;
	}
	return *this;
}

#endif // POCO_NEW_STATE_ON_MOVE


Socket& Socket::operator = (const Socket& socket)
{
	if (&socket != this)
	{
		if (_pImpl) _pImpl->release();
		_pImpl = socket._pImpl;
		if (_pImpl) _pImpl->duplicate();
	}
	return *this;
}

Socket::~Socket()
{
	if (_pImpl) _pImpl->release();
}


int Socket::select(SocketList& readList, SocketList& writeList, SocketList& exceptList, const Poco::Timespan& timeout)
{
#if defined(POCO_HAVE_FD_EPOLL)

	int epollSize = static_cast<int>(readList.size() + writeList.size() + exceptList.size());
	if (epollSize == 0) return 0;

	PollSet ps;
	for (const auto& s : readList) ps.add(s, PollSet::POLL_READ);
	for (const auto& s : writeList) ps.add(s, PollSet::POLL_WRITE);

	readList.clear();
	writeList.clear();
	exceptList.clear();

	PollSet::SocketModeMap sm = ps.poll(timeout);
	for (const auto& s : sm)
	{
		if (s.second & PollSet::POLL_READ) readList.push_back(s.first);
		if (s.second & PollSet::POLL_WRITE) writeList.push_back(s.first);
		if (s.second & PollSet::POLL_ERROR) exceptList.push_back(s.first);
	}

	return static_cast<int>(readList.size() + writeList.size() + exceptList.size());

#elif defined(POCO_HAVE_FD_POLL)
	typedef Poco::SharedPtr<pollfd, Poco::ReferenceCounter, Poco::ReleaseArrayPolicy<pollfd>> SharedPollArray;
#ifdef _WIN32
	typedef ULONG nfds_t;
#endif
	nfds_t nfd = static_cast<nfds_t>(readList.size() + writeList.size() + exceptList.size());

	if (0 == nfd) return 0;

	SharedPollArray pPollArr = new pollfd[nfd]();

	int idx = 0;
	for (SocketList::iterator it = readList.begin(); it != readList.end(); ++it)
	{
		pPollArr[idx].fd = int(it->sockfd());
		pPollArr[idx++].events |= POLLIN;
	}

	SocketList::iterator begR = readList.begin();
	SocketList::iterator endR = readList.end();
	for (SocketList::iterator it = writeList.begin(); it != writeList.end(); ++it)
	{
		SocketList::iterator pos = std::find(begR, endR, *it);
		if (pos != endR)
		{
			pPollArr[pos-begR].events |= POLLOUT;
			--nfd;
		}
		else
		{
			pPollArr[idx].fd = int(it->sockfd());
			pPollArr[idx++].events |= POLLOUT;
		}
	}

	SocketList::iterator begW = writeList.begin();
	SocketList::iterator endW = writeList.end();
	for (SocketList::iterator it = exceptList.begin(); it != exceptList.end(); ++it)
	{
		SocketList::iterator pos = std::find(begR, endR, *it);
		if (pos != endR) --nfd;
		else
		{
			SocketList::iterator pos = std::find(begW, endW, *it);
			if (pos != endW) --nfd;
			else pPollArr[idx++].fd = int(it->sockfd());
		}
	}

	Poco::Timespan remainingTime(timeout);
	int rc;
	do
	{
		Poco::Timestamp start;
#ifdef _WIN32
		rc = WSAPoll(pPollArr, nfd, static_cast<INT>(remainingTime.totalMilliseconds()));
#else
		rc = ::poll(pPollArr, nfd, remainingTime.totalMilliseconds());
#endif
		if (rc < 0 && SocketImpl::lastError() == POCO_EINTR)
		{
			Poco::Timestamp end;
			Poco::Timespan waited = end - start;
			if (waited < remainingTime) remainingTime -= waited;
			else remainingTime = 0;
		}
	}
	while (rc < 0 && SocketImpl::lastError() == POCO_EINTR);
	if (rc < 0) SocketImpl::error();

	SocketList readyReadList;
	SocketList readyWriteList;
	SocketList readyExceptList;

	SocketList::iterator begE = exceptList.begin();
	SocketList::iterator endE = exceptList.end();
	for (int idx = 0; idx < nfd; ++idx)
	{
		SocketList::iterator slIt = std::find_if(begR, endR, Socket::FDCompare(static_cast<int>(pPollArr[idx].fd)));
		if (POLLIN & pPollArr[idx].revents && slIt != endR) readyReadList.push_back(*slIt);
		slIt = std::find_if(begW, endW, Socket::FDCompare(static_cast<int>(pPollArr[idx].fd)));
		if (POLLOUT & pPollArr[idx].revents && slIt != endW) readyWriteList.push_back(*slIt);
		slIt = std::find_if(begE, endE, Socket::FDCompare(static_cast<int>(pPollArr[idx].fd)));
		if (POLLERR & pPollArr[idx].revents && slIt != endE) readyExceptList.push_back(*slIt);
	}
	std::swap(readList, readyReadList);
	std::swap(writeList, readyWriteList);
	std::swap(exceptList, readyExceptList);
	return static_cast<int>(readList.size() + writeList.size() + exceptList.size());

#else

	fd_set fdRead;
	fd_set fdWrite;
	fd_set fdExcept;
	int nfd = 0;
	FD_ZERO(&fdRead);
	for (SocketList::const_iterator it = readList.begin(); it != readList.end(); ++it)
	{
		poco_socket_t fd = it->sockfd();
		if (fd != POCO_INVALID_SOCKET)
		{
			if (int(fd) > nfd)
				nfd = int(fd);
			FD_SET(fd, &fdRead);
		}
	}
	FD_ZERO(&fdWrite);
	for (SocketList::const_iterator it = writeList.begin(); it != writeList.end(); ++it)
	{
		poco_socket_t fd = it->sockfd();
		if (fd != POCO_INVALID_SOCKET)
		{
			if (int(fd) > nfd)
				nfd = int(fd);
			FD_SET(fd, &fdWrite);
		}
	}
	FD_ZERO(&fdExcept);
	for (SocketList::const_iterator it = exceptList.begin(); it != exceptList.end(); ++it)
	{
		poco_socket_t fd = it->sockfd();
		if (fd != POCO_INVALID_SOCKET)
		{
			if (int(fd) > nfd)
				nfd = int(fd);
			FD_SET(fd, &fdExcept);
		}
	}
	if (nfd == 0) return 0;
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

	SocketList readyReadList;
	for (SocketList::const_iterator it = readList.begin(); it != readList.end(); ++it)
	{
		poco_socket_t fd = it->sockfd();
		if (fd != POCO_INVALID_SOCKET)
		{
			if (FD_ISSET(fd, &fdRead))
				readyReadList.push_back(*it);
		}
	}
	std::swap(readList, readyReadList);
	SocketList readyWriteList;
	for (SocketList::const_iterator it = writeList.begin(); it != writeList.end(); ++it)
	{
		poco_socket_t fd = it->sockfd();
		if (fd != POCO_INVALID_SOCKET)
		{
			if (FD_ISSET(fd, &fdWrite))
				readyWriteList.push_back(*it);
		}
	}
	std::swap(writeList, readyWriteList);
	SocketList readyExceptList;
	for (SocketList::const_iterator it = exceptList.begin(); it != exceptList.end(); ++it)
	{
		poco_socket_t fd = it->sockfd();
		if (fd != POCO_INVALID_SOCKET)
		{
			if (FD_ISSET(fd, &fdExcept))
				readyExceptList.push_back(*it);
		}
	}
	std::swap(exceptList, readyExceptList);
	return rc;

#endif // POCO_HAVE_FD_EPOLL
}


SocketBufVec Socket::makeBufVec(std::size_t size, std::size_t bufLen)
{
	SocketBufVec buf(size);
	SocketBufVec::iterator it = buf.begin();
	SocketBufVec::iterator end = buf.end();
	for (; it != end; ++it)
	{
		// TODO: use memory pool
		*it = makeBuffer(malloc(bufLen), bufLen);
	}
	return buf;
}


void Socket::destroyBufVec(SocketBufVec& buf)
{
	SocketBufVec::iterator it = buf.begin();
	SocketBufVec::iterator end = buf.end();
	for (; it != end; ++it)
	{
#if defined(POCO_OS_FAMILY_WINDOWS)
		free(it->buf);
#elif defined(POCO_OS_FAMILY_UNIX)
		free(it->iov_base);
#endif
	}
	SocketBufVec().swap(buf);
}


SocketBuf Socket::makeBuffer(void* buffer, std::size_t length)
{
	SocketBuf ret;
#if defined(POCO_OS_FAMILY_WINDOWS)
	ret.buf = reinterpret_cast<char*>(buffer);
	ret.len = static_cast<ULONG>(length);
#elif defined(POCO_OS_FAMILY_UNIX)
	ret.iov_base = buffer;
	ret.iov_len = length;
#else
	throw NotImplementedException("Socket::makeBuffer(void*, size_t)");
#endif
	return ret;
}


SocketBufVec Socket::makeBufVec(const std::vector<char*>& vec)
{
	SocketBufVec buf(vec.size());
	SocketBufVec::iterator it = buf.begin();
	SocketBufVec::iterator end = buf.end();
	std::vector<char*>::const_iterator vIt = vec.begin();
	for (; it != end; ++it, ++vIt)
	{
		*it = makeBuffer(*vIt, strlen(*vIt));
	}
	return buf;
}


SocketBufVec Socket::makeBufVec(const std::vector<std::string>& vec)
{
	SocketBufVec buf(vec.size());
	SocketBufVec::iterator it = buf.begin();
	SocketBufVec::iterator end = buf.end();
	std::vector<std::string>::const_iterator vIt = vec.begin();
	for (; it != end; ++it, ++vIt)
	{
		char* c = const_cast<char*>(vIt->data());
		*it = makeBuffer(reinterpret_cast<void*>(c), vIt->size());
	}
	return buf;
}


int Socket::lastError()
{
	return SocketImpl::lastError();
}


std::string Socket::lastErrorDesc()
{
	return Error::getMessage(SocketImpl::lastError());
}


void Socket::error()
{
	SocketImpl::error();
}


} } // namespace Poco::Net
