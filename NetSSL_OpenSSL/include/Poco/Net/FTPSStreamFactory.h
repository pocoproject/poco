//
// FTPSStreamFactory.h
//
// Library: NetSSL_OpenSSL
// Package: FTPS
// Module:  FTPSStreamFactory
//
// Definition of the FTPSStreamFactory class.
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Net_FTPSStreamFactory_INCLUDED
#define Net_FTPSStreamFactory_INCLUDED


#include "Poco/Net/NetSSL.h"
#include "Poco/Net/HTTPSession.h"
#include "Poco/Net/FTPStreamFactory.h"


namespace Poco {
namespace Net {


class NetSSL_API FTPSStreamFactory: public Poco::Net::FTPStreamFactory
	/// An implementation of the URIStreamFactory interface
	/// that handles secure File Transfer Protocol (ftps) URIs
	/// according to RFC 4217, based on the FTPSClientSession class.
	///
	/// The URI's path may end with an optional type specification
	/// in the form (;type=<typecode>), where <typecode> is
	/// one of a, i or d. If type=a, the file identified by the path
	/// is transferred in ASCII (text) mode. If type=i, the file
	/// is transferred in Image (binary) mode. If type=d, a directory
	/// listing (in NLST format) is returned. This corresponds with
	/// the FTP URL format specified in RFC 1738.
	///
	/// If the URI does not contain a username and password, the
	/// username "anonymous" and the password "poco@localhost".
	///
	/// Note that ftps is a non-standard URI scheme.
{
public:
	FTPSStreamFactory();
		/// Creates the FTPSStreamFactory.

	~FTPSStreamFactory();
		/// Destroys the FTPSStreamFactory.

	std::istream* open(const Poco::URI& uri);
		/// Creates and opens a HTTP stream for the given URI.
		/// The URI must be a ftps://... URI.
		///
		/// Throws a NetException if anything goes wrong.

	static void registerFactory();
		/// Registers the FTPSStreamFactory with the
		/// default URIStreamOpener instance.

	static void unregisterFactory();
		/// Unregisters the FTPSStreamFactory with the
		/// default URIStreamOpener instance.
};


} } // namespace Poco::Net


#endif // Net_FTPSStreamFactory_INCLUDED
