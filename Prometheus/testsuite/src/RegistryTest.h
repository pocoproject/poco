//
// RegistryTest.h
//
// Definition of the RegistryTest class.
//
// Copyright (c) 2026, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef RegistryTest_INCLUDED
#define RegistryTest_INCLUDED


#include "CppUnit/TestCase.h"


class RegistryTest: public CppUnit::TestCase
{
public:
	RegistryTest() = delete;
	explicit RegistryTest(const std::string& name);
	~RegistryTest() override;

	void testCollectorUnregistersWhenDestroyed();
	void testDestroyedCollectorKeepsReplacement();
	void testUnregisterIgnoresOtherCollectorWithSameName();
	void testCollectorOutlivesRegistry();
	void testUnregisteredCollectorCanRegisterElsewhere();
	void testRegisterWithSecondRegistryFails();
	void testConcurrentExportAndDestruction();

	void setUp() override;
	void tearDown() override;

	static CppUnit::Test* suite();
};


#endif // RegistryTest_INCLUDED
