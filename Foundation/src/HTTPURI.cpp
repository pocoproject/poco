//
// URI.cpp
//
// Library: Foundation
// Package: URI
// Module:  URI
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/HTTPURI.h"
#include <algorithm>

namespace Poco {


HTTPURI::HTTPURI()
{
}


HTTPURI::HTTPURI(const std::string& uri):
	URI(uri)
{
}


HTTPURI::HTTPURI(const char* uri):
	URI(uri)
{
}

	
HTTPURI::HTTPURI(const std::string& scheme, const std::string& pathEtc):
	URI(scheme, pathEtc)
{
}

	
HTTPURI::HTTPURI(const std::string& scheme, const std::string& authority, const std::string& pathEtc):
	URI(scheme, authority, pathEtc)
{
}


HTTPURI::HTTPURI(const std::string& scheme, const std::string& authority, const std::string& path, const std::string& query):
	URI(scheme, authority, path, query)
{
}


HTTPURI::HTTPURI(const std::string& scheme, const std::string& authority, const std::string& path, const std::string& query, const std::string& fragment):
	URI(scheme, authority, path, query, fragment)
{
}


HTTPURI::HTTPURI(const HTTPURI& uri):
	URI(uri)
{
	//TODO : :copy headers
}

	
HTTPURI::HTTPURI(const HTTPURI& baseURI, const std::string& relativeURI):
	URI(baseURI, relativeURI)
{
	//TODO : :copy headers
}


HTTPURI::HTTPURI(const Path& path):
	URI(path)
{
}


HTTPURI::~HTTPURI()
{
}


HTTPURI& HTTPURI::operator = (const HTTPURI& uri)
{
   URI::operator = (uri);
	if (&uri != this)
	{
		_headers  = uri._headers;
	}
	return *this;
}

	
HTTPURI& HTTPURI::operator = (const std::string& uri)
{
   URI::operator = (uri);
   return *this;
}


HTTPURI& HTTPURI::operator = (const char* uri)
{
   URI::operator = (uri);
   return *this;
}


void HTTPURI::swap(HTTPURI& uri)
{
	URI::swap(uri);
	std::swap(_headers, uri._headers);
}


void HTTPURI::clear()
{
	URI::clear();
	_headers.clear();
}

void HTTPURI::addHeader(const std::string& headerName, const std::string& headerValue)
{
	_headers.push_back(std::make_pair(headerName, headerValue));
}

const HTTPURI::Headers & HTTPURI::getHeaders()
{
	return _headers;
}

} // namespace Poco
