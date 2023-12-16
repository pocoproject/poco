//
// KeyPairImpl.cpp
//
//
// Library: Crypto
// Package: CryptoCore
// Module:  KeyPairImpl
//
// Copyright (c) 2008, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include <utility>

#include "Poco/Crypto/KeyPairImpl.h"


namespace Poco {
namespace Crypto {


KeyPairImpl::KeyPairImpl(std::string  name, Type type):
	_name(std::move(name)),
	_type(type)
{
}


KeyPairImpl::~KeyPairImpl()
{
}


} } // namespace Poco::Crypto
