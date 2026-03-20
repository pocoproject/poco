//
// PropertyFileConfigurationTest.cpp
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "PropertyFileConfigurationTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/Util/PropertyFileConfiguration.h"
#include "Poco/AutoPtr.h"
#include "Poco/Exception.h"
#include "Poco/TemporaryFile.h"
#include "Poco/FileStream.h"
#include <sstream>
#include <algorithm>
#include "Poco/File.h"
#include "Poco/Path.h"


using Poco::Util::PropertyFileConfiguration;
using Poco::Util::AbstractConfiguration;
using Poco::AutoPtr;
using Poco::NotFoundException;



PropertyFileConfigurationTest::PropertyFileConfigurationTest(const std::string& name): AbstractConfigurationTest(name)
{
}


PropertyFileConfigurationTest::~PropertyFileConfigurationTest()
{
}


void PropertyFileConfigurationTest::testLoad()
{
	static const std::string propFile =
		"! comment\n"
		"! comment\n"
		"prop1=value1\n"
		"prop2 = value2  \n"
		"prop3.prop31: value3\n"
		"# comment\n"
		"  prop3.prop32: value 4\n"
		"prop3.prop33: value5, value6, \\\n"
		"value7, value8, \\\r\n"
		"value9\n"
		"prop4 = escaped[\\t\\r\\n\\f]\n"
		"#prop4 = foo\n"
		"prop5:foo";

	std::istringstream istr(propFile);
	AutoPtr<PropertyFileConfiguration> pConf = new PropertyFileConfiguration(istr);

	assertTrue (pConf->getString("prop1") == "value1");
	assertTrue (pConf->getString("prop2") == "value2");
	assertTrue (pConf->getString("prop3.prop31") == "value3");
	assertTrue (pConf->getString("prop3.prop32") == "value 4");
	assertTrue (pConf->getString("prop3.prop33") == "value5, value6, value7, value8, value9");
	assertTrue (pConf->getString("prop4") == "escaped[\t\r\n\f]");
	assertTrue (pConf->getString("prop5") == "foo");

	AbstractConfiguration::Keys keys;
	pConf->keys(keys);
	assertTrue (keys.size() == 5);
	assertTrue (std::find(keys.begin(), keys.end(), "prop1") != keys.end());
	assertTrue (std::find(keys.begin(), keys.end(), "prop2") != keys.end());
	assertTrue (std::find(keys.begin(), keys.end(), "prop3") != keys.end());
	assertTrue (std::find(keys.begin(), keys.end(), "prop4") != keys.end());
	assertTrue (std::find(keys.begin(), keys.end(), "prop5") != keys.end());

	pConf->keys("prop3", keys);
	assertTrue (keys.size() == 3);
	assertTrue (std::find(keys.begin(), keys.end(), "prop31") != keys.end());
	assertTrue (std::find(keys.begin(), keys.end(), "prop32") != keys.end());
	assertTrue (std::find(keys.begin(), keys.end(), "prop33") != keys.end());

	try
	{
		std::string s = pConf->getString("foo");
		fail("No property - must throw");
	}
	catch (NotFoundException&)
	{
	}

	std::istringstream istr_err(propFile);
	istr_err.putback(std::ios_base::failbit);
	try
	{
		AutoPtr<PropertyFileConfiguration> pConf_err = new PropertyFileConfiguration(istr_err);
	}
	catch (Poco::IOException& exc)
	{
#ifndef POCO_ENABLE_TRACE
		std::string s(exc.message());
		assertTrue (s == "Broken input stream");
#endif
	}
}


void PropertyFileConfigurationTest::testSave()
{
	AutoPtr<PropertyFileConfiguration> pConf = new PropertyFileConfiguration;

	pConf->setString("prop1", "value1");
	pConf->setInt("prop2", 42);
	pConf->setString("prop3", "value\\1\txxx");

	std::ostringstream ostr;
	pConf->save(ostr);
	std::string propFile = ostr.str();
	assertTrue (propFile == "prop1: value1\n"
						"prop2: 42\n"
						"prop3: value\\\\1\\txxx\n");
}


void PropertyFileConfigurationTest::testInclude()
{
	// Write an included properties file
	Poco::TemporaryFile includedFile;
	{
		Poco::FileOutputStream ostr(includedFile.path());
		ostr << "included.prop1 = includedValue1\n";
		ostr << "included.prop2 = includedValue2\n";
	}

	// Write a main properties file that includes the other using an absolute path
	Poco::TemporaryFile mainFile;
	{
		Poco::FileOutputStream ostr(mainFile.path());
		ostr << "main.prop = mainValue\n";
		ostr << "!include " << includedFile.path() << "\n";
		ostr << "main.prop2 = mainValue2\n";
	}

	AutoPtr<PropertyFileConfiguration> pConf = new PropertyFileConfiguration(mainFile.path());

	assertTrue (pConf->getString("main.prop") == "mainValue");
	assertTrue (pConf->getString("main.prop2") == "mainValue2");
	assertTrue (pConf->getString("included.prop1") == "includedValue1");
	assertTrue (pConf->getString("included.prop2") == "includedValue2");

	// Relative include path (same directory, include by filename only)
	Poco::TemporaryFile includedFileRel;
	{
		Poco::FileOutputStream ostr(includedFileRel.path());
		ostr << "includedRel.prop1 = includedRelValue1\n";
		ostr << "includedRel.prop2 = includedRelValue2\n";
	}

	Poco::TemporaryFile mainFileRel;
	{
		Poco::FileOutputStream ostr(mainFileRel.path());
		ostr << "mainRel.prop = mainRelValue\n";
		// include by filename only; should be resolved relative to mainFileRel
		ostr << "!include " << Poco::Path(includedFileRel.path()).getFileName() << "\n";
		ostr << "mainRel.prop2 = mainRelValue2\n";
	}

	AutoPtr<PropertyFileConfiguration> pConfRel = new PropertyFileConfiguration(mainFileRel.path());

	assertTrue (pConfRel->getString("mainRel.prop") == "mainRelValue");
	assertTrue (pConfRel->getString("mainRel.prop2") == "mainRelValue2");
	assertTrue (pConfRel->getString("includedRel.prop1") == "includedRelValue1");
	assertTrue (pConfRel->getString("includedRel.prop2") == "includedRelValue2");

	// Nested includes: main includes A, A includes B (relative paths)
	Poco::TemporaryFile fileB;
	{
		Poco::FileOutputStream ostr(fileB.path());
		ostr << "nestedB.prop = nestedBValue\n";
	}

	Poco::TemporaryFile fileA;
	{
		Poco::FileOutputStream ostr(fileA.path());
		// A includes B by filename
		ostr << "!include " << Poco::Path(fileB.path()).getFileName() << "\n";
		ostr << "nestedA.prop = nestedAValue\n";
	}

	Poco::TemporaryFile mainFileNested;
	{
		Poco::FileOutputStream ostr(mainFileNested.path());
		ostr << "mainNested.prop = mainNestedValue\n";
		// main includes A by filename; A then includes B
		ostr << "!include " << Poco::Path(fileA.path()).getFileName() << "\n";
	}

	AutoPtr<PropertyFileConfiguration> pConfNested = new PropertyFileConfiguration(mainFileNested.path());

	assertTrue (pConfNested->getString("mainNested.prop") == "mainNestedValue");
	assertTrue (pConfNested->getString("nestedA.prop") == "nestedAValue");
	assertTrue (pConfNested->getString("nestedB.prop") == "nestedBValue");

	// Non-existent include should throw
	Poco::TemporaryFile mainFile2;
	{
		Poco::FileOutputStream ostr(mainFile2.path());
		ostr << "prop = value\n";

		// Construct a guaranteed-nonexistent include path in the same directory
		Poco::Path includePath(mainFile2.path());
		includePath.setFileName("nonexistent_include.properties");
		Poco::File includeFile(includePath);
		int counter = 0;
		while (includeFile.exists())
		{
			includePath.setFileName("nonexistent_include_" + std::to_string(++counter) + ".properties");
			includeFile = Poco::File(includePath);
		}

		ostr << "!include " << includePath.toString() << "\n";
	}
	try
	{
		AutoPtr<PropertyFileConfiguration> pConf2 = new PropertyFileConfiguration(mainFile2.path());
		fail("must throw");
	}
	catch (Poco::FileException&)
	{
	}

	// !include with no path should throw SyntaxException
	Poco::TemporaryFile mainFile3;
	{
		Poco::FileOutputStream ostr(mainFile3.path());
		ostr << "prop = value\n";
		ostr << "!include \n";
	}
	try
	{
		AutoPtr<PropertyFileConfiguration> pConf3 = new PropertyFileConfiguration(mainFile3.path());
		fail("must throw");
	}
	catch (Poco::SyntaxException&)
	{
	}

	// !include with tab separator should work
	Poco::TemporaryFile includedFileTab;
	{
		Poco::FileOutputStream ostr(includedFileTab.path());
		ostr << "tab.prop = tabValue\n";
	}

	Poco::TemporaryFile mainFileTab;
	{
		Poco::FileOutputStream ostr(mainFileTab.path());
		ostr << "!include\t" << includedFileTab.path() << "\n";
	}

	AutoPtr<PropertyFileConfiguration> pConfTab = new PropertyFileConfiguration(mainFileTab.path());
	assertTrue (pConfTab->getString("tab.prop") == "tabValue");

	// !includeSomething should be treated as a regular comment
	Poco::TemporaryFile mainFile4;
	{
		Poco::FileOutputStream ostr(mainFile4.path());
		ostr << "!includeSomething\n";
		ostr << "prop = value\n";
	}

	AutoPtr<PropertyFileConfiguration> pConf4 = new PropertyFileConfiguration(mainFile4.path());
	assertTrue (pConf4->getString("prop") == "value");
	assertTrue (!pConf4->hasProperty("includeSomething"));

	// Self-include should throw (cyclic include detection)
	Poco::TemporaryFile selfFile;
	{
		Poco::FileOutputStream ostr(selfFile.path());
		ostr << "prop = value\n";
		ostr << "!include " << selfFile.path() << "\n";
	}
	try
	{
		AutoPtr<PropertyFileConfiguration> pConf5 = new PropertyFileConfiguration(selfFile.path());
		fail("must throw");
	}
	catch (Poco::FileException&)
	{
	}
}


AbstractConfiguration::Ptr PropertyFileConfigurationTest::allocConfiguration() const
{
	return new PropertyFileConfiguration;
}


void PropertyFileConfigurationTest::setUp()
{
}


void PropertyFileConfigurationTest::tearDown()
{
}


CppUnit::Test* PropertyFileConfigurationTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("PropertyFileConfigurationTest");

	AbstractConfigurationTest_addTests(pSuite, PropertyFileConfigurationTest);
	CppUnit_addTest(pSuite, PropertyFileConfigurationTest, testLoad);
	CppUnit_addTest(pSuite, PropertyFileConfigurationTest, testSave);
	CppUnit_addTest(pSuite, PropertyFileConfigurationTest, testInclude);

	return pSuite;
}
