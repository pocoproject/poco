//
// PartSource.cpp
//
// $Id: //poco/1.4/Net/src/PartSource.cpp#1 $
//
// Library: Net
// Package: Messages
// Module:  PartSource
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Net/PartSource.h"


namespace Poco {
namespace Net {


PartSource::PartSource():
	_mediaType("application/octet-stream")
{
}

	
PartSource::PartSource(const std::string& mediaType):
	_mediaType(mediaType)
{
}


PartSource::~PartSource()
{
}


namespace
{
	static const std::string EMPTY;
}


const std::string& PartSource::filename() const
{
	return EMPTY;
}


} } // namespace Poco::Net
