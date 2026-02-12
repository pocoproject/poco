//
// Array.h
//
// Library: MongoDB
// Package: MongoDB
// Module:  ObjectId
//
// Definition of the ObjectId class.
//
// Copyright (c) 2012-2025, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef MongoDB_ObjectId_INCLUDED
#define MongoDB_ObjectId_INCLUDED


#include "Poco/MongoDB/MongoDB.h"
#include "Poco/MongoDB/Element.h"
#include "Poco/Timestamp.h"


namespace Poco {
namespace MongoDB {


class MongoDB_API ObjectId
	/// ObjectId is a 12-byte BSON type, constructed using:
	///
	///   - a 4-byte timestamp,
	///   - a 3-byte machine identifier,
	///   - a 2-byte process id, and
	///   - a 3-byte counter, starting with a random value.
	///
	/// In MongoDB, documents stored in a collection require a unique _id field that acts
	/// as a primary key. Because ObjectIds are small, most likely unique, and fast to generate,
	/// MongoDB uses ObjectIds as the default value for the _id field if the _id field is not
	/// specified; i.e., the mongod adds the _id field and generates a unique ObjectId to assign
	/// as its value.
{
public:
	using Ptr = SharedPtr<ObjectId>;

	explicit ObjectId(const std::string& id);
		/// Creates an ObjectId from a string.
		///
		/// The string must contain a hexadecimal representation
		/// of an object ID. This means a string of 24 characters.

	ObjectId(const ObjectId& copy) noexcept;
		/// Creates an ObjectId by copying another one.

	ObjectId(ObjectId&& other) noexcept;
		/// Creates an ObjectId by moving another one.

	ObjectId& operator=(const ObjectId& copy) noexcept;
		/// Assigns another ObjectId.

	ObjectId& operator=(ObjectId&& other) noexcept;
		/// Move-assigns another ObjectId.

	virtual ~ObjectId();
		/// Destroys the ObjectId.

	[[nodiscard]] Timestamp timestamp() const noexcept;
		/// Returns the timestamp which is stored in the first four bytes of the id

	[[nodiscard]] std::string toString(const std::string& fmt = "%02x") const;
		/// Returns the id in string format. The fmt parameter
		/// specifies the formatting used for individual members
		/// of the ID char array.

private:
	ObjectId();

	static constexpr int fromHex(char c) noexcept;
	static constexpr char fromHex(const char* c) noexcept;

	unsigned char _id[12];

	friend class BSONWriter;
	friend class BSONReader;
	friend class Document;
};


//
// inlines
//
inline Timestamp ObjectId::timestamp() const noexcept
{
	int time;
	char* T = reinterpret_cast<char*>(&time);
	T[0] = _id[3];
	T[1] = _id[2];
	T[2] = _id[1];
	T[3] = _id[0];
	return Timestamp::fromEpochTime(static_cast<time_t>(time));
}


constexpr inline int ObjectId::fromHex(char c) noexcept
{
	if ( '0' <= c && c <= '9' )
		return c - '0';
	if ( 'a' <= c && c <= 'f' )
		return c - 'a' + 10;
	if ( 'A' <= c && c <= 'F' )
		return c - 'A' + 10;
	return 0xff;
}


constexpr inline char ObjectId::fromHex(const char* c) noexcept
{
	return static_cast<char>((fromHex(c[0]) << 4) | fromHex(c[1]));
}


// BSON Embedded Document
// spec: ObjectId
template<>
struct ElementTraits<ObjectId::Ptr>
{
	enum { TypeId = 0x07 };

	static std::string toString(const ObjectId::Ptr& id,
		int indent = 0,
		const std::string& fmt = "%02x")
	{
		return R"(")" + id->toString(fmt) + '"';
	}
};


template<>
inline void BSONReader::read<ObjectId::Ptr>(ObjectId::Ptr& to)
{
	_reader.readRaw(reinterpret_cast<char*>(to->_id), 12);
}


template<>
inline void BSONWriter::write<ObjectId::Ptr>(const ObjectId::Ptr& from)
{
	_writer.writeRaw(reinterpret_cast<const char*>(from->_id), 12);
}


} } // namespace Poco::MongoDB


#endif // MongoDB_ObjectId_INCLUDED
