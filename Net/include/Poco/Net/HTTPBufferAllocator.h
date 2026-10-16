//
// HTTPBufferAllocator.h
//
// Library: Net
// Package: HTTP
// Module:  HTTPBufferAllocator
//
// Definition of the HTTPBufferAllocator class.
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Net_HTTPBufferAllocator_INCLUDED
#define Net_HTTPBufferAllocator_INCLUDED


#include "Poco/Net/Net.h"
#include <ios>


namespace Poco::Net {


class Net_API HTTPBufferAllocator
	/// A BufferAllocator for HTTP streams.
{
public:
	[[nodiscard]] static char* allocate(std::streamsize size);
	static void deallocate(char* ptr, std::streamsize size) noexcept;

	enum
	{
		BUFFER_SIZE = 4096
	};
};


} // namespace Poco::Net


#endif // Net_HTTPBufferAllocator_INCLUDED
