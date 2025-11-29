//
// BSONTest.cpp
//
// Copyright (c) 2025, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "BSONTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/MongoDB/Document.h"
#include "Poco/MongoDB/Array.h"
#include "Poco/MongoDB/Binary.h"
#include "Poco/MongoDB/ObjectId.h"
#include "Poco/MongoDB/RegularExpression.h"
#include "Poco/MongoDB/JavaScriptCode.h"
#include "Poco/DateTime.h"
#include "Poco/UUIDGenerator.h"
#include <sstream>
#include <iostream>


using namespace Poco::MongoDB;
using namespace std::string_literals;


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

	// Verify the string representation contains UUID formatting
	std::string uuidStr = retrieved->toString();
	assertTrue(uuidStr.find("UUID") != std::string::npos);
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
		std::string value = doc->get<std::string>("nonexistent");
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
		Poco::Int64 num = doc->getInteger("nonexistent");
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
		Poco::Int32 num = doc->get<Poco::Int32>("string");
		fail("Should have thrown BadCastException");
	}
	catch (Poco::BadCastException&)
	{
		// Expected
	}

	// Try to get number as string
	try
	{
		std::string str = doc->get<std::string>("number");
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

	return pSuite;
}
