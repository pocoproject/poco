//
// UnbufferedStreamBuf.cpp
//
// Library: Foundation
// Package: Streams
// Module:  StreamBuf
//
// Copyright (c) 2025, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/UnbufferedStreamBuf.h"

namespace Poco {

#if defined(POCO_OS_FAMILY_WINDOWS)
template class Foundation_API BasicUnbufferedStreamBuf<char, std::char_traits<char>>;
#else
template class BasicUnbufferedStreamBuf<char, std::char_traits<char>>;
#endif

}
