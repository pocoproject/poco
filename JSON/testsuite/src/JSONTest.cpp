//
// JSONTest.cpp
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "JSONTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/Path.h"
#include "Poco/Environment.h"
#include "Poco/File.h"
#include "Poco/FileStream.h"
#include "Poco/Glob.h"
#include "Poco/UTF8Encoding.h"
#include "Poco/Latin1Encoding.h"
#include "Poco/TextConverter.h"
#include "Poco/Nullable.h"
#include "Poco/Dynamic/Struct.h"
#include "Poco/DateTime.h"
#include "Poco/DateTimeFormatter.h"
#include <set>
#include <iostream>


using namespace Poco::JSON;
using namespace Poco::Dynamic;
using Poco::DynamicStruct;
using Poco::DateTime;
using Poco::DateTimeFormatter;

JSONTest::JSONTest(const std::string& name): CppUnit::TestCase("JSON")
{

}


JSONTest::~JSONTest()
{

}


void JSONTest::setUp()
{
}


void JSONTest::tearDown()
{
}


void JSONTest::testNullProperty()
{
	std::string json = "{ \"test\" : null }";
	Parser parser;

	Var result;
	try
	{
		result = parser.parse(json);
	}
	catch(JSONException& jsone)
	{
		std::cout << jsone.message() << std::endl;
		assertTrue (false);
	}
	assertTrue (result.type() == typeid(Object::Ptr));

	Object::Ptr object = result.extract<Object::Ptr>();
	assertTrue (object->isNull("test"));
	Var test = object->get("test");
	assertTrue (test.isEmpty());

	Poco::Nullable<int> test2 = object->getNullableValue<int>("test");
	assertTrue (test2.isNull());

	DynamicStruct ds = *object;
	assertTrue (ds["test"].isEmpty());

	const DynamicStruct& rds = *object;
	assertTrue (rds["test"].isEmpty());
}


void JSONTest::testTrueProperty()
{
	std::string json = "{ \"test\" : true }";
	Parser parser;
	Var result;

	try
	{
		result = parser.parse(json);
	}
	catch(JSONException& jsone)
	{
		std::cout << jsone.message() << std::endl;
		assertTrue (false);
	}

	assertTrue (result.type() == typeid(Object::Ptr));

	Object::Ptr object = result.extract<Object::Ptr>();
	Var test = object->get("test");
	assertTrue (test.type() == typeid(bool));
	bool value = test;
	assertTrue (value);

	DynamicStruct ds = *object;
	assertTrue (!ds["test"].isEmpty());
	assertTrue (ds["test"]);

	const DynamicStruct& rds = *object;
	assertTrue (!rds["test"].isEmpty());
	assertTrue (rds["test"]);
}


void JSONTest::testFalseProperty()
{
	std::string json = "{ \"test\" : false }";
	Parser parser;
	Var result;

	try
	{
		result = parser.parse(json);
	}
	catch(JSONException& jsone)
	{
		std::cout << jsone.message() << std::endl;
		assertTrue (false);
	}

	assertTrue (result.type() == typeid(Object::Ptr));

	Object::Ptr object = result.extract<Object::Ptr>();
	Var test = object->get("test");
	assertTrue (test.type() == typeid(bool));
	bool value = test;
	assertTrue (!value);

	DynamicStruct ds = *object;
	assertTrue (!ds["test"].isEmpty());
	assertTrue (!ds["test"]);

	const DynamicStruct& rds = *object;
	assertTrue (!rds["test"].isEmpty());
	assertTrue (!rds["test"]);
}


void JSONTest::testNumberProperty()
{
	testNumber(1969);
	testNumber(-1969);
	testNumber(1969.5);
	testNumber(-1969.5);
}


void JSONTest::testUnsignedNumberProperty()
{
	// 4294967295 == unsigned(-1)
	std::string json = "{ \"test\" : 4294967295 }";
	Parser parser;
	Var result;

	try
	{
		result = parser.parse(json);
	}
	catch(JSONException& jsone)
	{
		std::cout << jsone.message() << std::endl;
		assertTrue (false);
	}

	assertTrue (result.type() == typeid(Object::Ptr));

	Object::Ptr object = result.extract<Object::Ptr>();
	Var test = object->get("test");
	assertTrue (test.isNumeric());
	assertTrue (test.isInteger());
	Poco::UInt32 value = test;
	assertTrue (value == -1);

	DynamicStruct ds = *object;
	assertTrue (!ds["test"].isEmpty());
	assertTrue (ds["test"].isNumeric());
	assertTrue (ds["test"].isInteger());
	assertTrue (ds["test"] == 4294967295u);
	value = ds["test"];
	assertTrue (value == -1);

	const DynamicStruct& rds = *object;
	assertTrue (!rds["test"].isEmpty());
	assertTrue (rds["test"].isNumeric());
	assertTrue (rds["test"].isInteger());
	assertTrue (rds["test"] == 4294967295u);
	value = rds["test"];
	assertTrue (value == -1);
}

#if defined(POCO_HAVE_INT64)


void JSONTest::testNumber64Property()
{
	std::string json = "{ \"test\" : -5000000000000000 }";
	Parser parser;
	Var result;

	try
	{
		result = parser.parse(json);
	}
	catch(JSONException& jsone)
	{
		std::cout << jsone.message() << std::endl;
		assertTrue (false);
	}

	assertTrue (result.type() == typeid(Object::Ptr));

	Object object = *result.extract<Object::Ptr>();
	Var test = object.get("test");
	assertTrue (test.isInteger());
	Poco::Int64 value = test;
	assertTrue (value == -5000000000000000ll);

	DynamicStruct ds = object;
	assertTrue (!ds["test"].isEmpty());
	assertTrue (ds["test"].isNumeric());
	assertTrue (ds["test"].isInteger());
	assertTrue (ds["test"] == -5000000000000000ll);
	value = ds["test"];
	assertTrue (value == -5000000000000000ll);

	const DynamicStruct& rds = object;
	assertTrue (!rds["test"].isEmpty());
	assertTrue (rds["test"].isNumeric());
	assertTrue (rds["test"].isInteger());
	assertTrue (rds["test"] == -5000000000000000ll);
	value = rds["test"];
	assertTrue (value == -5000000000000000ll);
}


void JSONTest::testUnsignedNumber64Property()
{
	// 18446744073709551615 == UInt64(-1)
	std::string json = "{ \"test\" : 18446744073709551615 }";
	Parser parser;
	Var result;

	try
	{
		result = parser.parse(json);
	}
	catch(JSONException& jsone)
	{
		std::cout << jsone.message() << std::endl;
		assertTrue (false);
	}

	assertTrue (result.type() == typeid(Object::Ptr));

	Object::Ptr object = result.extract<Object::Ptr>();
	Var test = object->get("test");
	assertTrue (test.isInteger());
	Poco::UInt64 value = test;
	assertTrue (value == -1);
	DynamicStruct ds = *object;
	assertTrue (!ds["test"].isEmpty());
	assertTrue (ds["test"].isNumeric());
	assertTrue (ds["test"].isInteger());
	assertTrue (ds["test"] == 18446744073709551615ull);
	value = ds["test"];
	assertTrue (value == -1);

	const DynamicStruct& rds = *object;
	assertTrue (!rds["test"].isEmpty());
	assertTrue (rds["test"].isNumeric());
	assertTrue (rds["test"].isInteger());
	assertTrue (rds["test"] == 18446744073709551615ull);
	value = rds["test"];
	assertTrue (value == -1);
}

#endif


void JSONTest::testStringProperty()
{
	std::string json = "{ \"test\" : \"value\" }";
	Parser parser;
	Var result;

	try
	{
		result = parser.parse(json);
	}
	catch(JSONException& jsone)
	{
		std::cout << jsone.message() << std::endl;
		assertTrue (false);
	}

	assertTrue (result.type() == typeid(Object::Ptr));

	Object object = *result.extract<Object::Ptr>();
	Var test = object.get("test");
	assertTrue (test.isString());
	std::string value = test.convert<std::string>();
	assertTrue (value.compare("value") == 0);

	object.set("test2", 'a');
	std::ostringstream ostr;
	object.stringify(ostr);
	assertTrue (ostr.str() == "{\"test\":\"value\",\"test2\":\"a\"}");

	DynamicStruct ds = object;
	assertTrue (!ds["test"].isEmpty());
	assertTrue (ds["test"].isString());
	assertTrue (!ds["test"].isInteger());
	assertTrue (ds["test"] == "value");
	value = ds["test"].toString();
	assertTrue (value == "value");

	const DynamicStruct& rds = object;
	assertTrue (!rds["test"].isEmpty());
	assertTrue (rds["test"].isString());
	assertTrue (!rds["test"].isInteger());
	assertTrue (rds["test"] == "value");
	value = rds["test"].toString();
	assertTrue (value == "value");
}


void JSONTest::testEmptyObject()
{
	std::string json = "{}";
	Parser parser;
	Var result;

	try
	{
		result = parser.parse(json);
	}
	catch(JSONException& jsone)
	{
		std::cout << jsone.message() << std::endl;
		assertTrue (false);
	}

	assertTrue (result.type() == typeid(Object::Ptr));

	Object::Ptr object = result.extract<Object::Ptr>();
	assertTrue (object->size() == 0);

	DynamicStruct ds = *object;
	assertTrue (ds.size() == 0);

	const DynamicStruct& rds = *object;
	assertTrue (rds.size() == 0);
}


void JSONTest::testEmptyPropertyName()
{
	std::string json = "{\"\": 42}";
	Parser parser;
	Var result;

	try
	{
		result = parser.parse(json);
	}
	catch(JSONException& jsone)
	{
		std::cout << jsone.message() << std::endl;
		assertTrue (false);
	}

	assertTrue (result.type() == typeid(Object::Ptr));

	Object::Ptr object = result.extract<Object::Ptr>();
	assertTrue (object->size() == 1);

	DynamicStruct ds = *object;
	assertTrue (ds.size() == 1);

	const DynamicStruct& rds = *object;
	assertTrue (rds.size() == 1);

	assertTrue (ds[""] == 42);
}


void JSONTest::testComplexObject()
{
	std::string json =
	"{"
		"\"id\": 1,"
		"\"jsonrpc\": \"2.0\","
		"\"total\": 1,"
		"\"result\": ["
			"{"
				"\"id\": 1,"
				"\"guid\": \"67acfb26-17eb-4a75-bdbd-f0669b7d8f73\","
				"\"picture\": \"http://placehold.it/32x32\","
				"\"age\": 40,"
				"\"name\": \"Angelina Crossman\","
				"\"gender\": \"female\","
				"\"company\": \"Raylog\","
				"\"phone\": \"892-470-2803\","
				"\"email\": \"angelina@raylog.com\","
				"\"address\": \"20726, CostaMesa, Horatio Streets\","
				"\"about\": \"Consectetuer suscipit volutpat eros dolor euismod, "
				"et dignissim in feugiat sed, ea tation exerci quis. Consectetuer, "
				"dolor dolore ad vero ullamcorper, tincidunt facilisis at in facilisi, "
				"iusto illum illum. Autem nibh, sed elit consequat volutpat tation, "
				"nisl lorem lorem sed tation, facilisis dolore. Augue odio molestie, "
				"dolor zzril nostrud aliquam sed, wisi dolor et ut iusto, ea. Magna "
				"ex qui facilisi, hendrerit quis in eros ut, zzril nibh dolore nisl "
				"suscipit, vulputate elit ut lobortis exerci, nulla dolore eros at "
				"aliquam, ullamcorper vero ad iusto. Adipiscing, nisl eros exerci "
				"nisl vel, erat in luptatum in duis, iusto.\","
				"\"registered\": \"2008-04-09T11:13:17 +05:00\","
				"\"tags\": ["
					"\"ut\","
					"\"accumsan\","
					"\"feugait\","
					"\"ex\","
					"\"odio\","
					"\"consequat\","
					"\"delenit\""
				"],"
				"\"friends\": ["
					"{"
						"\"id\": 1,"
						"\"name\": \"Hailey Hardman\""
					"},"
					"{"
						"\"id\": 2,"
						"\"name\": \"Bailey Oldridge\""
					"},"
					"{"
						"\"id\": 3,"
						"\"name\": \"Makayla Campbell\""
					"}"
				"]"
			"}"
		"]"
	"}";

	Parser parser;
	Var result;

	try
	{
		result = parser.parse(json);
	}
	catch(JSONException& jsone)
	{
		std::cout << jsone.message() << std::endl;
		assertTrue (false);
	}

	assertTrue (result.type() == typeid(Object::Ptr));

	Object::Ptr object = result.extract<Object::Ptr>();
	assertTrue (object->size() > 0);

	Object::NameList names = object->getNames();
	assertTrue (names.size() == 4);
	assertTrue (names[0] == "id");
	assertTrue (names[1] == "jsonrpc");
	assertTrue (names[2] == "result");
	assertTrue (names[3] == "total");

	DynamicStruct ds = *object;
	assertTrue (ds.size() > 0);
	assertTrue (ds["id"] == 1);
	assertTrue (ds["jsonrpc"] == 2.0);

	assertTrue (ds["result"].isArray());
	assertTrue (ds["result"].size() == 1);
	assertTrue (ds["result"][0].isStruct());
	assertTrue (ds["result"][0]["id"] == 1);
	assertTrue (ds["result"][0]["guid"] == "67acfb26-17eb-4a75-bdbd-f0669b7d8f73");
	assertTrue (ds["result"][0]["age"] == 40);
	assertTrue (ds["result"][0]["name"] == "Angelina Crossman");
	assertTrue (ds["result"][0]["gender"] == "female");
	assertTrue (ds["result"][0]["company"] == "Raylog");
	assertTrue (ds["result"][0]["phone"] == "892-470-2803");
	assertTrue (ds["result"][0]["email"] == "angelina@raylog.com");
	assertTrue (ds["result"][0]["address"] == "20726, CostaMesa, Horatio Streets");
	assertTrue (ds["result"][0]["about"] == "Consectetuer suscipit volutpat eros dolor euismod, "
		"et dignissim in feugiat sed, ea tation exerci quis. Consectetuer, "
		"dolor dolore ad vero ullamcorper, tincidunt facilisis at in facilisi, "
		"iusto illum illum. Autem nibh, sed elit consequat volutpat tation, "
		"nisl lorem lorem sed tation, facilisis dolore. Augue odio molestie, "
		"dolor zzril nostrud aliquam sed, wisi dolor et ut iusto, ea. Magna "
		"ex qui facilisi, hendrerit quis in eros ut, zzril nibh dolore nisl "
		"suscipit, vulputate elit ut lobortis exerci, nulla dolore eros at "
		"aliquam, ullamcorper vero ad iusto. Adipiscing, nisl eros exerci "
		"nisl vel, erat in luptatum in duis, iusto.");
	assertTrue (ds["result"][0]["registered"] == "2008-04-09T11:13:17 +05:00");

	assertTrue (ds["result"][0]["tags"].isArray());
	assertTrue (ds["result"][0]["tags"].size() == 7);
	assertTrue (ds["result"][0]["tags"][0] == "ut");
	assertTrue (ds["result"][0]["tags"][1] == "accumsan");
	assertTrue (ds["result"][0]["tags"][2] == "feugait");
	assertTrue (ds["result"][0]["tags"][3] == "ex");
	assertTrue (ds["result"][0]["tags"][4] == "odio");
	assertTrue (ds["result"][0]["tags"][5] == "consequat");
	assertTrue (ds["result"][0]["tags"][6] == "delenit");

	assertTrue (ds["result"][0]["friends"][0].isStruct());
	assertTrue (ds["result"][0]["friends"][0]["id"] == 1);
	assertTrue (ds["result"][0]["friends"][0]["name"] == "Hailey Hardman");
	assertTrue (ds["result"][0]["friends"][1]["id"] == 2);
	assertTrue (ds["result"][0]["friends"][1]["name"] == "Bailey Oldridge");
	assertTrue (ds["result"][0]["friends"][2]["id"] == 3);
	assertTrue (ds["result"][0]["friends"][2]["name"] == "Makayla Campbell");

	const DynamicStruct& rds = *object;
	assertTrue (rds.size() > 0);
}


void JSONTest::testDoubleProperty()
{
	std::string json = "{ \"test\" : 1234.5 }";
	Parser parser;
	Var result;

	try
	{
		result = parser.parse(json);
	}
	catch(JSONException& jsone)
	{
		std::cout << jsone.message() << std::endl;
		assertTrue (false);
	}

	assertTrue (result.type() == typeid(Object::Ptr));

	Object::Ptr object = result.extract<Object::Ptr>();
	Var test = object->get("test");
	assertTrue (test.isNumeric());
	double value = test;
	assertTrue (value == 1234.5);

	DynamicStruct ds = *object;
	assertTrue (ds["test"] == 1234.5);
}


void JSONTest::testDouble2Property()
{
	std::string json = "{ \"test\" : 12e34 }";
	Parser parser;
	Var result;

	try
	{
		result = parser.parse(json);
	}
	catch(JSONException& jsone)
	{
		std::cout << jsone.message() << std::endl;
		assertTrue (false);
	}

	assertTrue (result.type() == typeid(Object::Ptr));

	Object::Ptr object = result.extract<Object::Ptr>();
	Var test = object->get("test");
	assertTrue (test.isNumeric());
	double value = test;
	assertTrue (value >= 1.99e34 && value <= 12.01e34);
}


void JSONTest::testDouble3Property()
{
	std::string json = "{ \"test\" : 12e-34 }";
	Parser parser;
	Var result;

	try
	{
		result = parser.parse(json);
	}
	catch(JSONException& jsone)
	{
		std::cout << jsone.message() << std::endl;
		assertTrue (false);
	}

	assertTrue (result.type() == typeid(Object::Ptr));

	Object::Ptr object = result.extract<Object::Ptr>();
	Var test = object->get("test");
	assertTrue (test.isNumeric());
	double value = test;
	assertTrue (value == 12e-34);
}


void JSONTest::testObjectProperty()
{
	std::string json = "{ \"test\" : { \"property\" : \"value\" } }";
	Parser parser;
	Var result;

	try
	{
		result = parser.parse(json);
	}
	catch(JSONException& jsone)
	{
		std::cout << jsone.message() << std::endl;
		assertTrue (false);
	}

	assertTrue (result.type() == typeid(Object::Ptr));

	Object::Ptr object = result.extract<Object::Ptr>();
	assertTrue (object->isObject("test"));
	assertTrue (!object->isArray("test"));

	assertTrue (!object->isArray("nonExistentKey"));
	assertTrue (!object->isObject("nonExistentKey"));

	Var test = object->get("test");
	assertTrue (test.type() == typeid(Object::Ptr));
	Object::Ptr subObject = test.extract<Object::Ptr>();

	test = subObject->get("property");
	assertTrue (test.isString());
	std::string value = test.convert<std::string>();
	assertTrue (value.compare("value") == 0);

	DynamicStruct ds = *object;
	assertTrue (ds["test"].isStruct());
	assertTrue (ds["test"]["property"] == "value");

	// make sure that Object is recognized as such
	{
		Object obj;
		Object inner;
		inner.set("some_number", 5);
		inner.set("some_string", "xyz");
		std::string key = "new_object";
		obj.set(key, inner);
		assertTrue (obj.isObject(key));
	}

	// make sure that Object pointer is recognized as Object
	{
		Object obj;
		Poco::JSON::Object::Ptr inner = new Poco::JSON::Object;
		inner->set("some_number", 5);
		inner->set("some_string", "xyz");
		std::string key = "new_object";
		obj.set(key, inner);
		assertTrue (obj.isObject(key));
	}
}


void JSONTest::testObjectArray()
{
	std::string json = "{ \"test\" : { \"test1\" : [1, 2, 3], \"test2\" : 4 } }";
	Parser parser;
	Var result;

	try
	{
		result = parser.parse(json);
	}
	catch(JSONException& jsone)
	{
		std::cout << jsone.message() << std::endl;
		assertTrue (false);
	}

	assertTrue (result.type() == typeid(Object::Ptr));
	Object::Ptr object = result.extract<Object::Ptr>();
	assertTrue (object->isObject("test"));
	Object::Ptr subObject = object->getObject("test");
	assertTrue (!subObject->isObject("test1"));
	assertTrue (subObject->isArray("test1"));
	assertTrue (!subObject->isObject("test2"));
	assertTrue (!subObject->isArray("test2"));

	DynamicStruct ds = *object;
	assertTrue (ds.size() > 0);
	assertTrue (ds.size() == 1);

	assertTrue (ds["test"].isStruct());
	assertTrue (ds["test"]["test1"].isArray());
	assertTrue (ds["test"]["test1"].size() == 3);
	assertTrue (ds["test"]["test1"][0] == 1);
	assertTrue (ds["test"]["test1"][1] == 2);
	assertTrue (ds["test"]["test1"][2] == 3);
	assertTrue (ds["test"]["test2"] == 4);

	object->set("test3", "another top level value");
	ds = std::move(*object);
	assertTrue (ds.size() == 2);
	assertTrue (ds["test"].isStruct());
	assertTrue (ds["test"]["test1"].isArray());
	assertTrue (ds["test"]["test1"].size() == 3);
	assertTrue (ds["test"]["test1"][0] == 1);
	assertTrue (ds["test"]["test1"][1] == 2);
	assertTrue (ds["test"]["test1"][2] == 3);
	assertTrue (ds["test"]["test2"] == 4);
	assertTrue (ds["test3"] == "another top level value");
}


void JSONTest::testArrayOfObjects()
{
	std::string json = "[ {\"test\" : 0}, { \"test1\" : [1, 2, 3], \"test2\" : 4 } ]";
	Parser parser;
	Var result;

	try
	{
		result = parser.parse(json);
	}
	catch(JSONException& jsone)
	{
		std::cout << jsone.message() << std::endl;
		assertTrue (false);
	}

	assertTrue (result.isArray());
	assertTrue (result.type() == typeid(Poco::JSON::Array::Ptr));
	Poco::JSON::Array::Ptr arr = result.extract<Poco::JSON::Array::Ptr>();
	Object::Ptr object = arr->getObject(0);
	assertTrue (object->getValue<int>("test") == 0);
	Object::Ptr subObject = arr->getObject(1);
	Poco::JSON::Array::Ptr subArr = subObject->getArray("test1");
	result = subArr->get(0);
	assertTrue (result == 1);

	Poco::Dynamic::Array da = *arr;
	assertTrue (da.size() == 2);
	assertTrue (da[0].isStruct());
	assertTrue (da[0]["test"] == 0);
	assertTrue (da[1].isStruct());
	assertTrue (da[1]["test1"].isArray());
	assertTrue (da[1]["test1"][0] == 1);
	assertTrue (da[1]["test1"][1] == 2);
	assertTrue (da[1]["test1"][2] == 3);
	assertTrue (da[1]["test2"] == 4);
}


void JSONTest::testEmptyArray()
{
	std::string json = "[]";
	Parser parser;
	Var result;

	try
	{
		result = parser.parse(json);
	}
	catch(JSONException& jsone)
	{
		std::cout << jsone.message() << std::endl;
		assertTrue (false);
	}

	assertTrue (result.type() == typeid(Poco::JSON::Array::Ptr));

	Poco::JSON::Array::Ptr array = result.extract<Poco::JSON::Array::Ptr>();
	assertTrue (array->size() == 0);
	assertTrue (array->empty());

	Poco::Dynamic::Array da = *array;
	assertTrue (da.size() == 0);
	assertTrue (da.empty());
}


void JSONTest::testNestedArray()
{
	std::string json = "[[[[]]]]";
	Parser parser;
	Var result;

	try
	{
		result = parser.parse(json);
	}
	catch(JSONException& jsone)
	{
		std::cout << jsone.message() << std::endl;
		assertTrue (false);
	}

	assertTrue (result.type() == typeid(Poco::JSON::Array::Ptr));

	Poco::JSON::Array::Ptr array = result.extract<Poco::JSON::Array::Ptr>();
	assertTrue (array->size() == 1);
	assertTrue (!array->empty());

	Poco::Dynamic::Array da = *array;
	assertTrue (da.size() == 1);
	assertTrue (da[0].size() == 1);
	assertTrue (!da.empty());
	assertTrue (da[0][0].size() == 1);
	assertTrue (da[0][0][0].size() == 0);
}


void JSONTest::testNullElement()
{
	std::string json = "[ null ]";
	Parser parser;
	Var result;

	try
	{
		result = parser.parse(json);
	}
	catch(JSONException& jsone)
	{
		std::cout << jsone.message() << std::endl;
		assertTrue (false);
	}

	assertTrue (result.type() == typeid(Poco::JSON::Array::Ptr));

	Poco::JSON::Array::Ptr array = result.extract<Poco::JSON::Array::Ptr>();
	assertTrue (array->isNull(0));
	Var test = array->get(0);
	assertTrue (test.isEmpty());

	Poco::Dynamic::Array da = *array;
	assertTrue (da.size() == 1);
	assertTrue (da[0].isEmpty());
}


void JSONTest::testTrueElement()
{
	std::string json = "[ true ]";
	Parser parser;
	Var result;

	try
	{
		result = parser.parse(json);
	}
	catch(JSONException& jsone)
	{
		std::cout << jsone.message() << std::endl;
		assertTrue (false);
	}

	assertTrue (result.type() == typeid(Poco::JSON::Array::Ptr));

	Poco::JSON::Array::Ptr array = result.extract<Poco::JSON::Array::Ptr>();
	Var test = array->get(0);
	assertTrue (test.type() == typeid(bool));
	bool value = test;
	assertTrue (value);

	Poco::Dynamic::Array da = *array;
	assertTrue (da.size() == 1);
	assertTrue (da[0]);
}


void JSONTest::testFalseElement()
{
	std::string json = "[ false ]";
	Parser parser;
	Var result;

	try
	{
		result = parser.parse(json);
	}
	catch(JSONException& jsone)
	{
		std::cout << jsone.message() << std::endl;
		assertTrue (false);
	}

	assertTrue (result.type() == typeid(Poco::JSON::Array::Ptr));

	Poco::JSON::Array::Ptr array = result.extract<Poco::JSON::Array::Ptr>();
	Var test = array->get(0);
	assertTrue (test.type() == typeid(bool));
	bool value = test;
	assertTrue (!value);

	Poco::Dynamic::Array da = *array;
	assertTrue (da.size() == 1);
	assertTrue (!da[0]);
}


void JSONTest::testNumberElement()
{
	std::string json = "[ 1969 ]";
	Parser parser;
	Var result;

	try
	{
		result = parser.parse(json);
	}
	catch(JSONException& jsone)
	{
		std::cout << jsone.message() << std::endl;
		assertTrue (false);
	}

	assertTrue (result.type() == typeid(Poco::JSON::Array::Ptr));

	Poco::JSON::Array::Ptr array = result.extract<Poco::JSON::Array::Ptr>();
	Var test = array->get(0);
	assertTrue (test.isInteger());
	int value = test;
	assertTrue (value == 1969);

	Poco::Dynamic::Array da = *array;
	assertTrue (da.size() == 1);
	assertTrue (da[0] == 1969);
}


void JSONTest::testStringElement()
{
	std::string json = "[ \"value\" ]";
	Parser parser;
	Var result;

	try
	{
		result = parser.parse(json);
	}
	catch(JSONException& jsone)
	{
		std::cout << jsone.message() << std::endl;
		assertTrue (false);
	}

	assertTrue (result.type() == typeid(Poco::JSON::Array::Ptr));

	Poco::JSON::Array::Ptr array = result.extract<Poco::JSON::Array::Ptr>();
	Var test = array->get(0);
	assertTrue (test.isString());
	std::string value = test.convert<std::string>();
	assertTrue (value.compare("value") == 0);

	Poco::Dynamic::Array da = *array;
	assertTrue (da.size() == 1);
	assertTrue (da[0] == "value");

	std::stringstream s;
	json = "[ \"\\u0017\" ]";
	Var v = Parser().parse(json);
	Stringifier::condense(v, s);
	std::string ss = s.str();
	assertTrue (s.str() == "[\"\\u0017\"]");
}


void JSONTest::testEmptyObjectElement()
{
	std::string json = "[{}]";
	Parser parser;
	Var result;

	try
	{
		result = parser.parse(json);
	}
	catch(JSONException& jsone)
	{
		std::cout << jsone.message() << std::endl;
		assertTrue (false);
	}

	assertTrue (result.type() == typeid(Poco::JSON::Array::Ptr));

	Poco::JSON::Array::Ptr array = result.extract<Poco::JSON::Array::Ptr>();
	Object::Ptr object = array->getObject(0);
	assertTrue (object->size() == 0);

	Poco::Dynamic::Array da = *array;
	assertTrue (da.size() == 1);
	assertTrue (da[0].isStruct());
	assertTrue (da[0].size() == 0);
}


void JSONTest::testDoubleElement()
{
	std::string json = "[ 1234.5 ]";
	Parser parser;
	Var result;

	try
	{
		result = parser.parse(json);
	}
	catch(JSONException& jsone)
	{
		std::cout << jsone.message() << std::endl;
		assertTrue (false);
	}

	assertTrue (result.type() == typeid(Poco::JSON::Array::Ptr));

	Poco::JSON::Array::Ptr array = result.extract<Poco::JSON::Array::Ptr>();
	Var test = array->get(0);
	assertTrue (test.isNumeric());
	double value = test;
	assertTrue (value == 1234.5);

	Poco::Dynamic::Array da = *array;
	assertTrue (da.size() == 1);
	assertTrue (da[0] == 1234.5);
}


void JSONTest::testSetArrayElement()
{
	std::string json = "[]";
	Parser parser;
	Var result = parser.parse(json);
	Poco::JSON::Array::Ptr array = result.extract<Poco::JSON::Array::Ptr>();

	Poco::Dynamic::Array dynArray = *array;
	assertTrue (dynArray.size() == 0);

	// array[0] = 7
	array->set(0, 7);
	assertTrue (array->size() == 1);
	assertTrue (array->getElement<int>(0) == 7);

	dynArray = *array;
	assertTrue (dynArray.size() == 1);
	assertTrue (dynArray[0] == 7);

	// array[2] = "foo"
	array->set(2, std::string("foo"));
	assertTrue (array->size() == 3);
	assertTrue (array->getElement<int>(0) == 7);
	assertTrue (array->isNull(1));
	assertTrue (array->getElement<std::string>(2) == "foo");

	dynArray = *array;
	assertTrue (dynArray.size() == 3);
	assertTrue (dynArray[0] == 7);
	assertTrue (dynArray[1].isEmpty());
	assertTrue (dynArray[2] == "foo");

	// array[1] = 13
	array->set(1, 13);
	assertTrue (array->size() == 3);
	assertTrue (array->getElement<int>(0) == 7);
	assertTrue (array->getElement<int>(1) == 13);
	assertTrue (array->getElement<std::string>(2) == "foo");

	dynArray = std::move(*array);
	assertTrue (dynArray.size() == 3);
	assertTrue (dynArray[0] == 7);
	assertTrue (dynArray[1] == 13);
	assertTrue (dynArray[2] == "foo");

	dynArray.clear();
	assertTrue (dynArray.size() == 0);
}


void JSONTest::testOptValue()
{
	std::string json = "{ }";
	Parser parser;
	Var result;

	try
	{
		result = parser.parse(json);
	}
	catch(JSONException& jsone)
	{
		std::cout << jsone.message() << std::endl;
		assertTrue (false);
	}

	assertTrue (result.type() == typeid(Object::Ptr));

	Object::Ptr object = result.extract<Object::Ptr>();
	int n = object->optValue("test", 123);
	assertTrue (n == 123);
}


void JSONTest::testQuery()
{
	std::string json = "{ \"name\" : \"Franky\", \"children\" : [ \"Jonas\", \"Ellen\" ], \"address\": { \"street\": \"A Street\", \"number\": 123, \"city\":\"The City\"} }";
	Parser parser;
	Var result;

	try
	{
		result = parser.parse(json);
	}
	catch(JSONException& jsone)
	{
		std::cout << jsone.message() << std::endl;
		assertTrue (false);
	}

	assertTrue (result.type() == typeid(Object::Ptr));

	Query query(result);

	std::string firstChild = query.findValue("children[0]", "");
	assertTrue (firstChild.compare("Jonas") == 0);
	std::string secondChild = query.findValue("children[1]", "");
	assertTrue (secondChild.compare("Ellen") == 0);
	std::string thirdChild = query.findValue("children[2]", "");
	assertTrue (thirdChild.empty());

	Poco::DynamicStruct ds = *result.extract<Object::Ptr>();
	assertTrue (ds["name"] == "Franky");
	assertTrue (ds["children"].size() == 2);
	assertTrue (ds["children"][0] == "Jonas");
	assertTrue (ds["children"][1] == "Ellen");

	Object::Ptr pAddress = query.findObject("address");
	assertTrue (pAddress->getValue<std::string>("street") == "A Street");
	pAddress = query.findObject("bad address");
	assertTrue (pAddress.isNull());

	Object address;
	address.set("dummy", 123);
	query.findObject("bad address", address);
	assertTrue (!address.has("dummy"));
	Object& rAddress = query.findObject("address", address);
	assertTrue (rAddress.getValue<int>("number") == 123);

	Var badAddr = query.find("address.street.anotherObject");
	assertTrue (badAddr.isEmpty());

	using Poco::JSON::Array;

	Array::Ptr pChildren = query.findArray("children");
	assertTrue (pChildren->getElement<std::string>(0) == "Jonas");
	pChildren = query.findArray("no children");
	assertTrue (pChildren.isNull());

	Array children;
	children.add("dummy");
	query.findArray("no children", children);
	assertTrue (children.size() == 0);
	Array& rChildren = query.findArray("children", children);
	assertTrue (rChildren.getElement<std::string>(1) == "Ellen");

	Object::Ptr pObj = new Poco::JSON::Object;
	pObj->set("Id", 22);

	Query queryPointer(pObj);
	Var idQueryPointer = queryPointer.find("Id");
	assertTrue (22 == idQueryPointer);

	Query queryObj(*pObj);
	Var idQueryObj = queryObj.find("Id");
	assertTrue (22 == idQueryObj);

	Var bad = 1;
	try
	{
		Query badQuery(bad);
		fail ("must throw");
	}
	catch (Poco::InvalidArgumentException&) { }

	json = R"json({"foo":["bar"]})json";
	try { result = parser.parse(json); }
	catch(JSONException& jsone)
	{
		fail (jsone.message());
	}
	Query queryFoo(result);
	result = queryFoo.find("foo[0]");
	assertTrue (!result.isEmpty());
	result = queryFoo.find("foo[1]");
	assertTrue (result.isEmpty());
	result = queryFoo.find("[1]");
	assertTrue (result.isEmpty());
	result = queryFoo.find("");
	assertTrue (result.convert<std::string>() == json);
}


void JSONTest::testComment()
{
	std::string json = "{ \"name\" : \"Franky\" /* father */, \"children\" : [ \"Jonas\" /* son */ , \"Ellen\" /* daughter */ ] }";
	Parser parser;
	Var result;

	try
	{
		parser.parse(json);
		fail ("must fail");
	}
	catch(Poco::SyntaxException&)
	{
	}
	catch (JSONException&)
	{
	}

	parser.reset();
	parser.setAllowComments(true);
	try
	{
		result = parser.parse(json);
	}
	catch(JSONException& jsone)
	{
		std::cout << jsone.message() << std::endl;
		assertTrue (false);
	}

	assertTrue (result.type() == typeid(Object::Ptr));

	Query query(result);

	std::string firstChild = query.findValue("children[0]", "");
	assertTrue (firstChild.compare("Jonas") == 0);
}


void JSONTest::testPrintHandler()
{
	std::string json = "{ \"name\" : \"Homer\", \"age\" : 38, \"wife\" : \"Marge\", \"age\" : 36, \"children\" : [ \"Bart\", \"Lisa\", \"Maggie\" ] }";
	Parser parser;
	std::ostringstream ostr;
	PrintHandler::Ptr pHandler = new PrintHandler(ostr);
	parser.setHandler(pHandler);
	parser.parse(json);
	assertTrue (ostr.str() == "{\"name\":\"Homer\",\"age\":38,\"wife\":\"Marge\",\"age\":36,\"children\":[\"Bart\",\"Lisa\",\"Maggie\"]}");

	pHandler->setIndent(1);
	ostr.str("");
	parser.reset();
	parser.parse(json);
	assertTrue (ostr.str() == "{\n"
		" \"name\" : \"Homer\",\n"
		" \"age\" : 38,\n"
		" \"wife\" : \"Marge\",\n"
		" \"age\" : 36,\n"
		" \"children\" : [\n"
		"  \"Bart\",\n"
		"  \"Lisa\",\n"
		"  \"Maggie\"\n"
		" ]\n"
		"}"
	);

	pHandler->setIndent(2);
	ostr.str("");
	parser.reset();
	parser.parse(json);
	assertTrue (ostr.str() == "{\n"
		"  \"name\" : \"Homer\",\n"
		"  \"age\" : 38,\n"
		"  \"wife\" : \"Marge\",\n"
		"  \"age\" : 36,\n"
		"  \"children\" : [\n"
		"    \"Bart\",\n"
		"    \"Lisa\",\n"
		"    \"Maggie\"\n"
		"  ]\n"
		"}"
	);

	pHandler->setIndent(4);
	ostr.str("");
	parser.reset();
	parser.parse(json);
	assertTrue (ostr.str() == "{\n"
		"    \"name\" : \"Homer\",\n"
		"    \"age\" : 38,\n"
		"    \"wife\" : \"Marge\",\n"
		"    \"age\" : 36,\n"
		"    \"children\" : [\n"
		"        \"Bart\",\n"
		"        \"Lisa\",\n"
		"        \"Maggie\"\n"
		"    ]\n"
		"}"
	);

	json =
		"{"
			"\"array\":"
			"["
				"{"
					"\"key1\":"
					"["
						"1,2,3,"
						"{"
							"\"subkey\":"
							"\"test\""
						"}"
					"]"
				"},"
				"{"
					"\"key2\":"
					"{"
						"\"anotherSubKey\":"
						"["
							"1,"
							"{"
								"\"subSubKey\":"
								"["
									"4,5,6"
								"]"
							"}"
						"]"
					"}"
				"}"
			"]"
		"}";


	ostr.str("");
	pHandler->setIndent(0);
	parser.reset();
	parser.parse(json);
	assertTrue (json == ostr.str());

	json="[[\"a\"],[\"b\"],[[\"c\"],[\"d\"]]]";
	ostr.str("");
	pHandler->setIndent(0);
	parser.reset();
	parser.parse(json);
	assertTrue (json == ostr.str());

	json="[{\"1\":\"one\",\"0\":[\"zero\",\"nil\"]}]";
	ostr.str("");
	pHandler->setIndent(0);
	parser.reset();
	parser.parse(json);
	assertTrue (json == ostr.str());

	json=
		"{"
			"\"a\":100,"
			"\"b\":234.456,"
			"\"child\":"
			"["
				"{"
					"\"id\":0,"
					"\"name\":\"lucy_0\""
				"},"
				"{"
					"\"id\":1,"
					"\"name\":\"lucy_1\""
				"},"
				"{"
					"\"id\":2,"
					"\"name\":\"lucy_4\""
				"},"
				"{"
					"\"id\":3,"
					"\"name\":\"lucy_9\""
				"},"
				"{"
					"\"id\":4,"
					"\"name\":\"lucy_16\""
				"}"
			"],"
			"\"pair\":{"
				"\"a\":123213,"
				"\"b\":\"weoifweifj\""
			"},"
			"\"str\":\"sdfsdf\""
		"}";
	ostr.str("");
	pHandler->setIndent(0);
	parser.reset();
	parser.parse(json);
	assertTrue (json == ostr.str());
}


void JSONTest::testStringify()
{
	std::ostringstream os;
	Var i = 123;
	Stringifier::stringify(i, os);
	assertTrue (os.str() == "123");

	os.str("");
	Var f = 123.456;
	Stringifier::stringify(f, os);
	assertTrue (os.str() == "123.456");

	os.str("");
	Var s = "abcdef";
	Stringifier::stringify(s, os);
	assertTrue (os.str() == "\"abcdef\"");

	os.str("");
	DateTime dt;
	Var d = dt;
	Stringifier::stringify(d, os);
	assertTrue (os.str() == std::string("\"" + DateTimeFormatter::format(dt, Poco::DateTimeFormat::ISO8601_FORMAT) + "\""));

	std::string str1 = "\r";
	std::string str2 = "\n";
	Object obj1, obj2;
	obj1.set("payload", str1);
	obj2.set("payload", str2);
	std::ostringstream oss1, oss2;
	Poco::JSON::Stringifier::stringify(obj1, oss1);
	Poco::JSON::Stringifier::stringify(obj2, oss2);
	assertTrue (oss1.str() == "{\"payload\":\"\\r\"}");
	assertTrue (oss2.str() == "{\"payload\":\"\\n\"}");

	Object jObj(false);
	jObj.set("foo\\", 0);
	jObj.set("bar/", 0);
	jObj.set("baz", 0);
	jObj.set("q\"uote\"d", 0);
	jObj.set("backspace", "bs\b");
	jObj.set("newline", "nl\n");
	jObj.set("tab", "tb\t");

	std::stringstream ss;
	jObj.stringify(ss);

	assertTrue (ss.str() == "{\"backspace\":\"bs\\b\",\"bar/\":0,\"baz\":0,\"foo\\\\\":0,"
		"\"newline\":\"nl\\n\",\"q\\\"uote\\\"d\":0,\"tab\":\"tb\\t\"}");

	std::string json = "{ \"Simpsons\" : { \"husband\" : { \"name\" : \"Homer\" , \"age\" : 38 }, \"wife\" : { \"name\" : \"Marge\", \"age\" : 36 }, "
						"\"children\" : [ \"Bart\", \"Lisa\", \"Maggie\" ], "
						"\"address\" : { \"number\" : 742, \"street\" : \"Evergreen Terrace\", \"town\" : \"Springfield\" } } }";
	Parser parser;
	Var result;

	try
	{
		result = parser.parse(json);
	}
	catch(JSONException& jsone)
	{
		std::cout << jsone.message() << std::endl;
		assertTrue (false);
	}

	assertTrue (result.type() == typeid(Object::Ptr));
	std::ostringstream ostr;

	Stringifier::condense(result, ostr);
	std::string str = "{"
						"\"Simpsons\":{"
						"\"address\":{"
						"\"number\":742,"
						"\"street\":\"Evergreen Terrace\","
						"\"town\":\"Springfield\""
						"},"
						"\"children\":["
						"\"Bart\","
						"\"Lisa\","
						"\"Maggie\"],"
						"\"husband\":{"
						"\"age\":38,"
						"\"name\":\"Homer\""
						"},"
						"\"wife\":{"
						"\"age\":36,\"name\":\"Marge\""
						"}}}";

	assertTrue (ostr.str() == str);

	ostr.str("");
	Stringifier::stringify(result, ostr);
	assertTrue (ostr.str() == str);

	ostr.str("");
	Stringifier::stringify(result, ostr, 1);
	str = "{\n"
		" \"Simpsons\": {\n"
		"  \"address\": {\n"
		"   \"number\": 742,\n"
		"   \"street\": \"Evergreen Terrace\",\n"
		"   \"town\": \"Springfield\"\n"
		"  },\n"
		"  \"children\": [\n"
		"   \"Bart\",\n"
		"   \"Lisa\",\n"
		"   \"Maggie\"\n"
		"  ],\n"
		"  \"husband\": {\n"
		"   \"age\": 38,\n"
		"   \"name\": \"Homer\"\n"
		"  },\n"
		"  \"wife\": {\n"
		"   \"age\": 36,\n"
		"   \"name\": \"Marge\"\n"
		"  }\n"
		" }\n"
		"}";
	assertTrue (ostr.str() == str);

	ostr.str("");
	Stringifier::stringify(result, ostr, 2);
	str = "{\n"
		"  \"Simpsons\": {\n"
		"    \"address\": {\n"
		"      \"number\": 742,\n"
		"      \"street\": \"Evergreen Terrace\",\n"
		"      \"town\": \"Springfield\"\n"
		"    },\n"
		"    \"children\": [\n"
		"      \"Bart\",\n"
		"      \"Lisa\",\n"
		"      \"Maggie\"\n"
		"    ],\n"
		"    \"husband\": {\n"
		"      \"age\": 38,\n"
		"      \"name\": \"Homer\"\n"
		"    },\n"
		"    \"wife\": {\n"
		"      \"age\": 36,\n"
		"      \"name\": \"Marge\"\n"
		"    }\n"
		"  }\n"
		"}";
	assertTrue (ostr.str() == str);

	ostr.str("");
	Stringifier::stringify(result, ostr, 4);
	str = "{\n"
		"    \"Simpsons\": {\n"
		"        \"address\": {\n"
		"            \"number\": 742,\n"
		"            \"street\": \"Evergreen Terrace\",\n"
		"            \"town\": \"Springfield\"\n"
		"        },\n"
		"        \"children\": [\n"
		"            \"Bart\",\n"
		"            \"Lisa\",\n"
		"            \"Maggie\"\n"
		"        ],\n"
		"        \"husband\": {\n"
		"            \"age\": 38,\n"
		"            \"name\": \"Homer\"\n"
		"        },\n"
		"        \"wife\": {\n"
		"            \"age\": 36,\n"
		"            \"name\": \"Marge\"\n"
		"        }\n"
		"    }\n"
		"}";
	assertTrue (ostr.str() == str);
}


void JSONTest::testStringifyNaN()
{
	Object::Ptr o = new Object;
	o->set("NaN", NAN);
	o->set("Infinity", INFINITY);
	std::ostringstream stream;
	o->stringify(stream, 0);
	assertEqual (stream.str(), std::string(R"({"Infinity":null,"NaN":null})"));
}


void JSONTest::testStringifyPreserveOrder()
{
	Object presObj(Poco::JSON_PRESERVE_KEY_ORDER);
	presObj.set("foo", 0);
	presObj.set("bar", 0);
	presObj.set("baz", 0);
	std::stringstream ss;
	presObj.stringify(ss);
	assertTrue (ss.str() == "{\"foo\":0,\"bar\":0,\"baz\":0}");
	Object::NameList nl = presObj.getNames();
	assertTrue (nl.size() == 3);
	assertTrue (nl[0] == "foo");
	assertTrue (nl[1] == "bar");
	assertTrue (nl[2] == "baz");

	ss.str("");
	Stringifier::stringify(presObj, ss);
	assertTrue (ss.str() == "{\"foo\":0,\"bar\":0,\"baz\":0}");

	Object noPresObj;
	noPresObj.set("foo", 0);
	noPresObj.set("bar", 0);
	noPresObj.set("baz", 0);
	ss.str("");
	noPresObj.stringify(ss);
	assertTrue (ss.str() == "{\"bar\":0,\"baz\":0,\"foo\":0}");
	ss.str("");
	Stringifier::stringify(noPresObj, ss);
	assertTrue (ss.str() == "{\"bar\":0,\"baz\":0,\"foo\":0}");

	std::string json = "{ \"Simpsons\" : { \"husband\" : { \"name\" : \"Homer\" , \"age\" : 38 }, \"wife\" : { \"name\" : \"Marge\", \"age\" : 36 }, "
						"\"children\" : [ \"Bart\", \"Lisa\", \"Maggie\" ], "
						"\"address\" : { \"number\" : 742, \"street\" : \"Evergreen Terrace\", \"town\" : \"Springfield\" } } }";

	ParseHandler::Ptr pHandler = new ParseHandler(true);
	Parser parser(pHandler);
	Var result;

	try
	{
		result = parser.parse(json);
	}
	catch(JSONException& jsone)
	{
		std::cout << jsone.message() << std::endl;
		assertTrue (false);
	}

	assertTrue (result.type() == typeid(Object::Ptr));
	std::ostringstream ostr;

	Stringifier::condense(result, ostr);
	assertTrue (ostr.str() == "{\"Simpsons\":{\"husband\":{\"name\":\"Homer\",\"age\":38},\"wife\":{\"name\":\"Marge\",\"age\":36},"
						"\"children\":[\"Bart\",\"Lisa\",\"Maggie\"],"
						"\"address\":{\"number\":742,\"street\":\"Evergreen Terrace\",\"town\":\"Springfield\"}}}");

	ostr.str("");
	Stringifier::stringify(result, ostr);
	assertTrue (ostr.str() == "{\"Simpsons\":{\"husband\":{\"name\":\"Homer\",\"age\":38},\"wife\":{\"name\":\"Marge\",\"age\":36},"
						"\"children\":[\"Bart\",\"Lisa\",\"Maggie\"],"
						"\"address\":{\"number\":742,\"street\":\"Evergreen Terrace\",\"town\":\"Springfield\"}}}");

	ostr.str("");
	Stringifier::stringify(result, ostr, 1);
	assertTrue (ostr.str() == "{\n"
						" \"Simpsons\": {\n"
						"  \"husband\": {\n"
						"   \"name\": \"Homer\",\n"
						"   \"age\": 38\n"
						"  },\n"
						"  \"wife\": {\n"
						"   \"name\": \"Marge\",\n"
						"   \"age\": 36\n"
						"  },\n"
						"  \"children\": [\n"
						"   \"Bart\",\n"
						"   \"Lisa\",\n"
						"   \"Maggie\"\n"
						"  ],\n"
						"  \"address\": {\n"
						"   \"number\": 742,\n"
						"   \"street\": \"Evergreen Terrace\",\n"
						"   \"town\": \"Springfield\"\n"
						"  }\n"
						" }\n"
						"}");

	ostr.str("");
	Stringifier::stringify(result, ostr, 2);
	assertTrue (ostr.str() == "{\n"
						"  \"Simpsons\": {\n"
						"    \"husband\": {\n"
						"      \"name\": \"Homer\",\n"
						"      \"age\": 38\n"
						"    },\n"
						"    \"wife\": {\n"
						"      \"name\": \"Marge\",\n"
						"      \"age\": 36\n"
						"    },\n"
						"    \"children\": [\n"
						"      \"Bart\",\n"
						"      \"Lisa\",\n"
						"      \"Maggie\"\n"
						"    ],\n"
						"    \"address\": {\n"
						"      \"number\": 742,\n"
						"      \"street\": \"Evergreen Terrace\",\n"
						"      \"town\": \"Springfield\"\n"
						"    }\n"
						"  }\n"
						"}");

	ostr.str("");
	Stringifier::stringify(result, ostr, 4);
	assertTrue (ostr.str() == "{\n"
						"    \"Simpsons\": {\n"
						"        \"husband\": {\n"
						"            \"name\": \"Homer\",\n"
						"            \"age\": 38\n"
						"        },\n"
						"        \"wife\": {\n"
						"            \"name\": \"Marge\",\n"
						"            \"age\": 36\n"
						"        },\n"
						"        \"children\": [\n"
						"            \"Bart\",\n"
						"            \"Lisa\",\n"
						"            \"Maggie\"\n"
						"        ],\n"
						"        \"address\": {\n"
						"            \"number\": 742,\n"
						"            \"street\": \"Evergreen Terrace\",\n"
						"            \"town\": \"Springfield\"\n"
						"        }\n"
						"    }\n"
						"}");

	Poco::DynamicStruct ds = *result.extract<Object::Ptr>();
	assertTrue(ds.toString() == "{ \"Simpsons\": { \"address\": { \"number\": 742, \"street\": \"Evergreen Terrace\", \"town\": \"Springfield\" }, "
		"\"children\": [ \"Bart\", \"Lisa\", \"Maggie\" ], "
		"\"husband\": { \"age\": 38, \"name\": \"Homer\" }, "
		"\"wife\": { \"age\": 36, \"name\": \"Marge\" } } }");
	assertTrue (ds["Simpsons"].isStruct());
	assertFalse(ds["Simpsons"].isOrdered());
	assertTrue (ds["Simpsons"]["husband"].isStruct());
	assertTrue (ds["Simpsons"]["husband"]["name"] == "Homer");
	assertTrue (ds["Simpsons"]["husband"]["age"] == 38);

	assertTrue (ds["Simpsons"]["wife"].isStruct());
	assertTrue (ds["Simpsons"]["wife"]["name"] == "Marge");
	assertTrue (ds["Simpsons"]["wife"]["age"] == 36);

	assertTrue (ds["Simpsons"]["children"].isArray());
	assertTrue (ds["Simpsons"]["children"][0] == "Bart");
	assertTrue (ds["Simpsons"]["children"][1] == "Lisa");
	assertTrue (ds["Simpsons"]["children"][2] == "Maggie");

	assertTrue (ds["Simpsons"]["address"].isStruct());
	assertTrue (ds["Simpsons"]["address"]["number"] == 742);
	assertTrue (ds["Simpsons"]["address"]["street"] == "Evergreen Terrace");
	assertTrue (ds["Simpsons"]["address"]["town"] == "Springfield");

	Poco::OrderedDynamicStruct ods = *result.extract<Object::Ptr>();
	assertTrue(ods["Simpsons"].isStruct());
	assertTrue(ods["Simpsons"].isOrdered());
	assertTrue(ods.toString() == "{ \"Simpsons\": { \"husband\": { \"name\": \"Homer\", \"age\": 38 }, "
		"\"wife\": { \"name\": \"Marge\", \"age\": 36 }, "
		"\"children\": [ \"Bart\", \"Lisa\", \"Maggie\" ], "
		"\"address\": { \"number\": 742, \"street\": \"Evergreen Terrace\", "
		"\"town\": \"Springfield\" } } }");
}


void JSONTest::testVarConvert()
{
	std::string json = "{ \"foo\" : { \"bar\" : \"baz\", \"arr\": [1, 2, 3]} }";
	Parser parser;
	Var result;

	try
	{
		result = parser.parse(json);
	}
	catch (JSONException& jsone)
	{
		std::cout << jsone.message() << std::endl;
		assertTrue(false);
	}

	assertTrue(result.type() == typeid(Object::Ptr));

	std::string cvt;
	result.convert(cvt);
	assertTrue(cvt == "{\"foo\":{\"arr\":[1,2,3],\"bar\":\"baz\"}}");

	Object::Ptr object = result.extract<Object::Ptr>();
	Object::Ptr f = object->getObject("foo");

	Var o = f;
	cvt.clear();
	o.convert(cvt);
	assertTrue(cvt == "{\"arr\":[1,2,3],\"bar\":\"baz\"}");

	Var a = f->get("arr");
	cvt.clear();
	a.convert(cvt);
	assertTrue(cvt == "[1,2,3]");
}

void JSONTest::testBasicJson()
{
	// Tests basic JSON structure and accessibility of members

	const auto json = R"(
		{
			"clientConfig" : "Franky",
			"arrayMember": [1, "A", 3.5],
			"objectMember": {
				"a": 1,
				"b": "B"
			}
		}
	)";
    Poco::JSON::Parser jsonParser;
    Poco::Dynamic::Var jsonObject = jsonParser.parse(json);

    Poco::JSON::Object::Ptr jsonPtr = jsonObject.extract<Poco::JSON::Object::Ptr>();

    assertFalse(jsonPtr->get("clientConfig").isEmpty());
    const auto testStr = jsonPtr->getValue<std::string>("clientConfig");
    assertEqual(testStr, "Franky");

    const auto testArr = jsonPtr->getArray("arrayMember");
    assertFalse(testArr.isNull());
    assertFalse(testArr->empty());
    assertEqual(testArr->size(), 3);
    assertEqual(testArr->getElement<int>(0), 1);
    assertEqual(testArr->getElement<std::string>(1), "A");

    const auto testObj = jsonPtr->getObject("objectMember");
    assertFalse(testObj.isNull());
    assertEqual(testObj->size(), 2);
    assertEqual(testObj->getValue<int>("a"), 1);
    assertEqual(testObj->getValue<std::string>("b"), "B");

}

void JSONTest::testValidJanssonFiles()
{
	Poco::Path pathPattern(getTestFilesPath("valid"));

	std::set<std::string> paths;
	Poco::Glob::glob(pathPattern, paths);

	for(std::set<std::string>::iterator it = paths.begin(); it != paths.end(); ++it)
	{
		Poco::Path filePath(*it, "input");

		if ( filePath.isFile() )
		{
			Poco::File inputFile(filePath);
			if ( inputFile.exists() )
			{
				Poco::FileInputStream fis(filePath.toString());
				std::cout << filePath.toString() << " ... ";

				Parser parser;
				Var result;

				try
				{
					parser.parse(fis);
					result = parser.asVar();
					std::cout << "Ok!" << std::endl;
				}
				catch(JSONException& jsone)
				{
					std::string err = jsone.displayText();
					std::cout << "Failed:" << err << std::endl;
					fail (err);
				}
				catch(Poco::Exception& e)
				{
					std::string err = e.displayText();
					std::cout << "Failed:" << err << std::endl;
					fail (err);
				}
			}
		}
	}
}


void JSONTest::testInvalidJanssonFiles()
{
	Poco::Path pathPattern(getTestFilesPath("invalid"));

	std::set<std::string> paths;
	Poco::Glob::glob(pathPattern, paths);

	for(std::set<std::string>::iterator it = paths.begin(); it != paths.end(); ++it)
	{
		Poco::Path filePath(*it, "input");

		if (filePath.isFile())
		{
			Poco::File inputFile(filePath);
			if (inputFile.exists())
			{
				Poco::FileInputStream fis(filePath.toString());
				std::cout << filePath.toString() << " ... ";

				Parser parser;
				parser.setAllowNullByte(false);
				Var result;

				try
				{
					parser.parse(fis);
					result = parser.asVar();
					// We shouldn't get here.
					std::cout << "We didn't get an exception. This is the result: " << result.convert<std::string>() << std::endl;
					fail(result.convert<std::string>());
				}
				catch(Poco::Exception& /*ex*/)
				{
					std::cout << /*" (" << ex.displayText() << ") " <<*/ "Ok!" << std::endl;
				}
			}
		}
	}
}


void JSONTest::testInvalidUnicodeJanssonFiles()
{
	Poco::Path pathPattern(getTestFilesPath("invalid-unicode"));

	std::set<std::string> paths;
	Poco::Glob::glob(pathPattern, paths);

	for(std::set<std::string>::iterator it = paths.begin(); it != paths.end(); ++it)
	{
		Poco::Path filePath(*it, "input");

		if ( filePath.isFile() )
		{
			Poco::File inputFile(filePath);
			if ( inputFile.exists() )
			{
				Poco::FileInputStream fis(filePath.toString());
				std::cout << filePath.toString() << " ... ";

				Parser parser;
				parser.setAllowNullByte(false);
				Var result;

				try
				{
					parser.parse(fis);
					result = parser.asVar();
					// We shouldn't get here.
					std::cout << "We didn't get an exception. This is the result: " << result.convert<std::string>() << std::endl;
					fail(result.convert<std::string>());
				}
				catch(Poco::Exception& /*ex*/)
				{
					std::cout << /*" (" << ex.displayText() << ") " <<*/ "Ok!" << std::endl;
				}
			}
		}
	}
}


void JSONTest::testTemplate()
{
	Template tpl;
	tpl.parse("Hello world! From <?= person.name?>.\n<?if person.tooOld?>You're too old.<?endif?>");

	Object::Ptr data = new Object();
	Object::Ptr person = new Object();
	data->set("person", person);
	person->set("name", "Franky");
	person->set("tooOld", true);
	std::ostringstream ostr;
	tpl.render(data, ostr);
	std::cout << ostr.str();
	assertTrue (ostr.str() == "Hello world! From Franky.\nYou're too old.");
}


void JSONTest::testUnicode()
{
	const unsigned char supp[] = {0x61, 0xE1, 0xE9, 0x78, 0xED, 0xF3, 0xFA, 0x0};
	std::string text((const char*) supp);

	std::string json = "{ \"test\" : \"a\\u00E1\\u00E9x\\u00ED\\u00F3\\u00FA\" }";
	Parser parser;

	Var result;
	parser.parse(json);
	result = parser.asVar();

	assertTrue (result.type() == typeid(Object::Ptr));

	Object::Ptr object = result.extract<Object::Ptr>();
	Var test = object->get("test");

	Poco::Latin1Encoding latin1;
	Poco::UTF8Encoding utf8;
	Poco::TextConverter converter(latin1, utf8);
	std::string original;
	converter.convert(text, original);

	assertTrue (test.convert<std::string>() == original);

	parser.reset();
	std::ostringstream os;
	os << '[' << (char) 0x92 << ']';
	try
	{
		parser.parse(os.str());
		fail("Invalid Unicode sequence, must fail.");
	}
	catch (JSONException&) {}

	parser.reset();
	os.str("");
	os << "[\"" << (char)0xC2 << (char)0x92 << "\"]";
	result = parser.parse(os.str());
	assertTrue (result.type() == typeid(Poco::JSON::Array::Ptr));

	parser.reset();
	os.str("");
	os << "[\"" << (char)0xAC << "\"]";
	try
	{
		parser.parse(os.str());
		fail("Invalid Unicode sequence, must fail.");
	}
	catch (JSONException&) {}

	parser.reset();
	os.str("");
	os << "[\"" << (char)0xE2 << (char)0x82 << (char)0xAC << "\"]";
	result = parser.parse(os.str());
	assertTrue (result.type() == typeid(Poco::JSON::Array::Ptr));

	parser.reset();
	os.str("");
	os << "[\"" << (char)0xA2 << "\"]";
	try
	{
		parser.parse(os.str());
		fail("Invalid Unicode sequence, must fail.");
	}
	catch (JSONException&){}

	parser.reset();
	os.str("");
	os << "[\"" << (char)0xF0 << (char)0xA4 << (char)0xAD << (char)0xAD << "\"]";
	result = parser.parse(os.str());
	assertTrue (result.type() == typeid(Poco::JSON::Array::Ptr));
}


void JSONTest::testEscape0()
{
	Poco::JSON::Object::Ptr json = new Poco::JSON::Object();

	std::string nullString("B\0b", 3);
	json->set("name", nullString);

	std::stringstream ss;
	json->stringify(ss);

	assertTrue (ss.str().compare("{\"name\":\"B\\u0000b\"}") == 0);

	// parse the JSON containing the escaped string
 	Poco::JSON::Parser parser(new Poco::JSON::ParseHandler());
 	Var result = parser.parse(ss.str());

 	assert(result.type() == typeid(Object::Ptr));

 	Object::Ptr object = result.extract<Object::Ptr>();
 	assert(object->get("name").extract<std::string>() == nullString);
}


void JSONTest::testNonEscapeUnicode()
{
	std::string chinese("{\"arr\":[{ \"name\" : \"\\u4e2d\" }]}");
	Poco::JSON::Parser parser(new Poco::JSON::ParseHandler());
	Var result = parser.parse(chinese);

	assertTrue (result.type() == typeid(Object::Ptr));

	Object::Ptr object = result.extract<Object::Ptr>();
	object->setEscapeUnicode(false);

	std::stringstream ss;
	object->stringify(ss);
	assertTrue (ss.str().compare("{\"arr\":[{\"name\":\"\xE4\xB8\xAD\"}]}") == 0);

	const unsigned char utf8Chars[]   = {'{', '"', 'a', 'r', 'r', '"', ':', '[', '{', '"', 'n', 'a', 'm', 'e', '"', ':',
										 '"', 'g', 0xC3, 0xBC, 'n', 't', 'e', 'r', '"', '}', ']', '}', 0};
	std::string utf8Text((const char*) utf8Chars);
	parser.reset();
	result = parser.parse(utf8Text);
	object = result.extract<Object::Ptr>();
	object->setEscapeUnicode(false);

	ss.str(""); object->stringify(ss);
	assertTrue (ss.str() == "{\"arr\":[{\"name\":\"g\xC3\xBCnter\"}]}");

	Poco::JSON::Object obj1;
	std::string shortEscapeStr("String with \t");
	std::string longEscapeStr("String with \a and \v plus \t for good measure");
	obj1.set("shortEscape", shortEscapeStr);
	obj1.set("longEscape", longEscapeStr);

	ss.str("");
	obj1.stringify(ss);

	parser.reset();
	parser.parse(ss.str());
	result = parser.asVar();

	assertTrue (result.type() == typeid(Object::Ptr));

	object = result.extract<Object::Ptr>();
	Var shortEscape = object->get("shortEscape");
	Var longEscape = object->get("longEscape");
	assertTrue (shortEscape.convert<std::string>() == shortEscapeStr);
	assertTrue (longEscape.convert<std::string>() == longEscapeStr);

	Poco::JSON::Object::Ptr json = new Poco::JSON::Object(Poco::JSON_PRESERVE_KEY_ORDER);
	Poco::JSON::Object::Ptr json2 = new Poco::JSON::Object(Poco::JSON_PRESERVE_KEY_ORDER);

	json->set("value", 15);
	json->set("unit", "°C");

	assertFalse (json->getEscapeUnicode());
	assertFalse (json2->getEscapeUnicode());
	json2->set("Temperature", json);
	std::ostringstream buffer {};
	json->stringify(buffer);
	std::string str = buffer.str();
	assertEqual (str, R"({"value":15,"unit":"°C"})");
	std::ostringstream buffer2 {};
	json2->stringify(buffer2);
	std::string str2 = buffer2.str();
	assertEqual (str2, R"({"Temperature":{"value":15,"unit":"°C"}})");
}


void JSONTest::testEscapeUnicode()
{
	Poco::JSON::Object::Ptr json = new Poco::JSON::Object();
	std::string chinese("{ \"name\" : \"\\u4e2d\" }");
	Poco::JSON::Parser parser(new Poco::JSON::ParseHandler());
	Var result = parser.parse(chinese);

	assertTrue (result.type() == typeid(Object::Ptr));

	Object::Ptr object = result.extract<Object::Ptr>();
	object->setEscapeUnicode(true);

	std::stringstream ss;
	object->stringify(ss, 0, -1);
	assertTrue (ss.str().compare("{\"name\":\"\\u4E2D\"}") == 0);

	const unsigned char utf8Chars[]   = {'{', '"', 'n', 'a', 'm', 'e', '"', ':',
			'"', 'g', 0xC3, 0xBC, 'n', 't', 'e', 'r', '"', '}', 0};
	std::string utf8Text((const char*) utf8Chars);
	parser.reset();
	result = parser.parse(utf8Text);
	object = result.extract<Object::Ptr>();
	object->setEscapeUnicode(true);
	ss.str(""); object->stringify(ss, 0, -1);
	assertTrue (ss.str() == "{\"name\":\"g\\u00FCnter\"}");
}


std::string JSONTest::getTestFilesPath(const std::string& type)
{
	std::ostringstream ostr;
	ostr << "data/" << type << '/';
	std::string validDir(ostr.str());
	Poco::Path pathPattern(validDir);
	if (Poco::File(pathPattern).exists())
	{
		validDir += '*';
		return validDir;
	}

	ostr.str("");
	ostr << "/JSON/testsuite/data/" << type << '/';
	validDir = Poco::Environment::get("POCO_BASE") + ostr.str();
	std::cout << validDir << std::endl;
	pathPattern = validDir;

	if (Poco::File(pathPattern).exists())
		validDir += '*';
	else
	{
		std::cout << "Can't find " << validDir << std::endl;
		throw Poco::NotFoundException("cannot locate directory containing valid JSON test files");
	}
	return validDir;
}


void JSONTest::testCopy()
{
	Object obj1(Poco::JSON_PRESERVE_KEY_ORDER);
	obj1.set("foo", 0);
	obj1.set("bar", 0);
	obj1.set("baz", 0);

	Object::NameList nl = obj1.getNames();
	assertTrue (nl.size() == 3);
	assertTrue (nl[0] == "foo");
	assertTrue (nl[1] == "bar");
	assertTrue (nl[2] == "baz");

	Object obj2;
	obj2 = obj1;
	nl = obj2.getNames();
	assertTrue (nl.size() == 3);
	assertTrue (nl[0] == "foo");
	assertTrue (nl[1] == "bar");
	assertTrue (nl[2] == "baz");

	Object obj3;
	obj3.set("foo", 0);
	obj3.set("bar", 0);
	obj3.set("baz", 0);
	nl = obj3.getNames();
	assertTrue (nl.size() == 3);
	assertTrue (nl[0] == "bar");
	assertTrue (nl[1] == "baz");
	assertTrue (nl[2] == "foo");

	Object obj4;
	obj4 = obj3;
	nl = obj4.getNames();
	assertTrue (nl.size() == 3);
	assertTrue (nl[0] == "bar");
	assertTrue (nl[1] == "baz");
	assertTrue (nl[2] == "foo");

	obj4 = obj1;
	nl = obj4.getNames();
	assertTrue (nl.size() == 3);
	assertTrue (nl[0] == "foo");
	assertTrue (nl[1] == "bar");
	assertTrue (nl[2] == "baz");

	Object obj5(obj1);
	nl = obj5.getNames();
	assertTrue (nl.size() == 3);
	assertTrue (nl[0] == "foo");
	assertTrue (nl[1] == "bar");
	assertTrue (nl[2] == "baz");
}


void JSONTest::testMove()
{
	Object obj1(Poco::JSON_PRESERVE_KEY_ORDER);
	obj1.set("foo", 0);
	obj1.set("bar", 0);
	obj1.set("baz", 0);

	Object::NameList nl = obj1.getNames();
	assertTrue (nl.size() == 3);
	assertTrue (nl[0] == "foo");
	assertTrue (nl[1] == "bar");
	assertTrue (nl[2] == "baz");

	Object obj2;
	obj2 = std::move(obj1);
	assertTrue (obj1.getNames().size() == 0);

	nl = obj2.getNames();
	assertTrue (nl.size() == 3);
	assertTrue (nl[0] == "foo");
	assertTrue (nl[1] == "bar");
	assertTrue (nl[2] == "baz");

	Object obj3;
	obj3.set("foo", 0);
	obj3.set("bar", 0);
	obj3.set("baz", 0);
	nl = obj3.getNames();
	assertTrue (nl.size() == 3);
	assertTrue (nl[0] == "bar");
	assertTrue (nl[1] == "baz");
	assertTrue (nl[2] == "foo");

	Object obj4;
	obj4 = std::move(obj3);
	assertTrue (obj3.getNames().size() == 0);

	nl = obj4.getNames();
	assertTrue (nl.size() == 3);
	assertTrue (nl[0] == "bar");
	assertTrue (nl[1] == "baz");
	assertTrue (nl[2] == "foo");

	Object obj5(Poco::JSON_PRESERVE_KEY_ORDER);
	obj5.set("foo", 0);
	obj5.set("bar", 0);
	obj5.set("baz", 0);
	nl = obj5.getNames();
	assertTrue (nl.size() == 3);
	assertTrue (nl[0] == "foo");
	assertTrue (nl[1] == "bar");
	assertTrue (nl[2] == "baz");

	obj4 = std::move(obj5);
	assertTrue (obj5.getNames().size() == 0);

	nl = obj4.getNames();
	assertTrue (nl.size() == 3);
	assertTrue (nl[0] == "foo");
	assertTrue (nl[1] == "bar");
	assertTrue (nl[2] == "baz");
}

void JSONTest::testRemove()
{
	Object obj1;
	obj1.set("foo", 0);
	obj1.set("bar", 0);
	obj1.set("baz", 0);

	Object::NameList nl = obj1.getNames();

	assertTrue(nl.size() == 3);
	assertTrue(nl[0] == "bar");
	assertTrue(nl[1] == "baz");
	assertTrue(nl[2] == "foo");

	obj1.remove("baz");

	nl = obj1.getNames();
	assertTrue(nl.size() == 2);
	assertTrue(nl[0] == "bar");
	assertTrue(nl[1] == "foo");

	Object obj2(Poco::JSON_PRESERVE_KEY_ORDER);
	obj2.set("foo", 0);
	obj2.set("bar", 0);
	obj2.set("baz", 0);

	nl = obj2.getNames();
	assertTrue(nl.size() == 3);
	assertTrue(nl[0] == "foo");
	assertTrue(nl[1] == "bar");
	assertTrue(nl[2] == "baz");

	obj2.remove("bar");
	nl = obj2.getNames();

	assertTrue(nl.size() == 2);
	assertTrue(nl[0] == "foo");
	assertTrue(nl[1] == "baz");

}


CppUnit::Test* JSONTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("JSONTest");

	CppUnit_addTest(pSuite, JSONTest, testNullProperty);
	CppUnit_addTest(pSuite, JSONTest, testTrueProperty);
	CppUnit_addTest(pSuite, JSONTest, testFalseProperty);
	CppUnit_addTest(pSuite, JSONTest, testNumberProperty);
	CppUnit_addTest(pSuite, JSONTest, testUnsignedNumberProperty);
#if defined(POCO_HAVE_INT64)
	CppUnit_addTest(pSuite, JSONTest, testNumber64Property);
	CppUnit_addTest(pSuite, JSONTest, testUnsignedNumber64Property);
#endif
	CppUnit_addTest(pSuite, JSONTest, testStringProperty);
	CppUnit_addTest(pSuite, JSONTest, testEmptyObject);
	CppUnit_addTest(pSuite, JSONTest, testEmptyPropertyName);
	CppUnit_addTest(pSuite, JSONTest, testComplexObject);
	CppUnit_addTest(pSuite, JSONTest, testDoubleProperty);
	CppUnit_addTest(pSuite, JSONTest, testDouble2Property);
	CppUnit_addTest(pSuite, JSONTest, testDouble3Property);
	CppUnit_addTest(pSuite, JSONTest, testObjectProperty);
	CppUnit_addTest(pSuite, JSONTest, testObjectArray);
	CppUnit_addTest(pSuite, JSONTest, testArrayOfObjects);
	CppUnit_addTest(pSuite, JSONTest, testEmptyArray);
	CppUnit_addTest(pSuite, JSONTest, testNestedArray);
	CppUnit_addTest(pSuite, JSONTest, testNullElement);
	CppUnit_addTest(pSuite, JSONTest, testTrueElement);
	CppUnit_addTest(pSuite, JSONTest, testFalseElement);
	CppUnit_addTest(pSuite, JSONTest, testNumberElement);
	CppUnit_addTest(pSuite, JSONTest, testStringElement);
	CppUnit_addTest(pSuite, JSONTest, testEmptyObjectElement);
	CppUnit_addTest(pSuite, JSONTest, testDoubleElement);
	CppUnit_addTest(pSuite, JSONTest, testSetArrayElement);
	CppUnit_addTest(pSuite, JSONTest, testOptValue);
	CppUnit_addTest(pSuite, JSONTest, testQuery);
	CppUnit_addTest(pSuite, JSONTest, testComment);
	CppUnit_addTest(pSuite, JSONTest, testPrintHandler);
	CppUnit_addTest(pSuite, JSONTest, testStringify);
	CppUnit_addTest(pSuite, JSONTest, testStringifyNaN);
	CppUnit_addTest(pSuite, JSONTest, testStringifyPreserveOrder);
	CppUnit_addTest(pSuite, JSONTest, testVarConvert);
	CppUnit_addTest(pSuite, JSONTest, testBasicJson);
	CppUnit_addTest(pSuite, JSONTest, testValidJanssonFiles);
	CppUnit_addTest(pSuite, JSONTest, testInvalidJanssonFiles);
	CppUnit_addTest(pSuite, JSONTest, testInvalidUnicodeJanssonFiles);
	CppUnit_addTest(pSuite, JSONTest, testTemplate);
	CppUnit_addTest(pSuite, JSONTest, testUnicode);
	CppUnit_addTest(pSuite, JSONTest, testEscape0);
	CppUnit_addTest(pSuite, JSONTest, testNonEscapeUnicode);
	CppUnit_addTest(pSuite, JSONTest, testEscapeUnicode);
	CppUnit_addTest(pSuite, JSONTest, testCopy);
	CppUnit_addTest(pSuite, JSONTest, testMove);
	CppUnit_addTest(pSuite, JSONTest, testRemove);

	return pSuite;
}
