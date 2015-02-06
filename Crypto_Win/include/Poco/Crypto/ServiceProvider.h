//
// ServiceProvider.h
//
// $Id$
//
// Library: Crypto_Win
// Package: CryptoCore
// Module:  ServiceProvider
//
// Definition of the ServiceProvider class.
//
// Copyright (c) 2006-2014, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Crypto_ServiceProvider_INCLUDED
#define Crypto_ServiceProvider_INCLUDED


#include "Poco/Crypto/Crypto.h"
#include "Poco/Mutex.h"
#include <wincrypt.h>


namespace Poco {
namespace Crypto {


class Crypto_Win_API ServiceProvider
	/// This class encapsulates a Windows Cryptographic Service Provider handle.
{
public:
	ServiceProvider();
		/// Creates the ServiceProvider and acquires a 
		/// Windows Cryptographic Service Provider.
		///
		/// Currently, this class always uses the 
		/// Microsoft AES Cryptographic Provider
		/// with the PROV_RSA_AES provider type.
		
	~ServiceProvider();
		/// Destroys the ServiceProvider.
	
	HCRYPTPROV handle() const;
		/// Returns the underlying handle.

protected:
	HCRYPTPROV _handle;

private:
	ServiceProvider(const ServiceProvider&);
	ServiceProvider& operator = (const ServiceProvider&);
};


//
// inlines
//
inline HCRYPTPROV ServiceProvider::handle() const
{
	return _handle;
}


} } // namespace Poco::Crypto


#endif // Crypto_ServiceProvider_INCLUDED
