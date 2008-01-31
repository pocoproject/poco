#
# Makefile
#
# $Id: //poco/Main/template/library.make#2 $
#
# Makefile for Poco NetSSL_OpenSSL
#

include $(POCO_BASE)/build/rules/global

SYSLIBS += -lssl -lcrypto

objects = AcceptCertificateHandler CertificateHandlerFactory \
	CertificateHandlerFactoryMgr ConsoleCertificateHandler \
	Context HTTPSClientSession HTTPSStreamFactory HTTPSSessionInstantiator \
	SSLInitializer InvalidCertificateHandler KeyConsoleHandler \
	KeyFileHandler PrivateKeyFactory PrivateKeyFactoryMgr \
	PrivateKeyPassphraseHandler SecureServerSocket SecureServerSocketImpl \
	SecureSocketImpl SecureStreamSocket SecureStreamSocketImpl \
	SSLException SSLManager Utility VerificationErrorArgs \
	X509Certificate

target         = PocoNetSSL
target_version = $(LIBVERSION)
target_libs    = PocoFoundation PocoNet PocoUtil

include $(POCO_BASE)/build/rules/lib
