//
// BufferedBidirectionalStreamBuf.cpp
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


#include "Poco/BufferedBidirectionalStreamBuf.h"

namespace Poco {

#if defined(POCO_OS_FAMILY_WINDOWS)
template class Foundation_API BasicBufferedBidirectionalStreamBuf<char, std::char_traits<char>>;
#else
template class BasicBufferedBidirectionalStreamBuf<char, std::char_traits<char>>;
#endif

}
