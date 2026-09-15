//
// BSONReader.h
//
// Library: MongoDB
// Package: MongoDB
// Module:  BSONReader
//
// Definition of the BSONReader class.
//
// Copyright (c) 2012-2025, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef MongoDB_BSONReader_INCLUDED
#define MongoDB_BSONReader_INCLUDED


#include "Poco/MongoDB/MongoDB.h"
#include "Poco/BinaryReader.h"
#include "Poco/Exception.h"
#include "Poco/SharedPtr.h"
#include <streambuf>
#include <string>


namespace Poco {
namespace MongoDB {


class Binary;


class MongoDB_API BSONReader
	/// Class for reading BSON using a Poco::BinaryReader.
	///
	/// The readers check only what keeps a malformed document from reading past
	/// its bounds, allocating without limit or recursing too deeply (see
	/// Document::read). The available argument holds the bytes left in the
	/// enclosing document and is decreased by what is read.
{
public:
	BSONReader(const Poco::BinaryReader& reader):
		_reader(reader)
		/// Creates the BSONReader using the given BinaryWriter.
	{
	}

	virtual ~BSONReader()
		/// Destroys the BSONReader.
	{
	}

	template<typename T>
	void read(T& t)
		/// Reads the value from the reader. The default implementation uses the >> operator to
		/// the given argument. Special types can write their own version.
	{
		_reader >> t;
	}

	template<typename T>
	void readFixed(T& value, Int32& available, Int32 size)
		/// Reads a value of size bytes with the read() specialization for T
		/// and subtracts size from available.
	{
		need(size, available, "value");
		read(value);
		checkStream();
		available -= size;
	}

	[[nodiscard]] unsigned char readByte();
		/// Reads one byte; throws DataFormatException at the end of the stream.

	[[nodiscard]] std::string readCString();
		/// Reads a cstring of at most MAX_MESSAGE_SIZE_BYTES.

	[[nodiscard]] std::string readCString(Int32& available);
		/// Reads a cstring that ends within the available bytes and subtracts
		/// its size from available.

	[[nodiscard]] std::string readString(Int32& available);
		/// Reads a BSON string (length, bytes, null character) that fits in the
		/// available bytes and subtracts its size from available.

	[[nodiscard]] Poco::SharedPtr<Binary> readBinary(Int32& available);
		/// Reads a BSON binary (length, subtype, bytes) that fits in the
		/// available bytes and subtracts its size from available. Defined in Binary.h.

private:
	static void need(Int32 bytes, Int32 available, const char* what);
	void checkStream();

	Poco::BinaryReader _reader;
};


//
// inlines
//
inline void BSONReader::need(Int32 bytes, Int32 available, const char* what)
{
	if (bytes > available)
	{
		throw DataFormatException("BSON " + std::string(what) + " of " + std::to_string(bytes) +
			" bytes exceeds the " + std::to_string(available) + " bytes left in its document");
	}
}


inline void BSONReader::checkStream()
{
	if (!_reader.good())
		throw DataFormatException("Truncated BSON document");
}


inline unsigned char BSONReader::readByte()
{
	// Reads from the stream buffer directly: an istream sentry per byte is too slow.
	const int c = _reader.stream().rdbuf()->sbumpc();
	if (c == std::char_traits<char>::eof())
		throw DataFormatException("Truncated BSON document");
	return static_cast<unsigned char>(c);
}


inline std::string BSONReader::readCString(Int32& available)
{
	// Reads from the stream buffer directly: an istream sentry per byte is too slow.
	std::streambuf& buffer = *_reader.stream().rdbuf();
	std::string value;
	for (;;)
	{
		if (available == 0)
			throw DataFormatException("BSON cstring is not terminated within its document");
		const int c = buffer.sbumpc();
		if (c == std::char_traits<char>::eof())
			throw DataFormatException("Truncated BSON document");
		--available;
		if (c == 0)
			return value;
		value += static_cast<char>(c);
	}
}


inline std::string BSONReader::readCString()
{
	Int32 available = MAX_MESSAGE_SIZE_BYTES;
	return readCString(available);
}


inline std::string BSONReader::readString(Int32& available)
{
	need(4, available, "string length");
	Int32 size = 0;
	_reader >> size;
	checkStream();
	available -= 4;
	if (size < BSON_MIN_STRING_SIZE)
		throw DataFormatException("Invalid BSON string size: " + std::to_string(size));
	need(size, available, "string");
	std::string value;
	_reader.readRaw(size, value);
	if (static_cast<Int32>(value.size()) != size)
		throw DataFormatException("Truncated BSON document");
	available -= size;
	if (value.back() != '\0')
		throw DataFormatException("BSON string is not terminated");
	value.pop_back();
	return value;
}


} } // namespace Poco::MongoDB


#endif // MongoDB_BSONReader_INCLUDED
