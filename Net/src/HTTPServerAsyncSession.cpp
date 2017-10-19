//
// HTTPServerAsyncSession.cpp
//
// Library: Net
// Package: HTTPServer
// Module:  HTTPServerAsyncSession
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Net/HTTPServerAsyncSession.h"


namespace Poco {
namespace Net {


HTTPServerAsyncSession::HTTPServerAsyncSession(HTTPServerParams::Ptr pParams) :
	//HTTPServerSession(true),
	_firstRequest(true),
	_keepAliveTimeout(pParams->getKeepAliveTimeout()),
	_maxKeepAliveRequests(pParams->getMaxKeepAliveRequests())
{
}


HTTPServerAsyncSession::~HTTPServerAsyncSession()
{
}


bool HTTPServerAsyncSession::hasMoreRequests()
{
	return false; // FIXME
}


} } // namespace Poco::Net
