//
// X509Certificate.cpp
//
// Library: NetSSL_OpenSSL
// Package: SSLCore
// Module:  X509Certificate
//
// Copyright (c) 2006-2009, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Net/X509Certificate.h"
#include "Poco/Net/SSLException.h"
#include "Poco/Net/SSLManager.h"
#include "Poco/Net/IPAddress.h"
#include "Poco/TemporaryFile.h"
#include "Poco/FileStream.h"
#include "Poco/StreamCopier.h"
#include "Poco/String.h"
#include "Poco/RegularExpression.h"
#include "Poco/DateTimeParser.h"
#include <openssl/pem.h>
#include <openssl/x509v3.h>
#include <openssl/x509_vfy.h>
#include <openssl/err.h>
#include <memory>


namespace Poco::Net {


X509Certificate::X509Certificate(std::istream& istr):
	Poco::Crypto::X509Certificate(istr)
{
}


X509Certificate::X509Certificate(const std::string& path):
	Poco::Crypto::X509Certificate(path)
{
}


X509Certificate::X509Certificate(X509* pCert):
	Poco::Crypto::X509Certificate(pCert)
{
}


X509Certificate::X509Certificate(X509* pCert, bool shared):
	Poco::Crypto::X509Certificate(pCert, shared)
{
}


X509Certificate::X509Certificate(const Poco::Crypto::X509Certificate& cert):
	Poco::Crypto::X509Certificate(cert)
{
}


X509Certificate::X509Certificate(const X509Certificate& cert):
	Poco::Crypto::X509Certificate(cert)
{
}


X509Certificate::X509Certificate(X509Certificate&& cert) noexcept:
	Poco::Crypto::X509Certificate(std::move(cert))
{
}


X509Certificate& X509Certificate::operator = (const Poco::Crypto::X509Certificate& cert)
{
	X509Certificate tmp(cert);
	swap(tmp);
	return *this;
}


X509Certificate& X509Certificate::operator = (const X509Certificate& cert)
{
	X509Certificate tmp(cert);
	swap(tmp);
	return *this;
}


X509Certificate& X509Certificate::operator = (X509Certificate&& cert) noexcept
{
	Poco::Crypto::X509Certificate::operator = (std::move(cert));
	return *this;
}


X509Certificate::~X509Certificate()
{
}


bool X509Certificate::verify(const std::string& hostName) const
{
	return verify(*this, hostName);
}


bool X509Certificate::verify(const Poco::Crypto::X509Certificate& certificate, const std::string& hostName)
{
	if (hostName.empty()) return false;

	X509* pCert = const_cast<X509*>(certificate.certificate());
	std::unique_ptr<X509_STORE, decltype(&X509_STORE_free)> pStore(X509_STORE_new(), &X509_STORE_free);
	std::unique_ptr<X509_STORE_CTX, decltype(&X509_STORE_CTX_free)> pStoreContext(X509_STORE_CTX_new(), &X509_STORE_CTX_free);
	if (pStore == nullptr || pStoreContext == nullptr) return false;

	// The certificate trusts itself and its validity period is ignored, so that neither its issuer nor the time affects the result.
	if (X509_STORE_add_cert(pStore.get(), pCert) != 1) return false;
	X509_VERIFY_PARAM* pParam = X509_STORE_get0_param(pStore.get());
	X509_VERIFY_PARAM_set_flags(pParam, X509_V_FLAG_PARTIAL_CHAIN | X509_V_FLAG_NO_CHECK_TIME);
	X509_VERIFY_PARAM_set_hostflags(pParam, X509_CHECK_FLAG_NO_PARTIAL_WILDCARDS | X509_CHECK_FLAG_NEVER_CHECK_SUBJECT);
	IPAddress ip;
	// X509_VERIFY_PARAM_set1_host() would match an address as text against the DNS names of the certificate.
	// The address is passed as POCO parsed it, because the text parser of OpenSSL may read the text differently.
	const int nameSet = IPAddress::tryParse(hostName, ip)
		? X509_VERIFY_PARAM_set1_ip(pParam, static_cast<const unsigned char*>(ip.addr()), ip.length())
		: X509_VERIFY_PARAM_set1_host(pParam, hostName.data(), hostName.length());
	if (nameSet != 1) return false;

	if (X509_STORE_CTX_init(pStoreContext.get(), pStore.get(), pCert, nullptr) != 1) return false;
	X509_STORE_CTX_set_verify_cb(pStoreContext.get(), [](int ok, X509_STORE_CTX* pContext)
	{
		const int error = X509_STORE_CTX_get_error(pContext);
		return (error == X509_V_ERR_HOSTNAME_MISMATCH || error == X509_V_ERR_IP_ADDRESS_MISMATCH) ? ok : 1;
	});
	// A failed verification is reported through the result, the errors it queued are not needed.
	ERR_set_mark();
	const int rc = X509_verify_cert(pStoreContext.get());
	ERR_pop_to_mark();
	return rc == 1;
}


bool X509Certificate::containsWildcards(const std::string& commonName)
{
	return (commonName.find('*') != std::string::npos || commonName.find('?') != std::string::npos);
}


bool X509Certificate::matchWildcard(const std::string& wildcard, const std::string& hostName)
{
	// fix wildcards
	std::string wildcardExpr("^");
	wildcardExpr += Poco::replace(wildcard, ".", "\\.");
	Poco::replaceInPlace(wildcardExpr, "*", ".*");
	Poco::replaceInPlace(wildcardExpr, "..*", ".*");
	Poco::replaceInPlace(wildcardExpr, "?", ".?");
	Poco::replaceInPlace(wildcardExpr, "..?", ".?");
	wildcardExpr += "$";

	Poco::RegularExpression expr(wildcardExpr, Poco::RegularExpression::RE_CASELESS);
	return expr.match(hostName);
}


} // namespace Poco::Net
