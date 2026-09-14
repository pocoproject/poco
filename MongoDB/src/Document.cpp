//
// Document.cpp
//
// Library: MongoDB
// Package: MongoDB
// Module:  Document
//
// Copyright (c) 2012-2025, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/MongoDB/Document.h"
#include "Poco/MongoDB/Array.h"
#include "Poco/MongoDB/Binary.h"
#include "Poco/MongoDB/Decimal128.h"
#include "Poco/MongoDB/JavaScriptCode.h"
#include "Poco/MongoDB/MaxKey.h"
#include "Poco/MongoDB/MinKey.h"
#include "Poco/MongoDB/ObjectId.h"
#include "Poco/MongoDB/RegularExpression.h"
#include "Poco/Exception.h"
#include "Poco/Timestamp.h"
#include <sstream>
#include <utility>


namespace Poco::MongoDB {


namespace {


// Servers 5.0 and later allow at most 250 nesting levels plus 20 for commands and
// replies; 3.6 to 4.4 allowed up to 1000. Reading, writing and destroying a document
// recurse once per level. 512 levels fit the smallest default thread stack (512 KiB,
// macOS) in a debug build as long as the recursive frames stay small: see readValue
// and beginDocument/endDocument.
constexpr int BSON_MAX_DEPTH = 512;


// Below this size a linear scan for a duplicate name is faster than the index;
// above it addElement() keeps the index current, so that building large arrays
// is not quadratic.
constexpr std::size_t MAX_LINEAR_SCAN_ELEMENTS = 128;


template <typename T>
[[nodiscard]] Element::Ptr fixedElement(BSONReader& reader, Int32& available, std::string&& name, T value, Int32 size)
	/// Reads a value of a fixed size with the BSONReader specialization of its type.
{
	reader.readFixed(value, available, size);
	return new ConcreteElement<T>(std::move(name), std::move(value));
}


[[nodiscard]] Int32 readDocumentSize(BinaryReader& reader, Int32 maxSize, int depth)
{
	if (depth > BSON_MAX_DEPTH)
		throw DataFormatException("BSON document nesting exceeds " + std::to_string(BSON_MAX_DEPTH) + " levels");
	Int32 size = 0;
	reader >> size;
	if (!reader.good())
		throw DataFormatException("Truncated BSON document");
	if (size < BSON_MIN_DOCUMENT_SIZE)
		throw DataFormatException("Invalid BSON document size: " + std::to_string(size));
	if (size > maxSize)
	{
		throw DataFormatException("BSON document size " + std::to_string(size) +
			" exceeds the " + std::to_string(maxSize) + " bytes available");
	}
	return size;
}


[[noreturn]] void throwMisplacedEnd(const char* message, Int32 size)
{
	throw DataFormatException(message + std::to_string(size));
}


[[noreturn]] void throwNestingTooDeep()
{
	throw InvalidArgumentException("BSON document nesting exceeds " + std::to_string(BSON_MAX_DEPTH) + " levels");
}


} // namespace


Element::Ptr Document::readValue(unsigned char type, std::string&& name, BSONReader& reader, Int32& available)
{
	switch (type)
	{
	case ElementTraits<double>::TypeId:
		return fixedElement<double>(reader, available, std::move(name), 0.0, 8);
	case ElementTraits<Int32>::TypeId:
		return fixedElement<Int32>(reader, available, std::move(name), 0, 4);
	case ElementTraits<std::string>::TypeId:
		return new ConcreteElement<std::string>(std::move(name), reader.readString(available));
	case ElementTraits<Binary::Ptr>::TypeId:
		return new ConcreteElement<Binary::Ptr>(std::move(name), reader.readBinary(available));
	case ElementTraits<ObjectId::Ptr>::TypeId:
		return fixedElement<ObjectId::Ptr>(reader, available, std::move(name), new ObjectId, 12);
	case ElementTraits<bool>::TypeId:
		return fixedElement<bool>(reader, available, std::move(name), false, 1);
	case ElementTraits<Poco::Timestamp>::TypeId:
		return fixedElement<Poco::Timestamp>(reader, available, std::move(name), Poco::Timestamp(0), 8);
	case ElementTraits<BSONTimestamp>::TypeId:
		return fixedElement<BSONTimestamp>(reader, available, std::move(name), BSONTimestamp(), 8);
	case ElementTraits<NullValue>::TypeId:
		return new ConcreteElement<NullValue>(std::move(name), NullValue(0));
	case ElementTraits<RegularExpression::Ptr>::TypeId:
		{
			const std::string pattern = reader.readCString(available);
			const std::string options = reader.readCString(available);
			return new ConcreteElement<RegularExpression::Ptr>(std::move(name), new RegularExpression(pattern, options));
		}
	case ElementTraits<JavaScriptCode::Ptr>::TypeId:
		{
			JavaScriptCode::Ptr code = new JavaScriptCode();
			code->setCode(reader.readString(available));
			return new ConcreteElement<JavaScriptCode::Ptr>(std::move(name), std::move(code));
		}
	case ElementTraits<Int64>::TypeId:
		return fixedElement<Int64>(reader, available, std::move(name), 0, 8);
	case ElementTraits<Decimal128::Ptr>::TypeId:
		return fixedElement<Decimal128::Ptr>(reader, available, std::move(name), Decimal128::Ptr(), 16);
	case ElementTraits<MinKey>::TypeId:
		return new ConcreteElement<MinKey>(std::move(name), MinKey{});
	case ElementTraits<MaxKey>::TypeId:
		return new ConcreteElement<MaxKey>(std::move(name), MaxKey{});
	default:
		{
			std::stringstream ss;
			ss << "Element " << name << " contains an unsupported type 0x" << std::hex << static_cast<int>(type);
			throw Poco::NotImplementedException(ss.str());
		}
	//TODO: x0F -> JavaScript code with scope
	}
}


Document::Document()
{
}


Document::~Document()
{
}


Array& Document::addNewArray(const std::string& name)
{
	Array::Ptr newArray = new Array();
	add(name, newArray);
	return *newArray;
}


void Document::rebuildElementSet() const
{
	_elementSet.clear();
	for (const auto& elem : _elements)
	{
		_elementSet.insert(elem);
	}
	_elementSetValid = true;
}


Element::Ptr Document::get(const std::string& name) const
{
	if (!_elementSetValid)
	{
		rebuildElementSet();
	}
	auto it = _elementSet.find(name);
	if (it == _elementSet.end())
		return nullptr;
	return *it;
}


Int64 Document::getInteger(const std::string& name) const
{
	Element::Ptr element = get(name);
	if (element.isNull()) throw Poco::NotFoundException(name);

	if (ElementTraits<double>::TypeId == element->type())
	{
		// Type is already verified, static_cast is safe and ~10x faster than dynamic_cast
		auto* concrete = static_cast<ConcreteElement<double>*>(element.get());
		return static_cast<Int64>(concrete->value());
	}
	else if (ElementTraits<Int32>::TypeId == element->type())
	{
		auto* concrete = static_cast<ConcreteElement<Int32>*>(element.get());
		return concrete->value();
	}
	else if (ElementTraits<Int64>::TypeId == element->type())
	{
		auto* concrete = static_cast<ConcreteElement<Int64>*>(element.get());
		return concrete->value();
	}
	throw Poco::BadCastException("Invalid type mismatch!");
}


void Document::read(BinaryReader& reader)
{
	(void) read(reader, MAX_MESSAGE_SIZE_BYTES);
}


Int32 Document::read(BinaryReader& reader, Int32 maxSize)
{
	// BSON is little-endian; the reader's byte order is not used, as in write().
	BinaryReader le(reader.stream(), BinaryReader::LITTLE_ENDIAN_BYTE_ORDER);
	return readImpl(le, maxSize, 0);
}


Int32 Document::readImpl(BinaryReader& reader, Int32 maxSize, int depth)
{
	const Int32 size = readDocumentSize(reader, maxSize, depth);
	BSONReader bsonReader(reader);

	// Set will be built lazily on first get() call
	_elementSetValid = false;

	// Bytes of the element list; the terminating 0x00 is read separately.
	Int32 available = size - BSON_MIN_DOCUMENT_SIZE;
	while (available > 0)
	{
		const unsigned char type = bsonReader.readByte();
		--available;
		if (type == '\0')
			throwMisplacedEnd("BSON document ends before its declared size ", size);

		std::string name = bsonReader.readCString(available);
		if (type == ElementTraits<Document::Ptr>::TypeId)
		{
			Document::Ptr doc = new Document;
			available -= doc->readImpl(reader, available, depth + 1);
			_elements.push_back(Element::Ptr(new ConcreteElement<Document::Ptr>(std::move(name), std::move(doc))));
		}
		else if (type == ElementTraits<Array::Ptr>::TypeId)
		{
			Array::Ptr array = new Array;
			available -= static_cast<Document&>(*array).readImpl(reader, available, depth + 1);
			_elements.push_back(Element::Ptr(new ConcreteElement<Array::Ptr>(std::move(name), std::move(array))));
		}
		else
		{
			_elements.push_back(readValue(type, std::move(name), bsonReader, available));
		}
	}

	if (bsonReader.readByte() != '\0')
		throwMisplacedEnd("BSON document is not terminated at its declared size ", size);

	return size;
}


std::string Document::toString(int indent) const
{
	std::ostringstream oss;
	oss << '{';

	if (indent > 0) oss << std::endl;

	for (auto it = _elements.begin(); it != _elements.end(); ++it)
	{
		if (it != _elements.begin())
		{
			oss << ',';
			if (indent > 0) oss << std::endl;
		}

		if (indent > 0)
		{
			const std::string indentStr(indent, ' ');
			oss << indentStr;
		}

		const auto& element = *it;
		oss << '"' << element->name() << '"';
		oss << (indent > 0  ? " : " : ":");

		oss << element->toString(indent > 0 ? indent + 2 : 0);
	}

	if (indent > 0)
	{
		oss << std::endl;
		if (indent >= 2) indent -= 2;

		const std::string indentStr(indent, ' ');
		oss << indentStr;
	}

	oss << '}';

	return oss.str();
}


void Document::write(BinaryWriter& writer) const
{
	if (_elements.empty())
	{
		writer << 5;
	}
	else
	{
		std::stringstream sstream;
		Poco::BinaryWriter tempWriter(sstream, BinaryWriter::LITTLE_ENDIAN_BYTE_ORDER);
		for (const auto& element : _elements)
		{
			tempWriter << static_cast<unsigned char>(element->type());
			BSONWriter(tempWriter).writeCString(element->name());
			element->write(tempWriter);
		}
		tempWriter.flush();

		Poco::Int32 len = static_cast<Poco::Int32>(5 + sstream.tellp()); /* 5 = sizeof(len) + 0-byte */
		writer << len;
		writer.writeRaw(sstream.str());
	}
	writer << '\0';
}


void Document::reserve(std::size_t size)
{
	_elements.reserve(size);
	// Note: std::set doesn't support reserve()
}


Document& Document::addElement(Element::Ptr element)
{
	const std::string& name = element->name();

	if (_elements.size() < MAX_LINEAR_SCAN_ELEMENTS)
	{
		auto it = std::find_if(_elements.begin(), _elements.end(), ElementFindByName(name));
		if (it != _elements.end())
			*it = element;
		else
			_elements.push_back(element);

		// Invalidate set - will be rebuilt lazily on first get()
		_elementSetValid = false;
		return *this;
	}

	if (!_elementSetValid)
		rebuildElementSet();
	auto it = _elementSet.find(name);
	// Stays invalid if an allocation below throws, so that get() rebuilds the set.
	_elementSetValid = false;
	if (it != _elementSet.end())
	{
		auto pos = std::find(_elements.begin(), _elements.end(), *it);
		if (pos != _elements.end())
			*pos = element;
		else
			_elements.push_back(element);
		_elementSet.erase(it);
	}
	else
	{
		_elements.push_back(element);
	}
	_elementSet.insert(element);
	_elementSetValid = true;
	return *this;
}


bool Document::remove(const std::string& name)
{
	// Linear scan to find element
	auto it = std::find_if(_elements.begin(), _elements.end(), ElementFindByName(name));
	if (it == _elements.end())
		return false;

	if (_elementSetValid)
	{
		// Keeping the index current is cheaper than rebuilding it for every removal.
		// A document read from a stream may hold a name twice; the index then
		// has to point at the remaining occurrence.
		const auto indexed = _elementSet.find(name);
		if (indexed != _elementSet.end())
			_elementSet.erase(indexed);
		const auto next = std::find_if(it + 1, _elements.end(), ElementFindByName(name));
		if (next != _elements.end())
			_elementSet.insert(*next);
	}

	_elements.erase(it);
	return true;
}


} // namespace Poco::MongoDB
