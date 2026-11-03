//
// UtilityTest.h
//
// Definition of the UtilityTest class.
//
// Copyright (c) 2026, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef UtilityTest_INCLUDED
#define UtilityTest_INCLUDED


#include "Poco/CppParser/CppParser.h"
#include "CppUnit/TestCase.h"
#include <string>


class UtilityTest: public CppUnit::TestCase
{
public:
	UtilityTest() = delete;
	explicit UtilityTest(const std::string& name);
	~UtilityTest() override;

	void testCollectDependenciesWildcardFileName();
	void testCollectDependenciesWildcardDirectory();
	void testCollectDependenciesLiteralPaths();
	void testCollectDependenciesExcludePattern();
	void testCollectDependenciesParentDirectory();
	void testCollectDependenciesAbsolutePattern();
	void testCollectDependenciesDanglingLink();
	void testCollectDependenciesLinkTargetNotSearchable();
	void testCollectDependenciesCaseOnlyRename();
	void testCollectDependenciesHomeAndDeviceNames();
	void testCollectDependenciesFilesAsParsed();

	void setUp() override;
	void tearDown() override;

	static CppUnit::Test* suite();

private:
	[[nodiscard]] std::string inRoot(const std::string& relativePath) const;

	std::string _savedCwd;
	std::string _tempDir;
	std::string _root;
};


#endif // UtilityTest_INCLUDED
