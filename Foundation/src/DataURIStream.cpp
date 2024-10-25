//
// DataURIStream.cpp
//
// Library: Foundation
// Package: Streams
// Module:  DataURIStreamFactory
//
// Copyright (c) 2019, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/DataURIStream.h"
#include "Poco/Base64Decoder.h"
#include "Poco/Exception.h"
#include "Poco/MemoryStream.h"
#include "Poco/Path.h"
#include "Poco/StreamUtil.h"
#include "Poco/URI.h"
#include <string.h>


namespace Poco {


DataURIStreamIOS::DataURIStreamIOS(const URI& uri)
{
	poco_assert (uri.getScheme() == "data");

	const std::string& path = uri.getPath();
	size_t comma = path.find(',');
	if (comma == std::string::npos)
		throw DataFormatException();
	_data = path.substr(comma + 1);
	_memoryStream.reset(new MemoryInputStream(_data.data(), _data.length()));
	constexpr char base64[] = ";base64";
	const size_t base64Len = strlen(base64);
	if ((comma >= base64Len) && !path.compare(comma - base64Len, base64Len, base64, base64Len))
	{
		_base64Decoder.reset(new Base64Decoder(*_memoryStream, 0));
		_buf = _base64Decoder->rdbuf();
	}
	else
		_buf = _memoryStream->rdbuf();
	poco_ios_init(_buf);
}


DataURIStreamIOS::~DataURIStreamIOS()
{
}


std::streambuf* DataURIStreamIOS::rdbuf()
{
	return _buf;
}


DataURIStream::DataURIStream(const URI& uri): DataURIStreamIOS(uri), std::istream(_buf)
{
}


DataURIStream::~DataURIStream()
{
}


} // namespace Poco
