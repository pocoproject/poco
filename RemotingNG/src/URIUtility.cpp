//
// URIUtility.cpp
//
// Library: RemotingNG
// Package: RemotingNG
// Module:  URIUtility
//
// Copyright (c) 2006-2014, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// License: Applied Informatics Software and Source Code License Agreement
//


#include "Poco/RemotingNG/URIUtility.h"
#include "Poco/StringTokenizer.h"
#include "Poco/URI.h"
#include "Poco/NumberFormatter.h"

namespace Poco {
namespace RemotingNG {


bool URIUtility::matchAlternatives(const std::string& segment, std::string::const_iterator& itTempl, std::string::const_iterator endTempl)
{
	bool matched = false;
	while (itTempl != endTempl && *itTempl != ')')
	{
		std::string::const_iterator altStart = itTempl;
		while (itTempl != endTempl && *itTempl != '|' && *itTempl != ')') ++itTempl;
		std::string alternative(altStart, itTempl);
		if (segment == alternative)
		{
			matched = true;
			break;
		}
		if (itTempl != endTempl && *itTempl == '|') ++itTempl;
	}
	while (itTempl != endTempl && *itTempl != ')') ++itTempl; // skip to ')'
	if (itTempl != endTempl) ++itTempl; // skip ')'
	return matched;
}


std::string URIUtility::createURIPath(const Identifiable::ObjectId& oid, const Identifiable::TypeId& tid, const std::string& protocol)
{
	poco_assert_dbg(!oid.empty());
	poco_assert_dbg(!tid.empty());
	poco_assert_dbg(!protocol.empty());
	std::string result("/");
	result += protocol;
	result += '/';
	result += tid;
	result += '/';
	result += oid;
	return result;
}


std::string URIUtility::createURIAuthority(const std::string& host, Poco::UInt16 port)
{
	std::string auth;
	if (host.find(':') != std::string::npos)
	{
		auth += '[';
		auth += host;
		auth += ']';
	}
	else auth += host;
	auth += ':';
	auth += Poco::NumberFormatter::format(port);
	return auth;
}


void URIUtility::parseURIPath(const std::string& path, Identifiable::ObjectId& oid, Identifiable::TypeId& tid, std::string& protocol)
{
	// note: we can get full URIs here
	Poco::StringTokenizer tok(path, "/"); // no trim, no ignore empty!

	if (tok.count() >= 3)
	{
		Poco::StringTokenizer::Iterator it = tok.end();
		--it;
		oid = *it;
		--it;
		tid = *it;
		--it;
		protocol = *it;
	}
	else throw Poco::SyntaxException("malformed RemotingNG URI path", path);
}


bool URIUtility::matchPath(const std::string& path, const std::string& pathTemplate)
{
	std::string::const_iterator itPath = path.begin();
	std::string::const_iterator endPath = path.end();
	std::string::const_iterator itTempl = pathTemplate.begin();
	std::string::const_iterator endTempl = pathTemplate.end();

	while (itPath != endPath && itTempl != endTempl)
	{
		if (*itPath == *itTempl)
		{
			++itPath;
			++itTempl;
		}
		else if (*itTempl == '{')
		{
			++itTempl;
			// Skip parameter name
			while (itTempl != endTempl && *itTempl != '}' && *itTempl != ':') ++itTempl;
			// Check for alternation pattern: {name:(option1|option2|...)}
			if (itTempl != endTempl && *itTempl == ':' && (itTempl + 1) != endTempl && *(itTempl + 1) == '(')
			{
				itTempl += 2; // skip ':('
				// Extract path segment to match
				std::string::const_iterator segStart = itPath;
				while (itPath != endPath && *itPath != '/') ++itPath;
				std::string segment(segStart, itPath);

				if (!matchAlternatives(segment, itTempl, endTempl)) return false;
				if (itTempl != endTempl && *itTempl == '}') ++itTempl; // skip '}'
			}
			else
			{
				// Regular placeholder: {name}
				while (itTempl != endTempl && *itTempl != '}') ++itTempl;
				if (itTempl != endTempl) ++itTempl;
				while (itPath != endPath && *itPath != '/') ++itPath;
			}
		}
		else if (*itTempl == '(')
		{
			// Standalone alternation pattern: (option1|option2|...)
			// Extract path segment to match
			std::string::const_iterator segStart = itPath;
			while (itPath != endPath && *itPath != '/') ++itPath;
			std::string segment(segStart, itPath);

			++itTempl; // skip '('
			if (!matchAlternatives(segment, itTempl, endTempl)) return false;
		}
		else return false;
	}

	return itPath == endPath && itTempl == endTempl;
}


} } // namespace Poco::RemotingNG
