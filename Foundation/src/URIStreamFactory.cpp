//
// URIStreamFactory.cpp
//
// Library: Foundation
// Package: URI
// Module:  URIStreamFactory
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/URIStreamFactory.h"
#include <algorithm>
#include <utility>


namespace Poco {


URIStreamFactory::URIStreamFactory()
= default;


URIStreamFactory::~URIStreamFactory()
= default;


URIRedirection::URIRedirection(std::string  uri):
	_uri(std::move(uri))
{
}


URIRedirection::URIRedirection(const URIRedirection& redir)
	
= default;


URIRedirection& URIRedirection::operator = (const URIRedirection& redir)
{
	URIRedirection tmp(redir);
	swap(tmp);
	return *this;
}


void URIRedirection::swap(URIRedirection& redir) noexcept
{
	std::swap(_uri, redir._uri);
}


} // namespace Poco
