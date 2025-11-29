//
// Binary.h
//
// Library: MongoDB
// Package: MongoDB
// Module:  Binary
//
// Definition of the Binary class.
//
// Copyright (c) 2012-2025, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef MongoDB_Binary_INCLUDED
#define MongoDB_Binary_INCLUDED


#include "Poco/MongoDB/MongoDB.h"
#include "Poco/MongoDB/Element.h"
#include "Poco/Buffer.h"
#include "Poco/UUID.h"


namespace Poco {
namespace MongoDB {


class MongoDB_API Binary
	/// Implements BSON Binary.
	///
	/// A Binary stores its data in a Poco::Buffer<unsigned char>.
{
public:
	using Ptr = SharedPtr<Binary>;

	/// BSON Binary subtypes
	enum Subtype
	{
		SUBTYPE_GENERIC      = 0x00,  /// Generic binary data
		SUBTYPE_FUNCTION     = 0x01,  /// Function
		SUBTYPE_BINARY_OLD   = 0x02,  /// Binary (Old)
		SUBTYPE_UUID_OLD     = 0x03,  /// UUID (Old)
		SUBTYPE_UUID         = 0x04,  /// UUID
		SUBTYPE_MD5          = 0x05,  /// MD5
		SUBTYPE_ENCRYPTED    = 0x06,  /// Encrypted BSON value
		SUBTYPE_USER_DEFINED = 0x80   /// User defined (start of range)
	};

	Binary();
		/// Creates an empty Binary with subtype 0.

	Binary(Poco::Int32 size, unsigned char subtype);
		/// Creates a Binary with a buffer of the given size and the given subtype.

	Binary(const UUID& uuid);
		/// Creates a Binary containing an UUID.

	Binary(const char* data, unsigned char subtype = 0);
		/// Creates a Binary with the contents of the given C-string and the given subtype.

	Binary(const std::string& data, unsigned char subtype = 0);
		/// Creates a Binary with the contents of the given string and the given subtype.

	Binary(const void* data, Poco::Int32 size, unsigned char subtype);
		/// Creates a Binary with the contents of the given buffer and the given subtype.

	virtual ~Binary();
		/// Destroys the Binary.

	Buffer<unsigned char>& buffer();
		/// Returns a reference to the internal buffer

	[[nodiscard]] unsigned char subtype() const;
		/// Returns the subtype.

	void subtype(unsigned char type);
		/// Sets the subtype.

	[[nodiscard]] std::string toString(int indent = 0) const;
		/// Returns the contents of the Binary as a string.
		/// For UUID subtype (SUBTYPE_UUID), returns a formatted UUID string
		/// wrapped in UUID() (e.g., UUID("550e8400-e29b-41d4-a716-446655440000")).
		/// For other subtypes, returns Base64-encoded data.

	[[nodiscard]] std::string toRawString() const;
		/// Returns the raw content of the Binary as a string.

	[[nodiscard]] UUID uuid() const;
		/// Returns the UUID when the binary subtype is 0x04.
		/// Otherwise, throws a Poco::BadCastException.

private:
	Buffer<unsigned char> _buffer;
	unsigned char _subtype;
};


//
// inlines
//
inline unsigned char Binary::subtype() const
{
	return _subtype;
}


inline void Binary::subtype(unsigned char type)
{
	_subtype = type;
}


inline Buffer<unsigned char>& Binary::buffer()
{
	return _buffer;
}


inline std::string Binary::toRawString() const
{
	return {reinterpret_cast<const char*>(_buffer.begin()), _buffer.size()};
}


// BSON Embedded Document
// spec: binary
template<>
struct ElementTraits<Binary::Ptr>
{
	enum { TypeId = 0x05 };

	static std::string toString(const Binary::Ptr& value, int indent = 0)
	{
		return value.isNull() ? "" : value->toString();
	}
};


template<>
inline void BSONReader::read<Binary::Ptr>(Binary::Ptr& to)
{
	Poco::Int32 size;
	_reader >> size;

	to->buffer().resize(size);

	unsigned char subtype;
	_reader >> subtype;
	to->subtype(subtype);

	_reader.readRaw((char*) to->buffer().begin(), size);
}


template<>
inline void BSONWriter::write<Binary::Ptr>(Binary::Ptr& from)
{
	_writer << (Poco::Int32) from->buffer().size();
	_writer << from->subtype();
	_writer.writeRaw(reinterpret_cast<char*>(from->buffer().begin()), from->buffer().size());
}


} } // namespace Poco::MongoDB


#endif // MongoDB_Binary_INCLUDED
