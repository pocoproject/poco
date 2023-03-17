//
// DataURIStreamFactory.cpp
//
// Library: Foundation
// Package: URI
// Module:  DataURIStreamFactory
//
// Copyright (c) 2019, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/DataURIStreamFactory.h"
#include "Poco/DataURIStream.h"
#include "Poco/URI.h"
#include "Poco/URIStreamOpener.h"


namespace Poco {


DataURIStreamFactory::DataURIStreamFactory()
{
}


DataURIStreamFactory::~DataURIStreamFactory()
{
}


std::istream* DataURIStreamFactory::open(const URI& uri)
{
	poco_assert (uri.getScheme() == "data");

	return new DataURIStream(uri);
}


void DataURIStreamFactory::registerFactory()
{
	URIStreamOpener::defaultOpener().registerStreamFactory("data", new DataURIStreamFactory);
}


void DataURIStreamFactory::unregisterFactory()
{
	URIStreamOpener::defaultOpener().unregisterStreamFactory("data");
}


} // namespace Poco
