//
// PathSerializer.cpp
//
// Library: RemotingNG/REST
// Package: REST
// Module:  PathSerializer
//
// Copyright (c) 2010-2016, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/RemotingNG/REST/PathSerializer.h"
#include "Poco/RemotingNG/RemotingException.h"
#include "Poco/Base64Encoder.h"
#include "Poco/URI.h"
#include "Poco/Path.h"
#include <sstream>


using namespace std::string_literals;


namespace Poco {
namespace RemotingNG {
namespace REST {


const std::string PathSerializer::EMPTY;


PathSerializer::PathSerializer(const std::string& pathTemplate, Poco::Net::HTTPRequest& request):
	_pathTemplate(pathTemplate),
	_request(request)
{
}


PathSerializer::~PathSerializer()
{
}


void PathSerializer::close()
{
	Poco::URI uri(_request.getURI());
	Poco::Path pathTempl(_pathTemplate, Poco::Path::PATH_UNIX);
	Poco::Path path(true);
	for (int i = 0; i < pathTempl.depth(); i++)
	{
		std::string encodedItem;
		Poco::URI::encode(replaceItem(pathTempl[i]), "?#"s, encodedItem);
		path.pushDirectory(encodedItem);
	}
	std::string encodedName;
	Poco::URI::encode(replaceItem(pathTempl.getFileName()), "?#"s, encodedName);
	path.setFileName(encodedName);
	uri.setPath(path.toString(Poco::Path::PATH_UNIX));
	_request.setURI(uri.toString());
}


const std::string& PathSerializer::replaceItem(const std::string& item)
{
	std::string::const_iterator it = item.begin();
	std::string::const_iterator end = item.end();
	if (it != end && *it == '{')
	{
		++it;
		std::string name;
		while (it != end && *it != '}') name += *it++;
		std::map<std::string, std::string>::const_iterator itm = _values.find(name);
		if (itm != _values.end())
			return itm->second;
		else
			throw Poco::RemotingNG::SerializerException("Missing parameter for path", name);
	}
	else return item;
}


void PathSerializer::serializeData(const std::string& name, const std::string& val)
{
	_values[name] = val;
}


} } } // namespace Poco::RemotingNG::REST
