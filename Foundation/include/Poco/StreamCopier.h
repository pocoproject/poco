//
// StreamCopier.h
//
// Library: Foundation
// Package: Streams
// Module:  StreamCopier
//
// Definition of class StreamCopier.
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Foundation_StreamCopier_INCLUDED
#define Foundation_StreamCopier_INCLUDED


#include "Poco/Foundation.h"
#include "Poco/Buffer.h"
#include <istream>
#include <ostream>
#include <cstddef>


namespace Poco {


class Foundation_API StreamCopier
	/// This class provides static methods to copy the contents from one stream
	/// into another.
{
public:
	static std::streamsize copyStream(std::istream& istr, std::ostream& ostr, std::size_t bufferSize = 8192);
		/// Writes all bytes readable from istr to ostr, using an internal buffer.
		///
		/// Returns the number of bytes copied.

#if defined(POCO_HAVE_INT64)
	static Poco::UInt64 copyStream64(std::istream& istr, std::ostream& ostr, std::size_t bufferSize = 8192);
		/// Writes all bytes readable from istr to ostr, using an internal buffer.
		///
		/// Returns the number of bytes copied as a 64-bit unsigned integer.
		///
		/// Note: the only difference to copyStream() is that a 64-bit unsigned
		/// integer is used to count the number of bytes copied.
#endif

	static std::streamsize copyStreamRange(std::istream& istr, std::ostream& ostr, std::streampos rangeStart, std::streamsize rangeLength, std::size_t bufferSize = 8192);
		/// Writes range of bytes readable from istr to ostr, using an internal buffer.
		///
		/// Returns the number of bytes copied.

#if defined(POCO_HAVE_INT64)
	static Poco::UInt64 copyStreamRange64(std::istream& istr, std::ostream& ostr, std::streampos rangeStart, std::streamsize rangeLength, std::size_t bufferSize = 8192);
		/// Writes range of bytes readable from istr to ostr, using an internal buffer.
		///
		/// Returns the number of bytes copied as a 64-bit unsigned integer.
		///
		/// Note: the only difference to copyStreamRange() is that a 64-bit unsigned
		/// integer is used to count the number of bytes copied.
#endif

	static std::streamsize copyStreamUnbuffered(std::istream& istr, std::ostream& ostr);
		/// Writes all bytes readable from istr to ostr.
		///
		/// Returns the number of bytes copied.

#if defined(POCO_HAVE_INT64)
	static Poco::UInt64 copyStreamUnbuffered64(std::istream& istr, std::ostream& ostr);
		/// Writes all bytes readable from istr to ostr.
		///
		/// Returns the number of bytes copied as a 64-bit unsigned integer.
		///
		/// Note: the only difference to copyStreamUnbuffered() is that a 64-bit unsigned
		/// integer is used to count the number of bytes copied.
#endif

static std::streamsize copyStreamRangeUnbuffered(std::istream& istr, std::ostream& ostr, std::streampos rangeStart, std::streamsize rangeLength);
		/// Writes range of bytes readable from istr to ostr.
		///
		/// Returns the number of bytes copied.

#if defined(POCO_HAVE_INT64)
	static Poco::UInt64 copyStreamRangeUnbuffered64(std::istream& istr, std::ostream& ostr, std::streampos rangeStart, std::streamsize rangeLength);
		/// Writes range of bytes readable from istr to ostr.
		///
		/// Returns the number of bytes copied as a 64-bit unsigned integer.
		///
		/// Note: the only difference to copyStreamRangeUnbuffered() is that a 64-bit unsigned
		/// integer is used to count the number of bytes copied.
#endif

	static std::streamsize copyToString(std::istream& istr, std::string& str, std::size_t bufferSize = 8192);
		/// Appends all bytes readable from istr to the given string, using an internal buffer.
		///
		/// Returns the number of bytes copied.

#if defined(POCO_HAVE_INT64)
	static Poco::UInt64 copyToString64(std::istream& istr, std::string& str, std::size_t bufferSize = 8192);
		/// Appends all bytes readable from istr to the given string, using an internal buffer.
		///
		/// Returns the number of bytes copied as a 64-bit unsigned integer.
		///
		/// Note: the only difference to copyToString() is that a 64-bit unsigned
		/// integer is used to count the number of bytes copied.
#endif

private:
	template <typename T>
	static T copyStreamImpl(std::istream& istr, std::ostream& ostr, std::size_t bufferSize)
	{
		poco_assert (bufferSize > 0);

		Buffer<char> buffer(bufferSize);
		T len = 0;
		istr.read(buffer.begin(), bufferSize);
		std::streamsize n = istr.gcount();
		while (n > 0)
		{
			len += n;
			ostr.write(buffer.begin(), n);
			if (istr && ostr)
			{
				istr.read(buffer.begin(), bufferSize);
				n = istr.gcount();
			}
			else n = 0;
		}
		return len;
	}

	template <typename T>
	static T copyStreamRangeImpl(std::istream& istr, std::ostream& ostr, std::streampos rangeStart, std::streamsize rangeLength, std::size_t bufferSize)
	{
		poco_assert (bufferSize > 0);

		if (bufferSize > rangeLength)
			bufferSize = rangeLength;

		Buffer<char> buffer(bufferSize);
		T len = 0;
		if (istr)
		{
			istr.seekg(rangeStart);
			istr.read(buffer.begin(), bufferSize);
			std::streamsize n = istr.gcount();
			while (n > 0)
			{
				len += n;
				ostr.write(buffer.begin(), n);
				if ((len < rangeLength) && istr && ostr)
				{
					if (bufferSize > (rangeLength - len))
						bufferSize = rangeLength - len;
					istr.read(buffer.begin(), bufferSize);
					n = istr.gcount();
				}
				else n = 0;
			}
		}
		return len;
	}

	template <typename T>
	static T copyToStringImpl(std::istream& istr, std::string& str, std::size_t bufferSize)
	{
		poco_assert (bufferSize > 0);

		Buffer<char> buffer(bufferSize);
		T len = 0;
		istr.read(buffer.begin(), bufferSize);
		std::streamsize n = istr.gcount();
		while (n > 0)
		{
			len += n;
			str.append(buffer.begin(), static_cast<std::string::size_type>(n));
			if (istr)
			{
				istr.read(buffer.begin(), bufferSize);
				n = istr.gcount();
			}
			else n = 0;
		}
		return len;
	}

	template <typename T>
	static T copyStreamUnbufferedImpl(std::istream& istr, std::ostream& ostr)
	{
		char c = 0;
		T len = 0;
		istr.get(c);
		while (istr && ostr)
		{
			++len;
			ostr.put(c);
			istr.get(c);
		}
		return len;
	}

	template <typename T>
	static T copyStreamRangeUnbufferedImpl(std::istream& istr, std::ostream& ostr, std::streampos rangeStart, std::streamsize rangeLength)
	{
		T len = 0;
		char c = 0;
		if (istr)
		{
			istr.seekg(rangeStart);
			istr.get(c);
			while (istr && ostr && (len < rangeLength))
			{
				ostr.put(c);
				++len;
				istr.get(c);
			}
		}
		return len;
	}
};


} // namespace Poco


#endif // Foundation_StreamCopier_INCLUDED
