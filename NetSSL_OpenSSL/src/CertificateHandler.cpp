//
// CertificateHandler.cpp
//
// Library: NetSSL_OpenSSL
// Package: SSLCore
// Module:  CertificateHandler
//
// Copyright (c) 2006-2009, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Net/CertificateHandler.h"
#include "Poco/Net/SSLManager.h"
#include "Poco/Delegate.h"


using Poco::Delegate;


namespace Poco {
namespace Net {


CertificateHandler::CertificateHandler(bool handleErrorsOnServerSide): _handleErrorsOnServerSide(handleErrorsOnServerSide)
{
	if (_handleErrorsOnServerSide)
	{
		SSLManager::instance().ServerVerification += Delegate<CertificateHandler, VerificationArgs>(this, &CertificateHandler::onValidCertificate);
		SSLManager::instance().ServerVerificationError += Delegate<InvalidCertificateHandler, VerificationErrorArgs>(this, &InvalidCertificateHandler::onInvalidCertificate);
	}
	else
	{
		SSLManager::instance().ClientVerification += Delegate<CertificateHandler, VerificationArgs>(this, &CertificateHandler::onValidCertificate);
		SSLManager::instance().ClientVerificationError += Delegate<InvalidCertificateHandler, VerificationErrorArgs>(this, &InvalidCertificateHandler::onInvalidCertificate);
	}
}


CertificateHandler::~CertificateHandler()
{
	try
	{
		if (_handleErrorsOnServerSide)
		{
			SSLManager::instance().ServerVerification -= Delegate<CertificateHandler, VerificationArgs>(this, &CertificateHandler::onValidCertificate);
			SSLManager::instance().ServerVerificationError -= Delegate<InvalidCertificateHandler, VerificationErrorArgs>(this, &InvalidCertificateHandler::onInvalidCertificate);
		}
		else
		{
			SSLManager::instance().ClientVerification -= Delegate<CertificateHandler, VerificationArgs>(this, &CertificateHandler::onValidCertificate);
			SSLManager::instance().ClientVerificationError -= Delegate<InvalidCertificateHandler, VerificationErrorArgs>(this, &InvalidCertificateHandler::onInvalidCertificate);
		}
	}
	catch (...)
	{
		poco_unexpected();
	}
}


} } // namespace Poco::Net
