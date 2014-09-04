//
// Context.cpp
//
// $Id: //poco/1.4/NetSSL_Schannel/src/Context.cpp#1 $
//
// Library: NetSSL_Schannel
// Package: SSLCore
// Module:  Context
//
// Copyright (c) 2006-2014, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Net/Context.h"
#include "Poco/Net/SSLManager.h"
#include "Poco/Net/SSLException.h"
#include "Poco/UnicodeConverter.h"


namespace Poco {
namespace Net {


const std::string Context::CERT_STORE_MY("MY");
const std::string Context::CERT_STORE_ROOT("ROOT");
const std::string Context::CERT_STORE_TRUST("TRUST");
const std::string Context::CERT_STORE_CA("CA");
const std::string Context::CERT_STORE_USERDS("USERDS");


Context::Context(Usage usage,
		const std::string& certName, 
		VerificationMode verMode,
		int options,
		const std::string& certStore):
	_usage(usage),
	_mode(verMode),
	_options(options),
	_certificateName(certName),
	_certificateStoreName(certStore),
	_hMemCertStore(0),
	_hCollectionCertStore(0),
	_hRootCertStore(0),
	_mutex()
{
	_hMemCertStore = CertOpenStore(
					CERT_STORE_PROV_MEMORY,   // The memory provider type
					0,                        // The encoding type is not needed
					NULL,                     // Use the default provider
					0,                        // Accept the default dwFlags
					NULL);                    // pvPara is not used

	if (!_hMemCertStore)
		throw SSLException("Failed to create memory certificate store", GetLastError());

	_hCollectionCertStore = CertOpenStore(
			CERT_STORE_PROV_COLLECTION, // A collection store
			0,                          // Encoding type; not used with a collection store
			NULL,                       // Use the default provider
			0,                          // No flags
			NULL);                      // Not needed

	if (!_hCollectionCertStore)
		throw SSLException("Failed to create collection store", GetLastError());

	if (!CertAddStoreToCollection(_hCollectionCertStore, _hMemCertStore, CERT_PHYSICAL_STORE_ADD_ENABLE_FLAG, 1))
		throw SSLException("Failed to add memory certificate store to collection store", GetLastError());

	if (_options & OPT_TRUST_ROOTS_WIN_CERT_STORE)
	{
		// add root certificates
		std::wstring rootStore;
		Poco::UnicodeConverter::convert(CERT_STORE_ROOT, rootStore);
		_hRootCertStore = CertOpenSystemStoreW(0, rootStore.c_str());
		if (!_hRootCertStore)
			throw SSLException("Failed to open root certificate store", GetLastError());
		if (!CertAddStoreToCollection(_hCollectionCertStore, _hRootCertStore, CERT_PHYSICAL_STORE_ADD_ENABLE_FLAG, 1))
			throw SSLException("Failed to add root certificate store to collection store", GetLastError());
	}
}


Context::~Context()
{
	CertCloseStore(_hCollectionCertStore, 0);
	CertCloseStore(_hMemCertStore, 0);
	if (_hRootCertStore)
	{
		CertCloseStore(_hRootCertStore, 0);
	}
}


void Context::addTrustedCert(const Poco::Net::X509Certificate& cert)
{
	Poco::FastMutex::ScopedLock lock(_mutex);
	CertAddCertificateContextToStore(_hMemCertStore, cert.system(), CERT_STORE_ADD_REPLACE_EXISTING, 0);
}


} } // namespace Poco::Net
