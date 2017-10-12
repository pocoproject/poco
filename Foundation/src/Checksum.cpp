//
// Checksum.cpp
//
// Library: Foundation
// Package: Core
// Module:  Checksum
//
// Copyright (c) 2007, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Checksum.h"
#include "Poco/Checksum32.h"
#include "Poco/Checksum64.h"


namespace Poco {


Checksum::Checksum(): _pImpl(new Checksum32)
{
}


Checksum::Checksum(Type t) :
	_pImpl(t == TYPE_CRC64 ?
		(ChecksumImpl*)new Checksum64() :
		(ChecksumImpl*)new Checksum32(static_cast<ChecksumImpl::Type>(t)))
{
}


Checksum::~Checksum()
{
	delete _pImpl;
}


} // namespace Poco
