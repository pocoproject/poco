//
// BSONTest.cpp
//
// Copyright (c) 2025, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "BSONTest.h"
#include "TestDocuments.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/MongoDB/Document.h"
#include "Poco/MongoDB/Array.h"
#include "Poco/MongoDB/Binary.h"
#include "Poco/MongoDB/Connection.h"
#include "Poco/MongoDB/Database.h"
#include "Poco/MongoDB/Decimal128.h"
#include "Poco/MongoDB/MaxKey.h"
#include "Poco/MongoDB/MinKey.h"
#include "Poco/MongoDB/ObjectId.h"
#include "Poco/MongoDB/OpMsgMessage.h"
#include "Poco/MongoDB/PoolableConnectionFactory.h"
#include "Poco/MongoDB/RegularExpression.h"
#include "Poco/MongoDB/JavaScriptCode.h"
#include "Poco/Net/ServerSocket.h"
#include "Poco/Net/StreamSocket.h"
#include "Poco/BinaryReader.h"
#include "Poco/BinaryWriter.h"
#include "Poco/Exception.h"
#include "Poco/DateTime.h"
#include "Poco/UTF8Encoding.h"
#include "Poco/UUIDGenerator.h"
#include <limits>
#include <sstream>
#include <iostream>
#include <typeinfo>


using namespace Poco::MongoDB;
using namespace std::string_literals;


namespace
{
	std::string int32LE(Poco::Int32 value)
	{
		std::ostringstream os;
		Poco::BinaryWriter writer(os, Poco::BinaryWriter::LITTLE_ENDIAN_BYTE_ORDER);
		writer << value;
		writer.flush();
		return os.str();
	}

	std::string int64LE(Poco::Int64 value)
	{
		std::ostringstream os;
		Poco::BinaryWriter writer(os, Poco::BinaryWriter::LITTLE_ENDIAN_BYTE_ORDER);
		writer << value;
		writer.flush();
		return os.str();
	}

	std::string serialize(const Document& doc)
	{
		std::ostringstream os;
		Poco::BinaryWriter writer(os, Poco::BinaryWriter::LITTLE_ENDIAN_BYTE_ORDER);
		doc.write(writer);
		writer.flush();
		return os.str();
	}
}


BSONTest::BSONTest(const std::string& name):
	CppUnit::TestCase("BSON")
{
}


BSONTest::~BSONTest()
{
}


void BSONTest::setUp()
{
}


void BSONTest::tearDown()
{
}


void BSONTest::testDocumentAddGet()
{
	Document::Ptr doc = new Document();

	// Test adding and getting various types
	doc->add("string"s, "test"s);
	doc->add("int32"s, static_cast<Poco::Int32>(42));
	doc->add("int64"s, static_cast<Poco::Int64>(9876543210LL));
	doc->add("double"s, 3.14159);
	doc->add("bool"s, true);

	assertEqual(doc->get<std::string>("string"), "test");
	assertEqual(doc->get<Poco::Int32>("int32"), 42);
	assertEqual(doc->get<Poco::Int64>("int64"), 9876543210LL);
	assertEqual(doc->get<double>("double"), 3.14159);
	assertEqual(doc->get<bool>("bool"), true);

	// Test get with default value
	assertEqual(doc->get<std::string>("nonexistent", "default"), "default");
	assertEqual(doc->get<Poco::Int32>("nonexistent", 99), 99);
}


void BSONTest::testDocumentExists()
{
	Document::Ptr doc = new Document();
	doc->add("field1"s, "value1"s);
	doc->add("field2"s, 123);

	assertTrue(doc->exists("field1"));
	assertTrue(doc->exists("field2"));
	assertFalse(doc->exists("field3"));
	assertFalse(doc->exists(""));
}


void BSONTest::testDocumentRemove()
{
	Document::Ptr doc = new Document();
	doc->add("field1"s, "value1"s);
	doc->add("field2"s, "value2"s);
	doc->add("field3"s, "value3"s);

	assertEqual(doc->size(), 3);
	assertTrue(doc->exists("field2"));

	assertTrue(doc->remove("field2"s));
	assertEqual(doc->size(), 2);
	assertFalse(doc->exists("field2"));

	// Try to remove non-existent field
	assertFalse(doc->remove("field2"s));
	assertFalse(doc->remove("nonexistent"s));

	// Remaining fields should still exist
	assertTrue(doc->exists("field1"));
	assertTrue(doc->exists("field3"));
}


void BSONTest::testDocumentClear()
{
	Document::Ptr doc = new Document();
	doc->add("field1"s, "value1"s);
	doc->add("field2"s, 123);
	doc->add("field3"s, true);

	assertEqual(doc->size(), 3);
	assertFalse(doc->empty());

	doc->clear();

	assertEqual(doc->size(), 0);
	assertTrue(doc->empty());
	assertFalse(doc->exists("field1"));
	assertFalse(doc->exists("field2"));
	assertFalse(doc->exists("field3"));
}


void BSONTest::testDocumentSize()
{
	Document::Ptr doc = new Document();
	assertEqual(doc->size(), 0);

	doc->add("field1"s, "value1"s);
	assertEqual(doc->size(), 1);

	doc->add("field2"s, 123);
	assertEqual(doc->size(), 2);

	doc->add("field3"s, true);
	assertEqual(doc->size(), 3);

	doc->remove("field2"s);
	assertEqual(doc->size(), 2);

	doc->clear();
	assertEqual(doc->size(), 0);
}


void BSONTest::testDocumentElementNames()
{
	Document::Ptr doc = new Document();
	doc->add("field1"s, "value1"s);
	doc->add("field2"s, 123);
	doc->add("field3"s, true);

	std::vector<std::string> names;
	doc->elementNames(names);

	assertEqual(names.size(), 3);
	assertTrue(std::find(names.begin(), names.end(), "field1") != names.end());
	assertTrue(std::find(names.begin(), names.end(), "field2") != names.end());
	assertTrue(std::find(names.begin(), names.end(), "field3") != names.end());
}


void BSONTest::testNestedDocuments()
{
	Document::Ptr doc = new Document();
	doc->add("name"s, "John"s);

	// Create nested document
	Document::Ptr address = new Document();
	address->add("street"s, "Main St"s);
	address->add("number"s, static_cast<Poco::Int32>(123));
	address->add("city"s, "New York"s);

	doc->add("address"s, address);

	// Verify nested document
	assertTrue(doc->exists("address"));
	Document::Ptr retrievedAddress = doc->get<Document::Ptr>("address");
	assertFalse(retrievedAddress.isNull());
	assertEqual(retrievedAddress->get<std::string>("street"), "Main St");
	assertEqual(retrievedAddress->get<Poco::Int32>("number"), 123);
	assertEqual(retrievedAddress->get<std::string>("city"), "New York");

	// Test addNewDocument
	Document& contact = doc->addNewDocument("contact");
	contact.add("email"s, "john@example.com"s);
	contact.add("phone"s, "+1234567890"s);

	assertTrue(doc->exists("contact"));
	Document::Ptr retrievedContact = doc->get<Document::Ptr>("contact");
	assertEqual(retrievedContact->get<std::string>("email"), "john@example.com");
	assertEqual(retrievedContact->get<std::string>("phone"), "+1234567890");
}


void BSONTest::testDuplicateDocumentMembers()
{
	Document::Ptr doc = new Document();

	// Add duplicate field names
	doc->add("field"s, "first"s);
	doc->add("field"s, "second"s);
	doc->add("field"s, 123);
	doc->add("field"s, "third"s);

	// The last value should be returned when getting by name
	// (due to hash map overwriting)
	assertEqual("third", doc->get<std::string>("field"));

	// Size should be 1 since duplicates are prevented
	assertEqual(1, doc->size());

	// exists should return true
	assertTrue(doc->exists("field"));

	// Removing should remove from both map and vector
	assertTrue(doc->remove("field"s));

	// After removal, exists should return false
	assertFalse(doc->exists("field"));

	// Size should be 0 (the only element was removed)
	assertEqual(0, doc->size());
}


void BSONTest::testDocumentAddElementMerge()
{
	// Copy every element from a source Document into a destination via
	// elementNames() + get() + addElement().
	Document::Ptr base = new Document();
	base->add("name"s, "idx_age"s);
	base->add("unique"s, true);

	Document::Ptr filter = new Document();
	filter->add("age"s, static_cast<Poco::Int32>(18));

	Document::Ptr extras = new Document();
	extras->add("partialFilterExpression"s, filter);
	extras->add("hidden"s, true);

	std::vector<std::string> names;
	extras->elementNames(names);
	for (const auto& name: names)
	{
		Element::Ptr element = extras->get(name);
		assertFalse(element.isNull());
		base->addElement(element);
	}

	assertEqual(static_cast<std::size_t>(4), base->size());
	assertTrue(base->exists("name"));
	assertTrue(base->exists("unique"));
	assertTrue(base->exists("partialFilterExpression"));
	assertTrue(base->exists("hidden"));

	Document::Ptr partial = base->get<Document::Ptr>("partialFilterExpression");
	assertFalse(partial.isNull());
	assertEqual(static_cast<Poco::Int32>(18), partial->get<Poco::Int32>("age"));
	assertEqual(true, base->get<bool>("hidden"));
}


void BSONTest::testLargeDocumentAddElement()
{
	// Large enough for addElement() to look names up in the index.
	constexpr int count = 1000;
	Array::Ptr array = new Array();
	for (int i = 0; i < count; ++i)
		array->add(i);
	assertEqual(static_cast<std::size_t>(count), array->size());
	for (int i = 0; i < count; ++i)
		assertEqual(i, array->get<Poco::Int32>(i));

	// Replacing an element keeps its position.
	array->add("500"s, -1);
	std::vector<std::string> names;
	array->elementNames(names);
	assertEqual(static_cast<std::size_t>(count), names.size());
	assertEqual("500"s, names[500]);
	assertEqual(-1, array->get<Poco::Int32>(500));

	// A name that was removed is appended again.
	assertTrue(array->remove("10"s));
	array->add("10"s, 10);
	names.clear();
	array->elementNames(names);
	assertEqual(static_cast<std::size_t>(count), names.size());
	assertEqual("10"s, names.back());
	assertEqual(10, array->get<Poco::Int32>(10));

	// Elements read from a stream are found when adding to the document.
	std::istringstream stream(serialize(*array));
	Document restored;
	Poco::BinaryReader reader(stream, Poco::BinaryReader::LITTLE_ENDIAN_BYTE_ORDER);
	restored.read(reader);
	restored.add("999"s, 1);
	restored.add("new"s, 2);
	assertEqual(static_cast<std::size_t>(count + 1), restored.size());
	assertEqual(1, restored.get<Poco::Int32>("999"s));
	assertEqual(2, restored.get<Poco::Int32>("new"s));
}


void BSONTest::testArray()
{
	Array::Ptr arr = new Array();

	arr->add("First"s);

	Poco::DateTime birthdate;
	birthdate.assign(1969, 3, 9);
	arr->add(birthdate.timestamp());

	arr->add(static_cast<Poco::Int32>(1993));
	arr->add(false);

	// Document-style interface
	arr->add("4", "12.4E");

	assertEqual(arr->size(), 5);
	assertTrue(arr->exists("0"));
	assertTrue(arr->exists("1"));
	assertTrue(arr->exists("2"));
	assertTrue(arr->exists("3"));
	assertTrue(arr->exists("4"));
	assertFalse(arr->exists("5"));

	assertEqual(arr->get<std::string>(0), "First");
	assertEqual(arr->get<Poco::Timestamp>(1).raw(), birthdate.timestamp().raw());
	assertEqual(arr->get<Poco::Int32>(2), 1993);
	assertEqual(arr->get<bool>(3), false);
	assertEqual(arr->get<std::string>(4), "12.4E");

	// Document-style interface
	assertEqual(arr->get<Poco::Int32>("2"), 1993);
	assertEqual(arr->get<std::string>("4"), "12.4E");
}


void BSONTest::testArrayIndexAccess()
{
	Array::Ptr arr = new Array();

	arr->add("zero"s);
	arr->add("one"s);
	arr->add("two"s);

	assertEqual(arr->size(), 3);
	assertEqual(arr->get<std::string>(0), "zero");
	assertEqual(arr->get<std::string>(1), "one");
	assertEqual(arr->get<std::string>(2), "two");

	// Test with default value
	assertEqual(arr->get<std::string>(3, "default"), "default");
	assertEqual(arr->get<std::string>(10, "default"), "default");

	// Test isType
	assertTrue(arr->isType<std::string>(0));
	assertTrue(arr->isType<std::string>(1));
	assertFalse(arr->isType<Poco::Int32>(0));
}


void BSONTest::testArrayNested()
{
	Document::Ptr doc = new Document();

	// Create nested array using addNewArray
	Array& tags = doc->addNewArray("tags");
	tags.add("mongodb"s);
	tags.add("database"s);
	tags.add("nosql"s);

	assertTrue(doc->exists("tags"));
	Array::Ptr retrievedTags = doc->get<Array::Ptr>("tags");
	assertFalse(retrievedTags.isNull());
	assertEqual(retrievedTags->size(), 3);
	assertEqual(retrievedTags->get<std::string>(0), "mongodb");
	assertEqual(retrievedTags->get<std::string>(1), "database");
	assertEqual(retrievedTags->get<std::string>(2), "nosql");

	// Create array with mixed types
	Array::Ptr mixed = new Array();
	mixed->add("string"s);
	mixed->add(static_cast<Poco::Int32>(42));
	mixed->add(3.14);
	mixed->add(true);

	doc->add("mixed", mixed);

	Array::Ptr retrievedMixed = doc->get<Array::Ptr>("mixed");
	assertEqual(retrievedMixed->size(), 4);
	assertEqual(retrievedMixed->get<std::string>(0), "string");
	assertEqual(retrievedMixed->get<Poco::Int32>(1), 42);
	assertEqual(retrievedMixed->get<double>(2), 3.14);
	assertEqual(retrievedMixed->get<bool>(3), true);
}


void BSONTest::testDouble()
{
	Document::Ptr doc = new Document();

	doc->add("pi", 3.14159265359);
	doc->add("e", 2.71828182846);
	doc->add("zero", 0.0);
	doc->add("negative", -123.456);
	doc->add("large", 1.7976931348623157e+308);
	doc->add("small", 2.2250738585072014e-308);

	assertEqual(doc->get<double>("pi"), 3.14159265359);
	assertEqual(doc->get<double>("e"), 2.71828182846);
	assertEqual(doc->get<double>("zero"), 0.0);
	assertEqual(doc->get<double>("negative"), -123.456);
	assertEqual(doc->get<double>("large"), 1.7976931348623157e+308);
	assertEqual(doc->get<double>("small"), 2.2250738585072014e-308);

	assertTrue(doc->isType<double>("pi"));
	assertFalse(doc->isType<Poco::Int32>("pi"));
}


void BSONTest::testString()
{
	Document::Ptr doc = new Document();

	doc->add("empty", ""s);
	doc->add("simple", "hello"s);
	doc->add("withSpaces", "hello world"s);
	doc->add("withSpecial", "!@#$%^&*()"s);
	doc->add("withNewline", "line1\nline2"s);
	doc->add("withTab", "col1\tcol2"s);
	doc->add("unicode", "Hello 世界"s);

	// Using const char* overload
	doc->add("literal", "test");

	assertEqual(doc->get<std::string>("empty"), "");
	assertEqual(doc->get<std::string>("simple"), "hello");
	assertEqual(doc->get<std::string>("withSpaces"), "hello world");
	assertEqual(doc->get<std::string>("withSpecial"), "!@#$%^&*()");
	assertEqual(doc->get<std::string>("withNewline"), "line1\nline2");
	assertEqual(doc->get<std::string>("withTab"), "col1\tcol2");
	assertEqual(doc->get<std::string>("unicode"), "Hello 世界");
	assertEqual(doc->get<std::string>("literal"), "test");

	assertTrue(doc->isType<std::string>("simple"));
}


void BSONTest::testBool()
{
	Document::Ptr doc = new Document();

	doc->add("true", true);
	doc->add("false", false);

	assertEqual(doc->get<bool>("true"), true);
	assertEqual(doc->get<bool>("false"), false);

	assertTrue(doc->isType<bool>("true"));
	assertTrue(doc->isType<bool>("false"));
	assertFalse(doc->isType<Poco::Int32>("true"));
}


void BSONTest::testInt32()
{
	Document::Ptr doc = new Document();

	doc->add("zero", static_cast<Poco::Int32>(0));
	doc->add("positive", static_cast<Poco::Int32>(12345));
	doc->add("negative", static_cast<Poco::Int32>(-12345));
	doc->add("max", std::numeric_limits<Poco::Int32>::max());
	doc->add("min", std::numeric_limits<Poco::Int32>::min());

	assertEqual(doc->get<Poco::Int32>("zero"), 0);
	assertEqual(doc->get<Poco::Int32>("positive"), 12345);
	assertEqual(doc->get<Poco::Int32>("negative"), -12345);
	assertEqual(doc->get<Poco::Int32>("max"), std::numeric_limits<Poco::Int32>::max());
	assertEqual(doc->get<Poco::Int32>("min"), std::numeric_limits<Poco::Int32>::min());

	assertTrue(doc->isType<Poco::Int32>("zero"));
	assertFalse(doc->isType<Poco::Int64>("zero"));
	assertFalse(doc->isType<double>("zero"));
}


void BSONTest::testInt64()
{
	Document::Ptr doc = new Document();

	doc->add("zero", static_cast<Poco::Int64>(0));
	doc->add("positive", static_cast<Poco::Int64>(9876543210LL));
	doc->add("negative", static_cast<Poco::Int64>(-9876543210LL));
	doc->add("max", std::numeric_limits<Poco::Int64>::max());
	doc->add("min", std::numeric_limits<Poco::Int64>::min());

	assertEqual(doc->get<Poco::Int64>("zero"), 0);
	assertEqual(doc->get<Poco::Int64>("positive"), 9876543210LL);
	assertEqual(doc->get<Poco::Int64>("negative"), -9876543210LL);
	assertEqual(doc->get<Poco::Int64>("max"), std::numeric_limits<Poco::Int64>::max());
	assertEqual(doc->get<Poco::Int64>("min"), std::numeric_limits<Poco::Int64>::min());

	assertTrue(doc->isType<Poco::Int64>("zero"));
	assertFalse(doc->isType<Poco::Int32>("zero"));

	// Test getInteger - should handle Int32, Int64, and double
	doc->add("int32val", static_cast<Poco::Int32>(42));
	doc->add("int64val", static_cast<Poco::Int64>(9876543210LL));
	doc->add("doubleval", 123.0);

	assertEqual(doc->getInteger("int32val"), 42);
	assertEqual(doc->getInteger("int64val"), 9876543210LL);
	assertEqual(doc->getInteger("doubleval"), 123);
}


void BSONTest::testTimestamp()
{
	Document::Ptr doc = new Document();

	Poco::Timestamp now;
	Poco::DateTime dt;
	dt.assign(2023, 6, 15, 10, 30, 45);
	Poco::Timestamp specificTime = dt.timestamp();

	doc->add("now", now);
	doc->add("specific", specificTime);

	Poco::Timestamp retrievedNow = doc->get<Poco::Timestamp>("now");
	Poco::Timestamp retrievedSpecific = doc->get<Poco::Timestamp>("specific");

	assertEqual(retrievedNow.epochMicroseconds(), now.epochMicroseconds());
	assertEqual(retrievedSpecific.epochMicroseconds(), specificTime.epochMicroseconds());

	assertTrue(doc->isType<Poco::Timestamp>("now"));
	assertFalse(doc->isType<Poco::Int64>("now"));
}


void BSONTest::testNull()
{
	Document::Ptr doc = new Document();

	doc->add("nullField", NullValue());

	assertTrue(doc->exists("nullField"));
	assertTrue(doc->isType<NullValue>("nullField"));
	assertFalse(doc->isType<std::string>("nullField"));
	assertFalse(doc->isType<Poco::Int32>("nullField"));
}


void BSONTest::testBSONTimestamp()
{
	Document::Ptr doc = new Document();

	BSONTimestamp ts1;
	ts1.ts = Poco::Timestamp();
	ts1.inc = 1;

	BSONTimestamp ts2;
	Poco::DateTime dt;
	dt.assign(2023, 6, 15, 10, 30, 45);
	ts2.ts = dt.timestamp();
	ts2.inc = 42;

	doc->add("ts1", ts1);
	doc->add("ts2", ts2);

	BSONTimestamp retrieved1 = doc->get<BSONTimestamp>("ts1");
	BSONTimestamp retrieved2 = doc->get<BSONTimestamp>("ts2");

	assertEqual(retrieved1.ts.epochMicroseconds(), ts1.ts.epochMicroseconds());
	assertEqual(retrieved1.inc, 1);
	assertEqual(retrieved2.ts.epochMicroseconds(), ts2.ts.epochMicroseconds());
	assertEqual(retrieved2.inc, 42);

	assertTrue(doc->isType<BSONTimestamp>("ts1"));
}


void BSONTest::testBSONTimestampSerializeDocument()
{
	const struct
	{
		Poco::UInt32 seconds;
		Poco::Int32 increment;
	} values[] = {
		{0, 0},
		{1700000000, 42},
		// Above INT32_MAX seconds; the increment must not change the seconds.
		{4000000000u, -1},
	};
	for (const auto& value: values)
	{
		BSONTimestamp timestamp;
		timestamp.ts = Poco::Timestamp(static_cast<Poco::Timestamp::TimeVal>(value.seconds) * Poco::Timestamp::resolution());
		timestamp.inc = value.increment;

		Document::Ptr doc = new Document();
		doc->add("ts"s, timestamp);
		const std::string bytes = serialize(*doc);

		// The value follows the document size, the type and "ts\0":
		// the increment, then the seconds, each a little-endian uint32.
		std::istringstream wire(bytes.substr(8, 8));
		Poco::BinaryReader wireReader(wire, Poco::BinaryReader::LITTLE_ENDIAN_BYTE_ORDER);
		Poco::UInt32 wireIncrement = 0;
		Poco::UInt32 wireSeconds = 0;
		wireReader >> wireIncrement >> wireSeconds;
		assertEqual(static_cast<Poco::UInt32>(value.increment), wireIncrement);
		assertEqual(value.seconds, wireSeconds);

		std::istringstream stream(bytes);
		Document::Ptr restored = new Document();
		Poco::BinaryReader reader(stream, Poco::BinaryReader::LITTLE_ENDIAN_BYTE_ORDER);
		restored->read(reader);
		const BSONTimestamp& result = restored->get<BSONTimestamp>("ts"s);
		assertEqual(timestamp.ts.epochMicroseconds(), result.ts.epochMicroseconds());
		assertEqual(value.increment, result.inc);
	}

	// Times before 1970 or after 2106 do not fit the unsigned 32-bit seconds.
	const Poco::Timestamp::TimeVal outOfRange[] = {
		-Poco::Timestamp::resolution(),
		(static_cast<Poco::Timestamp::TimeVal>(0xFFFFFFFF) + 1) * Poco::Timestamp::resolution()
	};
	for (const auto time: outOfRange)
	{
		BSONTimestamp timestamp;
		timestamp.ts = Poco::Timestamp(time);
		timestamp.inc = 1;
		Document doc;
		doc.add("ts"s, timestamp);
		try
		{
			(void) serialize(doc);
			failmsg("expected RangeException for " + std::to_string(time));
		}
		catch (const Poco::RangeException&)
		{
		}
	}
}


void BSONTest::testBinaryGeneric()
{
	Document::Ptr doc = new Document();

	// Test Binary(const std::string&, unsigned char) constructor
	std::string data = "Hello, Binary World!";
	Binary::Ptr bin = new Binary(data, Binary::SUBTYPE_GENERIC);

	doc->add("binary", bin);

	assertTrue(doc->exists("binary"));
	Binary::Ptr retrieved = doc->get<Binary::Ptr>("binary");
	assertFalse(retrieved.isNull());
	assertEqual(static_cast<int>(retrieved->subtype()), static_cast<int>(Binary::SUBTYPE_GENERIC));
	assertEqual(retrieved->toRawString(), data);

	// Verify toString produces quoted Base64 (valid JSON value)
	assertEqual(retrieved->toString(), R"("SGVsbG8sIEJpbmFyeSBXb3JsZCE=")");

	// Test Binary(const char*, unsigned char) constructor with string literal
	Binary::Ptr strLitBin = new Binary("String Literal", Binary::SUBTYPE_GENERIC);
	doc->add("strLitBinary", strLitBin);
	Binary::Ptr retrievedStrLit = doc->get<Binary::Ptr>("strLitBinary");
	assertEqual("String Literal", retrievedStrLit->toRawString());

	// Test Binary(const void*, Int32, unsigned char) constructor with raw data including nulls
	const char* rawData = "Raw\0Data\0Test";
	Binary::Ptr rawBin = new Binary(rawData, 13, Binary::SUBTYPE_GENERIC);
	doc->add("rawBinary", rawBin);

	Binary::Ptr retrievedRaw = doc->get<Binary::Ptr>("rawBinary");
	assertEqual(13, retrievedRaw->buffer().size());
	assertEqual(static_cast<int>(retrievedRaw->subtype()), static_cast<int>(Binary::SUBTYPE_GENERIC));
}


void BSONTest::testBinaryUUID()
{
	Document::Ptr doc = new Document();

	// Create UUID
	Poco::UUIDGenerator generator;
	Poco::UUID uuid = generator.create();
	Binary::Ptr uuidBinary = new Binary(uuid);

	doc->add("uuid", uuidBinary);

	assertTrue(doc->exists("uuid"));
	Binary::Ptr retrieved = doc->get<Binary::Ptr>("uuid");
	assertFalse(retrieved.isNull());
	assertEqual(static_cast<int>(retrieved->subtype()), static_cast<int>(Binary::SUBTYPE_UUID));

	Poco::UUID retrievedUuid = retrieved->uuid();
	assertTrue(uuid == retrievedUuid);

	// Verify toString produces quoted UUID (valid JSON value)
	std::string uuidStr = retrieved->toString();
	assertTrue(uuidStr.front() == '"');
	assertTrue(uuidStr.back() == '"');
	assertTrue(uuidStr.find("UUID(") != std::string::npos);

	// Test with a known UUID
	Poco::UUID knownUuid("550e8400-e29b-41d4-a716-446655440000");
	Binary::Ptr knownBin = new Binary(knownUuid);
	assertEqual(knownBin->toString(), R"--("UUID(550e8400-e29b-41d4-a716-446655440000)")--");

	// Verify document toString with UUID is valid JSON
	Document::Ptr uuidDoc = new Document();
	uuidDoc->add("id"s, knownBin);
	assertEqual(uuidDoc->toString(), R"--({"id":"UUID(550e8400-e29b-41d4-a716-446655440000)"})--");

}


void BSONTest::testBinarySubtypes()
{
	Document::Ptr doc = new Document();

	std::string data = "test data";

	// Test different subtypes
	Binary::Ptr generic = new Binary(data, Binary::SUBTYPE_GENERIC);
	Binary::Ptr function = new Binary(data, Binary::SUBTYPE_FUNCTION);
	Binary::Ptr md5 = new Binary(data, Binary::SUBTYPE_MD5);
	Binary::Ptr encrypted = new Binary(data, Binary::SUBTYPE_ENCRYPTED);
	Binary::Ptr userDefined = new Binary(data, Binary::SUBTYPE_USER_DEFINED);

	doc->add("generic", generic);
	doc->add("function", function);
	doc->add("md5", md5);
	doc->add("encrypted", encrypted);
	doc->add("userDefined", userDefined);

	assertEqual(static_cast<int>(doc->get<Binary::Ptr>("generic")->subtype()), static_cast<int>(Binary::SUBTYPE_GENERIC));
	assertEqual(static_cast<int>(doc->get<Binary::Ptr>("function")->subtype()), static_cast<int>(Binary::SUBTYPE_FUNCTION));
	assertEqual(static_cast<int>(doc->get<Binary::Ptr>("md5")->subtype()), static_cast<int>(Binary::SUBTYPE_MD5));
	assertEqual(static_cast<int>(doc->get<Binary::Ptr>("encrypted")->subtype()), static_cast<int>(Binary::SUBTYPE_ENCRYPTED));
	assertEqual(static_cast<int>(doc->get<Binary::Ptr>("userDefined")->subtype()), static_cast<int>(Binary::SUBTYPE_USER_DEFINED));
}


void BSONTest::testObjectID()
{
	ObjectId oid("536aeebba081de6815000002");
	std::string str2 = oid.toString();
	assertTrue(str2 == "536aeebba081de6815000002");

	// Test in document
	Document::Ptr doc = new Document();
	ObjectId::Ptr oidPtr = new ObjectId("507f1f77bcf86cd799439011");
	doc->add("_id", oidPtr);

	assertTrue(doc->exists("_id"));
	ObjectId::Ptr retrieved = doc->get<ObjectId::Ptr>("_id");
	assertFalse(retrieved.isNull());
	assertEqual(retrieved->toString(), "507f1f77bcf86cd799439011");

	// Verify document toString quotes the ObjectId (valid JSON value)
	assertEqual(doc->toString(), R"({"_id":"507f1f77bcf86cd799439011"})");
}


void BSONTest::testObjectIDTimestamp()
{
	// Create ObjectId with known timestamp
	ObjectId oid("536aeebba081de6815000002");

	Poco::Timestamp ts = oid.timestamp();

	// Verify timestamp is extracted correctly
	// The first 4 bytes of 536aeebb represent the timestamp
	assertTrue(ts.epochTime() > 0);

	// Verify timestamp is reasonable (should be in the past)
	Poco::Timestamp now;
	assertTrue(ts < now);
}


void BSONTest::testRegularExpression()
{
	Document::Ptr doc = new Document();

	RegularExpression::Ptr regex1 = new RegularExpression("^test.*", "i");
	RegularExpression::Ptr regex2 = new RegularExpression("[a-z]+", "");

	doc->add("regex1", regex1);
	doc->add("regex2", regex2);

	assertTrue(doc->exists("regex1"));
	assertTrue(doc->exists("regex2"));

	RegularExpression::Ptr retrieved1 = doc->get<RegularExpression::Ptr>("regex1");
	RegularExpression::Ptr retrieved2 = doc->get<RegularExpression::Ptr>("regex2");

	assertFalse(retrieved1.isNull());
	assertFalse(retrieved2.isNull());

	assertEqual(retrieved1->getPattern(), "^test.*");
	assertEqual(retrieved1->getOptions(), "i");
	assertEqual(retrieved2->getPattern(), "[a-z]+");
	assertEqual(retrieved2->getOptions(), "");

	// Test setPattern and setOptions
	RegularExpression::Ptr regex3 = new RegularExpression();
	regex3->setPattern("\\d+");
	regex3->setOptions("g");

	assertEqual(regex3->getPattern(), "\\d+");
	assertEqual(regex3->getOptions(), "g");

	// Verify document toString quotes the regex (valid JSON value)
	Document::Ptr regexDoc = new Document();
	regexDoc->add("re"s, regex1);
	assertEqual(regexDoc->toString(), R"({"re":"/^test.*/i"})");
}


void BSONTest::testJavaScriptCode()
{
	Document::Ptr doc = new Document();

	JavaScriptCode::Ptr js1 = new JavaScriptCode();
	js1->setCode("function() { return 42; }");

	JavaScriptCode::Ptr js2 = new JavaScriptCode();
	js2->setCode("var x = 10; var y = 20; return x + y;");

	doc->add("js1", js1);
	doc->add("js2", js2);

	assertTrue(doc->exists("js1"));
	assertTrue(doc->exists("js2"));

	JavaScriptCode::Ptr retrieved1 = doc->get<JavaScriptCode::Ptr>("js1");
	JavaScriptCode::Ptr retrieved2 = doc->get<JavaScriptCode::Ptr>("js2");

	assertFalse(retrieved1.isNull());
	assertFalse(retrieved2.isNull());

	assertEqual(retrieved1->getCode(), "function() { return 42; }");
	assertEqual(retrieved2->getCode(), "var x = 10; var y = 20; return x + y;");

	// Verify document toString quotes the JavaScript code (valid JSON value)
	Document::Ptr jsDoc = new Document();
	jsDoc->add("code"s, js1);
	assertEqual(jsDoc->toString(), R"({"code":"function() { return 42; }"})");
}


void BSONTest::testDecimal128Specials()
{
	Decimal128 zero;
	assertFalse(zero.isNaN());
	assertFalse(zero.isInfinite());
	assertFalse(zero.isNegative());
	assertEqual(std::string("0"), zero.toString());

	Decimal128 negZero = Decimal128::negativeZero();
	assertTrue(negZero.isNegative());
	assertEqual(std::string("-0"), negZero.toString());

	Decimal128 nan = Decimal128::nan();
	assertTrue(nan.isNaN());
	assertEqual(std::string("NaN"), nan.toString());

	Decimal128 posInf = Decimal128::positiveInfinity();
	assertTrue(posInf.isInfinite());
	assertFalse(posInf.isNegative());
	assertEqual(std::string("Infinity"), posInf.toString());

	Decimal128 negInf = Decimal128::negativeInfinity();
	assertTrue(negInf.isInfinite());
	assertTrue(negInf.isNegative());
	assertEqual(std::string("-Infinity"), negInf.toString());
}


void BSONTest::testDecimal128FromString()
{
	struct Case
	{
		const char* in;
		const char* out;
	};
	const Case cases[] = {
		{"0",         "0"},
		{"-0",        "-0"},
		{"1",         "1"},
		{"-1",        "-1"},
		{"123",       "123"},
		{"-456",      "-456"},
		{"1.5",       "1.5"},
		{"-1.5",      "-1.5"},
		{"0.1",       "0.1"},
		{"0.0001",    "0.0001"},
		{"1E+5",      "1E+5"},
		{"1.234E+10", "1.234E+10"},
		{"-1E-3",     "-0.001"},
		// Trailing zeros are part of the canonical (coefficient, exponent)
		// pair per the BSON Decimal128 spec and must round-trip.
		{"100",       "100"},
		{"1.10",      "1.10"},
		{"0.10",      "0.10"},
		{"NaN",       "NaN"},
		{"Infinity",  "Infinity"},
		{"-Infinity", "-Infinity"},
		{"inf",       "Infinity"},
		{"-inf",      "-Infinity"}
	};

	for (const Case& c: cases)
	{
		Decimal128 v = Decimal128::fromString(c.in);
		assertEqual(std::string(c.out), v.toString());
	}

	const char* syntaxErrors[] = {
		"not a number",
		"1E",        // exponent indicator with no digits
		"1E+",       // exponent sign with no digits
		"1e-",
		"1.2.3"      // multiple decimal points
	};
	for (const char* bad: syntaxErrors)
	{
		try
		{
			Decimal128::fromString(bad);
			failmsg(std::string("expected SyntaxException for '") + bad + "'");
		}
		catch (const Poco::SyntaxException&) {}
	}

	const char* rangeErrors[] = {
		"12345678901234567890123456789012345",  // 35 digits, > 34 max
		"1E10000"                                // exponent above EXPONENT_MAX (6144)
	};
	for (const char* bad: rangeErrors)
	{
		try
		{
			Decimal128::fromString(bad);
			failmsg(std::string("expected RangeException for '") + bad + "'");
		}
		catch (const Poco::RangeException&) {}
	}
}


void BSONTest::testDecimal128RoundTrip()
{
	// Round-trip via toString -> fromString -> toString.
	Decimal128 a = Decimal128::fromString("3.14159265358979323846");
	Decimal128 b = Decimal128::fromString(a.toString());
	assertTrue(a == b);
	assertEqual(a.toString(), b.toString());

	// Raw construction round-trips the bits.
	Decimal128 raw(0x0102030405060708ULL, 0x3040000000000000ULL);
	Decimal128 raw2(raw.low(), raw.high());
	assertTrue(raw == raw2);
}


void BSONTest::testDecimal128SerializeDocument()
{
	// Serialize a Document containing a Decimal128 to BSON, then read it
	// back and verify the value survives the round trip.
	Document::Ptr doc = new Document();
	Decimal128::Ptr value = new Decimal128(Decimal128::fromString("3.14159265358979"));
	doc->add("pi"s, value);

	std::stringstream stream;
	Poco::BinaryWriter writer(stream, Poco::BinaryWriter::LITTLE_ENDIAN_BYTE_ORDER);
	doc->write(writer);
	writer.flush();

	Document::Ptr restored = new Document();
	Poco::BinaryReader reader(stream, Poco::BinaryReader::LITTLE_ENDIAN_BYTE_ORDER);
	restored->read(reader);

	assertTrue(restored->exists("pi"));
	Decimal128::Ptr round = restored->get<Decimal128::Ptr>("pi");
	assertFalse(round.isNull());
	assertEqual(value->toString(), round->toString());
	assertTrue(*value == *round);
}


void BSONTest::testMinKeyMaxKeySerializeDocument()
{
	Document::Ptr doc = new Document();
	doc->add("min"s, MinKey{});
	doc->add("max"s, MaxKey{});

	std::stringstream stream;
	Poco::BinaryWriter writer(stream, Poco::BinaryWriter::LITTLE_ENDIAN_BYTE_ORDER);
	doc->write(writer);
	writer.flush();

	Document::Ptr restored = new Document();
	Poco::BinaryReader reader(stream, Poco::BinaryReader::LITTLE_ENDIAN_BYTE_ORDER);
	restored->read(reader);

	assertTrue(restored->exists("min"));
	assertTrue(restored->exists("max"));
	assertTrue(restored->isType<MinKey>("min"));
	assertTrue(restored->isType<MaxKey>("max"));
}


void BSONTest::testAuthMechanismConstants()
{
	// Lock wire strings; these are part of the public API.
	assertEqual(std::string("SCRAM-SHA-1"), Database::AUTH_SCRAM_SHA1);
	assertEqual(std::string("SCRAM-SHA-256"), Database::AUTH_SCRAM_SHA256);
}


void BSONTest::testAuthSCRAM256RejectsNonAscii()
{
	// Non-ASCII password must throw NotImplementedException before any
	// network I/O (SASLprep is not implemented).
	Poco::MongoDB::Database db("test");
	Poco::MongoDB::Connection conn;
	const std::string nonAsciiPassword = "p\xC3\xA9ncil"; // "pencil" with e-acute

	try
	{
		db.authenticate(conn, "alice", nonAsciiPassword,
			Poco::MongoDB::Database::AUTH_SCRAM_SHA256);
		failmsg("expected NotImplementedException on non-ASCII password");
	}
	catch (const Poco::NotImplementedException&)
	{
	}
}


namespace
{
	class RecordingSocketFactory: public Poco::MongoDB::Connection::SocketFactory
	{
	public:
		bool secureRequested = false;
		bool called = false;

		Poco::Net::StreamSocket createSocket(const std::string& /*host*/,
			int /*port*/, Poco::Timespan /*connectTimeout*/, bool secure) override
		{
			called = true;
			secureRequested = secure;
			// We don't actually want to open a socket; throw to abort the
			// connect() call after the URI options have been parsed.
			throw Poco::NotImplementedException("Test factory does not connect");
		}
	};
}


void BSONTest::testConnectionURITlsAlias()
{
	// "tls=" and "ssl=" must flip the same internal secure flag.
	auto check = [this](const std::string& uri, bool expectSecure)
	{
		Poco::MongoDB::Connection conn;
		RecordingSocketFactory factory;
		try { conn.connect(uri, factory); }
		catch (const Poco::NotImplementedException&) {}
		assertTrue(factory.called);
		assertEqual(expectSecure, factory.secureRequested);
	};

	check("mongodb://localhost:27017/admin?tls=true", true);
	check("mongodb://localhost:27017/admin?ssl=true", true);
	check("mongodb://localhost:27017/admin",          false);
}


void BSONTest::testDocumentSerialization()
{
	Document::Ptr doc = new Document();
	doc->add("name"s, "John Doe"s);
	doc->add("age", static_cast<Poco::Int32>(30));
	doc->add("salary", 50000.50);
	doc->add("active", true);

	// Serialize to stream
	std::stringstream ss;
	Poco::BinaryWriter writer(ss, Poco::BinaryWriter::LITTLE_ENDIAN_BYTE_ORDER);
	doc->write(writer);

	// Verify data was written
	assertTrue(ss.str().length() > 0);

	// Deserialize from stream
	ss.seekg(0, std::ios::beg);
	Poco::BinaryReader reader(ss, Poco::BinaryReader::LITTLE_ENDIAN_BYTE_ORDER);
	Document::Ptr doc2 = new Document();
	doc2->read(reader);

	// Verify deserialized document
	assertEqual(doc2->get<std::string>("name"), "John Doe");
	assertEqual(doc2->get<Poco::Int32>("age"), 30);
	assertEqual(doc2->get<double>("salary"), 50000.50);
	assertEqual(doc2->get<bool>("active"), true);
}


void BSONTest::testDocumentDeserialization()
{
	// Create and serialize a document
	Document::Ptr original = new Document();
	original->add("string"s, "test"s);
	original->add("int32"s, static_cast<Poco::Int32>(42));
	original->add("int64"s, static_cast<Poco::Int64>(9876543210LL));
	original->add("double"s, 3.14);
	original->add("bool"s, false);
	original->add("null", NullValue());

	Poco::Timestamp ts;
	original->add("timestamp", ts);

	std::stringstream ss;
	Poco::BinaryWriter writer(ss, Poco::BinaryWriter::LITTLE_ENDIAN_BYTE_ORDER);
	original->write(writer);

	// Deserialize
	ss.seekg(0, std::ios::beg);
	Poco::BinaryReader reader(ss, Poco::BinaryReader::LITTLE_ENDIAN_BYTE_ORDER);
	Document::Ptr restored = new Document();
	restored->read(reader);

	// Verify all fields
	assertEqual(restored->size(), original->size());
	assertEqual(restored->get<std::string>("string"), "test");
	assertEqual(restored->get<Poco::Int32>("int32"), 42);
	assertEqual(restored->get<Poco::Int64>("int64"), 9876543210LL);
	assertEqual(restored->get<double>("double"), 3.14);
	assertEqual(restored->get<bool>("bool"), false);
	assertTrue(restored->isType<NullValue>("null"));
	// MongoDB timestamps have millisecond precision, not microsecond
	assertEqual(restored->get<Poco::Timestamp>("timestamp").epochTime(), ts.epochTime());
}


void BSONTest::testArraySerialization()
{
	Array::Ptr arr = new Array();
	arr->add("first"s);
	arr->add(static_cast<Poco::Int32>(42));
	arr->add(3.14);
	arr->add(true);

	// Serialize
	std::stringstream ss;
	Poco::BinaryWriter writer(ss, Poco::BinaryWriter::LITTLE_ENDIAN_BYTE_ORDER);
	arr->write(writer);

	// Deserialize
	ss.seekg(0, std::ios::beg);
	Poco::BinaryReader reader(ss, Poco::BinaryReader::LITTLE_ENDIAN_BYTE_ORDER);
	Array::Ptr restored = new Array();
	restored->read(reader);

	// Verify
	assertEqual(restored->size(), 4);
	assertEqual(restored->get<std::string>(0), "first");
	assertEqual(restored->get<Poco::Int32>(1), 42);
	assertEqual(restored->get<double>(2), 3.14);
	assertEqual(restored->get<bool>(3), true);
}


void BSONTest::testComplexDocumentSerialization()
{
	// Create complex nested structure
	Document::Ptr doc = new Document();
	doc->add("name"s, "Test Document"s);
	doc->add("version", static_cast<Poco::Int32>(1));

	// Add nested document
	Document::Ptr nested = new Document();
	nested->add("field1"s, "value1"s);
	nested->add("field2"s, static_cast<Poco::Int32>(100));
	doc->add("nested", nested);

	// Add array
	Array::Ptr arr = new Array();
	arr->add("item1"s);
	arr->add("item2"s);
	arr->add("item3"s);
	doc->add("items", arr);

	// Add binary
	Binary::Ptr bin = new Binary("binary data", Binary::SUBTYPE_GENERIC);
	doc->add("binary", bin);

	// Add ObjectId
	ObjectId::Ptr oid = new ObjectId("507f1f77bcf86cd799439011");
	doc->add("_id", oid);

	// Serialize
	std::stringstream ss;
	Poco::BinaryWriter writer(ss, Poco::BinaryWriter::LITTLE_ENDIAN_BYTE_ORDER);
	doc->write(writer);

	// Deserialize
	ss.seekg(0, std::ios::beg);
	Poco::BinaryReader reader(ss, Poco::BinaryReader::LITTLE_ENDIAN_BYTE_ORDER);
	Document::Ptr restored = new Document();
	restored->read(reader);

	// Verify root fields
	assertEqual("Test Document", restored->get<std::string>("name"));
	assertEqual(1, restored->get<Poco::Int32>("version"));

	// Verify nested document
	Document::Ptr restoredNested = restored->get<Document::Ptr>("nested");
	assertFalse(restoredNested.isNull());
	assertEqual("value1", restoredNested->get<std::string>("field1"));
	assertEqual(100, restoredNested->get<Poco::Int32>("field2"));

	// Verify array
	Array::Ptr restoredArr = restored->get<Array::Ptr>("items");
	assertFalse(restoredArr.isNull());
	assertEqual(3, restoredArr->size());
	assertEqual("item1", restoredArr->get<std::string>(0));
	assertEqual("item2", restoredArr->get<std::string>(1));
	assertEqual("item3", restoredArr->get<std::string>(2));

	// Verify binary
	Binary::Ptr restoredBin = restored->get<Binary::Ptr>("binary");
	assertFalse(restoredBin.isNull());
	assertEqual("binary data", restoredBin->toRawString());

	// Verify ObjectId
	ObjectId::Ptr restoredOid = restored->get<ObjectId::Ptr>("_id");
	assertFalse(restoredOid.isNull());
	assertEqual(restoredOid->toString(), "507f1f77bcf86cd799439011");
}


void BSONTest::testSimpleDocumentToString()
{
	Document::Ptr doc = new Document();
	doc->add("name"s, "John"s);
	doc->add("age"s, static_cast<Poco::Int32>(30));
	doc->add("active"s, true);

	std::string str = doc->toString();
	assertEqual(str, R"({"name":"John","age":30,"active":true})");

	// Test with indentation
	std::string strIndented = doc->toString(2);
	std::string expected = R"({
  "name" : "John",
  "age" : 30,
  "active" : true
})";
	assertEqual(strIndented, expected);
}


void BSONTest::testNestedDocumentToString()
{
	Document::Ptr doc = new Document();
	doc->add("name"s, "Parent"s);

	Document::Ptr child = new Document();
	child->add("name"s, "Child"s);
	child->add("value"s, static_cast<Poco::Int32>(42));

	doc->add("child"s, child);

	std::string str = doc->toString();
	assertEqual(str, R"({"name":"Parent","child":{"name":"Child","value":42}})");

	// Test with indentation
	std::string strIndented = doc->toString(2);
	std::string expected = R"({
  "name" : "Parent",
  "child" : {
    "name" : "Child",
    "value" : 42
  }
})";
	assertEqual(strIndented, expected);
}


void BSONTest::testDocumentWithArrayToString()
{
	Document::Ptr doc = new Document();
	doc->add("title"s, "Shopping List"s);

	Array::Ptr items = new Array();
	items->add("apples"s);
	items->add("oranges"s);
	items->add("bananas"s);

	doc->add("items"s, items);

	std::string str = doc->toString();
	assertEqual(str, R"({"title":"Shopping List","items":["apples","oranges","bananas"]})");

	// Test with indentation
	std::string strIndented = doc->toString(2);
	std::string expected = R"({
  "title" : "Shopping List",
  "items" : [
    "apples",
    "oranges",
    "bananas"
  ]
})";
	assertEqual(strIndented, expected);
}


void BSONTest::testComplexDocumentToString()
{
	Document::Ptr doc = new Document();
	doc->add("name"s, "Complex Document"s);
	doc->add("version"s, static_cast<Poco::Int32>(1));
	doc->add("enabled"s, true);
	doc->add("score"s, 98.5);

	// Add nested document
	Document::Ptr metadata = new Document();
	metadata->add("author"s, "Test User"s);
	metadata->add("created"s, static_cast<Poco::Int64>(1234567890LL));
	doc->add("metadata"s, metadata);

	// Add array
	Array::Ptr tags = new Array();
	tags->add("test"s);
	tags->add("sample"s);
	tags->add("bson"s);
	doc->add("tags"s, tags);

	// Add nested array in nested document
	Document::Ptr config = new Document();
	Array::Ptr options = new Array();
	options->add("option1"s);
	options->add("option2"s);
	config->add("options"s, options);
	doc->add("config"s, config);

	std::string str = doc->toString();
	assertEqual(str, R"({"name":"Complex Document","version":1,"enabled":true,"score":98.5,"metadata":{"author":"Test User","created":1234567890},"tags":["test","sample","bson"],"config":{"options":["option1","option2"]}})");

	// Test with indentation
	std::string strIndented = doc->toString(2);
	std::string expected = R"({
  "name" : "Complex Document",
  "version" : 1,
  "enabled" : true,
  "score" : 98.5,
  "metadata" : {
    "author" : "Test User",
    "created" : 1234567890
  },
  "tags" : [
    "test",
    "sample",
    "bson"
  ],
  "config" : {
    "options" : [
      "option1",
      "option2"
    ]
  }
})";
	assertEqual(strIndented, expected);
}


void BSONTest::testToStringIndentation()
{
	Document::Ptr doc = new Document();
	doc->add("field1"s, "value1"s);

	Document::Ptr nested1 = new Document();
	nested1->add("field2"s, "value2"s);

	Document::Ptr nested2 = new Document();
	nested2->add("field3"s, "value3"s);
	nested1->add("nested"s, nested2);

	doc->add("nested"s, nested1);

	// Test with no indentation
	std::string str0 = doc->toString(0);
	assertEqual(str0, R"({"field1":"value1","nested":{"field2":"value2","nested":{"field3":"value3"}}})");

	// Test with 2-space indentation
	std::string str2 = doc->toString(2);
	std::string expected2 = R"({
  "field1" : "value1",
  "nested" : {
    "field2" : "value2",
    "nested" : {
      "field3" : "value3"
    }
  }
})";
	assertEqual(str2, expected2);

	// Test with 4-space indentation
	std::string str4 = doc->toString(4);
	std::string expected4 = R"({
    "field1" : "value1",
    "nested" : {
      "field2" : "value2",
      "nested" : {
        "field3" : "value3"
      }
    }
  })";
	assertEqual(str4, expected4);
}


void BSONTest::testArrayToString()
{
	Array::Ptr arr = new Array();
	arr->add("first"s);
	arr->add(static_cast<Poco::Int32>(42));
	arr->add(3.14);
	arr->add(true);
	arr->add(false);

	std::string str = arr->toString();
	assertEqual(str, R"(["first",42,3.14,true,false])");

	// Test with indentation
	std::string strIndented = arr->toString(2);
	std::string expected = R"([
  "first",
  42,
  3.14,
  true,
  false
])";
	assertEqual(strIndented, expected);

	// Test with nested array
	Array::Ptr outer = new Array();
	outer->add("outer"s);

	Array::Ptr inner = new Array();
	inner->add("inner1"s);
	inner->add("inner2"s);

	outer->add(inner);

	std::string strNested = outer->toString();
	assertEqual(strNested, R"(["outer",["inner1","inner2"]])");

	// Test nested array with indentation
	std::string strNestedIndented = outer->toString(2);
	std::string expectedNested = R"([
  "outer",
  [
    "inner1",
    "inner2"
  ]
])";
	assertEqual(strNestedIndented, expectedNested);
}


void BSONTest::testGetNonExistent()
{
	Document::Ptr doc = new Document();
	doc->add("existing", "value"s);

	// Test get throws NotFoundException
	try
	{
		(void)doc->get<std::string>("nonexistent");
		fail("Should have thrown NotFoundException");
	}
	catch (Poco::NotFoundException&)
	{
		// Expected
	}

	// Test get with default doesn't throw
	std::string value = doc->get<std::string>("nonexistent", "default");
	assertEqual(value, "default");

	// Test getInteger throws NotFoundException
	try
	{
		(void)doc->getInteger("nonexistent");
		fail("Should have thrown NotFoundException");
	}
	catch (Poco::NotFoundException&)
	{
		// Expected
	}
}


void BSONTest::testBadCast()
{
	Document::Ptr doc = new Document();
	doc->add("string"s, "not a number"s);
	doc->add("number"s, static_cast<Poco::Int32>(42));

	// Try to get string as number
	try
	{
		(void)doc->get<Poco::Int32>("string");
		fail("Should have thrown BadCastException");
	}
	catch (Poco::BadCastException&)
	{
		// Expected
	}

	// Try to get number as string
	try
	{
		(void)doc->get<std::string>("number");
		fail("Should have thrown BadCastException");
	}
	catch (Poco::BadCastException&)
	{
		// Expected
	}

	// Test get with default doesn't throw on type mismatch
	Poco::Int32 num = doc->get<Poco::Int32>("string", 999);
	assertEqual(num, 999);
}


void BSONTest::testInvalidObjectID()
{
	// Test invalid hex string (wrong length)
	try
	{
		ObjectId oid("invalid");
		fail("Should have thrown exception for invalid ObjectId");
	}
	catch (Poco::Exception&)
	{
		// Expected
	}

	// Test invalid hex string (correct length but invalid chars)
	try
	{
		ObjectId oid("zzzzzzzzzzzzzzzzzzzzzzzz");
		fail("Should have thrown exception for invalid ObjectId");
	}
	catch (Poco::Exception&)
	{
		// Expected
	}

	// Test valid ObjectId
	ObjectId validOid("507f1f77bcf86cd799439011");
	assertEqual(validOid.toString(), "507f1f77bcf86cd799439011");
}


void BSONTest::testEmptyDocument()
{
	Document::Ptr doc = new Document();

	assertTrue(doc->empty());
	assertEqual(doc->size(), 0);

	// Test serialization of empty document
	std::stringstream ss;
	Poco::BinaryWriter writer(ss, Poco::BinaryWriter::LITTLE_ENDIAN_BYTE_ORDER);
	doc->write(writer);

	// Deserialize
	ss.seekg(0, std::ios::beg);
	Poco::BinaryReader reader(ss, Poco::BinaryReader::LITTLE_ENDIAN_BYTE_ORDER);
	Document::Ptr restored = new Document();
	restored->read(reader);

	assertTrue(restored->empty());
	assertEqual(restored->size(), 0);
}


namespace
{
	std::string element(char type, const std::string& name, const std::string& value)
	{
		return std::string(1, type) + name + std::string(1, '\0') + value;
	}

	std::string bsonDocument(const std::string& elements)
	{
		return int32LE(static_cast<Poco::Int32>(4 + elements.size() + 1)) + elements + std::string(1, '\0');
	}

	std::string opMsgWithLength(Poco::Int32 length, const std::string& payload, Poco::Int32 opCode = 2013)
	{
		return int32LE(length) + int32LE(1) + int32LE(0) + int32LE(opCode) + payload;
	}

	std::string opMsg(const std::string& sections, Poco::UInt32 flags = 0, Poco::Int32 opCode = 2013)
	{
		const std::string payload = int32LE(static_cast<Poco::Int32>(flags)) + sections;
		return opMsgWithLength(static_cast<Poco::Int32>(16 + payload.size()), payload, opCode);
	}

	std::string bodySection(const std::string& document)
	{
		return std::string(1, '\0') + document;
	}

	std::string sequenceSection(const std::string& identifier, const std::string& documents)
	{
		return std::string(1, '\1') + int32LE(static_cast<Poco::Int32>(4 + identifier.size() + 1 + documents.size())) +
			identifier + std::string(1, '\0') + documents;
	}

	template <typename E>
	std::string readFailure(const std::string& message)
		/// Returns an empty string if reading the message throws exactly E,
		/// otherwise a description of what happened.
	{
		OpMsgMessage response;
		std::istringstream istr(message);
		try
		{
			response.read(istr);
		}
		catch (const Poco::Exception& e)
		{
			if (typeid(e) == typeid(E))
				return std::string();
			return std::string("unexpected ") + e.name() + ": " + e.displayText();
		}
		return "accepted"s;
	}
}


void BSONTest::testOpMsgReadBodyAbove16MB()
{
	// The largest two-document getMore batch the server builds (16,777,156 bytes of
	// documents) with a namespace of 255 bytes: the reply body is 16,777,484 bytes.
	Document body;
	Document& cursor = body.addNewDocument("cursor"s);
	Array& batch = cursor.addNewArray("nextBatch"s);
	batch.add(sizedDocument(0, 22));
	batch.add(sizedDocument(1, 16777156 - 22));
	cursor.add("id"s, static_cast<Poco::Int64>(0));
	cursor.add("ns"s, std::string(255, 'n'));
	body.add("ok"s, 1.0);
	const std::string bodyBytes = serialize(body);
	assertEqual(16777484, static_cast<int>(bodyBytes.size()));

	OpMsgMessage response;
	std::istringstream istr(opMsg(bodySection(bodyBytes)));
	response.read(istr);
	assertTrue(response.responseOk());
	assertEqual(2, static_cast<int>(response.documents().size()));
	assertEqual(16777156 - 44, static_cast<int>(response.documents()[1]->get<Binary::Ptr>("p"s)->buffer().size()));
}


void BSONTest::testOpMsgReadDocumentAbove16MB()
{
	// Oplog entries and documents stored with allowDocumentsGreaterThanMaxUserSize reach
	// 16 MiB + 16 KiB; a standalone MongoDB 8.2 with that parameter stores and returns
	// documents of 20 MiB.
	for (const Poco::Int32 size: {16793600, 20 * 1024 * 1024})
	{
		Document body;
		Document& cursor = body.addNewDocument("cursor"s);
		cursor.addNewArray("firstBatch"s).add(sizedDocument(1, size));
		cursor.add("id"s, static_cast<Poco::Int64>(0));
		cursor.add("ns"s, "db.c"s);
		body.add("ok"s, 1.0);

		OpMsgMessage response;
		std::istringstream istr(opMsg(bodySection(serialize(body))));
		response.read(istr);
		assertEqual(1, static_cast<int>(response.documents().size()));
		assertEqual(size - 22, static_cast<int>(response.documents()[0]->get<Binary::Ptr>("p"s)->buffer().size()));
	}

	Document body;
	body.add("s"s, std::string(32 * 1024 * 1024, 'x'));
	body.add("ok"s, 1.0);
	OpMsgMessage response;
	std::istringstream istr(opMsg(bodySection(serialize(body))));
	response.read(istr);
	assertEqual(32 * 1024 * 1024, static_cast<int>(response.body().get<std::string>("s"s).size()));
}


void BSONTest::testOpMsgReadSectionOrderAndChecksum()
{
	Document one;
	one.add("a"s, 1);
	const std::string doc = serialize(one);
	Document ok;
	ok.add("ok"s, 1.0);
	const std::string body = bodySection(serialize(ok));

	// A document sequence may precede the body.
	OpMsgMessage response;
	std::istringstream istr(opMsg(sequenceSection("documents"s, doc + doc) + body));
	response.read(istr);
	assertTrue(response.responseOk());
	assertEqual(2, static_cast<int>(response.documents().size()));

	// The checksum follows the sections and is not parsed as one.
	OpMsgMessage withChecksum;
	std::istringstream istr2(opMsg(body + int32LE(0x12345678), OpMsgMessage::MSG_CHECKSUM_PRESENT));
	withChecksum.read(istr2);
	assertTrue(withChecksum.responseOk());
}


void BSONTest::testDocumentReadLargeStandalone()
{
	std::istringstream istr(serialize(*sizedDocument(1, 20 * 1024 * 1024)));
	Poco::BinaryReader reader(istr, Poco::BinaryReader::LITTLE_ENDIAN_BYTE_ORDER);
	Document copy;
	copy.read(reader);
	assertEqual(20 * 1024 * 1024 - 22, static_cast<int>(copy.get<Binary::Ptr>("p"s)->buffer().size()));

	std::istringstream tooLarge(int32LE(MAX_MESSAGE_SIZE_BYTES + 1) + std::string(1, '\0'));
	Poco::BinaryReader tooLargeReader(tooLarge, Poco::BinaryReader::LITTLE_ENDIAN_BYTE_ORDER);
	Document rejected;
	try
	{
		rejected.read(tooLargeReader);
		fail("document larger than MAX_MESSAGE_SIZE_BYTES accepted");
	}
	catch (Poco::DataFormatException&)
	{
	}
}


void BSONTest::testOpMsgReadRejectsMalformedLengths()
{
	using Poco::DataFormatException;
	const std::string none;
	const std::string nul(1, '\0');
	const std::string int32Element = element('\x10', "i"s, int32LE(1));
	auto reply = [](const std::string& document) { return opMsg(bodySection(document)); };

	// Document size below the minimum, beyond the message, beyond the parent document
	assertEqual(none, readFailure<DataFormatException>(reply(int32LE(4) + nul)));
	assertEqual(none, readFailure<DataFormatException>(reply(int32LE(100) + int32Element + nul)));
	assertEqual(none, readFailure<DataFormatException>(reply(bsonDocument(element('\x03', "d"s, int32LE(50) + nul)))));
	// Terminator before the declared size, or missing at it
	assertEqual(none, readFailure<DataFormatException>(reply(int32LE(13) + int32Element + nul + nul)));
	assertEqual(none, readFailure<DataFormatException>(reply(int32LE(12) + int32Element + "X"s)));
	// String of length 0, beyond its document, of 2 GiB, without terminating null character
	assertEqual(none, readFailure<DataFormatException>(reply(bsonDocument(element('\x02', "s"s, int32LE(0) + nul)))));
	assertEqual(none, readFailure<DataFormatException>(reply(bsonDocument(element('\x02', "s"s, int32LE(100) + "ab"s + nul)))));
	assertEqual(none, readFailure<DataFormatException>(reply(bsonDocument(element('\x02', "s"s, int32LE(0x7FFFFFFF) + "ab"s + nul)))));
	assertEqual(none, readFailure<DataFormatException>(reply(bsonDocument(element('\x02', "s"s, int32LE(3) + "abc"s)))));
	// Binary of negative length or beyond its document
	assertEqual(none, readFailure<DataFormatException>(reply(bsonDocument(element('\x05', "b"s, int32LE(-1) + nul)))));
	assertEqual(none, readFailure<DataFormatException>(reply(bsonDocument(element('\x05', "b"s, int32LE(100) + nul + "xy"s)))));
	// Element name and regular expression without terminating null character
	assertEqual(none, readFailure<DataFormatException>(reply(bsonDocument("\x10" "abc"s))));
	assertEqual(none, readFailure<DataFormatException>(reply(bsonDocument(element('\x0B', "r"s, "pattern"s)))));
	// Value shorter than its type
	assertEqual(none, readFailure<DataFormatException>(reply(bsonDocument(element('\x12', "l"s, int32LE(1))))));
}


void BSONTest::testOpMsgReadRejectsBadFraming()
{
	// Only what would make parsing read past the message or allocate without bound is
	// rejected; replies are otherwise expected to be valid.
	using Poco::DataFormatException;
	const std::string none;
	const std::string nul(1, '\0');
	Document ok;
	ok.add("ok"s, 1.0);
	const std::string doc = serialize(ok);
	const std::string body = bodySection(doc);

	// Message length below the header and the flags, negative, above MAX_MESSAGE_SIZE_BYTES
	assertEqual(none, readFailure<DataFormatException>(opMsgWithLength(19, int32LE(0) + body)));
	assertEqual(none, readFailure<DataFormatException>(opMsgWithLength(-1, int32LE(0) + body)));
	assertEqual(none, readFailure<DataFormatException>(opMsgWithLength(MAX_MESSAGE_SIZE_BYTES + 1, int32LE(0) + body)));
	// Stream ends before the declared length
	assertEqual(none, readFailure<Poco::IOException>(opMsgWithLength(100, int32LE(0) + body)));
	// Unknown section kind, whose size cannot be known
	assertEqual(none, readFailure<DataFormatException>(opMsg(body + std::string(1, '\x02') + doc)));
	// Document sequence too small, beyond the message, not filled by its documents,
	// with an unterminated identifier
	assertEqual(none, readFailure<DataFormatException>(opMsg(body + std::string(1, '\x01') + int32LE(4))));
	assertEqual(none, readFailure<DataFormatException>(opMsg(body + std::string(1, '\x01') + int32LE(1000) + "documents"s + nul + doc)));
	assertEqual(none, readFailure<DataFormatException>(opMsg(body + sequenceSection("documents"s, doc + "xyz"s))));
	assertEqual(none, readFailure<DataFormatException>(opMsg(body + std::string(1, '\x01') + int32LE(8) + "docu"s)));
	// The body, or the last document of a sequence, reaching into the checksum
	assertEqual(none, readFailure<DataFormatException>(opMsg(body, OpMsgMessage::MSG_CHECKSUM_PRESENT)));
	assertEqual(none, readFailure<DataFormatException>(opMsg(body + sequenceSection("documents"s, doc), OpMsgMessage::MSG_CHECKSUM_PRESENT)));
	// An opcode other than OP_MSG
	assertEqual(none, readFailure<DataFormatException>(opMsg(body, 0, 2012)));
	// No section at all, and a document sequence without a body section
	assertEqual(none, readFailure<DataFormatException>(opMsg(""s)));
	assertEqual(none, readFailure<DataFormatException>(opMsg(sequenceSection("documents"s, doc))));
	// More than one body section
	assertEqual(none, readFailure<DataFormatException>(opMsg(body + body)));
	// A document sequence kind byte followed by too few bytes for its size
	assertEqual(none, readFailure<DataFormatException>(opMsg(body + "\x01"s)));
	assertEqual(none, readFailure<DataFormatException>(opMsg(body + "\x01"s + "a"s)));
	assertEqual(none, readFailure<DataFormatException>(opMsg(body + "\x01"s + "ab"s)));
	// An identifier that is terminated outside its own section
	assertEqual(none, readFailure<DataFormatException>(opMsg("\x01"s + int32LE(13) + "documents"s + nul + body)));
}


void BSONTest::testOpMsgReadClearsMessage()
{
	// A reused message must never hold parts of two responses.
	Document one;
	one.add("a"s, 1);
	Document ok;
	ok.add("ok"s, 1.0);

	OpMsgMessage response;
	response.body().add("stale"s, 2);
	response.documents().push_back(new Document());

	std::istringstream istr(opMsg(bodySection(serialize(ok)) + sequenceSection("documents"s, serialize(one))));
	response.read(istr);

	assertEqual(static_cast<std::size_t>(1), response.body().size());
	assertTrue(response.responseOk());
	assertFalse(response.body().exists("stale"s));
	assertEqual(static_cast<std::size_t>(1), response.documents().size());
	assertEqual(1, response.documents()[0]->get<Poco::Int32>("a"s));
}


void BSONTest::testDocumentReadStandaloneBounds()
{
	// Every length is checked against the bytes left in its document, not against the
	// stream, which here continues with zeros: without the checks each document would
	// be read to the end of a longer one and accepted.
	const std::string zeros(64, '\0');
	const std::string int32Element = element('\x10', "i"s, int32LE(1));
	std::vector<std::string> documents = {
		// String, binary and int64 reaching past the document
		bsonDocument(element('\x02', "s"s, int32LE(20) + "ab"s + std::string(1, '\0'))),
		bsonDocument(element('\x05', "b"s, int32LE(20) + std::string(1, '\0') + "xy"s)),
		bsonDocument(element('\x12', "l"s, int32LE(1))),
		// Name without its null character inside the document
		bsonDocument("\x10" "abc"s),
		// Declared size below the minimum, or negative
		int32LE(0) + "\0"s,
		int32LE(4) + "\0"s,
		int32LE(-1000) + "\0"s,
		// String length reaching past a document that ends right after it
		int32LE(14) + "\x02"s + "s\0"s + int32LE(3),
		// Null value whose name is not terminated inside the document
		bsonDocument("\x0A" "abc"s)
	};
	// Nested document and array of 19 bytes in a parent that holds only their first
	// element; the parent's terminator position holds the type of their second element.
	for (const char type: {'\x03', '\x04'})
	{
		documents.push_back(int32LE(19) + std::string(1, type) + "d"s + std::string(1, '\0') + int32LE(19) + int32Element + "\x10"s +
			"j"s + std::string(1, '\0') + int32LE(2) + std::string(1, '\0'));
	}
	for (const auto& data: documents)
	{
		std::istringstream istr(data + zeros);
		Poco::BinaryReader reader(istr, Poco::BinaryReader::LITTLE_ENDIAN_BYTE_ORDER);
		Document doc;
		try
		{
			doc.read(reader);
			fail("document read past its end: " + doc.toString());
		}
		catch (Poco::DataFormatException&)
		{
		}
	}

	// Stream ending inside a string that fits its document
	std::istringstream truncated(int32LE(15) + "\x02"s + "s\0"s + int32LE(3));
	Poco::BinaryReader truncatedReader(truncated, Poco::BinaryReader::LITTLE_ENDIAN_BYTE_ORDER);
	Document doc;
	try
	{
		doc.read(truncatedReader);
		fail("truncated document accepted");
	}
	catch (Poco::DataFormatException&)
	{
	}
}


void BSONTest::testDocumentReadFailureKeepsIndexConsistent()
{
	// The name index is invalidated before the elements are read, so that a read
	// that throws does not leave the index describing a different element list.
	Document d;
	d.add("a"s, 1);
	(void) d.getInteger("a"s);

	std::istringstream istr(bsonDocument(element('\x10', "b"s, int32LE(2)) + element('\x20', "c"s, ""s)));
	Poco::BinaryReader reader(istr, Poco::BinaryReader::LITTLE_ENDIAN_BYTE_ORDER);
	try
	{
		d.read(reader);
		fail("element of an unsupported type accepted");
	}
	catch (Poco::NotImplementedException&)
	{
	}

	assertTrue(d.exists("b"s) == !d.get("b"s).isNull());
	assertTrue(d.exists("c"s) == !d.get("c"s).isNull());
}


void BSONTest::testDocumentRemoveDuplicateName()
{
	// A document read from a stream may hold a name twice: remove() takes the
	// first occurrence and get() then finds the second.
	std::istringstream istr(bsonDocument(element('\x10', "a"s, int32LE(1)) + element('\x10', "a"s, int32LE(2))));
	Poco::BinaryReader reader(istr, Poco::BinaryReader::LITTLE_ENDIAN_BYTE_ORDER);
	Document d;
	d.read(reader);
	assertEqual(static_cast<std::size_t>(2), d.size());
	assertEqual(1, d.get<Poco::Int32>("a"s));

	assertTrue(d.remove("a"s));
	assertTrue(d.exists("a"s));
	assertEqual(2, d.get<Poco::Int32>("a"s));

	assertTrue(d.remove("a"s));
	assertFalse(d.exists("a"s));
	assertTrue(d.get("a"s).isNull());
	assertFalse(d.remove("a"s));
}


void BSONTest::testDocumentReadDepth()
{
	// Nesting is limited, so that a document cannot exhaust the stack.
	for (const char type: {'\x03', '\x04'})
	{
		std::string atLimit = bsonDocument(""s);
		for (int i = 0; i < 512; ++i)
			atLimit = bsonDocument(element(type, "d"s, atLimit));
		std::istringstream istr(atLimit);
		Poco::BinaryReader reader(istr, Poco::BinaryReader::LITTLE_ENDIAN_BYTE_ORDER);
		Document doc;
		doc.read(reader);

		std::istringstream tooDeep(bsonDocument(element(type, "d"s, atLimit)));
		Poco::BinaryReader tooDeepReader(tooDeep, Poco::BinaryReader::LITTLE_ENDIAN_BYTE_ORDER);
		Document rejected;
		try
		{
			rejected.read(tooDeepReader);
			fail("document nested 513 levels deep accepted");
		}
		catch (Poco::DataFormatException&)
		{
		}
	}
}


void BSONTest::testDocumentReadWithTextEncoding()
{
	// BSONReader and BSONWriter copy the given reader and writer; the copies
	// share their TextConverter instead of deleting it a second time.
	Document doc;
	doc.add("i"s, 1);
	doc.add("j"s, 2);
	Poco::UTF8Encoding encoding;

	std::istringstream istr(serialize(doc));
	Poco::BinaryReader reader(istr, encoding, Poco::BinaryReader::LITTLE_ENDIAN_BYTE_ORDER);
	Document copy;
	copy.read(reader);
	assertEqual(2, copy.get<Poco::Int32>("j"s));

	std::ostringstream ostr;
	const Poco::BinaryWriter writer(ostr, encoding, Poco::BinaryWriter::LITTLE_ENDIAN_BYTE_ORDER);
	BSONWriter(writer).write(Document::Ptr(new Document(doc)));
	assertEqual(serialize(doc), ostr.str());
}


void BSONTest::testDateTimeOutsideTimestampRange()
{
	// A BSON datetime holds milliseconds; values outside the range Poco::DateTime
	// accepts are clamped, so that formatting the parsed document cannot fail.
	const Poco::Timestamp::TimeVal minTime = Poco::DateTime(-4713, 1, 1).timestamp().epochMicroseconds();
	const Poco::Timestamp::TimeVal maxTime = Poco::DateTime(9999, 12, 31, 23, 59, 59, 999, 999).timestamp().epochMicroseconds();
	const struct
	{
		Poco::Int64 milliseconds;
		Poco::Timestamp::TimeVal microseconds;
	} values[] = {
		{std::numeric_limits<Poco::Int64>::max(), maxTime},
		{std::numeric_limits<Poco::Int64>::min(), minTime},
		{Poco::Timestamp::TIMEVAL_MAX / 1000, maxTime},
		{-1500, -1500000},
		{1700000000123, 1700000000123000},
	};
	for (const auto& value: values)
	{
		std::istringstream istr(bsonDocument(element('\x09', "t"s, int64LE(value.milliseconds))));
		Poco::BinaryReader reader(istr, Poco::BinaryReader::LITTLE_ENDIAN_BYTE_ORDER);
		Document doc;
		doc.read(reader);
		assertEqual(value.microseconds, doc.get<Poco::Timestamp>("t"s).epochMicroseconds());
		// Formatting must work for every value, including the clamped extremes.
		assertFalse(doc.toString().empty());
	}

	std::istringstream high(bsonDocument(element('\x09', "t"s, int64LE(std::numeric_limits<Poco::Int64>::max()))));
	Poco::BinaryReader highReader(high, Poco::BinaryReader::LITTLE_ENDIAN_BYTE_ORDER);
	Document highDoc;
	highDoc.read(highReader);
	assertEqual(9999, Poco::DateTime(highDoc.get<Poco::Timestamp>("t"s)).year());

	std::istringstream low(bsonDocument(element('\x09', "t"s, int64LE(std::numeric_limits<Poco::Int64>::min()))));
	Poco::BinaryReader lowReader(low, Poco::BinaryReader::LITTLE_ENDIAN_BYTE_ORDER);
	Document lowDoc;
	lowDoc.read(lowReader);
	assertEqual(-4713, Poco::DateTime(lowDoc.get<Poco::Timestamp>("t"s)).year());
}


void BSONTest::testRegularExpressionAndJavaScriptRoundTrip()
{
	Document doc;
	doc.add("r"s, RegularExpression::Ptr(new RegularExpression("^a.*z$"s, "im"s)));
	JavaScriptCode::Ptr code = new JavaScriptCode();
	code->setCode("function() { return 1; }"s);
	doc.add("c"s, code);

	std::istringstream istr(serialize(doc));
	Poco::BinaryReader reader(istr, Poco::BinaryReader::LITTLE_ENDIAN_BYTE_ORDER);
	Document copy;
	copy.read(reader);
	assertEqual("^a.*z$"s, copy.get<RegularExpression::Ptr>("r"s)->getPattern());
	assertEqual("im"s, copy.get<RegularExpression::Ptr>("r"s)->getOptions());
	assertEqual("function() { return 1; }"s, copy.get<JavaScriptCode::Ptr>("c"s)->getCode());
}


void BSONTest::testBinaryReadTruncated()
{
	// The length of a binary that is not there must not size the buffer.
	for (const std::string& data: {""s, "\xff\xff\xff"s})
	{
		std::istringstream istr(data);
		Poco::BinaryReader reader(istr, Poco::BinaryReader::LITTLE_ENDIAN_BYTE_ORDER);
		Binary::Ptr binary = new Binary();
		try
		{
			BSONReader(reader).read(binary);
			fail("truncated binary accepted");
		}
		catch (Poco::DataFormatException&)
		{
		}
		assertEqual(0, static_cast<int>(binary->buffer().size()));
	}
}


void BSONTest::testConnectionClosedAfterUnreadableReply()
{
	// The rest of a reply that cannot be read may still arrive, so the connection is
	// closed, and a pool does not hand it out again.
	Document ok;
	ok.add("ok"s, 1.0);
	const std::string replies[] = {
		// Stream ends before the declared length
		opMsgWithLength(100, int32LE(0)),
		// Unknown section kind
		opMsg(bodySection(serialize(ok)) + "\x02"s)
	};
	for (const auto& reply: replies)
	{
		Poco::Net::ServerSocket server(Poco::Net::SocketAddress("127.0.0.1"s, 0));
		Connection::Ptr connection = new Connection("127.0.0.1"s, server.address().port());
		Poco::Net::StreamSocket peer = server.acceptConnection();
		peer.sendBytes(reply.data(), static_cast<int>(reply.size()));
		peer.shutdownSend();

		Database db("db"s);
		Poco::SharedPtr<OpMsgMessage> request = db.createOpMsgMessage();
		request->setCommandName(OpMsgMessage::CMD_PING);
		OpMsgMessage response;
		assertTrue(connection->isConnected());
		try
		{
			connection->sendRequest(*request, response);
			fail("unreadable reply accepted");
		}
		catch (Poco::Exception&)
		{
		}
		assertFalse(connection->isConnected());

		Poco::PoolableObjectFactory<Connection, Connection::Ptr> factory(server.address().toString());
		assertFalse(factory.validateObject(connection));
	}
}


void BSONTest::testConnectionKeptAfterUnsupportedType()
{
	// A complete reply that holds an element Poco cannot represent leaves the socket
	// in sync, so the connection stays usable.
	const std::string reply = opMsg(bodySection(bsonDocument(
		element('\x0E', "s"s, int32LE(2) + "a\0"s) + element('\x10', "ok"s, int32LE(1)))));

	Poco::Net::ServerSocket server(Poco::Net::SocketAddress("127.0.0.1"s, 0));
	Connection::Ptr connection = new Connection("127.0.0.1"s, server.address().port());
	Poco::Net::StreamSocket peer = server.acceptConnection();
	peer.sendBytes(reply.data(), static_cast<int>(reply.size()));

	Database db("db"s);
	Poco::SharedPtr<OpMsgMessage> request = db.createOpMsgMessage();
	request->setCommandName(OpMsgMessage::CMD_PING);
	OpMsgMessage response;
	try
	{
		connection->sendRequest(*request, response);
		fail("unsupported element type accepted");
	}
	catch (Poco::NotImplementedException&)
	{
	}
	assertTrue(connection->isConnected());
}


void BSONTest::testLargeDocumentRemoveAndAdd()
{
	// Large enough for remove() and addElement() to work through the name index.
	constexpr int count = 300;
	constexpr int changes = 50;
	Document doc;
	for (int i = 0; i < count; ++i)
		doc.add("n"s + std::to_string(i), i);
	assertFalse(doc.get("n0"s).isNull());

	for (int i = 0; i < changes; ++i)
	{
		assertTrue(doc.remove("n"s + std::to_string(i)));
		doc.add("x"s + std::to_string(i), i);
	}

	for (int i = 0; i < changes; ++i)
	{
		assertTrue(doc.get("n"s + std::to_string(i)).isNull());
		assertEqual(i, doc.get<Poco::Int32>("x"s + std::to_string(i)));
	}
	for (int i = changes; i < count; ++i)
		assertEqual(i, doc.get<Poco::Int32>("n"s + std::to_string(i)));
	assertEqual(static_cast<std::size_t>(count), doc.size());
}


void BSONTest::testBSONReaderBounds()
{
	// The public readers bound every length, also outside a document.
	const std::string strings[] = {
		// Shorter than its length, without a terminator, of 2 GiB
		int32LE(8) + "abcdef\0"s,
		int32LE(3) + "abc"s,
		int32LE(0x7FFFFFFF) + "ab"s
	};
	for (const auto& data: strings)
	{
		std::istringstream istr(data);
		Poco::BinaryReader reader(istr, Poco::BinaryReader::LITTLE_ENDIAN_BYTE_ORDER);
		std::string value;
		try
		{
			BSONReader(reader).read(value);
			fail("malformed BSON string accepted: " + value);
		}
		catch (Poco::DataFormatException&)
		{
		}
	}

	const std::string binaries[] = {
		// Longer than a message, and shorter than its length
		int32LE(MAX_MESSAGE_SIZE_BYTES + 1) + "\0ab"s,
		int32LE(16) + "\0abc"s
	};
	for (const auto& data: binaries)
	{
		std::istringstream istr(data);
		Poco::BinaryReader reader(istr, Poco::BinaryReader::LITTLE_ENDIAN_BYTE_ORDER);
		Binary::Ptr binary;
		try
		{
			BSONReader(reader).read(binary);
			fail("malformed BSON binary accepted");
		}
		catch (Poco::DataFormatException&)
		{
		}
	}

	// A null target is allocated when the binary is complete.
	std::istringstream complete(int32LE(2) + "\0ab"s);
	Poco::BinaryReader completeReader(complete, Poco::BinaryReader::LITTLE_ENDIAN_BYTE_ORDER);
	Binary::Ptr binary;
	BSONReader(completeReader).read(binary);
	assertFalse(binary.isNull());
	assertEqual("ab"s, binary->toRawString());

	// A cstring that is not terminated before the end of the stream
	std::istringstream cstring("abc"s);
	Poco::BinaryReader cstringReader(cstring, Poco::BinaryReader::LITTLE_ENDIAN_BYTE_ORDER);
	BSONReader bsonReader(cstringReader);
	try
	{
		(void) bsonReader.readCString();
		fail("unterminated BSON cstring accepted");
	}
	catch (Poco::DataFormatException&)
	{
	}
}


CppUnit::Test* BSONTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("BSONTest");

	// Document tests
	CppUnit_addTest(pSuite, BSONTest, testDocumentAddGet);
	CppUnit_addTest(pSuite, BSONTest, testDocumentExists);
	CppUnit_addTest(pSuite, BSONTest, testDocumentRemove);
	CppUnit_addTest(pSuite, BSONTest, testDocumentClear);
	CppUnit_addTest(pSuite, BSONTest, testDocumentSize);
	CppUnit_addTest(pSuite, BSONTest, testDocumentElementNames);
	CppUnit_addTest(pSuite, BSONTest, testNestedDocuments);
	CppUnit_addTest(pSuite, BSONTest, testDuplicateDocumentMembers);
	CppUnit_addTest(pSuite, BSONTest, testDocumentAddElementMerge);
	CppUnit_addTest(pSuite, BSONTest, testLargeDocumentAddElement);

	// Array tests
	CppUnit_addTest(pSuite, BSONTest, testArray);
	CppUnit_addTest(pSuite, BSONTest, testArrayIndexAccess);
	CppUnit_addTest(pSuite, BSONTest, testArrayNested);

	// Type tests
	CppUnit_addTest(pSuite, BSONTest, testDouble);
	CppUnit_addTest(pSuite, BSONTest, testString);
	CppUnit_addTest(pSuite, BSONTest, testBool);
	CppUnit_addTest(pSuite, BSONTest, testInt32);
	CppUnit_addTest(pSuite, BSONTest, testInt64);
	CppUnit_addTest(pSuite, BSONTest, testTimestamp);
	CppUnit_addTest(pSuite, BSONTest, testNull);
	CppUnit_addTest(pSuite, BSONTest, testBSONTimestamp);
	CppUnit_addTest(pSuite, BSONTest, testBSONTimestampSerializeDocument);

	// Binary tests
	CppUnit_addTest(pSuite, BSONTest, testBinaryGeneric);
	CppUnit_addTest(pSuite, BSONTest, testBinaryUUID);
	CppUnit_addTest(pSuite, BSONTest, testBinarySubtypes);

	// ObjectId tests
	CppUnit_addTest(pSuite, BSONTest, testObjectID);
	CppUnit_addTest(pSuite, BSONTest, testObjectIDTimestamp);

	// RegularExpression tests
	CppUnit_addTest(pSuite, BSONTest, testRegularExpression);

	// JavaScriptCode tests
	CppUnit_addTest(pSuite, BSONTest, testJavaScriptCode);

	CppUnit_addTest(pSuite, BSONTest, testDecimal128Specials);
	CppUnit_addTest(pSuite, BSONTest, testDecimal128FromString);
	CppUnit_addTest(pSuite, BSONTest, testDecimal128RoundTrip);
	CppUnit_addTest(pSuite, BSONTest, testDecimal128SerializeDocument);
	CppUnit_addTest(pSuite, BSONTest, testMinKeyMaxKeySerializeDocument);

	CppUnit_addTest(pSuite, BSONTest, testAuthMechanismConstants);
	CppUnit_addTest(pSuite, BSONTest, testAuthSCRAM256RejectsNonAscii);

	CppUnit_addTest(pSuite, BSONTest, testConnectionURITlsAlias);

	// Serialization/Deserialization tests
	CppUnit_addTest(pSuite, BSONTest, testDocumentSerialization);
	CppUnit_addTest(pSuite, BSONTest, testDocumentDeserialization);
	CppUnit_addTest(pSuite, BSONTest, testArraySerialization);
	CppUnit_addTest(pSuite, BSONTest, testComplexDocumentSerialization);

	// toString tests
	CppUnit_addTest(pSuite, BSONTest, testSimpleDocumentToString);
	CppUnit_addTest(pSuite, BSONTest, testNestedDocumentToString);
	CppUnit_addTest(pSuite, BSONTest, testDocumentWithArrayToString);
	CppUnit_addTest(pSuite, BSONTest, testComplexDocumentToString);
	CppUnit_addTest(pSuite, BSONTest, testToStringIndentation);
	CppUnit_addTest(pSuite, BSONTest, testArrayToString);

	// Failure/Error tests
	CppUnit_addTest(pSuite, BSONTest, testGetNonExistent);
	CppUnit_addTest(pSuite, BSONTest, testBadCast);
	CppUnit_addTest(pSuite, BSONTest, testInvalidObjectID);
	CppUnit_addTest(pSuite, BSONTest, testEmptyDocument);

	// Large and malformed server replies
	CppUnit_addTest(pSuite, BSONTest, testOpMsgReadBodyAbove16MB);
	CppUnit_addTest(pSuite, BSONTest, testOpMsgReadDocumentAbove16MB);
	CppUnit_addTest(pSuite, BSONTest, testOpMsgReadSectionOrderAndChecksum);
	CppUnit_addTest(pSuite, BSONTest, testDocumentReadLargeStandalone);
	CppUnit_addTest(pSuite, BSONTest, testOpMsgReadRejectsMalformedLengths);
	CppUnit_addTest(pSuite, BSONTest, testOpMsgReadRejectsBadFraming);
	CppUnit_addTest(pSuite, BSONTest, testOpMsgReadClearsMessage);
	CppUnit_addTest(pSuite, BSONTest, testDocumentReadStandaloneBounds);
	CppUnit_addTest(pSuite, BSONTest, testDocumentReadFailureKeepsIndexConsistent);
	CppUnit_addTest(pSuite, BSONTest, testDocumentRemoveDuplicateName);
	CppUnit_addTest(pSuite, BSONTest, testDocumentReadDepth);
	CppUnit_addTest(pSuite, BSONTest, testDocumentReadWithTextEncoding);
	CppUnit_addTest(pSuite, BSONTest, testDateTimeOutsideTimestampRange);
	CppUnit_addTest(pSuite, BSONTest, testRegularExpressionAndJavaScriptRoundTrip);
	CppUnit_addTest(pSuite, BSONTest, testBinaryReadTruncated);

	CppUnit_addTest(pSuite, BSONTest, testConnectionClosedAfterUnreadableReply);
	CppUnit_addTest(pSuite, BSONTest, testConnectionKeptAfterUnsupportedType);
	CppUnit_addTest(pSuite, BSONTest, testLargeDocumentRemoveAndAdd);
	CppUnit_addTest(pSuite, BSONTest, testBSONReaderBounds);

	return pSuite;
}
