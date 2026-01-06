//
// Element.h
//
// Library: MongoDB
// Package: MongoDB
// Module:  Element
//
// Definition of the Element class.
//
// Copyright (c) 2012-2025, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef MongoDB_Element_INCLUDED
#define MongoDB_Element_INCLUDED


#include "Poco/BinaryReader.h"
#include "Poco/BinaryWriter.h"
#include "Poco/DateTimeFormatter.h"
#include "Poco/MongoDB/BSONReader.h"
#include "Poco/MongoDB/BSONWriter.h"
#include "Poco/MongoDB/MongoDB.h"
#include "Poco/Nullable.h"
#include "Poco/NumberFormatter.h"
#include "Poco/SharedPtr.h"
#include "Poco/Timestamp.h"
#include <cstdio>
#include <string>
#include <utility>


namespace Poco {
namespace MongoDB {


class MongoDB_API Element
	/// Represents an Element of a Document or an Array.
{
public:
	using Ptr = Poco::SharedPtr<Element>;

	explicit Element(const std::string& name);
		/// Creates the Element with the given name.

	explicit Element(std::string&& name);
		/// Creates the Element with the given name (move semantics).

	virtual ~Element();
		/// Destructor

	[[nodiscard]] const std::string& name() const noexcept;
		/// Returns the name of the element.

	[[nodiscard]] virtual std::string toString(int indent = 0) const = 0;
		/// Returns a string representation of the element.

	[[nodiscard]] virtual int type() const noexcept = 0;
		/// Returns the MongoDB type of the element.

private:
	virtual void read(BinaryReader& reader) = 0;
	virtual void write(BinaryWriter& writer) const = 0;

	friend class Document;
	std::string _name;
};


//
// inlines
//
inline const std::string& Element::name() const noexcept
{
	return _name;
}


template<typename T>
struct ElementTraits
{
};


// BSON Floating point
// spec: double
template<>
struct ElementTraits<double>
{
	enum { TypeId = 0x01 };

	static std::string toString(const double& value, int indent = 0)
	{
		return Poco::NumberFormatter::format(value);
	}
};


// BSON UTF-8 string
// spec: int32 (byte*) "\x00"
// int32 is the number bytes in byte* + 1 (for trailing "\x00")
template<>
struct ElementTraits<std::string>
{
	enum { TypeId = 0x02 };

	static std::string toString(const std::string& value, int indent = 0)
	{
		// Fast path: check if escaping is needed at all
		bool needsEscaping = false;
		for (char c : value)
		{
			if (c == '"' || c == '\\' || c == '\b' || c == '\f' ||
			    c == '\n' || c == '\r' || c == '\t' || (c > 0 && c <= 0x1F))
			{
				needsEscaping = true;
				break;
			}
		}

		// Fast path: no escaping needed - just wrap in quotes
		if (!needsEscaping)
		{
			std::string result;
			result.reserve(value.size() + 2);
			result += '"';
			result += value;
			result += '"';
			return result;
		}

		// Slow path: escaping needed
		std::string result;
		result.reserve(value.size() * 2 + 2);  // Pessimistic estimate
		result += '"';

		for (char c : value)
		{
			switch (c)
			{
			case '"':
				result += "\\\"";
				break;
			case '\\':
				result += "\\\\";
				break;
			case '\b':
				result += "\\b";
				break;
			case '\f':
				result += "\\f";
				break;
			case '\n':
				result += "\\n";
				break;
			case '\r':
				result += "\\r";
				break;
			case '\t':
				result += "\\t";
				break;
			default:
				if (c > 0 && c <= 0x1F)
				{
					// Unicode escape sequence
					char buf[7];  // "\uXXXX" + null terminator
					std::snprintf(buf, sizeof(buf), "\\u%04X", static_cast<unsigned char>(c));
					result += buf;
				}
				else
				{
					result += c;
				}
				break;
			}
		}
		result += '"';
		return result;
	}
};


template<>
inline void BSONReader::read<std::string>(std::string& to)
{
	Poco::Int32 size;
	_reader >> size;
	_reader.readRaw(size, to);
	to.erase(to.end() - 1); // remove terminating 0
}


template<>
inline void BSONWriter::write<std::string>(const std::string& from)
{
	_writer << static_cast<Poco::Int32>(from.length() + 1);
	writeCString(from);
}


// BSON bool
// spec: "\x00" "\x01"
template<>
struct ElementTraits<bool>
{
	enum { TypeId = 0x08 };

	static std::string toString(const bool& value, int indent = 0)
	{
		return value ? "true" : "false";
	}
};


template<>
inline void BSONReader::read<bool>(bool& to)
{
	unsigned char b;
	_reader >> b;
	to = b != 0;
}


template<>
inline void BSONWriter::write<bool>(const bool& from)
{
	unsigned char b = from ? 0x01 : 0x00;
	_writer << b;
}


// BSON 32-bit integer
// spec: int32
template<>
struct ElementTraits<Int32>
{
	enum { TypeId = 0x10 };


	static std::string toString(const Int32& value, int indent = 0)
	{
		return Poco::NumberFormatter::format(value);
	}
};


// BSON UTC datetime
// spec: int64
template<>
struct ElementTraits<Timestamp>
{
	enum { TypeId = 0x09 };

	static std::string toString(const Timestamp& value, int indent = 0)
	{
		std::string result;
		result.reserve(32);  // Pre-allocate for typical timestamp string length
		result += '"';
		result += DateTimeFormatter::format(value, "%Y-%m-%dT%H:%M:%s%z");
		result += '"';
		return result;
	}
};


template<>
inline void BSONReader::read<Timestamp>(Timestamp& to)
{
	Poco::Int64 value;
	_reader >> value;
	to = Timestamp::fromEpochTime(static_cast<std::time_t>(value / 1000));
	to += (value % 1000 * 1000);
}


template<>
inline void BSONWriter::write<Timestamp>(const Timestamp& from)
{
	_writer << (from.epochMicroseconds() / 1000);
}


using NullValue = Nullable<unsigned char>;


// BSON Null Value
// spec:
template<>
struct ElementTraits<NullValue>
{
	enum { TypeId = 0x0A };

	static std::string toString(const NullValue& value, int indent = 0)
	{
		return "null";
	}
};


template<>
inline void BSONReader::read<NullValue>(NullValue& to)
{
}


template<>
inline void BSONWriter::write<NullValue>(const NullValue& from)
{
}


struct BSONTimestamp
{
	Poco::Timestamp ts;
	Poco::Int32 inc;
};


// BSON Timestamp
// spec: int64
template<>
struct ElementTraits<BSONTimestamp>
{
	enum { TypeId = 0x11 };

	static std::string toString(const BSONTimestamp& value, int indent = 0)
	{
		std::string result;
		result.reserve(48);  // Pre-allocate for timestamp + space + increment + quotes
		result += '"';
		result += DateTimeFormatter::format(value.ts, "%Y-%m-%dT%H:%M:%s%z");
		result += ' ';
		result += NumberFormatter::format(value.inc);
		result += '"';
		return result;
	}
};


template<>
inline void BSONReader::read<BSONTimestamp>(BSONTimestamp& to)
{
	Poco::Int64 value;
	_reader >> value;
	to.inc = value & 0xffffffff;
	value >>= 32;
	to.ts = Timestamp::fromEpochTime(static_cast<std::time_t>(value));
}


template<>
inline void BSONWriter::write<BSONTimestamp>(const BSONTimestamp& from)
{
	Poco::Int64 value = from.ts.epochMicroseconds() / 1000;
	value <<= 32;
	value += from.inc;
	_writer << value;
}


// BSON 64-bit integer
// spec: int64
template<>
struct ElementTraits<Int64>
{
	enum { TypeId = 0x12 };

	static std::string toString(const Int64& value, int indent = 0)
	{
		return NumberFormatter::format(value);
	}
};


template<typename T>
class ConcreteElement: public Element
{
public:
	ConcreteElement(const std::string& name, const T& init):
		Element(name),
		_value(init)
	{
	}

	ConcreteElement(std::string&& name, T&& init):
		Element(std::move(name)),
		_value(std::move(init))
	{
	}

	~ConcreteElement() override = default;


	const T& value() const noexcept
	{
		return _value;
	}


	[[nodiscard]] std::string toString(int indent = 0) const override
	{
		return ElementTraits<T>::toString(_value, indent);
	}


	[[nodiscard]] int type() const noexcept override
	{
		return ElementTraits<T>::TypeId;
	}

	void read(BinaryReader& reader) override
	{
		BSONReader(reader).read(_value);
	}

	void write(BinaryWriter& writer) const override
	{
		BSONWriter(writer).write(_value);
	}

private:
	T _value;
};


} } // namespace Poco::MongoDB


#endif // MongoDB_Element_INCLUDED
