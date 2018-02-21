//
// VerificationErrorArgs.h
//
// Library: NetSSL_OpenSSL
// Package: SSLCore
// Module:  VerificationErrorArgs
//
// Definition of the VerificationErrorArgs class.
//
// Copyright (c) 2006-2009, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef NetSSL_VerificationErrorArgs_INCLUDED
#define NetSSL_VerificationErrorArgs_INCLUDED


#include "Poco/Net/NetSSL.h"
#include "Poco/Net/X509Certificate.h"


namespace Poco {
namespace Net {


class NetSSL_API VerificationErrorArgs
	/// A utility class for certificate error handling.
{
public:
	VerificationErrorArgs(const X509Certificate& cert, const X509Certificate& certToCheck, int errDepth, int errNum, const std::string& errMsg);
		/// Creates the VerificationErrorArgs. _ignoreError is per default set to false.

	~VerificationErrorArgs();
		/// Destroys the VerificationErrorArgs.

	const X509Certificate& certificate() const;
		/// Returns the certificate that caused the error.

	const X509Certificate& certificateToVerify() const;
		/// Returns the certificate to verify.

	int errorDepth() const;
		/// Returns the position of the certificate in the certificate chain.

	int errorNumber() const;
		/// Returns the openssl x509 store ctx return code. See also X509_STORE_CTX_get_error.

	void setErrorNumber(int osslErrorCode);
		/// Sets the error code of x509 store ctx to osslErrorCode. See also X509_STORE_CTX_set_error.

	const std::string& errorMessage() const;
		/// Returns the textual presentation of the errorNumber.

	void setIgnoreError(bool ignoreError);
		/// setIgnoreError to true, if a verification error is judged non-fatal by the user.

	bool getIgnoreError() const;
		/// returns the value of _ignoreError

private:
	X509Certificate	_cert;
	X509Certificate _certToCheck;
	int             _errorDepth;
	int             _errorNumber;
	std::string     _errorMessage; /// Textual representation of the _errorNumber
	bool            _ignoreError;
};


//
// inlines
//
inline const X509Certificate& VerificationErrorArgs::certificate() const
{
	return _cert;
}


inline const X509Certificate& VerificationErrorArgs::certificateToVerify() const
{
	return _certToCheck;
}


inline int VerificationErrorArgs::errorDepth() const
{
	return _errorDepth;
}


inline int VerificationErrorArgs::errorNumber() const
{
	return _errorNumber;
}


inline void VerificationErrorArgs::setErrorNumber(int osslErrorCode)
{
	_errorNumber = osslErrorCode;
}


inline const std::string& VerificationErrorArgs::errorMessage() const
{
	return _errorMessage;
}


inline void VerificationErrorArgs::setIgnoreError(bool ignoreError)
{
	_ignoreError = ignoreError;
}


inline bool VerificationErrorArgs::getIgnoreError() const
{
	return _ignoreError;
}


} } // namespace Poco::Net


#endif // NetSSL_VerificationErrorArgs_INCLUDED
