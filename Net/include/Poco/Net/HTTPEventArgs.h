//
// HTTPEventArgs.h
//
// $Id: //poco/1.4/Net/include/Poco/Net/HTTPEventArgs.h#1 $
//
// Library: Net
// Package: HTTPClient
// Module:  HTTPEventArgs
//
// Definition of HTTPEventArgs.
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Net_HTTPEventArgs_INCLUDED
#define Net_HTTPEventArgs_INCLUDED


#include "Poco/Net/Net.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Exception.h"


namespace Poco {
namespace Net {


class Net_API HTTPEventArgs
	/// The purpose of the HTTPEventArgs class is to be used as template parameter
	/// to instantiate event members in HTTPClient class.
	/// When clients register for an event notification, the reference to the class is 
	///	passed to the handler function to provide information about the event.
{
public:
	HTTPEventArgs(const std::string& uri,
		const HTTPResponse& response,
		const std::string& body,
		const Poco::Exception* pException = 0);
		/// Creates HTTPEventArgs.

	virtual ~HTTPEventArgs();
		/// Destroys HTTPEventArgs.

	std::string uri() const;
		/// Returns the URI for the HTTP response.

	const HTTPResponse& response() const;
		/// Returns the reference to the HTTP response.

	std::string body() const;
		/// Returns the response body.

	std::string error() const;
		/// Returns the error string for the request.

	const Poco::Exception* exception() const;
		/// Returns the exception if any, otherwise
		/// it returns null pointer.

private:
	HTTPEventArgs();

	std::string         _uri;
	const HTTPResponse& _response;
	std::string         _body;
	Poco::Exception*    _pException;
};


//
// inlines
//

inline std::string HTTPEventArgs::uri() const
{
	return _uri;
}


inline const HTTPResponse& HTTPEventArgs::response() const
{
	return _response;
}


inline std::string HTTPEventArgs::body() const
{
	return _body;
}


inline std::string HTTPEventArgs::error() const
{
	return _pException ? _pException->displayText() : std::string();
}


inline const Poco::Exception* HTTPEventArgs::exception() const
{
	return _pException;
}


} } // namespace Poco::Net


#endif
