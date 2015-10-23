//
// Socket.cpp
//
// $Id: //poco/1.4/Net/src/Socket.cpp#3 $
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
#include "Poco/Timestamp.h"


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
	_pImpl->release();
}


int Socket::select(SocketList& readList, SocketList& writeList, SocketList& exceptList, const Poco::Timespan& timeout)
{
	typedef SocketImpl::SocketImplList sockImpList;
	sockImpList sockImpReadList;
	sockImpList sockImpWriteList;
	sockImpList sockeImpExceptList;

	for (SocketList::const_iterator it = readList.begin(); it != readList.end(); ++it)
	{
		if (it->sockfd() != POCO_INVALID_SOCKET) sockImpReadList.push_back(it->impl());
	}
	for (SocketList::const_iterator it = writeList.begin(); it != writeList.end(); ++it)
	{
		if (it->sockfd() != POCO_INVALID_SOCKET) sockImpWriteList.push_back(it->impl());
	}
	for (SocketList::const_iterator it = exceptList.begin(); it != exceptList.end(); ++it)
	{
		if (it->sockfd() != POCO_INVALID_SOCKET) sockeImpExceptList.push_back(it->impl());
	}

	SocketImpl::select(sockImpReadList, sockImpWriteList, sockeImpExceptList, timeout);

 	SocketList readyReadList;
	SocketList readyWriteList;
	SocketList readyExceptList;

	for (sockImpList::iterator it = sockImpReadList.begin(); it != sockImpReadList.end(); ++it)
	{
		readyReadList.push_back(Socket(*it));
		(*it)->duplicate();
	}
	for (sockImpList::iterator it = sockImpWriteList.begin(); it != sockImpWriteList.end(); ++it)
	{
		readyWriteList.push_back(Socket(*it));
		(*it)->duplicate();
	}
	for (sockImpList::iterator it = sockeImpExceptList.begin(); it != sockeImpExceptList.end(); ++it)
	{
		readyExceptList.push_back(Socket(*it));
		(*it)->duplicate();
	}

	std::swap(readList, readyReadList);
	std::swap(writeList, readyWriteList);
	std::swap(exceptList, readyExceptList);
	return readList.size() + writeList.size() + exceptList.size();
}


} } // namespace Poco::Net
