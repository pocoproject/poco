//
// VerificationArgs.h
//
// Library: NetSSL_OpenSSL
// Package: SSLCore
// Module:  VerificationArgs
//
// Definition of the VerificationArgs class.
//
// Copyright (c) 2006-2009, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef NetSSL_VerificationArgs_INCLUDED
#define NetSSL_VerificationArgs_INCLUDED


#include "Poco/Net/NetSSL.h"
#include "Poco/Net/X509Certificate.h"


namespace Poco {
namespace Net {


class NetSSL_API VerificationArgs
	/// A utility class for certificate handling.
{
public:
	VerificationArgs(const X509Certificate& cert, const X509Certificate& certToCheck, int depth, int code, const std::string& msg);
		/// Creates the VerificationArgs. _error is per default set to false.

	~VerificationArgs();
		/// Destroys the VerificationArgs.

	const X509Certificate& certificate() const;
		/// Returns the current certificate from X509 store.

	const X509Certificate& certificateToVerify() const;
		/// Returns the certificate to verify.

	int depth() const;
		/// Returns the position of the certificate in the certificate chain.

	int errorNumber() const;
		/// Returns the openssl x509 store ctx return code. See also X509_STORE_CTX_get_error

	void setErrorNumber(int osslErrorCode);
		/// Sets the error code of openssl x509 store ctx to osslErrorCode. See also X509_STORE_CTX_set_error.

	const std::string& message() const;
		/// Returns the textual presentation of the openssl return code.

	void setError(bool error);
		/// setError to true, if a verification error is judged by the user.

	bool isError() const;
		/// is error set.

private:
	X509Certificate	_cert;
	X509Certificate _certToCheck;
	int             _depth;
	int             _code;
	std::string     _message; /// Textual representation of the _code
	bool            _error;
};


//
// inlines
//
inline const X509Certificate& VerificationArgs::certificate() const
{
	return _cert;
}


inline const X509Certificate& VerificationArgs::certificateToVerify() const
{
	return _certToCheck;
}


inline int VerificationArgs::depth() const
{
	return _depth;
}


inline int VerificationArgs::errorNumber() const
{
	return _code;
}


inline void VerificationArgs::setErrorNumber(int osslErrorCode)
{
	_code = osslErrorCode;
}


inline const std::string& VerificationArgs::message() const
{
	return _message;
}


inline void VerificationArgs::setError(bool error)
{
	_error = error;
}


inline bool VerificationArgs::isError() const
{
	return _error;
}


} } // namespace Poco::Net


#endif // NetSSL_VerificationArgs_INCLUDED
