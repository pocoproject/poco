//
// BinaryWriter.h
//
// Library: Foundation
// Package: Streams
// Module:  BinaryReaderWriter
//
// Definition of the BinaryWriter class.
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Foundation_BinaryWriter_INCLUDED
#define Foundation_BinaryWriter_INCLUDED


#include "Poco/Foundation.h"
#include "Poco/Buffer.h"
#include "Poco/MemoryStream.h"
#include "Poco/ByteOrder.h"
#include <vector>
#include <ostream>


namespace Poco {


class TextEncoding;
class TextConverter;


class Foundation_API BinaryWriter
	/// This class writes basic types (and std::vectors of these)
	/// in binary form into an output stream.
	/// It provides an inserter-based interface similar to ostream.
	/// The writer also supports automatic conversion from big-endian
	/// (network byte order) to little-endian and vice-versa.
	/// Use a BinaryReader to read from a stream created by a BinaryWriter.
	/// Be careful when exchanging data between systems with different
	/// data type sizes (e.g., 32-bit and 64-bit architectures), as the sizes
	/// of some of the basic types may be different. For example, writing a
	/// long integer on a 64-bit system and reading it on a 32-bit system
	/// may yield an incorrect result. Use fixed-size types (Int32, Int64, etc.)
	/// in such a case.
{
public:
	enum StreamByteOrder
	{
		NATIVE_BYTE_ORDER        = 1, /// the host's native byte-order
		BIG_ENDIAN_BYTE_ORDER    = 2, /// big-endian (network) byte-order
		NETWORK_BYTE_ORDER       = 2, /// big-endian (network) byte-order
		LITTLE_ENDIAN_BYTE_ORDER = 3  /// little-endian byte-order
	};
	
	BinaryWriter(std::ostream& ostr, StreamByteOrder order = NATIVE_BYTE_ORDER);
		/// Creates the BinaryWriter.

	BinaryWriter(std::ostream& ostr, TextEncoding& encoding, StreamByteOrder order = NATIVE_BYTE_ORDER);
		/// Creates the BinaryWriter using the given TextEncoding.
		///
		/// Strings will be converted from the currently set global encoding
		/// (see Poco::TextEncoding::global()) to the specified encoding.

	~BinaryWriter();
		/// Destroys the BinaryWriter.

	BinaryWriter& operator << (bool value);
	BinaryWriter& operator << (char value);
	BinaryWriter& operator << (unsigned char value);
	BinaryWriter& operator << (signed char value);
	BinaryWriter& operator << (short value);
	BinaryWriter& operator << (unsigned short value);
	BinaryWriter& operator << (int value);
	BinaryWriter& operator << (unsigned int value);
#ifndef POCO_LONG_IS_64_BIT
	BinaryWriter& operator << (long value);
	BinaryWriter& operator << (unsigned long value);
#endif // POCO_LONG_IS_64_BIT
	BinaryWriter& operator << (float value);
	BinaryWriter& operator << (double value);
	BinaryWriter& operator << (Int64 value);
	BinaryWriter& operator << (UInt64 value);
	BinaryWriter& operator << (const std::string& value);
	BinaryWriter& operator << (const char* value);

	template <typename T>
	BinaryWriter& operator << (const std::vector<T>& value)
	{
		Poco::UInt32 size(static_cast<Poco::UInt32>(value.size()));

		*this << size;
		for (typename std::vector<T>::const_iterator it = value.begin(); it != value.end(); ++it)
		{
			*this << *it;
		}

		return *this;
	}
	
	void write7BitEncoded(UInt32 value);
		/// Writes a 32-bit unsigned integer in a compressed format.
		/// The value is written out seven bits at a time, starting
		/// with the seven least-significant bits.
		/// The high bit of a byte indicates whether there are more bytes to be
		/// written after this one.
		/// If value will fit in seven bits, it takes only one byte of space.
		/// If value will not fit in seven bits, the high bit is set on the first byte and
		/// written out. value is then shifted by seven bits and the next byte is written.
		/// This process is repeated until the entire integer has been written.

	void write7BitEncoded(UInt64 value);
		/// Writes a 64-bit unsigned integer in a compressed format.
		/// The value written out seven bits at a time, starting
		/// with the seven least-significant bits.
		/// The high bit of a byte indicates whether there are more bytes to be
		/// written after this one.
		/// If value will fit in seven bits, it takes only one byte of space.
		/// If value will not fit in seven bits, the high bit is set on the first byte and
		/// written out. value is then shifted by seven bits and the next byte is written.
		/// This process is repeated until the entire integer has been written.

	void writeRaw(const std::string& rawData);
		/// Writes the string as-is to the stream.
		
	void writeRaw(const char* buffer, std::streamsize length);
		/// Writes length raw bytes from the given buffer to the stream.

	void writeBOM();
		/// Writes a byte-order mark to the stream. A byte order mark is
		/// a 16-bit integer with a value of 0xFEFF, written in host byte-order.
		/// A BinaryReader uses the byte-order mark to determine the byte-order
		/// of the stream.

	void flush();
		/// Flushes the underlying stream.

	bool good();
		/// Returns _ostr.good();

	bool fail();
		/// Returns _ostr.fail();

	bool bad();
		/// Returns _ostr.bad();

	std::ostream& stream() const;
		/// Returns the underlying stream.

	StreamByteOrder byteOrder() const;
		/// Returns the byte ordering used by the writer, which is
		/// either BIG_ENDIAN_BYTE_ORDER or LITTLE_ENDIAN_BYTE_ORDER.

private:

#ifdef POCO_OS_FAMILY_WINDOWS
#pragma warning(push)
#pragma warning(disable : 4800) // forcing value to bool 'true' or 'false' (performance warning)
#endif

	template<typename T>
	BinaryWriter& write(T value, bool flipBytes)
	{
		if (flipBytes)
		{
			T fValue = static_cast<T>(ByteOrder::flipBytes(value));
			_ostr.write((const char*) &fValue, sizeof(fValue));
		}
		else
		{
			_ostr.write((const char*) &value, sizeof(value));
		}
		return *this;
	}

#ifdef POCO_OS_FAMILY_WINDOWS
#pragma warning(pop)
#endif

	template<typename T>
	void write7BitEncoded(T value)
	{
		do
		{
			unsigned char c = (unsigned char) (value & 0x7F);
			value >>= 7;
			if (value) c |= 0x80;
			_ostr.write((const char*) &c, 1);
		}
		while (value);
	}

	BinaryWriter& write(const char* value, std::size_t length);

	std::ostream&  _ostr;
	bool           _flipBytes;
	TextConverter* _pTextConverter;
};


template <typename T>
class BasicMemoryBinaryWriter: public BinaryWriter
	/// A convenient wrapper for using Buffer and MemoryStream with BinarWriter.
{
public:
	BasicMemoryBinaryWriter(Buffer<T>& dataBuffer, StreamByteOrder order = NATIVE_BYTE_ORDER):
		BinaryWriter(_ostr, order),
		_data(dataBuffer),
		_ostr(dataBuffer.begin(), dataBuffer.capacity())
	{
	}

	BasicMemoryBinaryWriter(Buffer<T>& dataBuffer, TextEncoding& encoding, StreamByteOrder order = NATIVE_BYTE_ORDER):
		BinaryWriter(_ostr, encoding, order),
		_data(dataBuffer),
		_ostr(dataBuffer.begin(), dataBuffer.capacity())
	{
	}

	~BasicMemoryBinaryWriter()
	{
		try
		{
			flush();
		}
		catch (...)
		{
			poco_unexpected();
		}
	}

	Buffer<T>& data()
	{
		return _data;
	}

	const Buffer<T>& data() const
	{
		return _data;
	}

	const MemoryOutputStream& stream() const
	{
		return _ostr;
	}

	MemoryOutputStream& stream()
	{
		return _ostr;
	}

private:
	Buffer<T>& _data;
	MemoryOutputStream _ostr;
};


typedef BasicMemoryBinaryWriter<char> MemoryBinaryWriter;


//
// inlines
//


inline std::ostream& BinaryWriter::stream() const
{
	return _ostr;
}


inline bool BinaryWriter::good()
{
	return _ostr.good();
}


inline bool BinaryWriter::fail()
{
	return _ostr.fail();
}


inline bool BinaryWriter::bad()
{
	return _ostr.bad();
}


inline BinaryWriter::StreamByteOrder BinaryWriter::byteOrder() const
{
#if defined(POCO_ARCH_BIG_ENDIAN)
	return _flipBytes ? LITTLE_ENDIAN_BYTE_ORDER : BIG_ENDIAN_BYTE_ORDER;
#else
	return _flipBytes ? BIG_ENDIAN_BYTE_ORDER : LITTLE_ENDIAN_BYTE_ORDER;
#endif
}


} // namespace Poco


#endif // Foundation_BinaryWriter_INCLUDED
