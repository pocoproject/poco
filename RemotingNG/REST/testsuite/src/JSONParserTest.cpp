//
// JSONParserTest.cpp
//
// Copyright (c) 2010-2016, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "JSONParserTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/RemotingNG/REST/JSONParser.h"
#include <sstream>


using namespace Poco::RemotingNG::REST;


JSONParserTest::JSONParserTest(const std::string& name): CppUnit::TestCase(name)
{
}


JSONParserTest::~JSONParserTest()
{
}


void JSONParserTest::testNull()
{
	std::string json("null");
	std::istringstream istr(json);
	JSONParser jp(istr);
	JSValue::Ptr pValue = jp.parse();
	assert (pValue.isNull());
}


void JSONParserTest::testNullWithWhitespace()
{
	std::string json("  null  ");
	std::istringstream istr(json);
	JSONParser jp(istr);
	JSValue::Ptr pValue = jp.parse();
	assert (pValue.isNull());
}


void JSONParserTest::testInt()
{
	std::string json("42");
	std::istringstream istr(json);
	JSONParser jp(istr);
	JSValue::Ptr pValue = jp.parse();
	assert (pValue->asInt32() == 42);
}


void JSONParserTest::testIntWithWhitespace()
{
	std::string json("  42 ");
	std::istringstream istr(json);
	JSONParser jp(istr);
	JSValue::Ptr pValue = jp.parse();
	assert (pValue->asInt32() == 42);
}


void JSONParserTest::testBool()
{
	std::string json("true");
	std::istringstream istr(json);
	JSONParser jp(istr);
	JSValue::Ptr pValue = jp.parse();
	assert (pValue->asBool());
}


void JSONParserTest::testBoolWithWhitespace()
{
	std::string json("  false");
	std::istringstream istr(json);
	JSONParser jp(istr);
	JSValue::Ptr pValue = jp.parse();
	assert (!pValue->asBool());
}


void JSONParserTest::testDouble()
{
	std::string json("42.5");
	std::istringstream istr(json);
	JSONParser jp(istr);
	JSValue::Ptr pValue = jp.parse();
	assert (pValue->asDouble() == 42.5);
}


void JSONParserTest::testDoubleWithWhitespace()
{
	std::string json("  42.5");
	std::istringstream istr(json);
	JSONParser jp(istr);
	JSValue::Ptr pValue = jp.parse();
	assert (pValue->asDouble() == 42.5);
}


void JSONParserTest::testString()
{
	std::string json("\"abcde\\tfgh\\f\\r\\nijk\\blmn\\u0040opq\"");
	std::istringstream istr(json);
	JSONParser jp(istr);
	JSValue::Ptr pValue = jp.parse();
	assert (pValue->stringValue() == "abcde\tfgh\f\r\nijk\blmn@opq");
}


void JSONParserTest::testStringWithWhitespace()
{
	std::string json("    \"abcde\\tfgh\\f\\r\\nijk\\blmn\\u0040\\u0041\\u0020opq\"   ");
	std::istringstream istr(json);
	JSONParser jp(istr);
	JSValue::Ptr pValue = jp.parse();
	assert (pValue->stringValue() == "abcde\tfgh\f\r\nijk\blmn@A opq");
}


void JSONParserTest::testEmptyObject()
{
	std::string json("{}");
	std::istringstream istr(json);
	JSONParser jp(istr);
	JSValue::Ptr pValue = jp.parse();
	assert (pValue->type() == JSValue::JS_OBJECT);
	assert (pValue->size() == 0);
}


void JSONParserTest::testEmptyObjectWithWhitespace()
{
	std::string json(" { \r\n } ");
	std::istringstream istr(json);
	JSONParser jp(istr);
	JSValue::Ptr pValue = jp.parse();
	assert (pValue->type() == JSValue::JS_OBJECT);
	assert (pValue->size() == 0);
}


void JSONParserTest::testObject1()
{
	std::string json(
		"{\n"
		"\t\"f1\": 42\n"
		"}"
	);
	std::istringstream istr(json);
	JSONParser jp(istr);
	JSValue::Ptr pValue = jp.parse();
	assert (pValue->type() == JSValue::JS_OBJECT);
	assert (pValue->size() == 1);
	assert ((*pValue)["f1"]->asInt32() == 42);
}


void JSONParserTest::testObject2()
{
	std::string json(
		"{\n"
		"\t\"f1\": 42,\n"
		"\t\"f2\": \"foo\"\n"
		"}"
	);
	std::istringstream istr(json);
	JSONParser jp(istr);
	JSValue::Ptr pValue = jp.parse();
	assert (pValue->type() == JSValue::JS_OBJECT);
	assert (pValue->size() == 2);
	assert ((*pValue)["f1"]->asInt32() == 42);
	assert ((*pValue)["f2"]->stringValue() == "foo");
}


void JSONParserTest::testObject3()
{
	std::string json(
		"{"
		"\"f1\":42,"
		"\"f2\":\"foo\","
		"\"f3\":null"
		"}"
	);
	std::istringstream istr(json);
	JSONParser jp(istr);
	JSValue::Ptr pValue = jp.parse();
	assert (pValue->type() == JSValue::JS_OBJECT);
	assert (pValue->size() == 3);
	assert ((*pValue)["f1"]->asInt32() == 42);
	assert ((*pValue)["f2"]->stringValue() == "foo");
	assert ((*pValue)["f3"].isNull());
}


void JSONParserTest::testEmptyArray()
{
	std::string json("[]");
	std::istringstream istr(json);
	JSONParser jp(istr);
	JSValue::Ptr pValue = jp.parse();
	assert (pValue->type() == JSValue::JS_ARRAY);
	assert (pValue->size() == 0);
}


void JSONParserTest::testEmptyArrayWithWhitespace()
{
	std::string json(" [  \r\n  ]  ");
	std::istringstream istr(json);
	JSONParser jp(istr);
	JSValue::Ptr pValue = jp.parse();
	assert (pValue->type() == JSValue::JS_ARRAY);
	assert (pValue->size() == 0);
}


void JSONParserTest::testArray1()
{
	std::string json(
		"[\n"
		"\t42\n"
		"]"
	);
	std::istringstream istr(json);
	JSONParser jp(istr);
	JSValue::Ptr pValue = jp.parse();
	assert (pValue->type() == JSValue::JS_ARRAY);
	assert (pValue->size() == 1);
	assert ((*pValue)[0]->asInt32() == 42);
}


void JSONParserTest::testArray2()
{
	std::string json(
		"[\n"
		"\t42,\n"
		"\t2112\n"
		"]"
	);
	std::istringstream istr(json);
	JSONParser jp(istr);
	JSValue::Ptr pValue = jp.parse();
	assert (pValue->type() == JSValue::JS_ARRAY);
	assert (pValue->size() == 2);
	assert ((*pValue)[0]->asInt32() == 42);
	assert ((*pValue)[1]->asInt32() == 2112);
}


void JSONParserTest::testArray3()
{
	std::string json(
		"["
		"42,"
		"2112,"
		"90125"
		"]"
	);
	std::istringstream istr(json);
	JSONParser jp(istr);
	JSValue::Ptr pValue = jp.parse();
	assert (pValue->type() == JSValue::JS_ARRAY);
	assert (pValue->size() == 3);
	assert ((*pValue)[0]->asInt32() == 42);
	assert ((*pValue)[1]->asInt32() == 2112);
	assert ((*pValue)[2]->asInt32() == 90125);
}


void JSONParserTest::testComplexObject()
{
	std::string json(
		"{"
		"\"f1\":42,"
		"\"f2\":\"foo\","
		"\"f3\":[1,2,3],"
		"\"f4\":{\"f41\":1,\"f42\":2},"
		"\"f5\":[{\"f51\":1,\"f52\":2}]"
		"}"
	);
	std::istringstream istr(json);
	JSONParser jp(istr);
	JSValue::Ptr pValue = jp.parse();
	assert (pValue->type() == JSValue::JS_OBJECT);
	assert (pValue->size() == 5);
	assert ((*pValue)["f1"]->asInt32() == 42);
	assert ((*pValue)["f2"]->stringValue() == "foo");
	assert ((*pValue)["f3"]->type() == JSValue::JS_ARRAY);
	assert ((*pValue)["f3"]->size() == 3);
	assert ((*(*pValue)["f3"])[0]->asInt32() == 1);
	assert ((*(*pValue)["f3"])[1]->asInt32() == 2);
	assert ((*(*pValue)["f3"])[2]->asInt32() == 3);
	assert ((*pValue)["f4"]->type() == JSValue::JS_OBJECT);
	assert ((*pValue)["f4"]->size() == 2);
	assert ((*(*pValue)["f4"])["f41"]->asInt32() == 1);
	assert ((*(*pValue)["f4"])["f42"]->asInt32() == 2);
	assert ((*pValue)["f5"]->type() == JSValue::JS_ARRAY);
	assert ((*pValue)["f5"]->size() == 1);
	assert ((*(*(*pValue)["f5"])[0])["f51"]->asInt32() == 1);
	assert ((*(*(*pValue)["f5"])[0])["f52"]->asInt32() == 2);	
}


void JSONParserTest::setUp()
{
}


void JSONParserTest::tearDown()
{
}


CppUnit::Test* JSONParserTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("JSONParserTest");

	CppUnit_addTest(pSuite, JSONParserTest, testNull);
	CppUnit_addTest(pSuite, JSONParserTest, testNullWithWhitespace);
	CppUnit_addTest(pSuite, JSONParserTest, testInt);
	CppUnit_addTest(pSuite, JSONParserTest, testIntWithWhitespace);
	CppUnit_addTest(pSuite, JSONParserTest, testBool);
	CppUnit_addTest(pSuite, JSONParserTest, testBoolWithWhitespace);
	CppUnit_addTest(pSuite, JSONParserTest, testDouble);
	CppUnit_addTest(pSuite, JSONParserTest, testDoubleWithWhitespace);
	CppUnit_addTest(pSuite, JSONParserTest, testString);
	CppUnit_addTest(pSuite, JSONParserTest, testStringWithWhitespace);
	CppUnit_addTest(pSuite, JSONParserTest, testEmptyObject);
	CppUnit_addTest(pSuite, JSONParserTest, testEmptyObjectWithWhitespace);
	CppUnit_addTest(pSuite, JSONParserTest, testObject1);
	CppUnit_addTest(pSuite, JSONParserTest, testObject2);
	CppUnit_addTest(pSuite, JSONParserTest, testObject3);
	CppUnit_addTest(pSuite, JSONParserTest, testEmptyArray);
	CppUnit_addTest(pSuite, JSONParserTest, testEmptyArrayWithWhitespace);
	CppUnit_addTest(pSuite, JSONParserTest, testArray1);
	CppUnit_addTest(pSuite, JSONParserTest, testArray2);
	CppUnit_addTest(pSuite, JSONParserTest, testArray3);
	CppUnit_addTest(pSuite, JSONParserTest, testComplexObject);

	return pSuite;
}
