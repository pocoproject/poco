//
// CryptoException.cpp
//
//
// Library: Crypto
// Package: Crypto
// Module:  CryptoException
//
// Copyright (c) 2012, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Crypto/CryptoException.h"
#include <typeinfo>


namespace Poco::Crypto {


POCO_IMPLEMENT_EXCEPTION(CryptoException, Exception, "Crypto Exception")


OpenSSLException::OpenSSLException(int otherCode): CryptoException(otherCode)
{
	setExtMessage();
}


OpenSSLException::OpenSSLException(const std::string& msg, int otherCode): CryptoException(msg, otherCode)
{
	setExtMessage();
}


OpenSSLException::OpenSSLException(const std::string& msg, const std::string& arg, int otherCode): CryptoException(msg, arg, otherCode)
{
	setExtMessage();
}


OpenSSLException::OpenSSLException(const std::string& msg, const Poco::Exception& exc, int otherCode): CryptoException(msg, exc, otherCode)
{
	setExtMessage();
}


OpenSSLException::OpenSSLException(const OpenSSLException& exc): CryptoException(exc)
{
}


OpenSSLException::~OpenSSLException() noexcept
{
}


OpenSSLException& OpenSSLException::operator = (const OpenSSLException& exc)
{
	CryptoException::operator = (exc);
	return *this;
}


const char* OpenSSLException::name() const noexcept
{
	return "OpenSSLException";
}


const char* OpenSSLException::className() const noexcept
{
	return typeid(*this).name();
}


Poco::Exception* OpenSSLException::clone() const
{
	return new OpenSSLException(*this);
}


void OpenSSLException::setExtMessage()
{
	std::string msg;
	extendedMessage(getError(msg));
}


void OpenSSLException::rethrow() const
{
	throw *this;
}


} // namespace Poco::Crypto
