//
// XSDTypesTest.cpp
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "XSDTypesTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/XSD/Types/Sequence.h"
#include "Poco/XSD/Types/ElementTypeRef.h"
#include "Poco/XSD/Types/TypesManager.h"
#include "Poco/XSD/Types/OrderIterator.h"
#include "Poco/XSD/Types/XSDException.h"
#include "Poco/XSD/Types/Choice.h"
#include "Poco/XSD/Types/All.h"


using namespace Poco::XSD::Types;


XSDTypesTest::XSDTypesTest(const std::string& name): CppUnit::TestCase(name)
{
}


XSDTypesTest::~XSDTypesTest() = default;


void XSDTypesTest::testEmptySequence()
{
	Sequence seq("", 1, 1);
	seq.fixup();
	OrderIterator it = seq.iterator();
	assert (it.end());
	const std::set<std::string>& nexts = it.validNexts();
	assert (nexts.empty());
	assert (it.canClose());
	it.close();
	assert (it.end());
}


void XSDTypesTest::testSequenceIterator()
{
	Sequence seq("", 1, 1);
	ElementTypeRef::Ptr pE1 = new ElementTypeRef("", 1, 1, false, false, false, false, "", false, false, "" ,false, "elem1", "urn:ns1", true, QName(), QName("string", TypesManager::XSD_NAMESPACE));
	seq.add(pE1);
	seq.fixup();
	OrderIterator it = seq.iterator();
	assert (!it.end());
	const std::set<std::string>& nexts = it.validNexts();
	assert (nexts.size() == 1);
	assert (it.validNext("elem1"));
	assert (nexts.find("elem1") != nexts.end());
	ElementTypeRef::Ptr pE2 = it.next("elem1").cast<ElementTypeRef>();
	assert (pE2.get() == pE1.get());
	const std::set<std::string>& nexts2 = it.validNexts();
	assert (nexts2.empty());
	it.close();
	assert (it.end());
}


void XSDTypesTest::testSequenceIterator2()
{
	Sequence seq("", 1, 1);
	ElementTypeRef::Ptr pE1 = new ElementTypeRef("", 1, 2, false, false, false, false, "", false, false, "" ,false, "elem1", "urn:ns1", true, QName(), QName("string", TypesManager::XSD_NAMESPACE));
	seq.add(pE1);
	seq.fixup();
	OrderIterator it = seq.iterator();
	assert (!it.end());
	const std::set<std::string>& nexts = it.validNexts();
	assert (nexts.size() == 1);
	assert (it.validNext("elem1"));
	assert (nexts.find("elem1") != nexts.end());
	ElementTypeRef::Ptr pE2 = it.next("elem1").cast<ElementTypeRef>();
	assert (pE2.get() == pE1.get());
	const std::set<std::string>& nexts2 = it.validNexts();
	assert (nexts2.size() == 1);
	assert (it.validNext("elem1"));
	pE2 = it.next("elem1").cast<ElementTypeRef>();
	assert (pE2.get() == pE1.get());
	const std::set<std::string>& nexts3 = it.validNexts();
	assert (nexts3.empty());
	it.close();
	assert (it.end());
}


void XSDTypesTest::testSequenceIterator3()
{
	Sequence seq("", 1, 1);
	ElementTypeRef::Ptr pE1 = new ElementTypeRef("", 0, 2, false, false, false, false, "", false, false, "" ,false, "elem1", "urn:ns1", true, QName(), QName("string", TypesManager::XSD_NAMESPACE));
	seq.add(pE1);
	seq.fixup();
	OrderIterator it = seq.iterator();
	assert (!it.end());
	it.close();
	assert (it.end());
}


void XSDTypesTest::testSequenceIterator4()
{
	Sequence seq("", 1, 1);
	ElementTypeRef::Ptr pE1 = new ElementTypeRef("", 1, 1, false, false, false, false, "", false, false, "" ,false, "elem1", "urn:ns1", true, QName(), QName("string", TypesManager::XSD_NAMESPACE));
	seq.add(pE1);
	seq.fixup();
	OrderIterator it = seq.iterator();
	assert (!it.end());
	try
	{
		it.close();
		fail("must fail");
	}
	catch(IllegalOrderException&)
	{
	}
}


void XSDTypesTest::testSequenceIterator5()
{
	Sequence seq("", 1, 1);
	ElementTypeRef::Ptr pE1 = new ElementTypeRef("", 1, 1, false, false, false, false, "", false, false, "" ,false, "elem1", "urn:ns1", true, QName(), QName("string", TypesManager::XSD_NAMESPACE));
	seq.add(pE1);
	seq.fixup();
	OrderIterator it = seq.iterator();
	assert (!it.end());
	it.next("elem1");
	try
	{
		it.next("elem1");
		fail("must fail");
	}
	catch(IllegalOrderException&)
	{
	}
}


void XSDTypesTest::testSequenceIterator6()
{
	Sequence seq("", 1, 1);
	ElementTypeRef::Ptr pE1 = new ElementTypeRef("", 1, 1, false, false, false, false, "", false, false, "" ,false, "elem1", "urn:ns1", true, QName(), QName("string", TypesManager::XSD_NAMESPACE));
	ElementTypeRef::Ptr pE2 = new ElementTypeRef("", 1, 1, false, false, false, false, "", false, false, "" ,false, "elem2", "urn:ns1", true, QName(), QName("string", TypesManager::XSD_NAMESPACE));
	seq.add(pE1);
	seq.add(pE2);
	seq.fixup();
	OrderIterator it = seq.iterator();
	assert (!it.end());
	const std::set<std::string>& nexts = it.validNexts();
	assert (nexts.size() == 1);
	assert (it.validNext("elem1"));
	assert (nexts.find("elem1") != nexts.end());
	ElementTypeRef::Ptr pR1 = it.next("elem1").cast<ElementTypeRef>();
	assert (pR1.get() == pE1.get());
	const std::set<std::string>& nexts2 = it.validNexts();
	assert (nexts2.size() == 1);
	assert (it.validNext("elem2"));
	assert (nexts2.find("elem2") != nexts2.end());
	ElementTypeRef::Ptr pR2 = it.next("elem2").cast<ElementTypeRef>();
	assert (pR2.get() == pE2.get());
	assert (it.validNexts().empty());
	it.close();
	assert (it.end());
}


void XSDTypesTest::testSequenceIterator7()
{
	Sequence seq("", 1, 1);
	ElementTypeRef::Ptr pE1 = new ElementTypeRef("", 0, 2, false, false, false, false, "", false, false, "" ,false, "elem1", "urn:ns1", true, QName(), QName("string", TypesManager::XSD_NAMESPACE));
	ElementTypeRef::Ptr pE2 = new ElementTypeRef("", 0, 1, false, false, false, false, "", false, false, "" ,false, "elem2", "urn:ns1", true, QName(), QName("string", TypesManager::XSD_NAMESPACE));
	ElementTypeRef::Ptr pE3 = new ElementTypeRef("", 1, 1, false, false, false, false, "", false, false, "" ,false, "elem3", "urn:ns1", true, QName(), QName("string", TypesManager::XSD_NAMESPACE));
	seq.add(pE1);
	seq.add(pE2);
	seq.add(pE3);
	seq.fixup();
	OrderIterator it = seq.iterator();
	assert (!it.end());
	const std::set<std::string>& nexts = it.validNexts();
	assert (nexts.size() == 3);
	assert (it.validNext("elem1"));
	assert (it.validNext("elem2"));
	assert (it.validNext("elem3"));
	it.next("elem1");
	assert (it.validNexts().size() == 3);
	assert (it.validNext("elem1"));
	assert (it.validNext("elem2"));
	assert (it.validNext("elem3"));
	it.next("elem1");
	assert (it.validNexts().size() == 2);
	assert (!it.validNext("elem1"));
	assert (it.validNext("elem2"));
	assert (it.validNext("elem3"));
	it.next("elem3");
	assert (it.validNexts().size() == 0);
	it.close();
	assert (it.end());
}


void XSDTypesTest::testSequenceIterator8()
{
	Sequence seq("", 1, 1);
	ElementTypeRef::Ptr pE1 = new ElementTypeRef("", 0, 2, false, false, false, false, "", false, false, "" ,false, "elem1", "urn:ns1", true, QName(), QName("string", TypesManager::XSD_NAMESPACE));
	ElementTypeRef::Ptr pE2 = new ElementTypeRef("", 0, 1, false, false, false, false, "", false, false, "" ,false, "elem2", "urn:ns1", true, QName(), QName("string", TypesManager::XSD_NAMESPACE));
	ElementTypeRef::Ptr pE3 = new ElementTypeRef("", 2, 2, false, false, false, false, "", false, false, "" ,false, "elem3", "urn:ns1", true, QName(), QName("string", TypesManager::XSD_NAMESPACE));
	seq.add(pE1);
	seq.add(pE2);
	seq.add(pE3);
	seq.fixup();
	OrderIterator it = seq.iterator();
	assert (!it.end());
	const std::set<std::string>& nexts = it.validNexts();
	assert (nexts.size() == 3);
	assert (it.validNext("elem1"));
	assert (it.validNext("elem2"));
	assert (it.validNext("elem3"));
	it.next("elem1");
	assert (it.validNexts().size() == 3);
	assert (it.validNext("elem1"));
	assert (it.validNext("elem2"));
	assert (it.validNext("elem3"));
	it.next("elem1");
	assert (it.validNexts().size() == 2);
	assert (!it.validNext("elem1"));
	assert (it.validNext("elem2"));
	assert (it.validNext("elem3"));
	it.next("elem3");
	assert (it.validNexts().size() == 1);
	it.next("elem3");
	assert (it.validNexts().size() == 0);
	it.close();
	assert (it.end());
}


void XSDTypesTest::testSequenceIterator9()
{
	Sequence seq("", 1, 1);
	ElementTypeRef::Ptr pE1 = new ElementTypeRef("", 2, 2, false, false, false, false, "", false, false, "" ,false, "elem1", "urn:ns1", true, QName(), QName("string", TypesManager::XSD_NAMESPACE));
	seq.add(pE1);
	seq.fixup();
	OrderIterator it = seq.iterator();
	assert (!it.end());
	it.next("elem1");
	try
	{
		it.close();
		fail("must fail");
	}
	catch(IllegalOrderException&)
	{
	}
}


void XSDTypesTest::testSequenceIterator10()
{
	Sequence seq("", 1, 1);
	ElementTypeRef::Ptr pE1 = new ElementTypeRef("", 0, 2, false, false, false, false, "", false, false, "" ,false, "elem1", "urn:ns1", true, QName(), QName("string", TypesManager::XSD_NAMESPACE));
	ElementTypeRef::Ptr pE2 = new ElementTypeRef("", 0, 1, false, false, false, false, "", false, false, "" ,false, "elem2", "urn:ns1", true, QName(), QName("string", TypesManager::XSD_NAMESPACE));
	ElementTypeRef::Ptr pE3 = new ElementTypeRef("", 0, 1, false, false, false, false, "", false, false, "" ,false, "elem3", "urn:ns1", true, QName(), QName("string", TypesManager::XSD_NAMESPACE));
	ElementTypeRef::Ptr pE4 = new ElementTypeRef("", 1, 1, false, false, false, false, "", false, false, "" ,false, "elem4", "urn:ns1", true, QName(), QName("string", TypesManager::XSD_NAMESPACE));
	Sequence::Ptr pSeq = new Sequence("", 0, 1);
	seq.add(pE1);
	
	pSeq->add(pE2);
	pSeq->add(pE3);
	seq.add(pSeq);
	seq.add(pE4);
	seq.fixup();
	OrderIterator it = seq.iterator();
	assert (!it.end());
	const std::set<std::string>& nexts = it.validNexts();
	assert (nexts.size() == 4);
	assert (it.validNext("elem1"));
	assert (it.validNext("elem2"));
	assert (it.validNext("elem3"));
	assert (it.validNext("elem4"));
	it.next("elem2");
	assert (it.validNexts().size() == 2);
	assert (it.validNext("elem3"));
	assert (it.validNext("elem4"));
	it.next("elem4");
	assert (it.validNexts().size() == 0);
	it.close();
	assert (it.end());
}


void XSDTypesTest::testEmptyChoice()
{
	Choice ch("", 1, 1);
	ch.fixup();
	OrderIterator it = ch.iterator();
	assert (it.end());
	const std::set<std::string>& nexts = it.validNexts();
	assert (nexts.empty());
	assert (it.canClose());
	it.close();
	assert (it.end());
}


void XSDTypesTest::testChoiceIterator()
{
	Choice choice("", 1, 1);
	ElementTypeRef::Ptr pE1 = new ElementTypeRef("", 1, 1, false, false, false, false, "", false, false, "" ,false, "elem1", "urn:ns1", true, QName(), QName("string", TypesManager::XSD_NAMESPACE));
	choice.add(pE1);
	choice.fixup();
	OrderIterator it = choice.iterator();
	assert (!it.end());
	const std::set<std::string>& nexts = it.validNexts();
	assert (nexts.size() == 1);
	assert (it.validNext("elem1"));
	assert (nexts.find("elem1") != nexts.end());
	ElementTypeRef::Ptr pE2 = it.next("elem1").cast<ElementTypeRef>();
	assert (pE2.get() == pE1.get());
	const std::set<std::string>& nexts2 = it.validNexts();
	assert (nexts2.empty());
	it.close();
	assert (it.end());
}


void XSDTypesTest::testChoiceIterator2()
{
	Choice choice("", 0, 1);
	ElementTypeRef::Ptr pE1 = new ElementTypeRef("", 1, 1, false, false, false, false, "", false, false, "" ,false, "elem1", "urn:ns1", true, QName(), QName("string", TypesManager::XSD_NAMESPACE));
	choice.add(pE1);
	choice.fixup();
	OrderIterator it = choice.iterator();
	assert (!it.end());
	const std::set<std::string>& nexts = it.validNexts();
	assert (nexts.size() == 1);
	assert (it.validNext("elem1"));
	assert (nexts.find("elem1") != nexts.end());
	assert (it.canClose());
	it.close();
	assert (it.end());
}


void XSDTypesTest::testChoiceIterator3()
{
	Choice choice("", 0, 1);
	ElementTypeRef::Ptr pE1 = new ElementTypeRef("", 2, 2, false, false, false, false, "", false, false, "" ,false, "elem1", "urn:ns1", true, QName(), QName("string", TypesManager::XSD_NAMESPACE));
	choice.add(pE1);
	choice.fixup();
	OrderIterator it = choice.iterator();
	assert (!it.end());
	const std::set<std::string>& nexts = it.validNexts();
	assert (nexts.size() == 1);
	assert (it.validNext("elem1"));
	assert (nexts.find("elem1") != nexts.end());
	assert (it.canClose());
	it.next("elem1");
	assert (!it.canClose());
	assert (it.validNexts().size() == 1);
	it.next("elem1");
	assert (it.canClose());
	assert (it.validNexts().size() == 0);
	it.close();
	assert (it.end());
}


void XSDTypesTest::testChoiceIterator4()
{
	Choice choice("", 2, 2);
	ElementTypeRef::Ptr pE1 = new ElementTypeRef("", 1, 1, false, false, false, false, "", false, false, "" ,false, "elem1", "urn:ns1", true, QName(), QName("string", TypesManager::XSD_NAMESPACE));
	choice.add(pE1);
	choice.fixup();
	OrderIterator it = choice.iterator();
	assert (!it.end());
	const std::set<std::string>& nexts = it.validNexts();
	assert (nexts.size() == 1);
	assert (it.validNext("elem1"));
	assert (nexts.find("elem1") != nexts.end());
	assert (!it.canClose());
	it.next("elem1");
	assert (!it.canClose());
	assert (it.validNexts().size() == 1);
	it.next("elem1");
	assert (it.canClose());
	assert (it.validNexts().size() == 0);
	it.close();
	assert (it.end());
}


void XSDTypesTest::testChoiceIterator5()
{
	Choice choice("", 1, 1);
	ElementTypeRef::Ptr pE1 = new ElementTypeRef("", 1, 1, false, false, false, false, "", false, false, "" ,false, "elem1", "urn:ns1", true, QName(), QName("string", TypesManager::XSD_NAMESPACE));
	ElementTypeRef::Ptr pE2 = new ElementTypeRef("", 1, 1, false, false, false, false, "", false, false, "" ,false, "elem2", "urn:ns1", true, QName(), QName("string", TypesManager::XSD_NAMESPACE));
	ElementTypeRef::Ptr pE3 = new ElementTypeRef("", 1, 1, false, false, false, false, "", false, false, "" ,false, "elem3", "urn:ns1", true, QName(), QName("string", TypesManager::XSD_NAMESPACE));
	choice.add(pE1);
	choice.add(pE2);
	choice.add(pE3);
	choice.fixup();
	OrderIterator it = choice.iterator();
	assert (!it.end());
	const std::set<std::string>& nexts = it.validNexts();
	assert (nexts.size() == 3);
	assert (it.validNext("elem1"));
	assert (it.validNext("elem2"));
	assert (it.validNext("elem3"));
	it.next("elem1");
	assert (it.validNexts().empty());
	assert (it.canClose());
	it.reset();
	assert (it.validNexts().size() == 3);
	it.next("elem2");
	assert (it.validNexts().empty());
	assert (it.canClose());
	it.reset();
	assert (it.validNexts().size() == 3);
	it.next("elem2");
	assert (it.validNexts().empty());
	assert (it.canClose());
	it.close();
	assert (it.end());
}


void XSDTypesTest::testChoiceIterator6()
{
	Choice choice("", 1, 2);
	ElementTypeRef::Ptr pE1 = new ElementTypeRef("", 1, 1, false, false, false, false, "", false, false, "" ,false, "elem1", "urn:ns1", true, QName(), QName("string", TypesManager::XSD_NAMESPACE));
	ElementTypeRef::Ptr pE2 = new ElementTypeRef("", 1, 1, false, false, false, false, "", false, false, "" ,false, "elem2", "urn:ns1", true, QName(), QName("string", TypesManager::XSD_NAMESPACE));
	ElementTypeRef::Ptr pE3 = new ElementTypeRef("", 1, 1, false, false, false, false, "", false, false, "" ,false, "elem3", "urn:ns1", true, QName(), QName("string", TypesManager::XSD_NAMESPACE));
	choice.add(pE1);
	choice.add(pE2);
	choice.add(pE3);
	choice.fixup();
	OrderIterator it = choice.iterator();
	assert (!it.end());
	const std::set<std::string>& nexts = it.validNexts();
	assert (nexts.size() == 3);
	assert (it.validNext("elem1"));
	assert (it.validNext("elem2"));
	assert (it.validNext("elem3"));
	it.next("elem1");
	assert (it.validNexts().size() == 3);
	assert (it.canClose());
	it.next("elem3");
	assert (it.validNexts().empty());
	assert (it.canClose());
	it.close();
	assert (it.end());
}


void XSDTypesTest::testChoiceIterator7()
{
	Choice choice("", 1, 2);
	ElementTypeRef::Ptr pE1 = new ElementTypeRef("", 1, 1, false, false, false, false, "", false, false, "" ,false, "elem1", "urn:ns1", true, QName(), QName("string", TypesManager::XSD_NAMESPACE));
	ElementTypeRef::Ptr pE2 = new ElementTypeRef("", 0, 2, false, false, false, false, "", false, false, "" ,false, "elem2", "urn:ns1", true, QName(), QName("string", TypesManager::XSD_NAMESPACE));
	ElementTypeRef::Ptr pE3 = new ElementTypeRef("", 1, 1, false, false, false, false, "", false, false, "" ,false, "elem3", "urn:ns1", true, QName(), QName("string", TypesManager::XSD_NAMESPACE));
	choice.add(pE1);
	choice.add(pE2);
	choice.add(pE3);
	choice.fixup();
	OrderIterator it = choice.iterator();
	assert (!it.end());
	const std::set<std::string>& nexts = it.validNexts();
	assert (nexts.size() == 3);
	assert (it.validNext("elem1"));
	assert (it.validNext("elem2"));
	assert (it.validNext("elem3"));
	it.next("elem1");
	assert (it.validNexts().size() == 3);
	assert (it.canClose());
	it.next("elem2");
	assert (it.validNexts().size() == 1);
	assert (it.validNext("elem2"));
	assert (it.canClose());
	it.next("elem2");
	it.close();
	assert (it.end());
}


void XSDTypesTest::testChoiceIterator8()
{
	Choice choice("", 1, 2);
	ElementTypeRef::Ptr pE1 = new ElementTypeRef("", 1, 1, false, false, false, false, "", false, false, "" ,false, "elem1", "urn:ns1", true, QName(), QName("string", TypesManager::XSD_NAMESPACE));
	ElementTypeRef::Ptr pE2 = new ElementTypeRef("", 2, 2, false, false, false, false, "", false, false, "" ,false, "elem2", "urn:ns1", true, QName(), QName("string", TypesManager::XSD_NAMESPACE));
	ElementTypeRef::Ptr pE3 = new ElementTypeRef("", 1, 1, false, false, false, false, "", false, false, "" ,false, "elem3", "urn:ns1", true, QName(), QName("string", TypesManager::XSD_NAMESPACE));
	choice.add(pE1);
	choice.add(pE2);
	choice.add(pE3);
	choice.fixup();
	OrderIterator it = choice.iterator();
	assert (!it.end());
	const std::set<std::string>& nexts = it.validNexts();
	assert (nexts.size() == 3);
	assert (it.validNext("elem1"));
	assert (it.validNext("elem2"));
	assert (it.validNext("elem3"));
	it.next("elem2");
	assert (it.validNexts().size() == 1);
	assert (it.validNext("elem2"));
	assert (!it.canClose());
	it.next("elem2");
	assert (it.canClose());
	assert (it.validNexts().size() == 3);
	it.next("elem1");
	assert (it.canClose());
	assert (it.validNexts().size() == 0);
	assert (it.canClose());
	it.close();
	assert (it.end());
}


void XSDTypesTest::testChoiceIterator9()
{
	Choice choice("", 1, 1);
	ElementTypeRef::Ptr pE1 = new ElementTypeRef("", 0, 2, false, false, false, false, "", false, false, "" ,false, "elem1", "urn:ns1", true, QName(), QName("string", TypesManager::XSD_NAMESPACE));
	ElementTypeRef::Ptr pE2 = new ElementTypeRef("", 1, 1, false, false, false, false, "", false, false, "" ,false, "elem2", "urn:ns1", true, QName(), QName("string", TypesManager::XSD_NAMESPACE));
	ElementTypeRef::Ptr pE3 = new ElementTypeRef("", 0, 1, false, false, false, false, "", false, false, "" ,false, "elem3", "urn:ns1", true, QName(), QName("string", TypesManager::XSD_NAMESPACE));
	ElementTypeRef::Ptr pE4 = new ElementTypeRef("", 1, 1, false, false, false, false, "", false, false, "" ,false, "elem4", "urn:ns1", true, QName(), QName("string", TypesManager::XSD_NAMESPACE));
	ElementTypeRef::Ptr pE5 = new ElementTypeRef("", 1, 1, false, false, false, false, "", false, false, "" ,false, "elem5", "urn:ns1", true, QName(), QName("string", TypesManager::XSD_NAMESPACE));
	Sequence::Ptr pSeq = new Sequence("", 0, 1);
	pSeq->add(pE2);
	pSeq->add(pE3);
	pSeq->add(pE4);
	choice.add(pE1);
	choice.add(pSeq);
	choice.add(pE5);
	choice.fixup();
	OrderIterator it = choice.iterator();
	assert (!it.end());
	const std::set<std::string>& nexts = it.validNexts();
	assert (nexts.size() == 3);
	assert (it.validNext("elem1"));
	assert (it.validNext("elem2"));
	assert (it.validNext("elem5"));
	it.next("elem2");
	assert (it.validNexts().size() == 2);
	assert (!it.canClose());
	assert (it.validNext("elem3"));
	assert (it.validNext("elem4"));
	it.next("elem4");
	assert (it.validNexts().size() == 0);
	assert (it.canClose());
	it.close();
	assert (it.end());
}


void XSDTypesTest::testChoiceIterator10()
{
	Choice choice("", 1, 1);
	ElementTypeRef::Ptr pE1 = new ElementTypeRef("", 0, 2, false, false, false, false, "", false, false, "" ,false, "elem1", "urn:ns1", true, QName(), QName("string", TypesManager::XSD_NAMESPACE));
	ElementTypeRef::Ptr pE2 = new ElementTypeRef("", 0, 1, false, false, false, false, "", false, false, "" ,false, "elem2", "urn:ns1", true, QName(), QName("string", TypesManager::XSD_NAMESPACE));
	ElementTypeRef::Ptr pE3 = new ElementTypeRef("", 0, 1, false, false, false, false, "", false, false, "" ,false, "elem3", "urn:ns1", true, QName(), QName("string", TypesManager::XSD_NAMESPACE));
	ElementTypeRef::Ptr pE4 = new ElementTypeRef("", 0, 1, false, false, false, false, "", false, false, "" ,false, "elem4", "urn:ns1", true, QName(), QName("string", TypesManager::XSD_NAMESPACE));
	ElementTypeRef::Ptr pE5 = new ElementTypeRef("", 1, 1, false, false, false, false, "", false, false, "" ,false, "elem5", "urn:ns1", true, QName(), QName("string", TypesManager::XSD_NAMESPACE));
	Sequence::Ptr pSeq = new Sequence("", 0, 1);
	pSeq->add(pE2);
	pSeq->add(pE3);
	pSeq->add(pE4);
	choice.add(pE1);
	choice.add(pSeq);
	choice.add(pE5);
	choice.fixup();
	OrderIterator it = choice.iterator();
	assert (!it.end());
	const std::set<std::string>& nexts = it.validNexts();
	assert (nexts.size() == 5);
	assert (it.validNext("elem1"));
	assert (it.validNext("elem2"));
	assert (it.validNext("elem3"));
	assert (it.validNext("elem4"));
	assert (it.validNext("elem5"));
	it.next("elem2");
	assert (it.validNexts().size() == 2);
	assert (it.canClose());
	assert (it.validNext("elem3"));
	assert (it.validNext("elem4"));
	it.next("elem3");
	assert (it.validNexts().size() == 1);
	assert (it.canClose());
	assert (it.validNext("elem4"));
	it.next("elem4");
	it.close();
	assert (it.end());
}


void XSDTypesTest::testChoiceIterator11()
{
	Choice ch("", 1, 1);
	ElementTypeRef::Ptr pE1 = new ElementTypeRef("", 1, 2, false, false, false, false, "", false, false, "" ,false, "elem1", "urn:ns1", true, QName(), QName("string", TypesManager::XSD_NAMESPACE));
	ch.add(pE1);
	ch.fixup();
	OrderIterator it = ch.iterator();
	assert (!it.end());
	try
	{
		it.close();
		fail("must fail");
	}
	catch(IllegalOrderException&)
	{
	}
}


void XSDTypesTest::testChoiceIterator12()
{
	Choice ch("", 0, 1);
	ElementTypeRef::Ptr pE1 = new ElementTypeRef("", 0, 2, false, false, false, false, "", false, false, "" ,false, "elem1", "urn:ns1", true, QName(), QName("string", TypesManager::XSD_NAMESPACE));
	ch.add(pE1);
	ch.fixup();
	OrderIterator it = ch.iterator();
	assert (!it.end());
	it.close();
	assert (it.end());
}


void XSDTypesTest::testChoiceIterator13()
{
	Choice ch("", 0, 1);
	ElementTypeRef::Ptr pE1 = new ElementTypeRef("", 0, 0, false, false, false, false, "", false, false, "" ,false, "elem1", "urn:ns1", true, QName(), QName("string", TypesManager::XSD_NAMESPACE));
	ch.add(pE1);
	ch.fixup();
	OrderIterator it = ch.iterator();
	assert (it.end());
	try
	{
		it.next("elem1");
		fail("must fail");
	}
	catch(IllegalOrderException&)
	{
	}

	it.close();
}


void XSDTypesTest::testEmptyAll()
{
	All all("", 1);
	all.fixup();
	OrderIterator it = all.iterator();
	assert (it.end());
	const std::set<std::string>& nexts = it.validNexts();
	assert (nexts.empty());
	assert (it.canClose());
	it.close();
	assert (it.end());
}


void XSDTypesTest::testAllIterator()
{
	All all("", 1);
	ElementTypeRef::Ptr pE1 = new ElementTypeRef("", 1, 1, false, false, false, false, "", false, false, "" ,false, "elem1", "urn:ns1", true, QName(), QName("string", TypesManager::XSD_NAMESPACE));
	all.add(pE1);
	all.fixup();
	OrderIterator it = all.iterator();
	assert (!it.end());
	const std::set<std::string>& nexts = it.validNexts();
	assert (nexts.size() == 1);
	assert (it.validNext("elem1"));
	assert (nexts.find("elem1") != nexts.end());
	ElementTypeRef::Ptr pE2 = it.next("elem1").cast<ElementTypeRef>();
	assert (pE2.get() == pE1.get());
	const std::set<std::string>& nexts2 = it.validNexts();
	assert (nexts2.empty());
	assert (it.canClose());
	it.close();
	assert (it.end());
}


void XSDTypesTest::testAllIterator2()
{
	All all("", 1);
	ElementTypeRef::Ptr pE1 = new ElementTypeRef("", 1, 2, false, false, false, false, "", false, false, "" ,false, "elem1", "urn:ns1", true, QName(), QName("string", TypesManager::XSD_NAMESPACE));
	all.add(pE1);
	all.fixup();
	OrderIterator it = all.iterator();
	assert (!it.end());
	const std::set<std::string>& nexts = it.validNexts();
	assert (nexts.size() == 1);
	assert (it.validNext("elem1"));
	assert (nexts.find("elem1") != nexts.end());
	ElementTypeRef::Ptr pE2 = it.next("elem1").cast<ElementTypeRef>();
	assert (pE2.get() == pE1.get());
	const std::set<std::string>& nexts2 = it.validNexts();
	assert (!nexts2.empty());
	assert (it.canClose());
	it.close();
	assert (it.end());
}


void XSDTypesTest::testAllIterator3()
{
	All all("", 1);
	ElementTypeRef::Ptr pE1 = new ElementTypeRef("", 2, 2, false, false, false, false, "", false, false, "" ,false, "elem1", "urn:ns1", true, QName(), QName("string", TypesManager::XSD_NAMESPACE));
	all.add(pE1);
	all.fixup();
	OrderIterator it = all.iterator();
	assert (!it.end());
	const std::set<std::string>& nexts = it.validNexts();
	assert (nexts.size() == 1);
	assert (it.validNext("elem1"));
	assert (nexts.find("elem1") != nexts.end());
	ElementTypeRef::Ptr pE2 = it.next("elem1").cast<ElementTypeRef>();
	assert (pE2.get() == pE1.get());
	assert (!it.validNexts().empty());
	assert (!it.canClose());
	it.next("elem1");
	assert (it.validNexts().empty());
	assert (it.canClose());
	it.close();
	assert (it.end());
}


void XSDTypesTest::setUp()
{
}


void XSDTypesTest::tearDown()
{
}


CppUnit::Test* XSDTypesTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("XSDTypesTest");

	CppUnit_addTest(pSuite, XSDTypesTest, testEmptySequence);
	CppUnit_addTest(pSuite, XSDTypesTest, testSequenceIterator);
	CppUnit_addTest(pSuite, XSDTypesTest, testSequenceIterator2);
	CppUnit_addTest(pSuite, XSDTypesTest, testSequenceIterator3);
	CppUnit_addTest(pSuite, XSDTypesTest, testSequenceIterator4);
	CppUnit_addTest(pSuite, XSDTypesTest, testSequenceIterator5);
	CppUnit_addTest(pSuite, XSDTypesTest, testSequenceIterator6);
	CppUnit_addTest(pSuite, XSDTypesTest, testSequenceIterator7);
	CppUnit_addTest(pSuite, XSDTypesTest, testSequenceIterator8);
	CppUnit_addTest(pSuite, XSDTypesTest, testSequenceIterator9);
	CppUnit_addTest(pSuite, XSDTypesTest, testSequenceIterator10);

	CppUnit_addTest(pSuite, XSDTypesTest, testEmptyChoice);
	CppUnit_addTest(pSuite, XSDTypesTest, testChoiceIterator);
	CppUnit_addTest(pSuite, XSDTypesTest, testChoiceIterator2);
	CppUnit_addTest(pSuite, XSDTypesTest, testChoiceIterator3);
	CppUnit_addTest(pSuite, XSDTypesTest, testChoiceIterator4);
	CppUnit_addTest(pSuite, XSDTypesTest, testChoiceIterator5);
	CppUnit_addTest(pSuite, XSDTypesTest, testChoiceIterator6);
	CppUnit_addTest(pSuite, XSDTypesTest, testChoiceIterator7);
	CppUnit_addTest(pSuite, XSDTypesTest, testChoiceIterator8);
	CppUnit_addTest(pSuite, XSDTypesTest, testChoiceIterator9);
	CppUnit_addTest(pSuite, XSDTypesTest, testChoiceIterator10);
	CppUnit_addTest(pSuite, XSDTypesTest, testChoiceIterator11);
	CppUnit_addTest(pSuite, XSDTypesTest, testChoiceIterator12);
	CppUnit_addTest(pSuite, XSDTypesTest, testChoiceIterator13);

	CppUnit_addTest(pSuite, XSDTypesTest, testEmptyAll);
	CppUnit_addTest(pSuite, XSDTypesTest, testAllIterator);
	CppUnit_addTest(pSuite, XSDTypesTest, testAllIterator2);
	CppUnit_addTest(pSuite, XSDTypesTest, testAllIterator3);

	return pSuite;
}
