//
// HTTPEventArgs.cpp
//
// $Id: //poco/1.4/Net/src/HTTPEventArgs.cpp#1 $
//
// Library: Net
// Package: HTTPClient
// Module:  HTTPEventArgs
//
// Implementation of HTTPEventArgs
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Net/HTTPEventArgs.h"
#include "Poco/Net/DNS.h"
#include "Poco/Net/NetException.h"


namespace Poco {
namespace Net {


HTTPEventArgs::HTTPEventArgs(const std::string& uri,
	const HTTPResponse& response,
	const std::string& body,
	const Poco::Exception* pException) : _uri(uri),
	_response(response),
	_body(body),
	_pException(pException ? pException->clone() : 0)
{
}


HTTPEventArgs::~HTTPEventArgs()
{
	delete _pException;
}


} } // namespace Poco::Net
