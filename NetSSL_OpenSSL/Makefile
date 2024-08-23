#
# Makefile
#
# Makefile for Poco NetSSL_OpenSSL
#

include $(POCO_BASE)/build/rules/global
ifeq ($(findstring AIX, $(POCO_CONFIG)), AIX)
SYSLIBS += -l$(POCO_SSL_LIB) -l$(POCO_CRYPTO_LIB)
else
SYSLIBS += -l$(POCO_SSL_LIB) -l$(POCO_CRYPTO_LIB)
endif

objects = AcceptCertificateHandler RejectCertificateHandler ConsoleCertificateHandler \
	CertificateHandlerFactory CertificateHandlerFactoryMgr \
	Context HTTPSClientSession HTTPSStreamFactory HTTPSSessionInstantiator \
	InvalidCertificateHandler KeyConsoleHandler \
	KeyFileHandler PrivateKeyFactory PrivateKeyFactoryMgr \
	PrivateKeyPassphraseHandler SecureServerSocket SecureServerSocketImpl \
	SecureSocketImpl SecureStreamSocket SecureStreamSocketImpl \
	SSLException SSLManager Utility VerificationErrorArgs \
	X509Certificate Session SecureSMTPClientSession \
	FTPSClientSession FTPSStreamFactory

target         = PocoNetSSL
target_version = $(LIBVERSION)
target_libs    = PocoNet PocoCrypto PocoUtil PocoFoundation

include $(POCO_BASE)/build/rules/lib
