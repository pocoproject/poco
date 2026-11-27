//
// CookieFuzz.cpp
//
// Feeds a cookie header value as Set-Cookie on an HTTPResponse and as Cookie
// on an HTTPRequest.
//
// Copyright (c) 2026, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Net/HTTPCookie.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Net/NameValueCollection.h"
#include "Poco/Net/NetException.h"
#include "Poco/Exception.h"
#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>


using Poco::SyntaxException;
using Poco::Net::HTTPCookie;
using Poco::Net::HTTPRequest;
using Poco::Net::HTTPResponse;
using Poco::Net::MessageException;
using Poco::Net::NameValueCollection;


namespace
{
	constexpr std::size_t MAX_INPUT_SIZE = 8192;
}


extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
	if (size > MAX_INPUT_SIZE) return 0;

	const std::string input(reinterpret_cast<const char*>(data), size);

	try
	{
		HTTPResponse response;
		response.add(HTTPResponse::SET_COOKIE, input);
		std::vector<HTTPCookie> cookies;
		response.getCookies(cookies);
		for (const auto& cookie : cookies)
		{
			(void) cookie.toString();
		}
	}
	catch (const SyntaxException&)
	{
	}

	try
	{
		HTTPRequest request;
		request.add(HTTPRequest::COOKIE, input);
		NameValueCollection cookies;
		request.getCookies(cookies);
	}
	catch (const MessageException&)
	{
	}

	return 0;
}
