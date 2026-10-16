//
// HTTPBufferAllocator.cpp
//
// Library: Net
// Package: HTTP
// Module:  HTTPBufferAllocator
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Net/HTTPBufferAllocator.h"
#include <cstddef>


namespace Poco::Net {


char* HTTPBufferAllocator::allocate(std::streamsize size)
{
	return new char[static_cast<std::size_t>(size)];
}


void HTTPBufferAllocator::deallocate(char* ptr, std::streamsize /*size*/) noexcept
{
	delete [] ptr;
}


} // namespace Poco::Net
