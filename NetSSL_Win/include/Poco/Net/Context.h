//
// Context.h
//
// $Id: //poco/1.4/NetSSL_Schannel/include/Poco/Net/Context.h#1 $
//
// Library: NetSSL_Schannel
// Package: SSLCore
// Module:  Context
//
// Definition of the Context class.
//
// Copyright (c) 2006-2014, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef NetSSL_Context_INCLUDED
#define NetSSL_Context_INCLUDED


#include "Poco/Net/NetSSL.h"
#include "Poco/Net/X509Certificate.h"
#include "Poco/RefCountedObject.h"
#include "Poco/AutoPtr.h"
#include "Poco/Mutex.h"
#include <vector>
#include <windows.h>
#include <wincrypt.h>
#include <schannel.h>


namespace Poco {
namespace Net {


class NetSSL_Win_API Context: public Poco::RefCountedObject
	/// This class encapsulates context information for
	/// an SSL server or client, such as the certificate
	/// verification mode and which certificates and
	/// protocols to use.
	///
	/// The Context class is also used to control
	/// SSL session caching on the server and client side.
{
public:
	typedef Poco::AutoPtr<Context> Ptr;

	enum Usage
	{
		CLIENT_USE, 	    /// Context is used by a client.
		SERVER_USE,         /// Context is used by a server.
		TLSV1_CLIENT_USE,   /// Context is used by a client requiring TLSv1.
		TLSV1_SERVER_USE,   /// Context is used by a server requiring TLSv1.
		TLSV1_1_CLIENT_USE, /// Context is used by a client requiring TLSv1.1. Not supported on Windows Embedded Compact.
		TLSV1_1_SERVER_USE, /// Context is used by a server requiring TLSv1.1. Not supported on Windows Embedded Compact.
		TLSV1_2_CLIENT_USE, /// Context is used by a client requiring TLSv1.2. Not supported on Windows Embedded Compact.
		TLSV1_2_SERVER_USE  /// Context is used by a server requiring TLSv1.2. Not supported on Windows Embedded Compact.
	};

	enum VerificationMode 
	{
		VERIFY_NONE    = 0, 
			/// Server: The server will not send a client certificate 
			/// request to the client, so the client will not send a certificate. 
			///
			/// Client: If not using an anonymous cipher (by default disabled), 
			/// the server will send a certificate which will be checked, but
			/// the result of the check will be ignored.

		VERIFY_RELAXED = 1, 
			/// Server: The server sends a client certificate request to the 
			/// client. The certificate returned (if any) is checked. 
			/// If the verification process fails, the TLS/SSL handshake is 
			/// immediately terminated with an alert message containing the 
			/// reason for the verification failure. 
			///
			/// Client: The server certificate is verified, if one is provided. 
			/// If the verification process fails, the TLS/SSL handshake is
			/// immediately terminated with an alert message containing the 
			/// reason for the verification failure. 

		VERIFY_STRICT  = 2,
			/// Server: If the client did not return a certificate, the TLS/SSL 
			/// handshake is immediately terminated with a handshake failure
			/// alert. 
			///
			/// Client: Same as VERIFY_RELAXED. 

		VERIFY_ONCE    = 3
			/// Server: Only request a client certificate on the initial 
			/// TLS/SSL handshake. Do not ask for a client certificate 
			/// again in case of a renegotiation.
			///
			/// Client: Same as VERIFY_RELAXED.	
	};

	enum Options
	{
		OPT_PERFORM_REVOCATION_CHECK    = 0x01,
			/// Check certificates against revocation list. Not supported (ignored) on Windows Embedded Compact.
		OPT_TRUST_ROOTS_WIN_CERT_STORE  = 0x02,
			/// Trust root certificates from Windows root certificate store.
		OPT_USE_MACHINE_STORE           = 0x04,
			/// If specified, the windows machine certificate store is used (server only).
			/// Otherwise, the user's certificate store is used.
		OPT_USE_STRONG_CRYPTO           = 0x08,
			/// Disable known weak cryptographic algorithms, cipher suites, and 
			/// SSL/TLS protocol versions that may be otherwise enabled for better interoperability. 
		OPT_DEFAULTS                    = OPT_PERFORM_REVOCATION_CHECK | OPT_TRUST_ROOTS_WIN_CERT_STORE | OPT_USE_STRONG_CRYPTO
	};

	Context(Usage usage,
		const std::string& certificateName, 
		VerificationMode verMode = VERIFY_RELAXED,
		int options = OPT_DEFAULTS,
		const std::string& certificateStoreName = CERT_STORE_MY);
			/// Creates a Context.
			/// 
			///   * usage specifies whether the context is used by a client or server,
			///     as well as which protocol to use.
			///   * certificateName specifies the subject name of the certificate to use.
			///   * options is a combination of Option flags.
			///   * certificateStoreName specifies the name of the Windows certificate store
			///   * to use for loading the certificate. You can use predefined constants
			///     CERT_STORE_MY, CERT_STORE_ROOT, etc.
			///   * verificationMode specifies whether and how peer certificates are validated.
			///
			/// Note: If the private key is protected by a passphrase, a PrivateKeyPassphraseHandler
			/// must have been setup with the SSLManager, or the SSLManager's PrivateKeyPassphraseRequired
			/// event must be handled.

	~Context();
		/// Destroys the Context.

	VerificationMode verificationMode() const;
		/// Returns the certificate verification mode.

	Usage usage() const;
		/// Returns whether the context is for use by a client or by a server
		/// and whether TLSv1.x is required.

	bool isForServerUse() const;
		/// Returns true iff the context is for use by a server.

	bool sessionCacheEnabled() const;
		/// Returns true iff the session cache is enabled.

	int options() const;
		/// Returns the options flags.

	const std::string& certificateName() const;
		/// Returns the name of the certificate to use.

	const std::string& certificateStoreName() const;
		/// Returns the name of the certificate store to use.

	void addTrustedCert(const Poco::Net::X509Certificate& cert);
		/// Adds the certificate to the trusted certs. Takes ownership of pCert.

	HCERTSTORE certificateStore() const;
		/// Returns a handle to the certificate store.

	static const std::string CERT_STORE_MY;
	static const std::string CERT_STORE_ROOT;
	static const std::string CERT_STORE_TRUST;
	static const std::string CERT_STORE_CA;
	static const std::string CERT_STORE_USERDS;

private:
	Context(const Context&);
	Context& operator=(const Context&);

	Usage                      _usage;
	Context::VerificationMode  _mode;
	int                        _options;
	std::string                _certificateName;
	std::string                _certificateStoreName;
	HCERTSTORE                 _hMemCertStore;
	HCERTSTORE                 _hCollectionCertStore;
	HCERTSTORE                 _hRootCertStore;
	Poco::FastMutex            _mutex;
};


//
// inlines
//
inline Context::VerificationMode Context::verificationMode() const
{
	return _mode;
}


inline Context::Usage Context::usage() const
{
	return _usage;
}


inline int Context::options() const
{
	return _options;
}


inline bool Context::isForServerUse() const
{
	return _usage == SERVER_USE
		|| _usage == TLSV1_SERVER_USE
		|| _usage == TLSV1_1_SERVER_USE
		|| _usage == TLSV1_2_SERVER_USE;
}


inline bool Context::sessionCacheEnabled() const
{
	return false;
}


inline const std::string& Context::certificateName() const
{
	return _certificateName;
}


inline const std::string& Context::certificateStoreName() const
{
	return _certificateStoreName;
}


inline HCERTSTORE Context::certificateStore() const
{
	return _hCollectionCertStore;
}


} } // namespace Poco::Net


#endif // NetSSL_Context_INCLUDED
