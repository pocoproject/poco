About NetSSL_Win
================

NetSSL_Win is an implementation of the POCO NetSSL library based on Windows 
Schannel. The original NetSSL implementation is based on OpenSSL and thus
has a few OpenSSL-isms in the interface. Generally, source code based
on NetSSL_OpenSSL can be easily ported to use NetSSL_Win. Just a few
minor code changes are required, due to differences in the API. 
These are discussed below:

  - Context: The Context constructor uses different arguments. While
    the first argument is the same, all others are different.
    Instead of a certificate file name, a certificate subject name is
    specified. Certificates are always loaded from a Windows
    certificate store, not from the filesystem.
    Please refer to the header file documentation for more information.
    Furthermore, the following Context methods are not available in NetSSL_Win:
    addChainCertificate(), disableStatelessSessionResumption(), 
    enableExtendedCertificateVerification(), enableSessionCache(),
    extendedCertificateVerificationEnabled(), flushSessionCache(),
    getSessionCacheSize(), getSessionTimeout(), sessionCacheEnabled(),
    setSessionCacheSize(), setSessionTimeout(), sslContext(),
    useCertificate(), usePrivateKey().

  - SSLManager: The configuration properties used to configure the SSLManager
    are different from the ones used in NetSSL_OpenSSL. Please see the
    SSLManager header file for more information. 
    Furthermore, PrivateKeyPassphraseHandler's are not supported. The
    initializeClient() and initializeServer() methods therefore do not
    have the PrivateKeyPassphraseHandler argument.
    The isFIPSEnabled() method is not available.

  - X509Certificate: Loading a certificate from a file or stream, as well as saving
    a certificate is not supported.


Certificate and Certificate Stores
----------------------------------

The test suite and samples expect a certificate with a subject name
same as the host name to be available from the user's personal certificate
store. A suitable self-signed certificate can be created using the Windows 
IIS administration console, then exported to a file, and re-imported into 
the user's personal store.

To create the certificate, follow these steps:

  1. Launch the Computer Management utility.
  2. Go to Services and Applications > Internet Information Server.
  3. Open the Server Certificates feature.
  4. Under Actions, click "Create Self-Signed Certificate...".
  5. Enter a user-friendly name for the certificate.
     The certificate will be issued to the name of your computer.
  6. Export the certificate by right-clicking it and chosing "Export...".
  7. In Explorer, double-click the exported certificate file (.pfx)
     to launch the Certificate Import Wizard.
  8. Import the certificate into your personal store by 
     selecting "Place all certificates in the following store" and
     selecting the "Personal" store in the import wizard.


Windows Embedded Compact
------------------------

Windows Embedded Compact does not support the following Schannel
features: 
  - certificate revocation checking (Context::OPT_PERFORM_REVOCATION_CHECK)
  - TLS 1.1 and 1.2 (Context::TLSV1_1_CLIENT_USE, etc.)
  - disabling weak security algorithms (Context::OPT_USE_STRONG_CRYPTO)
