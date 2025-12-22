//
// Error.h
//
// $Id: //poco/1.7/DNSSD/include/Poco/DNSSD/Error.h#1 $
//
// Library: DNSSD
// Package: Core
// Module:  Error
//
// Definition of the Error class.
//
// Copyright (c) 2006-2024, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef DNSSD_Error_INCLUDED
#define DNSSD_Error_INCLUDED


#include "Poco/DNSSD/DNSSD.h"


namespace Poco {
namespace DNSSD {


class DNSSD_API Error
	/// Error stores information about an error
	/// that occured during browsing or service
	/// registration.
{
public:
	Error();
		/// Creates an uninitialized ServiceError.

	Error(Poco::Int32 networkInterface, Poco::Int32 code, const std::string& message);
		/// Creates the ServiceError using the given information.
		///
		///    - networkInterface specifies the index of the interface the error occured on.
		///    - code contains the implementation-specific error code.
		///    - message contains a human-readable error message.

	~Error();
		/// Destroys the ServiceError.

	Poco::Int32 networkInterface() const;
		/// Returns the network interface on which the error occurred.

	Poco::Int32 code() const;
		/// Returns the implementation-specific error code.

	const std::string& message() const;
		/// Returns the human-readable error message.

private:
	Poco::Int32   _networkInterface;
	Poco::Int32   _code;
	std::string   _message;
};


//
// inlines
//
inline Poco::Int32 Error::networkInterface() const
{
	return _networkInterface;
}


inline Poco::Int32 Error::code() const
{
	return _code;
}


inline const std::string& Error::message() const
{
	return _message;
}


} } // namespace Poco::DNSSD


#endif // DNSSD_Error_INCLUDED
