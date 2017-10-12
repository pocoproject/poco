//
// ChecksumImpl.h
//
// Library: Foundation
// Package: Core
// Module:  ChecksumImpl
//
// Definition of the ChecksumImpl class.
//
// Copyright (c) 2007, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Foundation_ChecksumImpl_INCLUDED
#define Foundation_ChecksumImpl_INCLUDED


#include "Poco/Foundation.h"


namespace Poco {


class ChecksumImpl
{
public:
	enum Type
	{
		TYPE_ADLER32_IMPL = 0,
		TYPE_CRC32_IMPL,
		TYPE_CRC64_IMPL
	};

	virtual ~ChecksumImpl() {}
	virtual void update(const char* data, unsigned length) = 0;
	virtual void update(const std::string& data) = 0;
	virtual void update(char data) = 0;
	virtual Poco::UInt64 checksum() const = 0;
	virtual Type type() const = 0;
};


} // namespace Poco


#endif // Foundation_ChecksumImpl_INCLUDED
