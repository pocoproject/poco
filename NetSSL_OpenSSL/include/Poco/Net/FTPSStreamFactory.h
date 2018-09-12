//
// FTPSStreamFactory.h
//
// Library: Net
// Package: FTP
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
#include "Poco/URIStreamFactory.h"
#include "Poco/Net/FTPStreamFactory.h"
#include "Poco/Net/FTPPasswordProvider.h"

namespace Poco {
namespace Net {


class NetSSL_API FTPSStreamFactory: public Poco::URIStreamFactory
	/// An implementation of the URIStreamFactory interface
	/// that handles File Transfer Protocol (ftp) URIs.
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
	/// username "anonymous" and the password "
{
public:
	FTPSStreamFactory();
		/// Creates the FTPSStreamFactory.

	~FTPSStreamFactory();
		/// Destroys the FTPSStreamFactory.
		
	std::istream* open(const Poco::URI& uri);
		/// Creates and opens a HTTP stream for the given URI.
		/// The URI must be a ftp://... URI.
		///
		/// Throws a NetException if anything goes wrong.
		
	static void setAnonymousPassword(const std::string& password);
		/// Sets the password used for anonymous FTP.
		///
		/// WARNING: Setting the anonymous password is not
		/// thread-safe, so it's best to call this method
		/// during application initialization, before the
		/// FTPSStreamFactory is used for the first time.
		
	static const std::string& getAnonymousPassword();
		/// Returns the password used for anonymous FTP.
		
	static void setPasswordProvider(FTPPasswordProvider* pProvider);
		/// Sets the FTPPasswordProvider. If NULL is given,
		/// no password provider is used.
		///
		/// WARNING: Setting the password provider is not
		/// thread-safe, so it's best to call this method
		/// during application initialization, before the
		/// FTPSStreamFactory is used for the first time.
		
	static FTPPasswordProvider* getPasswordProvider();
		/// Returns the FTPPasswordProvider currently in use,
		/// or NULL if no one has been set.

	static void registerFactory();
		/// Registers the FTPSStreamFactory with the
		/// default URIStreamOpener instance.

	static void unregisterFactory();
		/// Unregisters the FTPSStreamFactory with the
		/// default URIStreamOpener instance.

protected:
	static void splitUserInfo(const std::string& userInfo, std::string& username, std::string& password);
	static void getUserInfo(const Poco::URI& uri, std::string& username, std::string& password);
	static void getPathAndType(const Poco::URI& uri, std::string& path, char& type);
	
private:
	static std::string          _anonymousPassword;
	static FTPPasswordProvider* _pPasswordProvider;
};


} } // namespace Poco::Net


#endif // Net_FTPSStreamFactory_INCLUDED