//
// AbstractConfigurationTest.cpp
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "AbstractConfigurationTest.h"
#include "CppUnit/TestCaller.h"
#include "Poco/Util/MapConfiguration.h"
#include "Poco/AutoPtr.h"
#include "Poco/Exception.h"
#include "Poco/Delegate.h"
#include "Poco/NumberFormatter.h"
#include "Poco/Types.h"
#include <algorithm>
#undef min
#undef max
#include <limits>


using Poco::Util::AbstractConfiguration;
using Poco::Util::MapConfiguration;
using Poco::NumberFormatter;
using Poco::AutoPtr;
using Poco::Int64;
using Poco::UInt64;


AbstractConfigurationTest::AbstractConfigurationTest(const std::string& name): CppUnit::TestCase(name)
{
}


AbstractConfigurationTest::~AbstractConfigurationTest()
{
}


void AbstractConfigurationTest::testHasProperty()
{
	AutoPtr<AbstractConfiguration> pConf = createConfiguration();

	assertTrue (pConf->hasProperty("prop1"));
	assertTrue (pConf->hasProperty("prop2"));
	assertTrue (pConf->hasProperty("prop3.string1"));
	assertTrue (!pConf->hasProperty("prop3.string3"));
	assertTrue (!pConf->hasProperty("foobar"));
}


void AbstractConfigurationTest::testGetString()
{
	AutoPtr<AbstractConfiguration> pConf = createConfiguration();

	assertTrue (pConf->getString("prop1") == "foo");
	assertTrue (pConf->getString("prop2") == "bar");
	assertTrue (pConf->getString("prop3.string1") == "foo");
	assertTrue (pConf->getString("prop3.string2") == "bar");
	assertTrue (pConf->getString("ref1") == "foobar");
	assertTrue (pConf->getRawString("ref1") == "${prop3.string1}${prop3.string2}");

	try
	{
		std::string res = pConf->getString("foo");
		fail("nonexistent property - must throw");
	}
	catch (Poco::NotFoundException&)
	{
	}

	assertTrue (pConf->getString("prop1", "FOO") == "foo");
	assertTrue (pConf->getString("prop2", "BAR") == "bar");
	assertTrue (pConf->getString("prop3.string1", "FOO") == "foo");
	assertTrue (pConf->getString("prop3.string2", "BAR") == "bar");
	assertTrue (pConf->getString("prop3.string3", "FOOBAR") == "FOOBAR");
}


void AbstractConfigurationTest::testGetInt()
{
	AutoPtr<AbstractConfiguration> pConf = createConfiguration();

	assertTrue (pConf->getInt("prop4.int1") == 42);
	assertTrue (pConf->getInt("prop4.int2") == -42);
	assertTrue (pConf->getInt("prop4.hex") == 0x1f);
	assertTrue (pConf->getUInt("prop4.hex") == 0x1f);
	assertTrue (pConf->getInt("ref2") == 42);

	try
	{
		pConf->getInt("prop1");
		fail("not a number - must throw");
	}
	catch (Poco::SyntaxException&)
	{
	}

	assertTrue (pConf->getInt("prop4.int1", 100) == 42);
	assertTrue (pConf->getInt("prop4.int2", 100) == -42);
	assertTrue (pConf->getInt("prop4.int3", 100) == 100);
}


void AbstractConfigurationTest::testGetInt16()
{
	AutoPtr<AbstractConfiguration> pConf = createConfiguration();

	assertTrue (pConf->getInt16("prop4.int1") == 42);
	assertTrue (pConf->getInt16("prop4.int2") == -42);
	assertTrue (pConf->getInt16("prop4.hex") == 0x1f);
	assertTrue (pConf->getUInt16("prop4.hex") == 0x1f);
	assertTrue (pConf->getInt16("ref2") == 42);
	
	try
	{
		pConf->getInt16("prop1");
		fail("not a number - must throw");
	}
	catch (Poco::SyntaxException&)
	{
	}

	try
	{
		pConf->getInt16("prop4.notint16");
		fail("too big for UInt16 - must throw");
	}
	catch (Poco::RangeException&)
	{
	}

	try
	{
		pConf->getUInt16("prop4.notuint16");
		fail("too big for UInt16 - must throw");
	}
	catch (Poco::RangeException&)
	{
	}

	assertTrue (pConf->getInt16("prop4.int1", 100) == 42);
	assertTrue (pConf->getInt16("prop4.int2", 100) == -42);
	assertTrue (pConf->getInt16("prop4.int3", 100) == 100);
}


void AbstractConfigurationTest::testGetInt64()
{
#if defined(POCO_HAVE_INT64)
	AutoPtr<AbstractConfiguration> pConf = createConfiguration();

	assertTrue (pConf->getInt64("prop4.bigint1") == std::numeric_limits<Int64>::max());
	assertTrue (pConf->getInt64("prop4.bigint2") == std::numeric_limits<Int64>::min());
	assertTrue (pConf->getUInt64("prop4.biguint") == std::numeric_limits<UInt64>::max());
	assertTrue (pConf->getInt64("prop4.hex") == 0x1f);
	assertTrue (pConf->getUInt64("prop4.hex") == 0x1f);
	assertTrue (pConf->getInt64("ref2") == 42);

	try
	{
		Int64 x = pConf->getInt64("prop1");
		x=x;
		fail("not a number - must throw");
	}
	catch (Poco::SyntaxException&)
	{
	}

	assertTrue (pConf->getInt64("prop4.bigint1", 100) == std::numeric_limits<Int64>::max());
	assertTrue (pConf->getInt64("prop4.bigint2", 100) == std::numeric_limits<Int64>::min());
	assertTrue (pConf->getUInt64("prop4.biguint", 100) == std::numeric_limits<UInt64>::max());
#endif
}


void AbstractConfigurationTest::testGetDouble()
{
	AutoPtr<AbstractConfiguration> pConf = createConfiguration();

	assertTrue (pConf->getDouble("prop4.double1") == 1);
	assertTrue (pConf->getDouble("prop4.double2") == -1.5);

	try
	{
		pConf->getDouble("prop1");
		fail("not a number - must throw");
	}
	catch (Poco::SyntaxException&)
	{
	}

	assertTrue (pConf->getDouble("prop4.double1", 123.5) == 1);
	assertTrue (pConf->getDouble("prop4.double2", 123.5) == -1.5);
	assertTrue (pConf->getDouble("prop4.double3", 123.5) == 123.5);
}


void AbstractConfigurationTest::testGetBool()
{
	AutoPtr<AbstractConfiguration> pConf = createConfiguration();

	assertTrue (pConf->getBool("prop4.bool1"));
	assertTrue (!pConf->getBool("prop4.bool2"));
	assertTrue (pConf->getBool("prop4.bool3"));
	assertTrue (!pConf->getBool("prop4.bool4"));
	assertTrue (pConf->getBool("prop4.bool5"));
	assertTrue (!pConf->getBool("prop4.bool6"));
	assertTrue (pConf->getBool("prop4.bool7"));
	assertTrue (!pConf->getBool("prop4.bool8"));

	try
	{
		pConf->getBool("prop1");
		fail("not a boolean - must throw");
	}
	catch (Poco::SyntaxException&)
	{
	}

	assertTrue (pConf->getBool("prop4.bool1", false));
	assertTrue (!pConf->getBool("prop4.bool2", true));
	assertTrue (pConf->getBool("prop4.boolx", true));
	assertTrue (!pConf->getBool("prop4.booly", false));
}


void AbstractConfigurationTest::testExpand()
{
	AutoPtr<AbstractConfiguration> pConf = createConfiguration();

	assertTrue (pConf->getString("ref1") == "foobar");
	assertTrue (pConf->getInt("ref2") == 42);

	try
	{
		std::string s = pConf->getString("ref3");
		fail("circular reference - must throw");
	}
	catch (Poco::CircularReferenceException&)
	{
	}

	assertTrue (pConf->getString("ref5") == "${refx}");
	assertTrue (pConf->getString("ref6") == "${refx}");

	assertTrue (pConf->expand("answer=${prop4.int1}") == "answer=42");
	assertTrue (pConf->expand("bool5='${prop4.bool5}'") == "bool5='Yes'");
	assertTrue (pConf->expand("undef='${undef}'") == "undef='${undef}'");
	assertTrue (pConf->expand("deep='${ref1}'") == "deep='foobar'");
	assertTrue (pConf->expand("deep='${ref7}'") == "deep='foobar'");

	assertTrue (pConf->getString("dollar.atend") == "foo$");
	assertTrue (pConf->getString("dollar.middle") == "foo$bar");

	assertTrue (pConf->expand("default=${undefined:-default value}") == "default=default value");
	assertTrue (pConf->expand("default=${undefined:-}") == "default=");
	try
	{
		assertTrue (pConf->expand("default=${undefined:value}") == "default=${undefined:value}");
		assertTrue (pConf->expand("default:${undefined::value}") == "default:${undefined::value}");
	}
	catch (Poco::PathSyntaxException&)
	{
		// Note: This will result in an invalid path (on Windows), throwing an exception which can be safely ignored.
	}
}


void AbstractConfigurationTest::testSetString()
{
	AutoPtr<AbstractConfiguration> pConf = createConfiguration();

	pConf->setString("set.string1", "foobar");
	pConf->setString("set.string2", "");
	assertTrue (pConf->getString("set.string1") == "foobar");
	assertTrue (pConf->getString("set.string2") == "");
}

void AbstractConfigurationTest::testSetInt()
{
	AutoPtr<AbstractConfiguration> pConf = createConfiguration();

	pConf->setInt("set.int1", 42);
	pConf->setInt("set.int2", -100);
	pConf->setInt("set.uint", 42U);
	assertTrue (pConf->getInt("set.int1") == 42);
	assertTrue (pConf->getInt("set.int2") == -100);
	assertTrue (pConf->getInt("set.uint") == 42U);
}


void AbstractConfigurationTest::testSetInt64()
{
#if defined(POCO_HAVE_INT64)
	AutoPtr<AbstractConfiguration> pConf = createConfiguration();

	pConf->setInt64("set.bigint1", std::numeric_limits<Int64>::max());
	pConf->setInt64("set.bigint2", std::numeric_limits<Int64>::min());
	pConf->setInt64("set.biguint", std::numeric_limits<UInt64>::max());

	assertTrue (pConf->getInt64("set.bigint1") == std::numeric_limits<Int64>::max());
	assertTrue (pConf->getInt64("set.bigint2") == std::numeric_limits<Int64>::min());
	assertTrue (pConf->getInt64("set.biguint") == std::numeric_limits<UInt64>::max());
#endif //defined(POCO_HAVE_INT64)
}


void AbstractConfigurationTest::testSetDouble()
{
	AutoPtr<AbstractConfiguration> pConf = createConfiguration();

	pConf->setDouble("set.double1", 1.5);
	pConf->setDouble("set.double2", -1.5);
	assertTrue (pConf->getDouble("set.double1") == 1.5);
	assertTrue (pConf->getDouble("set.double2") == -1.5);
}


void AbstractConfigurationTest::testSetBool()
{
	AutoPtr<AbstractConfiguration> pConf = createConfiguration();

	pConf->setBool("set.bool1", true);
	pConf->setBool("set.bool2", false);
	assertTrue (pConf->getBool("set.bool1"));
	assertTrue (!pConf->getBool("set.bool2"));
}


void AbstractConfigurationTest::testChangeEvents()
{
	AutoPtr<AbstractConfiguration> pConf = createConfiguration();

	pConf->propertyChanging += Poco::delegate(this, &AbstractConfigurationTest::onPropertyChanging);
	pConf->propertyChanged += Poco::delegate(this, &AbstractConfigurationTest::onPropertyChanged);

	pConf->setString("set.string1", "foobar");
	assertTrue (_changingKey == "set.string1");
	assertTrue (_changingValue == "foobar");
	assertTrue (_changedKey == "set.string1");
	assertTrue (_changedValue == "foobar");

	pConf->setInt("set.int1", 42);
	assertTrue (_changingKey == "set.int1");
	assertTrue (_changingValue == "42");
	assertTrue (_changedKey == "set.int1");
	assertTrue (_changedValue == "42");

	pConf->setDouble("set.double1", 1.5);
	assertTrue (_changingKey == "set.double1");
	assertTrue (_changingValue == "1.5");
	assertTrue (_changedKey == "set.double1");
	assertTrue (_changedValue == "1.5");

	pConf->setBool("set.bool1", true);
	assertTrue (_changingKey == "set.bool1");
	assertTrue (_changingValue == "true");
	assertTrue (_changedKey == "set.bool1");
	assertTrue (_changedValue == "true");
}


void AbstractConfigurationTest::testRemoveEvents()
{
	AutoPtr<AbstractConfiguration> pConf = createConfiguration();

	pConf->propertyRemoving += Poco::delegate(this, &AbstractConfigurationTest::onPropertyRemoving);
	pConf->propertyRemoved += Poco::delegate(this, &AbstractConfigurationTest::onPropertyRemoved);

	pConf->remove("prop4.bool1");
	assertTrue (_removingKey == "prop4.bool1");
	assertTrue (_removedKey == "prop4.bool1");
}


void AbstractConfigurationTest::testKeys()
{
	AutoPtr<AbstractConfiguration> pConf = createConfiguration();

	AbstractConfiguration::Keys keys;
	pConf->keys(keys);
	assertTrue (keys.size() == 13);
	assertTrue (std::find(keys.begin(), keys.end(), "prop1") != keys.end());
	assertTrue (std::find(keys.begin(), keys.end(), "prop2") != keys.end());
	assertTrue (std::find(keys.begin(), keys.end(), "prop3") != keys.end());
	assertTrue (std::find(keys.begin(), keys.end(), "prop4") != keys.end());
	assertTrue (std::find(keys.begin(), keys.end(), "ref1") != keys.end());
	assertTrue (std::find(keys.begin(), keys.end(), "ref2") != keys.end());
	assertTrue (std::find(keys.begin(), keys.end(), "ref3") != keys.end());
	assertTrue (std::find(keys.begin(), keys.end(), "ref4") != keys.end());
	assertTrue (std::find(keys.begin(), keys.end(), "ref5") != keys.end());
	assertTrue (std::find(keys.begin(), keys.end(), "ref6") != keys.end());
	assertTrue (std::find(keys.begin(), keys.end(), "ref7") != keys.end());
	assertTrue (std::find(keys.begin(), keys.end(), "dollar") != keys.end());

	pConf->keys("prop1", keys);
	assertTrue (keys.empty());

	pConf->keys("prop3", keys);
	assertTrue (keys.size() == 2);
	assertTrue (std::find(keys.begin(), keys.end(), "string1") != keys.end());
	assertTrue (std::find(keys.begin(), keys.end(), "string2") != keys.end());

	assertTrue (!pConf->hasProperty("nonexistent.sub"));
	pConf->keys("nonexistent.sub", keys);
	assertTrue (keys.empty());
}


void AbstractConfigurationTest::testRemove()
{
	AutoPtr<AbstractConfiguration> pConf = createConfiguration();
	AbstractConfiguration::Keys keys;

	assertTrue (pConf->hasProperty("prop1"));
	assertTrue (pConf->hasProperty("prop4.bool1"));
	assertTrue (pConf->hasProperty("prop4.bool2"));
	assertTrue (pConf->hasProperty("prop4.bool3"));
	pConf->keys(keys);
	assertTrue (keys.size() == 13);
	pConf->keys("prop4", keys);
	assertTrue (keys.size() == 19);

	pConf->remove("prop4.bool1");
	assertTrue (!pConf->hasProperty("prop4.bool1"));
	assertTrue (pConf->hasProperty("prop4.bool2"));
	assertTrue (pConf->hasProperty("prop4.bool3"));
	pConf->keys(keys);
	assertTrue (keys.size() == 13);
	pConf->keys("prop4", keys);
	assertTrue (keys.size() == 18);

	pConf->remove("prop4");
	assertTrue (!pConf->hasProperty("prop4.bool1"));
	assertTrue (!pConf->hasProperty("prop4.bool2"));
	assertTrue (!pConf->hasProperty("prop4.bool3"));
	assertTrue (pConf->hasProperty("prop1"));
	pConf->keys(keys);
	assertTrue (keys.size() == 12);
	pConf->keys("prop4", keys);
	assertTrue (keys.size() == 0);

	assertTrue (!pConf->hasProperty("nonexistent.sub.value"));
	pConf->remove("nonexistent.sub.value");
	assertTrue (!pConf->hasProperty("nonexistent.sub.value"));
}


AbstractConfiguration::Ptr AbstractConfigurationTest::createConfiguration() const
{
	AbstractConfiguration::Ptr pConfig = allocConfiguration();

	pConfig->setString("prop1", "foo");
	pConfig->setString("prop2", "bar");
	pConfig->setString("prop3.string1", "foo");
	pConfig->setString("prop3.string2", "bar");
	pConfig->setString("prop4.int1", "42");
	pConfig->setString("prop4.int2", "-42");
	pConfig->setString("prop4.uint", NumberFormatter::format(std::numeric_limits<unsigned>::max()));
	pConfig->setString("prop4.notint16", "32768");
	pConfig->setString("prop4.notuint16", "65536");
#if defined(POCO_HAVE_INT64)
	pConfig->setString("prop4.bigint1", NumberFormatter::format(std::numeric_limits<Int64>::max()));
	pConfig->setString("prop4.bigint2", NumberFormatter::format(std::numeric_limits<Int64>::min()));
	pConfig->setString("prop4.biguint", NumberFormatter::format(std::numeric_limits<UInt64>::max()));
#else /// just to make sure property count is consistent
	pConfig->setString("prop4.bigint1", 0);
	pConfig->setString("prop4.bigint2", 0);
	pConfig->setString("prop4.biguint", 0);
#endif
	pConfig->setString("prop4.hex", "0x1f");
	pConfig->setString("prop4.double1", "1");
	pConfig->setString("prop4.double2", "-1.5");
	pConfig->setString("prop4.bool1", "1");
	pConfig->setString("prop4.bool2", "0");
	pConfig->setString("prop4.bool3", "True");
	pConfig->setString("prop4.bool4", "FALSE");
	pConfig->setString("prop4.bool5", "Yes");
	pConfig->setString("prop4.bool6", "no");
	pConfig->setString("prop4.bool7", "ON");
	pConfig->setString("prop4.bool8", "Off");
	pConfig->setString("prop5.string1", "foo");
	pConfig->setString("prop5.string2", "bar");
	pConfig->setString("prop5.sub1.string1", "FOO");
	pConfig->setString("prop5.sub1.string2", "BAR");
	pConfig->setString("prop5.sub2.string1", "Foo");
	pConfig->setString("prop5.sub2.string2", "Bar");
	pConfig->setString("ref1", "${prop3.string1}${prop3.string2}");
	pConfig->setString("ref2", "${prop4.int1}");
	pConfig->setString("ref3", "${ref4}");
	pConfig->setString("ref4", "${ref3}");
	pConfig->setString("ref5", "${refx}");
	pConfig->setString("ref6", "${refx");
	pConfig->setString("ref7", "${ref1}");
	pConfig->setString("dollar.atend", "foo$");
	pConfig->setString("dollar.middle", "foo$bar");

	return pConfig;
}


void AbstractConfigurationTest::setUp()
{
	_changingKey.clear();
	_changingValue.clear();
	_changedKey.clear();
	_changedValue.clear();
	_removingKey.clear();
	_removedKey.clear();
}


void AbstractConfigurationTest::tearDown()
{
}


void AbstractConfigurationTest::onPropertyChanging(const void*, AbstractConfiguration::KeyValue& kv)
{
	_changingKey   = kv.key();
	_changingValue = kv.value();
}


void AbstractConfigurationTest::onPropertyChanged(const void*, const AbstractConfiguration::KeyValue& kv)
{
	_changedKey   = kv.key();
	_changedValue = kv.value();
}


void AbstractConfigurationTest::onPropertyRemoving(const void*, const std::string& key)
{
	_removingKey = key;
}


void AbstractConfigurationTest::onPropertyRemoved(const void*, const std::string& key)
{
	_removedKey = key;
}

