//
// SQLTest.cpp
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "SQLTest.h"
#include "Poco/CppUnit/TestCaller.h"
#include "Poco/CppUnit/TestSuite.h"
#include "Poco/SQL/Session.h"
#include "Poco/SQL/SessionFactory.h"
#include "Poco/SQL/LOB.h"
#include "Poco/SQL/LOBStream.h"
#include "Poco/SQL/MetaColumn.h"
#include "Poco/SQL/Column.h"
#include "Poco/SQL/Date.h"
#include "Poco/SQL/Time.h"
#include "Poco/SQL/SimpleRowFormatter.h"
#include "Poco/SQL/JSONRowFormatter.h"
#include "Poco/SQL/SQLException.h"
#include "Connector.h"
#include "Poco/BinaryReader.h"
#include "Poco/BinaryWriter.h"
#include "Poco/DateTime.h"
#include "Poco/Types.h"
#include "Poco/Dynamic/Var.h"
#include "Poco/SQL/DynamicLOB.h"
#include "Poco/SQL/DynamicDateTime.h"
#include "Poco/Exception.h"
#include <cstring>
#include <sstream>
#include <iomanip>
#include <set>
#include <tuple>


using namespace Poco::SQL::Keywords;


using Poco::BinaryReader;
using Poco::BinaryWriter;
using Poco::UInt32;
using Poco::Int64;
using Poco::UInt64;
using Poco::DateTime;
using Poco::Dynamic::Var;
using Poco::InvalidAccessException;
using Poco::IllegalStateException;
using Poco::RangeException;
using Poco::NotFoundException;
using Poco::InvalidArgumentException;
using Poco::NotImplementedException;
using Poco::SQL::Session;
using Poco::SQL::SessionFactory;
using Poco::SQL::Statement;
using Poco::SQL::NotSupportedException;
using Poco::SQL::CLOB;
using Poco::SQL::CLOBInputStream;
using Poco::SQL::CLOBOutputStream;
using Poco::SQL::MetaColumn;
using Poco::SQL::Column;
using Poco::SQL::Row;
using Poco::SQL::RowFormatter;
using Poco::SQL::SimpleRowFormatter;
using Poco::SQL::JSONRowFormatter;
using Poco::SQL::Date;
using Poco::SQL::Time;
using Poco::SQL::AbstractExtraction;
using Poco::SQL::AbstractExtractionVec;
using Poco::SQL::AbstractExtractionVecVec;
using Poco::SQL::AbstractBinding;
using Poco::SQL::AbstractBindingVec;
using Poco::SQL::NotConnectedException;


SQLTest::SQLTest(const std::string& name): CppUnit::TestCase(name)
{
	Poco::SQL::Test::Connector::addToFactory();
}


SQLTest::~SQLTest()
{
	Poco::SQL::Test::Connector::removeFromFactory();
}


void SQLTest::testSession()
{
	Session sess(SessionFactory::instance().create("test", "cs"));
	assertTrue ("test" == sess.impl()->connectorName());
	assertTrue (sess.connector() == sess.impl()->connectorName());
	assertTrue ("cs" == sess.impl()->connectionString());
	assertTrue ("test:///cs" == sess.uri());

	assertTrue (sess.getLoginTimeout() == Session::LOGIN_TIMEOUT_DEFAULT);
	sess.setLoginTimeout(123);
	assertTrue (sess.getLoginTimeout() == 123);

	Session sess2(SessionFactory::instance().create("TeSt:///Cs"));
	assertTrue ("test" == sess2.impl()->connectorName());
	assertTrue ("Cs" == sess2.impl()->connectionString());
	assertTrue ("test:///Cs" == sess2.uri());

	sess << "DROP TABLE IF EXISTS Test", now;
	int count;
	sess << "SELECT COUNT(*) FROM PERSON", into(count), now;

	std::string str;
	Statement stmt = (sess << "SELECT * FROM Strings", into(str), limit(50));
	stmt.execute();

	sess.close();
	assertTrue (!sess.getFeature("connected"));
	assertTrue (!sess.isConnected());

	try
	{
		stmt.execute();
		fail ("must fail");
	} catch (NotConnectedException&) { }

	try
	{
		sess << "SELECT * FROM Strings", now;
		fail ("must fail");
	} catch (NotConnectedException&) { }

	sess.open();
	assertTrue (sess.getFeature("connected"));
	assertTrue (sess.isConnected());
	
	sess << "SELECT * FROM Strings", now;
	stmt.execute();

	sess.reconnect();
	assertTrue (sess.getFeature("connected"));
	assertTrue (sess.isConnected());

	sess << "SELECT * FROM Strings", now;
	stmt.execute();
}


void SQLTest::testStatementFormatting()
{
	Session sess(SessionFactory::instance().create("test", "cs"));

	Statement stmt = (sess << "SELECT %s%c%s,%d,%u,%f,%s FROM Person WHERE Name LIKE 'Simp%%'",
		"'",'a',"'",-1, 1u, 1.5, "42", now);
	
	assertTrue ("SELECT 'a',-1,1,1.500000,42 FROM Person WHERE Name LIKE 'Simp%'" == stmt.toString());
}


void SQLTest::testFeatures()
{
	Session sess(SessionFactory::instance().create("test", "cs"));
	
	sess.setFeature("f1", true);
	assertTrue (sess.getFeature("f1"));
	assertTrue (sess.getFeature("f2"));

	try
	{
		sess.setFeature("f2", false);
	}
	catch (NotImplementedException&)
	{
	}

	sess.setFeature("f3", false);
	assertTrue (!sess.getFeature("f2"));

	try
	{
		sess.setFeature("f3", true);
	}
	catch (NotImplementedException&)
	{
	}

	try
	{
		sess.setFeature("f4", false);
	}
	catch (NotSupportedException&)
	{
	}
}


void SQLTest::testProperties()
{
	Session sess(SessionFactory::instance().create("test", "cs"));

	sess.setProperty("p1", 1);
	Poco::Any v1 = sess.getProperty("p1");
	assertTrue (Poco::AnyCast<int>(v1) == 1);
	Poco::Any v2 = sess.getProperty("p2");
	assertTrue (Poco::AnyCast<int>(v2) == 1);

	try
	{
		sess.setProperty("p2", 2);
	}
	catch (NotImplementedException&)
	{
	}

	sess.setProperty("p3", 2);
	v1 = sess.getProperty("p2");
	assertTrue (Poco::AnyCast<int>(v1) == 2);

	try
	{
		sess.setProperty("p3", 3);
	}
	catch (NotImplementedException&)
	{
	}

	try
	{
		sess.setProperty("p4", 4);
	}
	catch (NotSupportedException&)
	{
	}
}


void SQLTest::testLOB()
{
	std::vector<int> vec;
	vec.push_back(0);
	vec.push_back(1);
	vec.push_back(2);
	vec.push_back(3);
	vec.push_back(4);
	vec.push_back(5);
	vec.push_back(6);
	vec.push_back(7);
	vec.push_back(8);
	vec.push_back(9);

	Poco::SQL::LOB<int> lobNum1(&vec[0], vec.size());
	assertTrue (lobNum1.size() == vec.size());
	assertTrue (0 == std::memcmp(&vec[0], lobNum1.rawContent(), lobNum1.size() * sizeof(int)));
	assertTrue (*lobNum1.begin() == 0);
	Poco::SQL::LOB<int>::Iterator it1 = lobNum1.end();
	assertTrue (*(--it1) == 9);

	Poco::SQL::LOB<int> lobNum2(lobNum1);
	assertTrue (lobNum2.size() == lobNum1.size());
	assertTrue (lobNum2 == lobNum1);
	lobNum1.swap(lobNum2);
	assertTrue (lobNum2 == lobNum1);
}


void SQLTest::testCLOB()
{
	std::string strDigit = "1234567890";
	std::string strAlpha = "abcdefghijklmnopqrstuvwxyz";
	std::vector<char> vecAlpha(strAlpha.begin(), strAlpha.end());
	std::vector<char> vecDigit(strDigit.begin(), strDigit.end());

	CLOB blobNumStr(strDigit.c_str(), strDigit.size());
	assertTrue (blobNumStr.size() == strDigit.size());
	assertTrue (0 == std::strncmp(strDigit.c_str(), blobNumStr.rawContent(), blobNumStr.size()));
	assertTrue (*blobNumStr.begin() == '1');
	CLOB::Iterator itNumStr = blobNumStr.end();
	assertTrue (*(--itNumStr) == '0');
	CLOB blobNumVec(vecDigit);
	assertTrue (blobNumVec.size() == vecDigit.size());
	assertTrue (blobNumVec == blobNumStr);
	blobNumVec.swap(blobNumStr);
	assertTrue (blobNumVec.size() == blobNumStr.size());
	assertTrue (blobNumVec == blobNumStr);

	CLOB blobChrStr(strAlpha.c_str(), strAlpha.size());
	CLOB blobChrVec(vecAlpha);
	assertTrue (blobChrStr.size() == strAlpha.size());
	assertTrue (0 == std::strncmp(strAlpha.c_str(), blobChrStr.rawContent(), blobChrStr.size()));
	assertTrue (*blobChrStr.begin() == 'a');
	CLOB::Iterator itChrStr = blobChrStr.end();
	assertTrue (*(--itChrStr) == 'z');
	assertTrue (blobChrStr == blobChrVec);

	blobNumStr.swap(blobChrStr);
	assertTrue (blobNumStr != blobChrStr);
	assertTrue (&blobNumStr != &blobChrStr);
	assertTrue (blobNumStr.content() != blobChrStr.content());
	assertTrue (&blobNumStr.content() != &blobChrStr.content());
	assertTrue (blobNumStr == blobChrVec);

	Poco::SQL::swap(blobNumStr, blobChrVec);
	assertTrue (blobNumStr == blobChrVec);
	std::swap(blobNumStr, blobChrVec);
	assertTrue (blobNumStr == blobChrVec);

	assertTrue (blobChrStr != blobNumStr);
	Var vLOB = blobNumStr;
	std::string sss = vLOB.convert<std::string>();
	blobChrStr = CLOB(sss);
	assertTrue (blobChrStr == blobNumStr);

	std::string xyz = "xyz";
	vLOB = xyz;
	blobChrStr = sss = vLOB.convert<std::string>();
	assertTrue (0 == std::strncmp(xyz.c_str(), blobChrStr.rawContent(), blobChrStr.size()));
}


void SQLTest::testCLOBStreams()
{
	CLOB blob;
	assertTrue (0 == blob.size());

	CLOBOutputStream bos(blob);
	BinaryWriter bw(bos);

	assertTrue (0 == blob.size());
	writeToCLOB(bw);
	assertTrue (blob.size() > 0);

	CLOBInputStream bis(blob);
	BinaryReader br(bis);

	readFromCLOB(br);
}


void SQLTest::writeToCLOB(BinaryWriter& writer)
{
	writer << true;
	writer << false;
	writer << 'a';

	writer << (short) -100;
	writer << (unsigned short) 50000;
	writer << -123456;
	writer << (unsigned) 123456;
#ifndef POCO_LONG_IS_64_BIT
	writer << (long) -1234567890;
	writer << (unsigned long) 1234567890;
#endif // POCO_LONG_IS_64_BIT
	writer << (Int64) -1234567890;
	writer << (UInt64) 1234567890;

	writer << (float) 1.5;
	writer << (double) -1.5;

	writer << "foo";
	writer << "";

	writer << std::string("bar");
	writer << std::string();

	writer.write7BitEncoded((UInt32) 100);
	writer.write7BitEncoded((UInt32) 1000);
	writer.write7BitEncoded((UInt32) 10000);
	writer.write7BitEncoded((UInt32) 100000);
	writer.write7BitEncoded((UInt32) 1000000);

	writer.write7BitEncoded((UInt64) 100);
	writer.write7BitEncoded((UInt64) 1000);
	writer.write7BitEncoded((UInt64) 10000);
	writer.write7BitEncoded((UInt64) 100000);
	writer.write7BitEncoded((UInt64) 1000000);

	writer.writeRaw("RAW");
}


void SQLTest::readFromCLOB(BinaryReader& reader)
{
	bool b = false;
	reader >> b;
	assertTrue (b);
	reader >> b;
	assertTrue (!b);

	char c = ' ';
	reader >> c;
	assertTrue (c == 'a');

	short shortv = 0;
	reader >> shortv;
	assertTrue (shortv == -100);

	unsigned short ushortv = 0;
	reader >> ushortv;
	assertTrue (ushortv == 50000);

	int intv = 0;
	reader >> intv;
	assertTrue (intv == -123456);

	unsigned uintv = 0;
	reader >> uintv;
	assertTrue (uintv == 123456);
#ifndef POCO_LONG_IS_64_BIT
	long longv = 0;
	reader >> longv;
	assertTrue (longv == -1234567890);

	unsigned long ulongv = 0;
	reader >> ulongv;
	assertTrue (ulongv == 1234567890);
#endif // POCO_LONG_IS_64_BIT
	Int64 int64v = 0;
	reader >> int64v;
	assertTrue (int64v == -1234567890);

	UInt64 uint64v = 0;
	reader >> uint64v;
	assertTrue (uint64v == 1234567890);

	float floatv = 0.0;
	reader >> floatv;
	assertTrue (floatv == 1.5);

	double doublev = 0.0;
	reader >> doublev;
	assertTrue (doublev == -1.5);

	std::string str;
	reader >> str;
	assertTrue (str == "foo");
	reader >> str;
	assertTrue (str == "");

	reader >> str;
	assertTrue (str == "bar");
	reader >> str;
	assertTrue (str == "");

	UInt32 uint32v;
	reader.read7BitEncoded(uint32v);
	assertTrue (uint32v == 100);
	reader.read7BitEncoded(uint32v);
	assertTrue (uint32v == 1000);
	reader.read7BitEncoded(uint32v);
	assertTrue (uint32v == 10000);
	reader.read7BitEncoded(uint32v);
	assertTrue (uint32v == 100000);
	reader.read7BitEncoded(uint32v);
	assertTrue (uint32v == 1000000);

	reader.read7BitEncoded(uint64v);
	assertTrue (uint64v == 100);
	reader.read7BitEncoded(uint64v);
	assertTrue (uint64v == 1000);
	reader.read7BitEncoded(uint64v);
	assertTrue (uint64v == 10000);
	reader.read7BitEncoded(uint64v);
	assertTrue (uint64v == 100000);
	reader.read7BitEncoded(uint64v);
	assertTrue (uint64v == 1000000);

	reader.readRaw(3, str);
	assertTrue (str == "RAW");
}


void SQLTest::testColumnVector()
{
	MetaColumn mc(0, "mc", MetaColumn::FDT_DOUBLE, 2, 3, true);

	assertTrue (mc.name() == "mc");
	assertTrue (mc.position() == 0);
	assertTrue (mc.length() == 2);
	assertTrue (mc.precision() == 3);
	assertTrue (mc.type() == MetaColumn::FDT_DOUBLE);
	assertTrue (mc.isNullable());

	std::vector<int>* pData = new std::vector<int>;
	pData->push_back(1);
	pData->push_back(2);
	pData->push_back(3);
	pData->push_back(4);
	pData->push_back(5);
	
	Column<std::vector<int> > c(mc, pData);

	assertTrue (c.rowCount() == 5);
	assertTrue (c[0] == 1);
	assertTrue (c[1] == 2);
	assertTrue (c[2] == 3);
	assertTrue (c[3] == 4);
	assertTrue (c[4] == 5);
	assertTrue (c.name() == "mc");
	assertTrue (c.position() == 0);
	assertTrue (c.length() == 2);
	assertTrue (c.precision() == 3);
	assertTrue (c.type() == MetaColumn::FDT_DOUBLE);

	try
	{
		POCO_UNUSED int i; i = c[100]; // to silence gcc
		fail ("must fail");
	}
	catch (RangeException&) { }

	Column<std::vector<int> > c1 = c;

	assertTrue (c1.rowCount() == 5);
	assertTrue (c1[0] == 1);
	assertTrue (c1[1] == 2);
	assertTrue (c1[2] == 3);
	assertTrue (c1[3] == 4);
	assertTrue (c1[4] == 5);

	Column<std::vector<int> > c2(c1);

	assertTrue (c2.rowCount() == 5);
	assertTrue (c2[0] == 1);
	assertTrue (c2[1] == 2);
	assertTrue (c2[2] == 3);
	assertTrue (c2[3] == 4);
	assertTrue (c2[4] == 5);

	std::vector<int> vi;
	vi.assign(c.begin(), c.end());
	assertTrue (vi.size() == 5);
	assertTrue (vi[0] == 1);
	assertTrue (vi[1] == 2);
	assertTrue (vi[2] == 3);
	assertTrue (vi[3] == 4);
	assertTrue (vi[4] == 5);

	c.reset();
	assertTrue (c.rowCount() == 0);
	assertTrue (c1.rowCount() == 0);
	assertTrue (c2.rowCount() == 0);

	std::vector<int>* pV1 = new std::vector<int>;
	pV1->push_back(1);
	pV1->push_back(2);
	pV1->push_back(3);
	pV1->push_back(4);
	pV1->push_back(5);
	std::vector<int>* pV2 = new std::vector<int>;
	pV2->push_back(5);
	pV2->push_back(4);
	pV2->push_back(3);
	pV2->push_back(2);
	pV2->push_back(1);
	Column<std::vector<int> > c3(mc, pV1);
	Column<std::vector<int> > c4(mc, pV2);
	
	Poco::SQL::swap(c3, c4);
	assertTrue (c3[0] == 5);
	assertTrue (c3[1] == 4);
	assertTrue (c3[2] == 3);
	assertTrue (c3[3] == 2);
	assertTrue (c3[4] == 1);

	assertTrue (c4[0] == 1);
	assertTrue (c4[1] == 2);
	assertTrue (c4[2] == 3);
	assertTrue (c4[3] == 4);
	assertTrue (c4[4] == 5);

	std::swap(c3, c4);
	assertTrue (c3[0] == 1);
	assertTrue (c3[1] == 2);
	assertTrue (c3[2] == 3);
	assertTrue (c3[3] == 4);
	assertTrue (c3[4] == 5);

	assertTrue (c4[0] == 5);
	assertTrue (c4[1] == 4);
	assertTrue (c4[2] == 3);
	assertTrue (c4[3] == 2);
	assertTrue (c4[4] == 1);
}


void SQLTest::testColumnVectorBool()
{
	MetaColumn mc(0, "mc", MetaColumn::FDT_BOOL);

	std::vector<bool>* pData = new std::vector<bool>;
	pData->push_back(true);
	pData->push_back(false);
	pData->push_back(true);
	pData->push_back(false);
	pData->push_back(true);
	
	Column<std::vector<bool> > c(mc, pData);

	assertTrue (c.rowCount() == 5);
	assertTrue (c[0] == true);
	assertTrue (c[1] == false);
	assertTrue (c[2] == true);
	assertTrue (c[3] == false);
	assertTrue (c[4] == true);
	assertTrue (c.type() == MetaColumn::FDT_BOOL);

	try
	{
		POCO_UNUSED bool b; b = c[100]; // to silence gcc
		fail ("must fail");
	}
	catch (RangeException&) { }

	Column<std::vector<bool> > c1 = c;

	assertTrue (c1.rowCount() == 5);
	assertTrue (c1[0] == true);
	assertTrue (c1[1] == false);
	assertTrue (c1[2] == true);
	assertTrue (c1[3] == false);
	assertTrue (c1[4] == true);

	Column<std::vector<bool> > c2(c1);

	assertTrue (c2.rowCount() == 5);
	assertTrue (c2[0] == true);
	assertTrue (c2[1] == false);
	assertTrue (c2[2] == true);
	assertTrue (c2[3] == false);
	assertTrue (c2[4] == true);

	std::vector<bool> vi;
	vi.assign(c.begin(), c.end());
	assertTrue (vi.size() == 5);
	assertTrue (vi[0] == true);
	assertTrue (vi[1] == false);
	assertTrue (vi[2] == true);
	assertTrue (vi[3] == false);
	assertTrue (vi[4] == true);

	c.reset();
	assertTrue (c.rowCount() == 0);
	assertTrue (c1.rowCount() == 0);
	assertTrue (c2.rowCount() == 0);
}


void SQLTest::testColumnDeque()
{
	typedef std::deque<int> ContainerType;
	typedef Column<ContainerType> ColumnType;

	MetaColumn mc(0, "mc", MetaColumn::FDT_DOUBLE, 2, 3, true);

	assertTrue (mc.name() == "mc");
	assertTrue (mc.position() == 0);
	assertTrue (mc.length() == 2);
	assertTrue (mc.precision() == 3);
	assertTrue (mc.type() == MetaColumn::FDT_DOUBLE);
	assertTrue (mc.isNullable());

	ContainerType* pData = new ContainerType;
	pData->push_back(1);
	pData->push_back(2);
	pData->push_back(3);
	pData->push_back(4);
	pData->push_back(5);
	
	ColumnType c(mc, pData);

	assertTrue (c.rowCount() == 5);
	assertTrue (c[0] == 1);
	assertTrue (c[1] == 2);
	assertTrue (c[2] == 3);
	assertTrue (c[3] == 4);
	assertTrue (c[4] == 5);
	assertTrue (c.name() == "mc");
	assertTrue (c.position() == 0);
	assertTrue (c.length() == 2);
	assertTrue (c.precision() == 3);
	assertTrue (c.type() == MetaColumn::FDT_DOUBLE);

	try
	{
		POCO_UNUSED int i; i = c[100]; // to silence gcc
		fail ("must fail");
	}
	catch (RangeException&) { }

	ColumnType c1 = c;

	assertTrue (c1.rowCount() == 5);
	assertTrue (c1[0] == 1);
	assertTrue (c1[1] == 2);
	assertTrue (c1[2] == 3);
	assertTrue (c1[3] == 4);
	assertTrue (c1[4] == 5);

	ColumnType c2(c1);

	assertTrue (c2.rowCount() == 5);
	assertTrue (c2[0] == 1);
	assertTrue (c2[1] == 2);
	assertTrue (c2[2] == 3);
	assertTrue (c2[3] == 4);
	assertTrue (c2[4] == 5);

	ContainerType vi;
	vi.assign(c.begin(), c.end());
	assertTrue (vi.size() == 5);
	assertTrue (vi[0] == 1);
	assertTrue (vi[1] == 2);
	assertTrue (vi[2] == 3);
	assertTrue (vi[3] == 4);
	assertTrue (vi[4] == 5);

	c.reset();
	assertTrue (c.rowCount() == 0);
	assertTrue (c1.rowCount() == 0);
	assertTrue (c2.rowCount() == 0);

	ContainerType* pV1 = new ContainerType;
	pV1->push_back(1);
	pV1->push_back(2);
	pV1->push_back(3);
	pV1->push_back(4);
	pV1->push_back(5);
	ContainerType* pV2 = new ContainerType;
	pV2->push_back(5);
	pV2->push_back(4);
	pV2->push_back(3);
	pV2->push_back(2);
	pV2->push_back(1);
	Column<ContainerType> c3(mc, pV1);
	Column<ContainerType> c4(mc, pV2);
	
	Poco::SQL::swap(c3, c4);
	assertTrue (c3[0] == 5);
	assertTrue (c3[1] == 4);
	assertTrue (c3[2] == 3);
	assertTrue (c3[3] == 2);
	assertTrue (c3[4] == 1);

	assertTrue (c4[0] == 1);
	assertTrue (c4[1] == 2);
	assertTrue (c4[2] == 3);
	assertTrue (c4[3] == 4);
	assertTrue (c4[4] == 5);

	std::swap(c3, c4);
	assertTrue (c3[0] == 1);
	assertTrue (c3[1] == 2);
	assertTrue (c3[2] == 3);
	assertTrue (c3[3] == 4);
	assertTrue (c3[4] == 5);

	assertTrue (c4[0] == 5);
	assertTrue (c4[1] == 4);
	assertTrue (c4[2] == 3);
	assertTrue (c4[3] == 2);
	assertTrue (c4[4] == 1);
}


void SQLTest::testColumnList()
{
	typedef std::list<int> ContainerType;
	typedef Column<ContainerType> ColumnType;

	MetaColumn mc(0, "mc", MetaColumn::FDT_DOUBLE, 2, 3, true);

	assertTrue (mc.name() == "mc");
	assertTrue (mc.position() == 0);
	assertTrue (mc.length() == 2);
	assertTrue (mc.precision() == 3);
	assertTrue (mc.type() == MetaColumn::FDT_DOUBLE);
	assertTrue (mc.isNullable());

	ContainerType* pData = new ContainerType;
	pData->push_back(1);
	pData->push_back(2);
	pData->push_back(3);
	pData->push_back(4);
	pData->push_back(5);
	
	ColumnType c(mc, pData);

	assertTrue (c.rowCount() == 5);
	assertTrue (c[0] == 1);
	assertTrue (c[1] == 2);
	assertTrue (c[2] == 3);
	assertTrue (c[3] == 4);
	assertTrue (c[4] == 5);
	assertTrue (c.name() == "mc");
	assertTrue (c.position() == 0);
	assertTrue (c.length() == 2);
	assertTrue (c.precision() == 3);
	assertTrue (c.type() == MetaColumn::FDT_DOUBLE);

	try
	{
		POCO_UNUSED int i; i = c[100]; // to silence gcc
		fail ("must fail");
	}
	catch (RangeException&) { }

	ColumnType c1 = c;

	assertTrue (c1.rowCount() == 5);
	assertTrue (c1[0] == 1);
	assertTrue (c1[1] == 2);
	assertTrue (c1[2] == 3);
	assertTrue (c1[3] == 4);
	assertTrue (c1[4] == 5);

	ColumnType c2(c1);
	assertTrue (c2.rowCount() == 5);
	assertTrue (c2[0] == 1);
	assertTrue (c2[1] == 2);
	assertTrue (c2[2] == 3);
	assertTrue (c2[3] == 4);
	assertTrue (c2[4] == 5);

	ContainerType vi;
	vi.assign(c.begin(), c.end());
	assertTrue (vi.size() == 5);
	ContainerType::const_iterator it = vi.begin();
	ContainerType::const_iterator end = vi.end();
	for (int i = 1; it != end; ++it, ++i)
		assertTrue (*it == i);

	c.reset();
	assertTrue (c.rowCount() == 0);
	assertTrue (c1.rowCount() == 0);
	assertTrue (c2.rowCount() == 0);

	ContainerType* pV1 = new ContainerType;
	pV1->push_back(1);
	pV1->push_back(2);
	pV1->push_back(3);
	pV1->push_back(4);
	pV1->push_back(5);
	ContainerType* pV2 = new ContainerType;
	pV2->push_back(5);
	pV2->push_back(4);
	pV2->push_back(3);
	pV2->push_back(2);
	pV2->push_back(1);
	Column<ContainerType> c3(mc, pV1);
	Column<ContainerType> c4(mc, pV2);
	
	Poco::SQL::swap(c3, c4);
	assertTrue (c3[0] == 5);
	assertTrue (c3[1] == 4);
	assertTrue (c3[2] == 3);
	assertTrue (c3[3] == 2);
	assertTrue (c3[4] == 1);

	assertTrue (c4[0] == 1);
	assertTrue (c4[1] == 2);
	assertTrue (c4[2] == 3);
	assertTrue (c4[3] == 4);
	assertTrue (c4[4] == 5);

	std::swap(c3, c4);
	assertTrue (c3[0] == 1);
	assertTrue (c3[1] == 2);
	assertTrue (c3[2] == 3);
	assertTrue (c3[3] == 4);
	assertTrue (c3[4] == 5);

	assertTrue (c4[0] == 5);
	assertTrue (c4[1] == 4);
	assertTrue (c4[2] == 3);
	assertTrue (c4[3] == 2);
	assertTrue (c4[4] == 1);
}


void SQLTest::testRow()
{
	Row row;
	row.append("field0", 0);
	row.append("field1", 1);
	row.append("field2", 2);
	row.append("field3", 3);
	row.append("field4", 4);

	assertTrue (row["field0"] == 0);
	assertTrue (row["field1"] == 1);
	assertTrue (row["field2"] == 2);
	assertTrue (row["field3"] == 3);
	assertTrue (row["field4"] == 4);

	assertTrue (row["FIELD0"] == 0);
	assertTrue (row["FIELD1"] == 1);
	assertTrue (row["FIELD2"] == 2);
	assertTrue (row["FIELD3"] == 3);
	assertTrue (row["FIELD4"] == 4);

	assertTrue (row[0] == 0);
	assertTrue (row[1] == 1);
	assertTrue (row[2] == 2);
	assertTrue (row[3] == 3);
	assertTrue (row[4] == 4);

	const Row& cr = row;
	assertTrue (cr["field0"] == 0);
	assertTrue (cr[0] == 0);
	assertTrue (cr.get(0) == 0);

	try
	{
		POCO_UNUSED int i; i = row[5].convert<int>(); // to silence gcc
		fail ("must fail");
	}catch (RangeException&) {}

	try
	{
		POCO_UNUSED int i; i = row["a bad name"].convert<int>(); // to silence gcc
		fail ("must fail");
	}catch (NotFoundException&) {}

	assertTrue (5 == row.fieldCount());
	assertTrue (row[0] == 0);
	assertTrue (row["field0"] == 0);
	assertTrue (row[1] == 1);
	assertTrue (row["field1"] == 1);
	assertTrue (row[2] == 2);
	assertTrue (row["field2"] == 2);
	assertTrue (row[3] == 3);
	assertTrue (row["field3"] == 3);
	assertTrue (row[4] == 4);
	assertTrue (row["field4"] == 4);

	Row row2;

	row2.append("field0", 5);
	row2.append("field1", 4);
	row2.append("field2", 3);
	row2.append("field3", 2);
	row2.append("field4", 1);

	assertTrue (row != row2);

	Row row3;

	row3.append("field0", 0);
	row3.append("field1", 1);
	row3.append("field2", 2);
	row3.append("field3", 3);
	row3.append("field4", 4);

	assertTrue (row3 == row);
	assertTrue (!(row < row3 || row3 < row));

	Row row4(row3.names());
	try
	{
		row4.set("badfieldname", 0);
		fail ("must fail");
	}catch (NotFoundException&) {}

	row4.set("field0", 0);
	row4.set("field1", 1);
	row4.set("field2", 2);
	row4.set("field3", 3);
	row4.set("field4", 4);
	assertTrue (row3 == row4);
	try
	{
		row4.set(5, 0);
		fail ("must fail");
	}catch (RangeException&) {}
	row4.set("field0", 1);
	assertTrue (row3 != row4);
	assertTrue (row3 < row4);
}


void SQLTest::testRowSort()
{
	Row row1;
	row1.append("0", 0);
	row1.append("1", 1);
	row1.append("2", 2);
	row1.append("3", 3);
	row1.append("4", 4);

	Row row2;
	row2.append("0", 0);
	row2.append("1", 1);
	row2.append("2", 2);
	row2.append("3", 3);
	row2.append("4", 4);

	std::multiset<Row> rowSet1;
	rowSet1.insert(row1);
	rowSet1.insert(row2);
	std::multiset<Row>::iterator it1 = rowSet1.begin();
	assertTrue (row1 == *it1);
	++it1;
	assertTrue (row2 == *it1);

	Row row3;
	row3.append("0", 1);
	row3.append("1", 1);
	row3.append("2", 2);
	row3.append("3", 3);
	row3.append("4", 4);

	Row row4;
	row4.append("0", 0);
	row4.append("1", 1);
	row4.append("2", 2);
	row4.append("3", 3);
	row4.append("4", 4);

	std::set<Row> rowSet2;
	rowSet2.insert(row4);
	rowSet2.insert(row3);
	std::set<Row>::iterator it2 = rowSet2.begin();
	assertTrue (row4 == *it2);
	++it2;
	assertTrue (row3 == *it2);

	Row row5;
	row5.append("0", 2);
	row5.append("1", 2);
	row5.append("2", 0);
	row5.append("3", 3);
	row5.append("4", 4);
	row5.addSortField("1");

	Row row6;
	row6.append("0", 1);
	row6.append("1", 0);
	row6.append("2", 1);
	row6.append("3", 3);
	row6.append("4", 4);
	row6.addSortField("1");

	Row row7;
	row7.append("0", 0);
	row7.append("1", 1);
	row7.append("2", 2);
	row7.append("3", 3);
	row7.append("4", 4);

	std::set<Row> rowSet3;
	rowSet3.insert(row5);
	rowSet3.insert(row6);
	try
	{
		rowSet3.insert(row7);//has no same sort criteria
		fail ("must fail");
	} catch (InvalidAccessException&) {}

	row7.addSortField("1");
	testRowStrictWeak(row7, row6, row5);
	rowSet3.insert(row7);

	std::set<Row>::iterator it3 = rowSet3.begin();
	assertTrue (row7 == *it3);
	++it3;
	assertTrue (row6 == *it3);
	++it3;
	assertTrue (row5 == *it3);

	row5.replaceSortField("0", "2");
	row6.replaceSortField("0", "2");
	row7.replaceSortField("0", "2");

	rowSet3.clear();
	rowSet3.insert(row7);
	rowSet3.insert(row6);
	rowSet3.insert(row5);

	it3 = rowSet3.begin();
	assertTrue (row5 == *it3);
	++it3;
	assertTrue (row6 == *it3);
	++it3;
	assertTrue (row7 == *it3);

	row5.resetSort();
	row6.resetSort();
	row7.resetSort();

	rowSet3.clear();
	rowSet3.insert(row5);
	rowSet3.insert(row6);
	rowSet3.insert(row7);

	it3 = rowSet3.begin();
	assertTrue (row7 == *it3);
	++it3;
	assertTrue (row6 == *it3);
	++it3;
	assertTrue (row5 == *it3);

	Row row8;
	row8.append("0", "2");
	row8.append("1", "2");
	row8.append("2", "0");
	row8.append("3", "3");
	row8.append("4", "4");
	row8.addSortField("1");

	Row row9;
	row9.append("0", "1");
	row9.append("1", "0");
	row9.append("2", "1");
	row9.append("3", "3");
	row9.append("4", "4");
	row9.addSortField("1");

	Row row10;
	row10.append("0", "0");
	row10.append("1", "1");
	row10.append("2", "2");
	row10.append("3", "3");
	row10.append("4", "4");
	row10.addSortField("1");

	testRowStrictWeak(row10, row9, row8);


	Row row11;
	row11.append("0", 2.5);
	row11.append("1", 2.5);
	row11.append("2", 0.5);
	row11.append("3", 3.5);
	row11.append("4", 4.5);
	row11.addSortField("1");

	Row row12;
	row12.append("0", 1.5);
	row12.append("1", 0.5);
	row12.append("2", 1.5);
	row12.append("3", 3.5);
	row12.append("4", 4.5);
	row12.addSortField("1");

	Row row13;
	row13.append("0", 0.5);
	row13.append("1", 1.5);
	row13.append("2", 2.5);
	row13.append("3", 3.5);
	row13.append("4", 4.5);
	row13.addSortField("1");

	testRowStrictWeak(row13, row12, row11);
}


void SQLTest::testRowStrictWeak(const Row& row1, const Row& row2, const Row& row3)
{
	assertTrue (row1 < row2 && !(row2 < row1)); // antisymmetric
	assertTrue (row1 < row2 && row2 < row3 && row1 < row3); // transitive
	assertTrue (!(row1 < row1)); // irreflexive
}


void SQLTest::testSimpleRowFormatter()
{
	Row row1;
	row1.append("field0", 0);
	row1.append("field1", 1);
	row1.append("field2", 2);
	row1.append("field3", 3);
	row1.append("field4", 4);

	SimpleRowFormatter rf;
	std::streamsize sz = rf.getColumnWidth();
	std::streamsize sp = rf.getSpacing();

	std::string line(std::string::size_type(sz * 5 + sp * 4), '-');
	std::string spacer(static_cast<std::size_t>(sp), ' ');
	std::ostringstream os;
	os << std::left
		<< std::setw(sz) << "field0"
		<< spacer
		<< std::setw(sz) << "field1"
		<< spacer
		<< std::setw(sz) << "field2"
		<< spacer
		<< std::setw(sz) << "field3"
		<< spacer
		<< std::setw(sz) << "field4" << std::endl
		<< line << std::endl;
	assertTrue (row1.namesToString() == os.str());

	os.str("");
	os << std::right
		<< std::setw(sz) << "0"
		<< spacer
		<< std::setw(sz) << "1"
		<< spacer
		<< std::setw(sz) << "2"
		<< spacer
		<< std::setw(sz) << "3"
		<< spacer
		<< std::setw(sz) << "4" << std::endl;
	assertTrue (row1.valuesToString() == os.str());
}


void SQLTest::testJSONRowFormatter()
{
	Row row1;
	row1.append("field0", 0);
	row1.append("field1", "1");
	row1.append("field2", DateTime(2007, 3, 13, 8, 12, 15));
	row1.append("field3", Var());
	row1.append("field4", 4);
	row1.setFormatter(new JSONRowFormatter);
	
	assertTrue (row1.getFormatter().prefix() == "{");
	assertTrue (row1.getFormatter().postfix() == "]}");
	assertTrue (row1.getFormatter().getMode() == RowFormatter::FORMAT_PROGRESSIVE);
	assertTrue (row1.namesToString() == "\"names\":[\"field0\",\"field1\",\"field2\",\"field3\",\"field4\"]");
	assertTrue (row1.valuesToString() == ",\"values\":[[0,\"1\",\"2007-03-13T08:12:15Z\",null,4]");

	row1.setFormatter(new JSONRowFormatter(JSONRowFormatter::JSON_FMT_MODE_SMALL));
	assertTrue (row1.getFormatter().getMode() == RowFormatter::FORMAT_PROGRESSIVE);
	assertTrue (row1.namesToString() == "");
	assertTrue (row1.valuesToString() == "[[0,\"1\",\"2007-03-13T08:12:15Z\",null,4]");
	assertTrue (row1.valuesToString() == ",[0,\"1\",\"2007-03-13T08:12:15Z\",null,4]");

	row1.setFormatter(new JSONRowFormatter(JSONRowFormatter::JSON_FMT_MODE_FULL));
	assertTrue (row1.getFormatter().prefix() == "{\"count\":0,[");
	assertTrue (row1.getFormatter().postfix() == "]}");
	assertTrue (row1.getFormatter().getMode() == RowFormatter::FORMAT_PROGRESSIVE);
	assertTrue (row1.namesToString() == "");
	assertTrue (row1.valuesToString() == "{\"field0\":0,\"field1\":\"1\",\"field2\":\"2007-03-13T08:12:15Z\",\"field3\":null,\"field4\":4}");
	assertTrue (row1.valuesToString() == ",{\"field0\":0,\"field1\":\"1\",\"field2\":\"2007-03-13T08:12:15Z\",\"field3\":null,\"field4\":4}");
}


void SQLTest::testDateAndTime()
{
	DateTime dt;
	Date d(dt);
	Time t(dt);

	assertTrue (dt.year() == d.year());
	assertTrue (dt.month() == d.month());
	assertTrue (dt.day() == d.day());

	assertTrue (dt.hour() == t.hour());
	assertTrue (dt.minute() == t.minute());
	assertTrue (dt.second() == t.second());
	
	Date d1(2007, 6, 15);
	d1.assign(d.year() - 1, d.month(), (d.month() == 2 && d.day() == 29) ? 28 : d.day());
	assertTrue (d1 < d); assertTrue (d1 != d);

	d1.assign(d.year() - 1, 12, d.day());
	assertTrue (d1 < d); assertTrue (d1 != d);

	if (d.day() > 1)
	{
		d1.assign(d.year(), d.month(), d.day() - 1);
		assertTrue (d1 < d); assertTrue (d1 != d);
	}

	d1.assign(d.year() + 1, d.month(), (d.month() == 2 && d.day() == 29) ? 28 : d.day());
	assertTrue (d1 > d); assertTrue (d1 != d);
	
	d1.assign(d.year() + 1, 1, d.day());
	assertTrue (d1 > d); assertTrue (d1 != d);

	if (d.day() < dt.daysOfMonth(dt.year(), dt.month()))
	{
		d1.assign(d.year(), d.month(), d.day() + 1);
		assertTrue (d1 > d); assertTrue (d1 != d);
	}
	
	d1.assign(d.year(), d.month(), d.day());
	assertTrue (d1 == d);

	try { d1.assign(-1, 1, 1); fail ("must fail"); }
	catch (InvalidArgumentException&) { }
	try { d1.assign(1, 0, 1); fail ("must fail"); }
	catch (InvalidArgumentException&) { }
	try { d1.assign(1, 1, 0); fail ("must fail"); }
	catch (InvalidArgumentException&) { }

	Time t1(12, 30, 15);
	
	if (t.hour() > 1)
	{
		t1.assign(t.hour() - 1, t.minute(), t.second());
		assertTrue (t1 < t); assertTrue (t1 != t);
	}

	if (t.minute() > 1)
	{
		t1.assign(t.hour(), t.minute() - 1, t.second());
		assertTrue (t1 < t); assertTrue (t1 != t);
	}
	
	if (t.second() > 1)
	{
		t1.assign(t.hour(), t.minute(), t.second() - 1);
		assertTrue (t1 < t); assertTrue (t1 != t);
	}

	if (t.hour() < 23)
	{
		t1.assign(t.hour() + 1, t.minute(), t.second());
		assertTrue (t1 > t); assertTrue (t1 != t);
	}

	if (t.minute() < 59)
	{
		t1.assign(t.hour(), t.minute() + 1, t.second());
		assertTrue (t1 > t); assertTrue (t1 != t);
	}

	if (t.second() < 59)
	{
		t1.assign(t.hour(), t.minute(), t.second() + 1);
		assertTrue (t1 > t); assertTrue (t1 != t);
	}

	t1.assign(t.hour(), t.minute(), t.second());
	assertTrue (t1 == t);

	try { t1.assign(-1, 0, 0); fail ("must fail"); }
	catch (InvalidArgumentException&) { }
	try { t1.assign(0, -1, 0); fail ("must fail"); }
	catch (InvalidArgumentException&) { }
	try { t1.assign(0, 0, -1); fail ("must fail"); }
	catch (InvalidArgumentException&) { }

	d1 = dt;
	assertTrue (d1 == dt);

	t1 = dt;
	assertTrue (t1 == dt);

	d.assign(2007, 6, 15);
	d1.assign(2007, 6, 16);
	assertTrue (d != d1);
	Var vDate = d;
	d1 = vDate;
	assertTrue (d == d1);

	t.assign(12, 30, 15);
	t1.assign(12, 30, 16);
	assertTrue (t != t1);
	Var vTime = t;
	t1 = vTime;
	assertTrue (t == t1);
}


void SQLTest::testExternalBindingAndExtraction()
{
	Session tmp (Poco::SQL::Test::Connector::KEY, "dummy.db");

	int i;
	AbstractExtraction::Ptr pExt1 = into(i);
	AbstractExtraction::Ptr pExt2 = into(i);
	assertTrue (1 == pExt1.referenceCount());
	assertTrue (1 == pExt2.referenceCount());
	{
		Statement stmt(tmp);
		stmt.addExtract(pExt1);
		assertTrue (2 == pExt1.referenceCount());
	}
	assertTrue (1 == pExt1.referenceCount());
	assertTrue (1 == pExt2.referenceCount());

	AbstractBinding::Ptr pBind1 = use(i, "mybind1");
	AbstractBinding::Ptr pBind2 = use(i, "mybind2");
	AbstractBinding::Ptr pBind3 = use(i, "mybind3");
	assertTrue (1 == pBind1.referenceCount());
	assertTrue (1 == pBind2.referenceCount());
	assertTrue (1 == pBind3.referenceCount());
	{
		Statement stmt(tmp);
		stmt.addBind(pBind1);
		assertTrue (2 == pBind1.referenceCount());
		stmt.removeBind(pBind1->name());
		assertTrue (1 == pBind1.referenceCount());
		stmt.addBind(pBind2);
		assertTrue (2 == pBind2.referenceCount());
		stmt.addBind(pBind3);
		assertTrue (2 == pBind3.referenceCount());

		try { stmt.removeBind("a bad name"); fail("must fail"); }
		catch (NotFoundException&) { }
	}
	assertTrue (1 == pBind1.referenceCount());
	assertTrue (1 == pBind2.referenceCount());
	assertTrue (1 == pBind3.referenceCount());
}


void SQLTest::testStdTuple()
{
	using Row = std::tuple<std::string, std::string, int>;

	Session sess(SessionFactory::instance().create("test", "cs"));
	Row person = std::make_tuple(std::string("Scott"), std::string("Washington, DC"), 42);
	sess << "INSERT INTO Person(name, address, age) VALUES (?, ?, ?)", use(person), now;
	std::vector<Row> rows;
	sess << "SELECT name, address, age FROM Person", into(rows) , now;
}


void SQLTest::setUp()
{
}


void SQLTest::tearDown()
{
}


CppUnit::Test* SQLTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("SQLTest");

	CppUnit_addTest(pSuite, SQLTest, testSession);
	CppUnit_addTest(pSuite, SQLTest, testStatementFormatting);
	CppUnit_addTest(pSuite, SQLTest, testFeatures);
	CppUnit_addTest(pSuite, SQLTest, testProperties);
	CppUnit_addTest(pSuite, SQLTest, testLOB);
	CppUnit_addTest(pSuite, SQLTest, testCLOB);
	CppUnit_addTest(pSuite, SQLTest, testCLOBStreams);
	CppUnit_addTest(pSuite, SQLTest, testColumnVector);
	CppUnit_addTest(pSuite, SQLTest, testColumnVectorBool);
	CppUnit_addTest(pSuite, SQLTest, testColumnDeque);
	CppUnit_addTest(pSuite, SQLTest, testColumnList);
	CppUnit_addTest(pSuite, SQLTest, testRow);
	CppUnit_addTest(pSuite, SQLTest, testRowSort);
	CppUnit_addTest(pSuite, SQLTest, testSimpleRowFormatter);
	CppUnit_addTest(pSuite, SQLTest, testJSONRowFormatter);
	CppUnit_addTest(pSuite, SQLTest, testDateAndTime);
	CppUnit_addTest(pSuite, SQLTest, testExternalBindingAndExtraction);
	CppUnit_addTest(pSuite, SQLTest, testStdTuple);


	return pSuite;
}
