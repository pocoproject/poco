//
// PathDeserializer.cpp
//
// Library: RemotingNG/REST
// Package: REST
// Module:  PathDeserializer
//
// Copyright (c) 2010-2016, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/RemotingNG/REST/PathDeserializer.h"
#include "Poco/RemotingNG/REST/PathSerializer.h"
#include "Poco/RemotingNG/RemotingException.h"
#include "Poco/NumberParser.h"
#include "Poco/Base64Decoder.h"
#include "Poco/URI.h"
#include "Poco/Path.h"
#include "Poco/String.h"
#include <sstream>
#include <vector>


namespace Poco {
namespace RemotingNG {
namespace REST {


PathDeserializer::PathDeserializer(const std::string& pathTemplate, const Poco::Net::HTTPRequest& request)
{
	parsePath(pathTemplate, request);
}


PathDeserializer::~PathDeserializer()
{
}


bool PathDeserializer::deserializeNullableBegin(const std::string& name, bool isMandatory, bool& isNull)
{
	isNull = _values.find(name) == _values.end();
	return !isNull;
}


const std::string& PathDeserializer::deserializeData(const std::string& name, bool isMandatory, bool& found)
{
	std::map<std::string, std::string>::const_iterator it = _values.find(name);
	if (it != _values.end())
	{
		found = true;
		return it->second;
	}
	else if (isMandatory)
	{
		throw Poco::RemotingNG::MissingParamException(name);
	}
	else
	{
		found = false;
		return PathSerializer::EMPTY;
	}
}


void PathDeserializer::parsePath(const std::string& pathTemplate, const Poco::Net::HTTPRequest& request)
{
	Poco::URI uri(request.getURI());
	Poco::Path path(uri.getPath(), Poco::Path::PATH_UNIX);

	// Manually tokenize pathTemplate by '/' to avoid Poco::Path mangling special characters
	std::vector<std::string> templParts;
	std::string::size_type start = 0;
	if (!pathTemplate.empty() && pathTemplate[0] == '/') start = 1;
	while (start < pathTemplate.size())
	{
		std::string::size_type end = pathTemplate.find('/', start);
		if (end == std::string::npos) end = pathTemplate.size();
		templParts.push_back(pathTemplate.substr(start, end - start));
		start = end + 1;
	}

	for (std::size_t i = 0; i < templParts.size(); i++)
	{
		const std::string& item = templParts[i];
		if (item.size() > 2 && item[0] == '{')
		{
			// Check for alternation pattern: {name:(option1|option2|...)}
			std::size_t colonPos = item.find(':');
			if (colonPos != std::string::npos && colonPos > 1 && colonPos + 1 < item.size() && item[colonPos + 1] == '(')
			{
				std::string name(item, 1, colonPos - 1);
				if (i <= static_cast<std::size_t>(path.depth()))
				{
					_values[name] = path[static_cast<int>(i)];
				}
			}
			else
			{
				std::string name(item, 1, item.size() - 2);
				if (i <= static_cast<std::size_t>(path.depth()))
				{
					_values[name] = path[static_cast<int>(i)];
				}
			}
		}
	}
}


} } } // namespace Poco::RemotingNG::REST
