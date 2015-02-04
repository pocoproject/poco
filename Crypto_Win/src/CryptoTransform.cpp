//
// CryptoTransform.cpp
//
// $Id$
//
// Library: Crypto_Win
// Package: Cipher
// Module:  CryptoTransform
//
// Copyright (c) 2006-2014, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Crypto/CryptoTransform.h"


namespace Poco {
namespace Crypto {


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


} } // namespace Poco::Crypto
