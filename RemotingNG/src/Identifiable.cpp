//
// Identifiable.cpp
//
// Library: RemotingNG
// Package: ORB
// Module:  Identifiable
//
// Copyright (c) 2006-2014, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// License: Applied Informatics Software and Source Code License Agreement
//


#include "Poco/RemotingNG/Identifiable.h"


namespace Poco {
namespace RemotingNG {


Identifiable::Identifiable(const Identifiable::ObjectId& oid): 
	_remoting__oid(oid)
{
}


Identifiable::~Identifiable()
{
}


void Identifiable::remoting__setURI(const Poco::URI& uri)
{
	std::string str = uri.toString();
	if (!str.empty())
	{
		if (str[str.length() - 1] == '/')
			str.resize(str.length() - 1);
	}
	_remoting__uri = str;
}


} } // namespace Poco::RemotingNG
