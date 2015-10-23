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
	SocketImplList socketImpReadList;
	SocketImplList socketImpWriteList;
	SocketImplList socketImpExceptList;

	SocketListToSocketImplList(readList,   socketImpReadList);
	SocketListToSocketImplList(writeList,  socketImpWriteList);
	SocketListToSocketImplList(exceptList, socketImpExceptList);

	SocketImpl::select(socketImpReadList, socketImpWriteList, socketImpExceptList, timeout);

 	SocketList readyReadList;
	SocketList readyWriteList;
	SocketList readyExceptList;

	SocketImplListToSocketList(socketImpReadList,   readyReadList);
	SocketImplListToSocketList(socketImpWriteList,  readyWriteList);
	SocketImplListToSocketList(socketImpExceptList, readyExceptList);

	std::swap(readList,   readyReadList);
	std::swap(writeList,  readyWriteList);
	std::swap(exceptList, readyExceptList);
	return readList.size() + writeList.size() + exceptList.size();
}


void Socket::SocketListToSocketImplList(const SocketList& socketList, SocketImplList& socketImplList)
{
	for (SocketList::const_iterator it = socketList.begin(); it != socketList.end(); ++it)
	{
		if (it->sockfd() != POCO_INVALID_SOCKET) socketImplList.push_back(it->impl());
	}
}


void Socket::SocketImplListToSocketList(SocketImplList& socketImplList, SocketList& socketList)
{
	for (SocketImplList::iterator it = socketImplList.begin(); it != socketImplList.end(); ++it)
	{
		socketList.push_back(Socket(*it));
		(*it)->duplicate();
	}
}


} } // namespace Poco::Net
