//
// HTTPResponseEventArgs.cpp
//
// $Id: //poco/1.4/Net/src/HTTPResponseEventArgs.cpp#1 $
//
// Library: Net
// Package: HTTPClient
// Module:  HTTPResponseEventArgs
//
// Implementation of HTTPResponseEventArgs
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Net/HTTPResponseEventArgs.h"
#include "Poco/Net/DNS.h"
#include "Poco/Net/NetException.h"


namespace Poco {
namespace Net {


HTTPResponseEventArgs::HTTPResponseEventArgs(const std::string& uri,
	const std::string& response,
	const std::string& error) : _uri(uri),
		_response(response),
		_error(error)
{
}


HTTPResponseEventArgs::~HTTPResponseEventArgs()
{
}


} } // namespace Poco::Net
