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


namespace Poco {
namespace MongoDB {


class MongoDB_API BSONReader
	/// Class for reading BSON using a Poco::BinaryReader
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

	[[nodiscard]] std::string readCString();
		/// Reads a cstring from the reader.
		/// A cstring is a string terminated with a 0x00.

private:
	Poco::BinaryReader _reader;
};


//
// inlines
//
inline std::string BSONReader::readCString()
{
	std::string val;
	while(_reader.good())
	{
		char c;
		_reader >> c;
		if ( _reader.good() )
		{
			if (c == 0x00) return val;
			else val += c;
			if (val.size() > static_cast<std::size_t>(BSON_MAX_DOCUMENT_SIZE))
				throw Poco::DataFormatException("BSON cstring exceeds maximum size");
		}
	}
	return val;
}


} } // namespace Poco::MongoDB


#endif // MongoDB_BSONReader_INCLUDED
