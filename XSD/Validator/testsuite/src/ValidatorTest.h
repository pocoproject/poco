//
// ValidatorTest.h
//
// Definition of the ValidatorTest class.
//
// Copyright (c) 2021-2026, Applied Informatics Software Engineering GmbH.,
// Aleph ONE Software Engineering LLC
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef ValidatorTest_INCLUDED
#define ValidatorTest_INCLUDED


#include "CppUnit/TestCase.h"


class ValidatorTest: public CppUnit::TestCase
{
public:
	ValidatorTest(const std::string& name);
	~ValidatorTest() override;

	void testValidateAcceptsValidXml();
	void testValidateAcceptsNestedStructure();
	void testValidateAllowsExtraAttributesViaAnyAttribute();
	void testValidateRejectsInvalidEnumValue();
	void testValidateRejectsMissingRequiredAttribute();
	void testValidateRejectsMalformedXml();
	void testValidateRejectsInvalidSchema();
	void testValidateRejectsMalformedSchema();
	void testValidateThrowsOnEmptySchema();
	void testValidatorCannotBeInstantiated();
	void testSchemaIncludeNamedSchemaXsd();
	void testSchemaInternalEntities();
	void testMalformedDocumentFirstError();
	void testValidationMessageFormat();
	void testDocumentTypeDeclarationRejected();
	void testErrorMessageBounded();
	void testValidationMessageSingleLine();
	void testSchemaExternalEntityRejected();
	void testReferencedLocalSchemaLoaded();
	void testNestedReferencedLocalSchemasLoaded();
	void testReferencedSchemaCycle();

	void setUp() override;
	void tearDown() override;

	static CppUnit::Test* suite();
};


#endif // ValidatorTest_INCLUDED
