//
// StreamCopier.cpp
//
// Library: Foundation
// Package: Streams
// Module:  StreamCopier
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/StreamCopier.h"


namespace Poco {


std::streamsize StreamCopier::copyStream(std::istream& istr, std::ostream& ostr, std::size_t bufferSize)
{
	return copyStreamImpl<std::streamsize>(istr, ostr, bufferSize);
}


#if defined(POCO_HAVE_INT64)
Poco::UInt64 StreamCopier::copyStream64(std::istream& istr, std::ostream& ostr, std::size_t bufferSize)
{
	return copyStreamImpl<Poco::UInt64>(istr, ostr, bufferSize);
}
#endif


std::streamsize StreamCopier::copyStreamRange(std::istream& istr, std::ostream& ostr, std::streampos rangeStart, std::streamsize rangeLength, std::size_t bufferSize)
{
	return copyStreamRangeImpl<std::streamsize>(istr, ostr, rangeStart, rangeLength, bufferSize);
}


#if defined(POCO_HAVE_INT64)
Poco::UInt64 StreamCopier::copyStreamRange64(std::istream& istr, std::ostream& ostr, std::streampos rangeStart, std::streamsize rangeLength, std::size_t bufferSize)
{
	return copyStreamRangeImpl<Poco::UInt64>(istr, ostr, rangeStart, rangeLength, bufferSize);
}
#endif


std::streamsize StreamCopier::copyToString(std::istream& istr, std::string& str, std::size_t bufferSize)
{
	return copyToStringImpl<std::streamsize>(istr, str, bufferSize);
}


#if defined(POCO_HAVE_INT64)
Poco::UInt64 StreamCopier::copyToString64(std::istream& istr, std::string& str, std::size_t bufferSize)
{
	return copyToStringImpl<Poco::UInt64>(istr, str, bufferSize);
}
#endif


std::streamsize StreamCopier::copyStreamUnbuffered(std::istream& istr, std::ostream& ostr)
{
	return copyStreamUnbufferedImpl<std::streamsize>(istr, ostr);
}


#if defined(POCO_HAVE_INT64)
Poco::UInt64 StreamCopier::copyStreamUnbuffered64(std::istream& istr, std::ostream& ostr)
{
	return copyStreamUnbufferedImpl<Poco::UInt64>(istr, ostr);
}
#endif

std::streamsize StreamCopier::copyStreamRangeUnbuffered(std::istream& istr, std::ostream& ostr, std::streampos rangeStart, std::streamsize rangeLength)
{
	return copyStreamRangeUnbufferedImpl<std::streamsize>(istr, ostr, rangeStart, rangeLength);
}


#if defined(POCO_HAVE_INT64)
Poco::UInt64 StreamCopier::copyStreamRangeUnbuffered64(std::istream& istr, std::ostream& ostr, std::streampos rangeStart, std::streamsize rangeLength)
{
	return copyStreamRangeUnbufferedImpl<Poco::UInt64>(istr, ostr, rangeStart, rangeLength);
}
#endif


} // namespace Poco
