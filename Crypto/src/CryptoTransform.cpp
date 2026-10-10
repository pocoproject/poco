//
// CryptoTransform.cpp
//
// Library: Crypto
// Package: Cipher
// Module:  CryptoTransform
//
// Copyright (c) 2008, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Crypto/CryptoTransform.h"


namespace Poco::Crypto {


CryptoTransform::CryptoTransform()
{
}


CryptoTransform::~CryptoTransform()
{
}


int CryptoTransform::setPadding(int padding)
{
	return 1;
}


} // namespace Poco::Crypto
