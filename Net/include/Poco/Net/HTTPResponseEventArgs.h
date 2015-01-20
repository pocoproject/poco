//
// HTTPResponseEventArgs.h
//
// $Id: //poco/1.4/Net/include/Poco/Net/HTTPResponseEventArgs.h#1 $
//
// Library: Net
// Package: HTTPClient
// Module:  HTTPResponseEventArgs
//
// Definition of HTTPResponseEventArgs.
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Net_HTTPResponseEventArgs_INCLUDED
#define Net_HTTPResponseEventArgs_INCLUDED


#include "Poco/Net/Net.h"
#include "Poco/Net/SocketAddress.h"


namespace Poco {
namespace Net {


class Net_API HTTPResponseEventArgs
	/// The purpose of the HTTPResponseEventArgs class is to be used as template parameter
	/// to instantiate event members in HTTPClient class.
	/// When clients register for an event notification, the reference to the class is 
	///	passed to the handler function to provide information about the event.
{
public:
	HTTPResponseEventArgs(const std::string& uri,
		const std::string& response,
		const std::string& error = "");
		/// Creates HTTPResponseEventArgs.

	virtual ~HTTPResponseEventArgs();
		/// Destroys HTTPResponseEventArgs.

	std::string uri() const;
		/// Tries to resolve the target IP address into host name.
		/// If unsuccessful, all exceptions are silently ignored and 
		///	the IP address is returned.

	std::string response() const;
		/// Returns the target IP address.

	std::string error() const;
		/// Returns the error string for the request.

private:
	HTTPResponseEventArgs();

	std::string _uri;
	std::string _response;
	std::string _error;
};


//
// inlines
//

inline std::string HTTPResponseEventArgs::uri() const
{
	return _uri;
}


inline std::string HTTPResponseEventArgs::response() const
{
	return _response;
}


inline std::string HTTPResponseEventArgs::error() const
{
	return _error;
}


} } // namespace Poco::Net


#endif
