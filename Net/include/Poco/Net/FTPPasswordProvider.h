//
// FTPPasswordProvider.h
//
// Library: Net
// Package: FTP
// Module:  FTPPasswordProvider
//
// Definition of the FTPPasswordProvider class.
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Net_FTPPasswordProvider_INCLUDED
#define Net_FTPPasswordProvider_INCLUDED


#include <string>


namespace Poco {
namespace Net {


class Net_API FTPPasswordProvider
	/// The base class for all password providers.
	/// An instance of a subclass of this class can be
	/// registered with the FTPPasswordProvider to
	/// provide a password
{
public:
	virtual std::string password(const std::string& username, const std::string& host) = 0;
		/// Provide the password for the given user on the given host.

protected:
	FTPPasswordProvider();
	virtual ~FTPPasswordProvider();
};

} } // namespace Poco::Net


#endif // Net_FTPPasswordProvider_INCLUDED
