//
// DataTest.cpp
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "DataTest.h"
#include "Extractor.h"
#include "TestStatementImpl.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/Data/Session.h"
#include "Poco/Data/SessionFactory.h"
#include "Poco/Data/Transaction.h"
#include "Poco/Data/RecordSet.h"
#include "Poco/Data/RowFilter.h"
#include "Poco/Data/ArchiveStrategy.h"
#include "Poco/Data/LOB.h"
#include "Poco/Data/LOBStream.h"
#include "Poco/Data/MetaColumn.h"
#include "Poco/Data/Column.h"
#include "Poco/Data/Date.h"
#include "Poco/Data/Time.h"
#include "Poco/Data/SQLChannel.h"
#include "Poco/Data/SimpleRowFormatter.h"
#include "Poco/Data/JSONRowFormatter.h"
#include "Poco/Data/DataException.h"
#include "Connector.h"
#include "Poco/BinaryReader.h"
#include "Poco/BinaryWriter.h"
#include "Poco/DateTime.h"
#include "Poco/Stopwatch.h"
#include "Poco/Types.h"
#include "Poco/Dynamic/Var.h"
#include "Poco/Data/DynamicLOB.h"
#include "Poco/Data/DynamicDateTime.h"
#include "Poco/Latin1Encoding.h"
#include "Poco/Exception.h"
#include "Poco/DirectoryIterator.h"
#include "Poco/Glob.h"
#include "Poco/File.h"
#include "Poco/Path.h"
#include "Poco/TemporaryFile.h"
#include "Poco/FileStream.h"
#include "Poco/StreamCopier.h"
#include "Poco/Nullable.h"
#include "Poco/Logger.h"
#include "Poco/Mutex.h"
#include "Poco/SharedPtr.h"
#include "Poco/Thread.h"
#include <string>
#include <cstring>
#include <sstream>
#include <iomanip>
#include <set>
#include <type_traits>
#include <utility>


using namespace Poco;
using Poco::Dynamic::Var;
using namespace Poco::Data;
using namespace Poco::Data::Keywords;
using namespace std::string_literals;


DataTest::DataTest(const std::string& name): CppUnit::TestCase(name)
{
	Poco::Data::Test::Connector::addToFactory();
}


DataTest::~DataTest()
{
	Poco::Data::Test::Connector::removeFromFactory();
}


void DataTest::testSession()
{
	static_assert(std::is_nothrow_swappable_v<Poco::Data::Session>);

	Session sess(SessionFactory::instance().create("test", "cs"));
	assertTrue ("test" == sess.impl()->connectorName());
	assertTrue (sess.connector() == sess.impl()->connectorName());
	assertTrue ("cs" == sess.impl()->connectionString());
	assertTrue ("test:///cs" == sess.uri());
	assertTrue ("Test" == sess.dbmsName());

	assertTrue (sess.getLoginTimeout() == Session::LOGIN_TIMEOUT_DEFAULT);
	sess.setLoginTimeout(123);
	assertTrue (sess.getLoginTimeout() == 123);

	Session sess2(SessionFactory::instance().create("TeSt:///Cs"));
	assertTrue ("test" == sess2.impl()->connectorName());
	assertTrue ("Cs" == sess2.impl()->connectionString());
	assertTrue ("test:///Cs" == sess2.uri());
	static_assert(noexcept(sess.swap(sess2)));

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

	assertTrue(stmt.done());

	try
	{
		sess << "SELECT * FROM Strings", now;
		fail ("must fail");
	} catch (NotConnectedException&) { }

	sess.open();
	assertTrue (sess.getFeature("connected"));
	assertTrue (sess.isConnected());

	// ensure that throwing during execution leaves
	// statement in valid state (ST_DONE)
	sess.setFeature("throwOnHasNext", true);
	Statement stmt1 = (sess << "SELECT * FROM Strings", into(str), limit(50));
	assertTrue (sess.getFeature("throwOnHasNext"));
	try
	{
		stmt1.execute();
		fail ("must trow UnknownDataBaseException");
	}
	catch(const Poco::Data::UnknownDataBaseException&) {}
	assertTrue(stmt1.done());

	// reset session back to normal operation
	sess.setFeature("throwOnHasNext", false);
	sess << "SELECT * FROM Strings", now;
	stmt.execute();

	sess.reconnect();
	assertTrue (sess.getFeature("connected"));
	assertTrue (sess.isConnected());

	sess << "SELECT * FROM Strings", now;
	stmt.execute();
}


void DataTest::testStatementFormatting()
{
	Session sess(SessionFactory::instance().create("test", "cs"));

	Statement stmt = (sess << "SELECT %s%c%s,%d,%u,%f,%s FROM Person WHERE Name LIKE 'Simp%%'",
		"'",'a',"'",-1, 1u, 1.5, "42", now);

	assertTrue ("SELECT 'a',-1,1,1.500000,42 FROM Person WHERE Name LIKE 'Simp%'" == stmt.toString());
}


void DataTest::testStatementMoveAssignment()
{
	Session sess(SessionFactory::instance().create("test", "cs"));

	Statement source = (sess << "SELEC * FROM Person");
	source.parse();
	const std::string parseError = source.parseError();
#ifndef POCO_DATA_NO_SQL_PARSER
	assertTrue (!parseError.empty());
#endif

	Statement target(sess);
	target = std::move(source);
	assertTrue (target.toString() == "SELEC * FROM Person");
	assertTrue (target.parseError() == parseError);
	assertTrue (source.parseError().empty());
}


void DataTest::testStatementSwap()
{
	static_assert(std::is_nothrow_swappable_v<Statement>);

	Session sess(SessionFactory::instance().create("test", "cs"));

	Statement stmt1 = (sess << "SELECT 1");
	Statement stmt2 = (sess << "SELECT 2");

	Poco::Data::swap(stmt1, stmt2);
	assertEqual ("SELECT 2"s, stmt1.toString());
	assertEqual ("SELECT 1"s, stmt2.toString());

	using std::swap;
	swap(stmt1, stmt2);
	assertEqual ("SELECT 1"s, stmt1.toString());
	assertEqual ("SELECT 2"s, stmt2.toString());
}


void DataTest::testFeatures()
{
	Session sess(SessionFactory::instance().create("test", "cs"));

	// AbstractSession features
	assertTrue (sess.hasFeature("bulk"));
	assertTrue (!sess.getFeature("bulk"));
	sess.setFeature("bulk", true);
	assertTrue (sess.getFeature("bulk"));
	sess.setFeature("bulk", false);
	assertTrue (!sess.getFeature("bulk"));

	assertTrue (sess.hasFeature("emptyStringIsNull"));
	assertTrue (!sess.getFeature("emptyStringIsNull"));
	sess.setFeature("emptyStringIsNull", true);
	assertTrue (sess.getFeature("emptyStringIsNull"));
	sess.setFeature("emptyStringIsNull", false);
	assertTrue (!sess.getFeature("emptyStringIsNull"));

	assertTrue (sess.hasFeature("forceEmptyString"));
	assertTrue (!sess.getFeature("forceEmptyString"));
	sess.setFeature("forceEmptyString", true);
	assertTrue (sess.getFeature("forceEmptyString"));
	sess.setFeature("forceEmptyString", false);
	assertTrue (!sess.getFeature("forceEmptyString"));

	assertTrue (sess.hasFeature("sqlParse"));
	assertFalse (sess.getFeature("sqlParse"));
	sess.setFeature("sqlParse", true);
	assertTrue (sess.getFeature("sqlParse"));
	sess.setFeature("sqlParse", false);
	assertFalse (sess.getFeature("sqlParse"));

	assertTrue (sess.hasFeature("autoCommit"));
	assertTrue (sess.getFeature("autoCommit"));
	sess.setFeature("autoCommit", false);
	assertTrue (!sess.getFeature("autoCommit"));
	sess.setFeature("autoCommit", true);
	assertTrue (sess.getFeature("autoCommit"));

	// Session implementation features
	sess.setFeature("f1", true);
	assertTrue (sess.getFeature("f1"));
	assertTrue (sess.getFeature("f2"));

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


void DataTest::testProperties()
{
	Session sess(SessionFactory::instance().create("test", "cs"));

	// AbstractSession properties
	sess.setProperty("storage", "myStorage"s);
	Poco::Any s1 = sess.getProperty("storage");
	assertTrue (Poco::AnyCast<std::string>(s1) == "myStorage"s);

	sess.setProperty("handle", 1);
	Poco::Any h1 = sess.getProperty("handle");
	assertTrue (Poco::AnyCast<int>(h1) == 1);

	// Session implementation properties
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


void DataTest::testLOB()
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

	Poco::Data::LOB<int> lobNum1(&vec[0], vec.size());
	assertTrue (lobNum1.size() == vec.size());
	assertTrue (0 == std::memcmp(&vec[0], lobNum1.rawContent(), lobNum1.size() * sizeof(int)));
	assertTrue (*lobNum1.begin() == 0);
	Poco::Data::LOB<int>::Iterator it1 = lobNum1.end();
	assertTrue (*(--it1) == 9);

	Poco::Data::LOB<int> lobNum2(lobNum1);
	assertTrue (lobNum2.size() == lobNum1.size());
	assertTrue (lobNum2 == lobNum1);
	lobNum1.swap(lobNum2);
	assertTrue (lobNum2 == lobNum1);
}


void DataTest::testCLOB()
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

	Poco::Data::swap(blobNumStr, blobChrVec);
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


void DataTest::testCLOBStreams()
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


void DataTest::writeToCLOB(BinaryWriter& writer)
{
	writer << true;
	writer << false;
	writer << 'a';

	writer << (short) -100;
	writer << (unsigned short) 50000;
	writer << -123456;
	writer << (unsigned) 123456;
	writer << (long) -1234567890;
	writer << (unsigned long) 1234567890;

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


void DataTest::readFromCLOB(BinaryReader& reader)
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

	long longv = 0;
	reader >> longv;
	assertTrue (longv == -1234567890);

	unsigned long ulongv = 0;
	reader >> ulongv;
	assertTrue (ulongv == 1234567890);

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


void DataTest::testColumnVector()
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

	Column<std::vector<int>> c(mc, pData);

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
		(void) c[100];
		fail ("must fail");
	}
	catch (RangeException&) { }

	Column<std::vector<int>> c1 = c;

	assertTrue (c1.rowCount() == 5);
	assertTrue (c1[0] == 1);
	assertTrue (c1[1] == 2);
	assertTrue (c1[2] == 3);
	assertTrue (c1[3] == 4);
	assertTrue (c1[4] == 5);

	Column<std::vector<int>> c2(c1);

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
	Column<std::vector<int>> c3(mc, pV1);
	Column<std::vector<int>> c4(mc, pV2);

	Poco::Data::swap(c3, c4);
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


void DataTest::testColumnVectorBool()
{
	MetaColumn mc(0, "mc", MetaColumn::FDT_BOOL);

	std::vector<bool>* pData = new std::vector<bool>;
	pData->push_back(true);
	pData->push_back(false);
	pData->push_back(true);
	pData->push_back(false);
	pData->push_back(true);

	Column<std::vector<bool>> c(mc, pData);

	assertTrue (c.rowCount() == 5);
	assertTrue (c[0] == true);
	assertTrue (c[1] == false);
	assertTrue (c[2] == true);
	assertTrue (c[3] == false);
	assertTrue (c[4] == true);
	assertTrue (c.type() == MetaColumn::FDT_BOOL);

	try
	{
		(void) c[100];
		fail ("must fail");
	}
	catch (RangeException&) { }

	Column<std::vector<bool>> c1 = c;

	assertTrue (c1.rowCount() == 5);
	assertTrue (c1[0] == true);
	assertTrue (c1[1] == false);
	assertTrue (c1[2] == true);
	assertTrue (c1[3] == false);
	assertTrue (c1[4] == true);

	Column<std::vector<bool>> c2(c1);

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


void DataTest::testColumnDeque()
{
	using ContainerType = std::deque<int>;
	using ColumnType = Column<ContainerType>;

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
		(void) c[100];
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

	Poco::Data::swap(c3, c4);
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


void DataTest::testColumnList()
{
	using ContainerType = std::list<int>;
	using ColumnType = Column<ContainerType>;

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
		(void) c[100];
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

	Poco::Data::swap(c3, c4);
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


void DataTest::testRow()
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

	try
	{
		(void) row[5].convert<int>();
		fail ("must fail");
	}catch (RangeException&) {}

	try
	{
		(void) row["a bad name"].convert<int>();
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


void DataTest::testRowSort()
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


void DataTest::testRowStrictWeak(const Row& row1, const Row& row2, const Row& row3)
{
	assertTrue (row1 < row2 && !(row2 < row1)); // antisymmetric
	assertTrue (row1 < row2 && row2 < row3 && row1 < row3); // transitive
	assertTrue (!(row1 < row1)); // irreflexive
}


void DataTest::testSimpleRowFormatter()
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
	std::string spacer(sp, ' ');
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


void DataTest::testSimpleRowFormatterSwap()
{
	static_assert(std::is_nothrow_swappable_v<SimpleRowFormatter>);

	SimpleRowFormatter rf1(10, 2);
	SimpleRowFormatter rf2(20, 4);

	Poco::Data::swap(rf1, rf2);
	assertEqual (20, rf1.getColumnWidth());
	assertEqual (4, rf1.getSpacing());
	assertEqual (10, rf2.getColumnWidth());
	assertEqual (2, rf2.getSpacing());

	using std::swap;
	swap(rf1, rf2);
	assertEqual (10, rf1.getColumnWidth());
	assertEqual (2, rf1.getSpacing());
	assertEqual (20, rf2.getColumnWidth());
	assertEqual (4, rf2.getSpacing());
}


void DataTest::testJSONRowFormatter()
{
	Row row1;
	row1.append("field0", 0);
	row1.append("field1", "1");
	row1.append("field2", DateTime(2007, 3, 13, 8, 12, 15));
	row1.append("field3", Var());
	row1.append("field4", 4);
	row1.setFormatter(new JSONRowFormatter);

	assertTrue(row1.getFormatter().prefix() == "{");
	assertTrue(row1.getFormatter().postfix() == "]}");
	assertTrue(row1.getFormatter().getMode() == RowFormatter::FORMAT_PROGRESSIVE);
	assertTrue(row1.namesToString() == "\"names\":[\"field0\",\"field1\",\"field2\",\"field3\",\"field4\"]");
	assertTrue(row1.valuesToString() == ",\"values\":[[0,\"1\",\"2007-03-13T08:12:15Z\",null,4]");

	row1.setFormatter(new JSONRowFormatter(JSONRowFormatter::JSON_FMT_MODE_SMALL));
	assertTrue(row1.getFormatter().getMode() == RowFormatter::FORMAT_PROGRESSIVE);
	assertTrue(row1.namesToString() == "");
	assertTrue(row1.valuesToString() == "[[0,\"1\",\"2007-03-13T08:12:15Z\",null,4]");
	assertTrue(row1.valuesToString() == ",[0,\"1\",\"2007-03-13T08:12:15Z\",null,4]");

	row1.setFormatter(new JSONRowFormatter(JSONRowFormatter::JSON_FMT_MODE_FULL));
	assertTrue(row1.getFormatter().prefix() == "{\"count\":0,[");
	assertTrue(row1.getFormatter().postfix() == "]}");
	assertTrue(row1.getFormatter().getMode() == RowFormatter::FORMAT_PROGRESSIVE);
	assertTrue(row1.namesToString() == "");
	assertTrue(row1.valuesToString() == "{\"field0\":0,\"field1\":\"1\",\"field2\":\"2007-03-13T08:12:15Z\",\"field3\":null,\"field4\":4}");
	assertTrue(row1.valuesToString() == ",{\"field0\":0,\"field1\":\"1\",\"field2\":\"2007-03-13T08:12:15Z\",\"field3\":null,\"field4\":4}");

	Row row2;
	row2.append("date", Date(2007, 3, 13));
	row2.append("time", Time(8, 12, 15));
	row2.setFormatter(new JSONRowFormatter(JSONRowFormatter::JSON_FMT_MODE_FULL));
	(void) row2.namesToString();
	assertTrue(row2.valuesToString() == "{\"date\":\"2007/03/13\",\"time\":\"08:12:15\"}");
}


void DataTest::testDateAndTime()
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

	assertTrue (vDate.isDate());
	assertFalse (vDate.isTime());
	assertTrue (vTime.isTime());
	assertFalse (vTime.isDate());
}


void DataTest::testExternalBindingAndExtraction()
{
	Session tmp (Poco::Data::Test::Connector::KEY, "dummy.db");

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


void DataTest::testTranscode()
{
	Latin1Encoding::Ptr pL2E = new Latin1Encoding();

	const unsigned char latin1Chars[] = { 'g', 252, 'n', 't', 'e', 'r', 0 };
	const unsigned char utf8Chars[] = { 'g', 195, 188, 'n', 't', 'e', 'r', 0 };
	std::string latin1Text((const char*)latin1Chars);
	std::string utf8Text((const char*)utf8Chars);

	Poco::Data::Test::Extractor ext;
	ext.setString(latin1Text);
	std::string latin1Out;
	assertTrue (ext.extract(0, latin1Out));
	assertTrue(latin1Out == latin1Text);

	Poco::Data::Test::Extractor ext2(new Latin1Encoding());
	ext2.setString(latin1Text);
	std::string utf8Out;
	assertTrue(ext2.extract(0, utf8Out));
	assertTrue(utf8Out == utf8Text);
}


void DataTest::testSQLParse()
{
	Session sess(SessionFactory::instance().create("test", "cs"));

	assertTrue (sess.getFeature("autoCommit"));
	sess.setFeature("autoCommit", false);
	assertTrue (!sess.getFeature("autoCommit"));

	assertFalse (sess.getFeature("sqlParse"));
	sess.setFeature("sqlParse", true);
	assertTrue (sess.getFeature("sqlParse"));

	Statement stmt = (sess << "SELECT %s%c%s,%d,%u,%f,%s FROM Person WHERE Name LIKE 'Simp%%'",
		"'",'a',"'",-1, 1u, 1.5, "42", now);

	assertTrue ("SELECT 'a',-1,1,1.500000,42 FROM Person WHERE Name LIKE 'Simp%'" == stmt.toString());

#ifndef POCO_DATA_NO_SQL_PARSER

	assertEqual (1u, stmt.statementsCount().value());
	assertTrue (stmt.isSelect().value());
	assertTrue (stmt.hasSelect().value());
	assertTrue (!stmt.isUpdate().value());
	assertTrue (!stmt.hasUpdate().value());
	assertTrue (!stmt.isInsert().value());
	assertTrue (!stmt.hasInsert().value());
	assertTrue (!stmt.isDelete().value());
	assertTrue (!stmt.hasDelete().value());

	stmt.reset();
	stmt = (sess << "INSERT INTO Test VALUES ('1', 2, 3.5);"
		"SELECT * FROM Test WHERE First = ?;"
		"UPDATE Test SET value=1 WHERE First = '1';"
		"DELETE FROM Test WHERE First = ?;"
		"DROP TABLE table_name;"
		"ALTER TABLE mytable DROP COLUMN IF EXISTS mycolumn;"
		"PREPARE prep_inst FROM 'INSERT INTO test VALUES (?, ?, ?)';"
		"EXECUTE prep_inst(1, 2, 3);");
	stmt.execute();
	assertEqual (8u, stmt.statementsCount().value());
	assertTrue (!stmt.isSelect().value());
	assertTrue (stmt.hasSelect().value());
	assertTrue (!stmt.isUpdate().value());
	assertTrue (stmt.hasUpdate().value());
	assertTrue (!stmt.isInsert().value());
	assertTrue (stmt.hasInsert().value());
	assertTrue (!stmt.isDelete().value());
	assertTrue (stmt.hasDelete().value());

	sess.setFeature("sqlParse", false);
	assertTrue (!sess.getFeature("sqlParse"));

#else

	std::cout << "partial test (parser not available)";

#endif // POCO_DATA_NO_SQL_PARSER

	stmt.reset();
	stmt = (sess << "INSERT INTO Test VALUES ('1', 2, 3.5);"
		"SELECT * FROM Test WHERE First = ?;"
		"UPDATE Test SET value=1 WHERE First = '1';"
		"DELETE FROM Test WHERE First = ?;"
		"DROP TABLE table_name;"
		"ALTER TABLE mytable DROP COLUMN IF EXISTS mycolumn;"
		"PREPARE prep_inst FROM 'INSERT INTO test VALUES (?, ?, ?)';"
		"EXECUTE prep_inst(1, 2, 3);");

	stmt.execute();
	assertTrue (!stmt.isSelect().isSpecified());
	assertTrue (!stmt.hasSelect().isSpecified());
	assertTrue (!stmt.isUpdate().isSpecified());
	assertTrue (!stmt.hasUpdate().isSpecified());
	assertTrue (!stmt.isInsert().isSpecified());
	assertTrue (!stmt.hasInsert().isSpecified());
	assertTrue (!stmt.isDelete().isSpecified());
	assertTrue (!stmt.hasDelete().isSpecified());
}


void DataTest::testSQLChannel()
{
	AutoPtr<SQLChannel> pChannel = new SQLChannel();
	const std::string dir = pChannel->getProperty("directory");
	Stopwatch sw; sw.start();
	while (!pChannel->isRunning())
	{
		Thread::sleep(10);
		if (sw.elapsedSeconds() > 3)
			fail("SQLChannel timed out");
	}

	Glob g("*.log.sql");
	if (File(dir).exists())
	{
		{
			DirectoryIterator it(dir);
			const DirectoryIterator end;
			while (it != end)
			{
				if (g.match(it->path()))
				{
					File(it->path()).remove();
				}
				++it;
			}
		}
	}

	assertEqual(int(SQLChannel::DEFAULT_FLUSH_SECONDS), NumberParser::parse(pChannel->getProperty("flush")));

	constexpr int mcount{10};
	constexpr int batch{3};
	pChannel->setProperty("minBatch", std::to_string(batch));
	constexpr int flush{1};
	pChannel->setProperty("flush", std::to_string(flush));
	assertEqual(flush, NumberParser::parse(pChannel->getProperty("flush")));
	assertTrue(pChannel->getProperty("times") == "UTC");
	pChannel->setProperty("times", "local");
	assertTrue(pChannel->getProperty("times") == "local");
	pChannel->setProperty("times", "UTC");
	assertTrue(pChannel->getProperty("times") == "UTC");
	for (int i = 0; i < mcount; i++)
	{
		Message msgInfA("InformationSource", Poco::format("%d Informational sync message", i), Message::PRIO_INFORMATION);
		pChannel->log(msgInfA);
	}
	Thread::sleep(2000*flush); // give it time to flush
	auto logged = pChannel->logged();
	assertEqual(mcount, logged);
	pChannel.reset();

	int count = 0;
	DirectoryIterator it(dir);
	const DirectoryIterator end;
	while (it != end)
	{
		if (g.match(it->path()))
		{
			++count;
			File(it->path()).remove();
		}
		++it;
	}
	assertEqual(count, (mcount / batch) + (mcount % batch));
}


void DataTest::testNullableExtract()
{
	Poco::Data::Test::Extractor ext;
	Poco::Nullable<Poco::Int32> ni;
	assertTrue (ni.isNull());
	assertTrue (ext.extract(0, ni));
	assertFalse (ni.isNull());
	assertEqual (ni.value(), 1);
}


void DataTest::testTransactionAutoCommit()
{
	// Transaction must work on an auto-commit session: it disables auto-commit for
	// the duration (so a real SessionImpl::begin() does not throw "Session in auto
	// commit mode") and restores it on commit(), rollback() AND destruction - the
	// last path being what keeps a pooled session from being handed back in
	// manual-commit mode. Uses the no-op Test connector, so no server is involved.
	Session sess(SessionFactory::instance().create("test", "cs"));

	// --- auto-commit ON: Transaction disables it for the txn, then restores it ---
	sess.setFeature("autoCommit", true);
	assertTrue (sess.getFeature("autoCommit"));

	// commit() restores auto-commit
	{
		Transaction trans(sess);
		assertTrue (sess.isTransaction());
		assertTrue (!sess.getFeature("autoCommit"));
		trans.commit();
		assertTrue (!sess.isTransaction());
		assertTrue (sess.getFeature("autoCommit"));
	}
	assertTrue (sess.getFeature("autoCommit"));

	// rollback() restores auto-commit
	{
		Transaction trans(sess);
		assertTrue (!sess.getFeature("autoCommit"));
		trans.rollback();
		assertTrue (sess.getFeature("autoCommit"));
	}
	assertTrue (sess.getFeature("autoCommit"));

	// destruction without commit/rollback rolls back AND restores auto-commit
	{
		Transaction trans(sess);
		assertTrue (sess.isTransaction());
		assertTrue (!sess.getFeature("autoCommit"));
	}
	assertTrue (!sess.isTransaction());
	assertTrue (sess.getFeature("autoCommit"));

	// --- begin() failure must restore auto-commit (regression) ---
	// The Transaction ctor unwinds on a failed begin(), so its destructor never runs;
	// begin() itself must restore the feature, else a pooled session is handed back in
	// manual-commit mode.
	{
		bool threw = false;
		sess.setFeature("throwOnBegin", true);
		try { Transaction trans(sess); }
		catch (Poco::Exception&) { threw = true; }
		sess.setFeature("throwOnBegin", false);
		assertTrue (threw);
		assertTrue (!sess.isTransaction());
		assertTrue (sess.getFeature("autoCommit"));
	}
	assertTrue (sess.getFeature("autoCommit"));

	// --- auto-commit OFF: Transaction must leave the feature untouched ---
	sess.setFeature("autoCommit", false);
	{
		Transaction trans(sess);
		assertTrue (!sess.getFeature("autoCommit"));
		trans.commit();
		assertTrue (!sess.getFeature("autoCommit"));
	}
	assertTrue (!sess.getFeature("autoCommit"));

	// --- dtor restores auto-commit even if rollback() throws (regression) ---
	sess.setFeature("autoCommit", true);
	{
		Transaction trans(sess);
		assertTrue (!sess.getFeature("autoCommit"));
		sess.setFeature("throwOnRollback", true);
	} // dtor: rollback() throws (swallowed) and auto-commit is still restored
	sess.setFeature("throwOnRollback", false);
	assertTrue (sess.getFeature("autoCommit"));
}


void DataTest::testExecuteDirectNotImplemented()
{
	// The Test connector does not override StatementImpl::execDirectImpl(); a
	// backend without direct execution must report that instead of silently
	// doing nothing with the query.
	Session sess(SessionFactory::instance().create("test", "cs"));
	Statement stmt(sess);
	try
	{
		stmt.executeDirect("SELECT 1");
		failmsg("executeDirect() must throw NotImplementedException");
	}
	catch (NotImplementedException&)
	{
	}
}


void DataTest::testSessionNotConnected()
{
	Session sess(SessionFactory::instance().create("test", "cs"));
	Statement stmt = (sess << "SELECT * FROM Strings");
	Statement stmt2(sess);
	sess.close();
	assertTrue (!sess.isConnected());

	// begin() fails before it switches auto-commit off
	assertTrue (sess.getFeature("autoCommit"));
	try { sess.begin(); fail ("must fail"); }
	catch (NotConnectedException&) { }
	assertTrue (sess.getFeature("autoCommit"));
	assertTrue (!sess.isTransaction());

	sess.setFeature("autoCommit", false);
	try { sess.begin(); fail ("must fail"); }
	catch (NotConnectedException&) { }
	assertTrue (!sess.isTransaction());

	try { sess.commit(); fail ("must fail"); }
	catch (NotConnectedException&) { }

	try { sess.rollback(); fail ("must fail"); }
	catch (NotConnectedException&) { }

	// in manual-commit mode, execution fails without starting a transaction
	try { stmt.execute(); fail ("must fail"); }
	catch (NotConnectedException&) { }
	assertTrue (stmt.done());
	assertTrue (!sess.isTransaction());

	try { stmt2.executeDirect("SELECT 1"); fail ("must fail"); }
	catch (NotConnectedException&) { }
	assertTrue (!sess.isTransaction());
}


void DataTest::testSessionNullImpl()
{
	Session sess(SessionFactory::instance().create("test", "cs"));
	Session sess2(std::move(sess));
	assertTrue (sess2.getFeature("autoCommit"));

	try { (void) sess.getFeature("autoCommit"); fail ("must fail"); }
	catch (NullPointerException&) { }

	try { (void) sess.getProperty("storage"); fail ("must fail"); }
	catch (NullPointerException&) { }

	Session sess3(SessionFactory::instance().create("test", "cs"));
	sess3 = std::move(sess2);
	assertTrue (sess3.getFeature("autoCommit"));

	try { (void) sess2.getFeature("autoCommit"); fail ("must fail"); }
	catch (NullPointerException&) { }

	try { (void) sess2.getProperty("storage"); fail ("must fail"); }
	catch (NullPointerException&) { }

	assertTrue (sess.impl() == nullptr);
	assertTrue (sess2.impl() == nullptr);

	// a self-move leaves the session as it was
	Session& self = sess3;
	sess3 = std::move(self);
	assertTrue (sess3.impl() != nullptr);
	assertTrue (sess3.isConnected());
	assertTrue (sess3.getFeature("autoCommit"));

	SessionImpl::Ptr pNull;
	try { Session nullSess(pNull); fail ("must fail"); }
	catch (NullPointerException&) { }
}


void DataTest::testRecordSetMovedFrom()
{
	Session sess(SessionFactory::instance().create("test", "cs"));
	RecordSet rs(sess, "SELECT * FROM Strings");
	RecordSet rs2(std::move(rs));
	assertTrue (rs2.begin() == rs2.end());

	const RecordSet& crs = rs;
	std::ostringstream os;

	try { (void) crs.begin(); fail ("must fail"); }
	catch (NullPointerException&) { }

	try { (void) crs.end(); fail ("must fail"); }
	catch (NullPointerException&) { }

	try { (void) rs.begin(); fail ("must fail"); }
	catch (NullPointerException&) { }

	try { (void) rs.end(); fail ("must fail"); }
	catch (NullPointerException&) { }

	try { rs.formatNames(); fail ("must fail"); }
	catch (NullPointerException&) { }

	try { rs.copyNames(os); fail ("must fail"); }
	catch (NullPointerException&) { }

	try { rs.copyValues(os); fail ("must fail"); }
	catch (NullPointerException&) { }

	try { rs.formatValues(0, RowIterator::POSITION_END); fail ("must fail"); }
	catch (NullPointerException&) { }

	try { rs.copy(os); fail ("must fail"); }
	catch (NullPointerException&) { }

	try { os << rs; fail ("must fail"); }
	catch (NullPointerException&) { }

	// moving from a moved-from recordset gives an empty one
	RecordSet rs3(std::move(rs));
	assertTrue (rs3.begin() == rs3.end());

	RecordSet rs4(sess, "SELECT * FROM Strings");
	rs4 = std::move(rs);
	assertTrue (rs4.begin() == rs4.end());

	// a self-move leaves the recordset as it was
	RecordSet& self = rs2;
	rs2 = std::move(self);
	assertTrue (rs2.begin() == rs2.end());

	// move assignment over a recordset with iterators of its own
	RecordSet rs5(sess, "SELECT * FROM Strings");
	rs5 = std::move(rs2);
	assertTrue (rs5.begin() == rs5.end());

	try { (void) rs2.begin(); fail ("must fail"); }
	catch (NullPointerException&) { }

	try { rs2.copy(os); fail ("must fail"); }
	catch (NullPointerException&) { }

	// reset() with a moved-from statement fails before it changes anything
	Statement stmt = (sess << "SELECT * FROM Strings", now);
	Statement stmt2(std::move(stmt));
	try { rs5.reset(stmt); fail ("must fail"); }
	catch (NullPointerException&) { }
	assertTrue (rs5.begin() == rs5.end());

	rs5.reset(stmt2);
	assertTrue (rs5.begin() == rs5.end());
}


void DataTest::testRecordSetMoveFilter()
{
	Session sess(SessionFactory::instance().create("test", "cs"));

	// the filter, with its child filters, moves with the recordset
	RecordSet rs(sess, "SELECT * FROM Strings");
	RowFilter::Ptr pFilter = new RowFilter(&rs);
	RowFilter::Ptr pChild = new RowFilter(pFilter, RowFilter::OP_AND);
	RecordSet rs2(std::move(rs));
	pFilter->add("str0", RowFilter::VALUE_EQUAL, std::string("1"));
	pChild->add("str1", RowFilter::VALUE_EQUAL, std::string("2"));
	assertTrue (rs2.isFiltered());
	assertTrue (!rs.isFiltered());

	// move assignment: the filter of the source replaces the one of the target,
	// and letting the replaced filter go leaves the target filtered
	RecordSet rs3(sess, "SELECT * FROM Strings");
	RowFilter::Ptr pOld = new RowFilter(&rs3);
	pOld->add("str0", RowFilter::VALUE_EQUAL, std::string("3"));
	rs3 = std::move(rs2);
	pFilter->add("str2", RowFilter::VALUE_EQUAL, std::string("4"));
	assertTrue (rs3.isFiltered());
	pOld->add("str1", RowFilter::VALUE_EQUAL, std::string("5"));
	pOld = nullptr;
	assertTrue (rs3.isFiltered());

	// a parent and its child hold each other; remove the child to free both
	pFilter->removeFilter(pChild);

	// a recordset lets go of its filters when it is destroyed first
	RowFilter::Ptr pOrphan;
	RowFilter::Ptr pOrphanChild;
	{
		RecordSet rs4(sess, "SELECT * FROM Strings");
		pOrphan = new RowFilter(&rs4);
		pOrphanChild = new RowFilter(pOrphan, RowFilter::OP_OR);
	}
	pOrphan->add("str0", RowFilter::VALUE_EQUAL, std::string("6"));
	try { (void) pOrphan->isAllowed(0); fail ("must fail"); }
	catch (NullPointerException&) { }

	// the child looks for a recordset through its parents, and finds none
	try { (void) pOrphanChild->isAllowed(0); fail ("must fail"); }
	catch (NullPointerException&) { }

	// the parent without a recordset still lets its child go, which frees both
	pOrphan->removeFilter(pOrphanChild);
	assertTrue (1 == pOrphan->referenceCount());
	assertTrue (1 == pOrphanChild->referenceCount());

	// a second filter replaces the first one, which then has no recordset;
	// letting the first one go leaves the recordset filtered by the second
	RecordSet rs5(sess, "SELECT * FROM Strings");
	RowFilter::Ptr pFirst = new RowFilter(&rs5);
	RowFilter::Ptr pSecond = new RowFilter(&rs5);
	pSecond->add("str0", RowFilter::VALUE_EQUAL, std::string("7"));
	try { (void) pFirst->isAllowed(0); fail ("must fail"); }
	catch (NullPointerException&) { }
	pFirst = nullptr;
	assertTrue (rs5.isFiltered());
	assertTrue (pSecond->isAllowed(0));

	// a replaced filter tree can still be changed: comparisons and child filters
	// can be removed and added, and removing the child frees both
	RowFilter::Ptr pReplaced = new RowFilter(&rs5);
	RowFilter::Ptr pReplacedChild = new RowFilter(pReplaced, RowFilter::OP_AND);
	pReplaced->add("str0", RowFilter::VALUE_EQUAL, std::string("8"));
	RowFilter::Ptr pThird = new RowFilter(&rs5);
	assertTrue (1 == pReplaced->remove("str0"));
	RowFilter::Ptr pAdded = new RowFilter(pReplaced, RowFilter::OP_OR);
	pReplaced->removeFilter(pAdded);
	pReplaced->removeFilter(pReplacedChild);
	assertTrue (1 == pReplaced->referenceCount());
	assertTrue (1 == pReplacedChild->referenceCount());
	assertTrue (1 == pAdded->referenceCount());

	// a replaced filter let go after its recordset does not touch that recordset
	{
		RecordSet rs6(sess, "SELECT * FROM Strings");
		pFirst = new RowFilter(&rs6);
		RowFilter::Ptr pReplacing = new RowFilter(&rs6);
	}
	pFirst = nullptr;

	// a filter tree attached to a filter of another recordset takes that recordset
	// into all its filters: none of them uses the recordset the tree came from,
	// which is destroyed here
	RecordSet rs7(sess, "SELECT * FROM Strings");
	RowFilter::Ptr pTarget = new RowFilter(&rs7);
	RowFilter::Ptr pAttached;
	RowFilter::Ptr pAttachedChild;
	{
		RecordSet rs8(sess, "SELECT * FROM Strings");
		pAttached = new RowFilter(&rs8);
		pAttachedChild = new RowFilter(pAttached, RowFilter::OP_AND);
		pTarget->addFilter(pAttached, RowFilter::OP_OR);
	}
	pAttachedChild->add("str0", RowFilter::VALUE_EQUAL, std::string("9"));
	assertTrue (pAttachedChild->isAllowed(0));
	assertTrue (1 == pAttachedChild->remove("str0"));
	pTarget->removeFilter(pAttached);
	pAttached->removeFilter(pAttachedChild);
	assertTrue (1 == pAttached->referenceCount());
	assertTrue (1 == pAttachedChild->referenceCount());
}


void DataTest::testNotConnectedMessage()
{
	// The message names the connector, never the connection string,
	// which can hold a password.
	Session sess(SessionFactory::instance().create("test", "user=scott;password=tiger"));
	Statement stmt = (sess << "SELECT * FROM Strings");
	Statement stmt2(sess);
	sess.close();

	std::vector<std::string> messages;

	try { sess.begin(); fail ("must fail"); }
	catch (NotConnectedException& exc) { messages.push_back(exc.message()); }

	try { sess.commit(); fail ("must fail"); }
	catch (NotConnectedException& exc) { messages.push_back(exc.message()); }

	try { sess.rollback(); fail ("must fail"); }
	catch (NotConnectedException& exc) { messages.push_back(exc.message()); }

	try { Statement stmt3 = (sess << "SELECT 1"); fail ("must fail"); }
	catch (NotConnectedException& exc) { messages.push_back(exc.message()); }

	try { Statement stmt4(sess); fail ("must fail"); }
	catch (NotConnectedException& exc) { messages.push_back(exc.message()); }

	try { stmt.execute(); fail ("must fail"); }
	catch (NotConnectedException& exc) { messages.push_back(exc.message()); }

	try { stmt2.executeDirect("SELECT 1"); fail ("must fail"); }
	catch (NotConnectedException& exc) { messages.push_back(exc.message()); }

	assertEqual (7, messages.size());
	for (const auto& message: messages)
		assertMessageEqual (Poco::Data::Test::Connector::KEY, message);
}


namespace
{
	struct ClosingTransactor
		/// Closes the session, then fails.
	{
		[[noreturn]] void operator () (Session& session) const
		{
			session.close();
			throw Poco::IllegalStateException("transactor failed");
		}
	};


	struct FailingTransactor
		/// Makes the rollback fail, then fails.
	{
		[[noreturn]] void operator () (Session& session) const
		{
			session.setFeature("throwOnRollback", true);
			throw Poco::IllegalStateException("transactor failed");
		}
	};


	struct CommittingTransactor
		/// Commits, then fails.
	{
		[[noreturn]] void operator () (Session& session) const
		{
			session.commit();
			throw Poco::IllegalStateException("transactor failed");
		}
	};


	class TestArchiveStrategy: public ArchiveByAgeStrategy
		/// Gives the test access to the session and the statements of the strategy.
	{
	public:
		TestArchiveStrategy():
			ArchiveByAgeStrategy("test", "cs", "T_POCO_LOG", "T_POCO_LOG_ARCHIVE", "1 seconds")
		{
		}

		[[nodiscard]] Session& strategySession()
		{
			return session();
		}

		[[nodiscard]] bool strategyHasStatements() const
		{
			return hasStatements();
		}

		[[nodiscard]] Statement& countStatement()
			/// Returns the count statement; throws NullPointerException if there is none.
		{
			return getCountStatement();
		}
	};


	class ExecutionHold
		/// Holds the next execution of a TestStatementImpl in hasNext()
		/// until the hold is destroyed, so that a failing test never
		/// leaves the execution waiting.
	{
	public:
		POCO_NODISCARD_CTOR ExecutionHold()
		{
			Poco::Data::Test::TestStatementImpl::holdNextExecution();
		}

		~ExecutionHold()
		{
			try
			{
				Poco::Data::Test::TestStatementImpl::releaseExecution();
			}
			catch (...)
			{
				poco_unexpected();
			}
		}

		ExecutionHold(const ExecutionHold&) = delete;
		ExecutionHold(ExecutionHold&&) = delete;
		ExecutionHold& operator = (const ExecutionHold&) = delete;
		ExecutionHold& operator = (ExecutionHold&&) = delete;
	};


	[[nodiscard]] std::string contentOf(const Path& dir)
		/// Returns the content of all files in the directory.
	{
		std::string content;
		DirectoryIterator it(dir);
		const DirectoryIterator end;
		for (; it != end; ++it)
		{
			FileInputStream istr(it->path());
			std::string text;
			StreamCopier::copyToString(istr, text);
			content += text;
		}
		return content;
	}
}


void DataTest::testTransactionNotConnected()
{
	// The connection is lost while the transaction is open: the destructor
	// neither rolls back nor switches auto-commit back on.
	Session sess(SessionFactory::instance().create("test", "cs"));
	assertTrue (sess.getFeature("autoCommit"));
	{
		Transaction trans(sess);
		assertTrue (sess.isTransaction());
		assertTrue (!sess.getFeature("autoCommit"));
		sess.setFeature("connected", false);
	}
	assertTrue (!sess.getFeature("autoCommit"));
}


void DataTest::testTransactorFailure()
{
	// The transactor closes the session and fails: nothing is rolled back,
	// and the caller gets the exception of the transactor.
	{
		Session sess(SessionFactory::instance().create("test", "cs"));
		ClosingTransactor tr;
		try { Transaction trans(sess, tr); fail ("must fail"); }
		catch (IllegalStateException&) { }
		assertTrue (!sess.isConnected());
	}

	// The rollback fails: the caller still gets the exception of the transactor,
	// and auto-commit is switched back on.
	{
		Session sess(SessionFactory::instance().create("test", "cs"));
		FailingTransactor tr;
		try { Transaction trans(sess, tr); fail ("must fail"); }
		catch (IllegalStateException&) { }
		assertTrue (!sess.isTransaction());
		assertTrue (sess.getFeature("autoCommit"));
	}

	// The transactor ends the transaction itself, then fails: there is nothing
	// to roll back, and auto-commit is switched back on.
	{
		Session sess(SessionFactory::instance().create("test", "cs"));
		CommittingTransactor tr;
		try { Transaction trans(sess, tr); fail ("must fail"); }
		catch (IllegalStateException&) { }
		assertTrue (!sess.isTransaction());
		assertTrue (sess.getFeature("autoCommit"));
	}
}


void DataTest::testStatementAsyncKeepsImpl()
{
	using Poco::Data::Test::TestStatementImpl;

	Session sess(SessionFactory::instance().create("test", "cs"));
	const int instances = TestStatementImpl::instances();

	// The execution lets go of the implementation before wait() returns:
	// the Statement destroys it, and the result of the execution does not hold it.
	{
		SharedPtr<Statement::Result> pResult;
		{
			Statement stmt = (sess << "SELECT * FROM Strings");
			assertEqual (instances + 1, TestStatementImpl::instances());
			pResult = new Statement::Result(stmt.executeAsync());
			pResult->wait();
			assertEqual (instances + 1, TestStatementImpl::instances());
		}
		assertEqual (instances, TestStatementImpl::instances());
		assertTrue (!pResult->failed());
		assertEqual (0, pResult->data());
	}

	// The Statement is destroyed while the execution runs: the execution holds
	// the implementation until it ends, and lets it go before wait() returns.
	{
		SharedPtr<Statement::Result> pResult;
		int whileRunning = 0;
		{
			ExecutionHold hold;
			{
				Statement stmt = (sess << "SELECT * FROM Strings");
				pResult = new Statement::Result(stmt.executeAsync());
				assertTrue (TestStatementImpl::waitUntilHeld(10000));
			}
			whileRunning = TestStatementImpl::instances();
		}
		pResult->wait();
		assertEqual (instances + 1, whileRunning);
		assertEqual (instances, TestStatementImpl::instances());
		assertTrue (!pResult->failed());
		assertEqual (0, pResult->data());
	}
}


void DataTest::testSQLChannelUnreachable()
{
	// The database cannot be reached: the entries stay queued while the channel
	// tries to reconnect, wait() waits as long as it is told to, and the channel,
	// when destroyed, writes the entries to local files instead of hanging.
	Path dir(TemporaryFile::tempName());
	dir.makeDirectory();
	{
		AutoPtr<SQLChannel> pChannel = new SQLChannel("test", Poco::Data::Test::Connector::UNREACHABLE, "TestSQLChannel");
		pChannel->setProperty("directory", dir.toString());
		for (int i = 0; i < 3; ++i)
		{
			Message msg("DataTest", Poco::format("unreachable entry %d", i), Message::PRIO_INFORMATION);
			pChannel->log(msg);
		}

		Stopwatch sw; sw.start();
		assertTrue (pChannel->wait(300) > 0);
		assertTrue (sw.elapsed() >= 300 * 1000);
	}

	const std::string content = contentOf(dir);
	File(dir).remove(true);

	for (int i = 0; i < 3; ++i)
		assertTrue (content.find(Poco::format("unreachable entry %d", i)) != std::string::npos);
}


void DataTest::testArchiveStrategyReopen()
{
	TestArchiveStrategy strategy;
	strategy.archive();

	// keep the first session alive, so that its statements stay valid objects
	Session first = strategy.strategySession();
	first.close();

	// the strategy opens a new session and runs new statements on it
	strategy.archive();
	assertTrue (!first.isConnected());
	assertTrue (strategy.strategySession().isConnected());
	strategy.archive();

	// open() releases the statements; archive() creates them again
	strategy.open();
	assertTrue (!strategy.strategyHasStatements());
	strategy.archive();
	assertTrue (strategy.strategyHasStatements());
}


namespace
{
	class MessageCollector: public Poco::Channel
		/// Keeps the text of every message logged to it.
	{
	public:
		using Ptr = Poco::AutoPtr<MessageCollector>;

		void log(const Message& msg) override
		{
			Poco::FastMutex::ScopedLock lock(_mutex);
			_messages.push_back(msg.getText());
		}

		[[nodiscard]] std::vector<std::string> messages() const
			/// Returns the texts of the messages logged so far.
		{
			Poco::FastMutex::ScopedLock lock(_mutex);
			return _messages;
		}

		void clear()
			/// Forgets the messages logged so far.
		{
			Poco::FastMutex::ScopedLock lock(_mutex);
			_messages.clear();
		}

	private:
		mutable Poco::FastMutex  _mutex;
		std::vector<std::string> _messages;
	};


	struct IdleTransactor
		/// Does nothing.
	{
		void operator () (Session&) const
		{
		}
	};
}


void DataTest::testStatementAsyncError()
{
	using Poco::Data::Test::TestStatementImpl;

	// An execution that fails reports its error through the Result. It holds the
	// implementation until it ends, also when the Statement is destroyed first,
	// and lets it go before wait() returns.
	Session sess(SessionFactory::instance().create("test", "cs"));
	sess.setFeature("throwOnHasNext", true);
	const int instances = TestStatementImpl::instances();

	SharedPtr<Statement::Result> pResult;
	int whileRunning = 0;
	{
		ExecutionHold hold;
		{
			Statement stmt = (sess << "SELECT * FROM Strings");
			pResult = new Statement::Result(stmt.executeAsync());
			assertTrue (TestStatementImpl::waitUntilHeld(10000));
		}
		whileRunning = TestStatementImpl::instances();
	}
	pResult->wait();
	assertEqual (instances + 1, whileRunning);
	assertEqual (instances, TestStatementImpl::instances());
	assertTrue (pResult->failed());
	assertNotNullPtr (pResult->exception());
	try { pResult->exception()->rethrow(); fail ("must fail"); }
	catch (UnknownDataBaseException&) { }
}


void DataTest::testStatementWaitErrorType()
{
	// wait() rethrows the error of an asynchronous execution with its type.
	Session sess(SessionFactory::instance().create("test", "cs"));
	sess.setFeature("throwOnHasNext", true);
	Statement stmt = (sess << "SELECT * FROM Strings");
	stmt.executeAsync();
	try
	{
		stmt.wait();
		fail ("must throw UnknownDataBaseException");
	}
	catch (UnknownDataBaseException&) { }
}


void DataTest::testStatementSetAsyncMovedFrom()
{
	// setAsync() only sets the flag: the asynchronous execution is set up when
	// it starts, so a statement without an implementation can be marked too.
	Session sess(SessionFactory::instance().create("test", "cs"));
	Statement stmt = (sess << "SELECT * FROM Strings");
	Statement moved(std::move(stmt));
	stmt.setAsync(true);
	assertTrue (stmt.isAsync());

	moved.setAsync(true);
	assertEqual (0, moved.execute());
	assertEqual (0, moved.wait());
}


void DataTest::testTransactionNotConnectedLog()
{
	// The connection is lost while the transaction is open: the destructor does
	// not roll back, and logs that it does not.
	MessageCollector::Ptr pCollector = new MessageCollector;
	Logger& logger = Logger::get("DataTest.TransactionNotConnected");
	logger.setChannel(pCollector);
	logger.setLevel(Message::PRIO_DEBUG);

	Session sess(SessionFactory::instance().create("test", "cs"));
	{
		Transaction trans(sess, &logger);
		assertTrue (sess.isTransaction());
		sess.setFeature("connected", false);
	}
	Logger::destroy("DataTest.TransactionNotConnected");

	// the test session ends its transaction only when it is rolled back
	assertTrue (sess.isTransaction());
	assertTrue (!sess.getFeature("autoCommit"));
	const std::vector<std::string> messages = pCollector->messages();
	assertEqual (1, messages.size());
	assertEqual ("Not rolling back transaction: session is not connected."s, messages[0]);
}


void DataTest::testTransactorFailureLog()
{
	// What the Transaction logs shows whether it rolls back.
	MessageCollector::Ptr pCollector = new MessageCollector;
	Logger& logger = Logger::get("DataTest.TransactorFailure");
	logger.setChannel(pCollector);
	logger.setLevel(Message::PRIO_DEBUG);

	// The rollback fails: its error is logged, and the caller gets the exception
	// of the transactor.
	{
		Session sess(SessionFactory::instance().create("test", "cs"));
		FailingTransactor tr;
		try { Transaction trans(sess, tr, &logger); fail ("must fail"); }
		catch (IllegalStateException&) { }
	}
	std::vector<std::string> messages = pCollector->messages();
	assertEqual (3, messages.size());
	assertEqual ("Error executing transaction."s, messages[0]);
	assertEqual ("Rolling back transaction."s, messages[1]);
	assertTrue (messages[2].find("Error while rolling back database transaction: ") == 0);

	// begin() fails: no transaction is open, so nothing is rolled back, and the
	// caller gets the exception of begin().
	pCollector->clear();
	{
		Session sess(SessionFactory::instance().create("test", "cs"));
		sess.setFeature("throwOnBegin", true);
		sess.setFeature("throwOnRollback", true);
		IdleTransactor tr;
		try { Transaction trans(sess, tr, &logger); fail ("must fail"); }
		catch (ConnectionFailedException& exc)
		{
			assertMessageEqual ("Test: begin() failed", exc.message());
		}
		assertTrue (sess.getFeature("autoCommit"));
	}
	messages = pCollector->messages();
	assertEqual (1, messages.size());
	assertEqual ("Error executing transaction."s, messages[0]);

	// The transactor closes the session: nothing is rolled back.
	pCollector->clear();
	{
		Session sess(SessionFactory::instance().create("test", "cs"));
		ClosingTransactor tr;
		try { Transaction trans(sess, tr, &logger); fail ("must fail"); }
		catch (IllegalStateException&) { }
	}
	messages = pCollector->messages();
	Logger::destroy("DataTest.TransactorFailure");
	assertEqual (2, messages.size());
	assertEqual ("Error executing transaction."s, messages[0]);
	assertEqual ("Not rolling back transaction: session is not connected."s, messages[1]);
}


void DataTest::testSQLChannelReconnectNotDelayed()
{
	// Before each attempt to reconnect, the logging thread closes the session, and
	// close() waits up to the timeout for the queue to empty. Only the logging thread
	// empties the queue, so it does not wait for itself: with a timeout of 10 s,
	// it tries again after the back-off of 200 ms.
	Path dir(TemporaryFile::tempName());
	dir.makeDirectory();
	{
		AutoPtr<SQLChannel> pChannel = new SQLChannel("test", Poco::Data::Test::Connector::UNREACHABLE, "TestSQLChannel", "T_POCO_LOG", 10000);
		pChannel->setProperty("directory", dir.toString());
		Message msg("DataTest", "entry while reconnecting", Message::PRIO_INFORMATION);
		pChannel->log(msg);

		// every failed attempt queues an entry of its own
		Stopwatch sw;
		sw.start();
		std::size_t queued = pChannel->wait(10);
		while (queued < 3 && sw.elapsedSeconds() < 5)
			queued = pChannel->wait(10);
		assertTrue (queued >= 3);
	}
	File(dir).remove(true);
}


void DataTest::testSQLChannelWaitStopped()
{
	// Once the logging thread has stopped, nothing takes queued entries any more:
	// wait() returns their number at once, also when told to wait without limit.
	Path dir(TemporaryFile::tempName());
	dir.makeDirectory();
	{
		AutoPtr<SQLChannel> pChannel = new SQLChannel;
		pChannel->setProperty("directory", dir.toString());
		pChannel->stop();
		assertTrue (!pChannel->isRunning());
		Message msg("DataTest", "entry after stop", Message::PRIO_INFORMATION);
		pChannel->log(msg);

		Stopwatch sw;
		sw.start();
		assertEqual (1, pChannel->wait(10000));
		assertTrue (sw.elapsedSeconds() < 5);
		assertEqual (1, pChannel->wait(0));
	}
	File(dir).remove(true);
}


void DataTest::testSQLChannelBackOff()
{
	// While the database cannot be reached, the channel tries to connect again
	// after 200, 400, 800 ... ms; the entries logged in the meantime, the errors
	// of the failed attempts among them, do not make it try sooner.
	Path dir(TemporaryFile::tempName());
	dir.makeDirectory();
	{
		AutoPtr<SQLChannel> pChannel = new SQLChannel("test", Poco::Data::Test::Connector::UNREACHABLE, "TestSQLChannel");
		pChannel->setProperty("directory", dir.toString());
		std::size_t entries = 0;
		std::size_t queued = 0;
		Stopwatch sw;
		sw.start();
		while (sw.elapsedSeconds() < 1)
		{
			Message msg("DataTest", "entry while backing off", Message::PRIO_INFORMATION);
			pChannel->log(msg);
			++entries;
			queued = pChannel->wait(20);
		}
		// within the second, the attempts at about 0, 200 and 600 ms have failed
		assertTrue (queued > entries);
		assertTrue (queued - entries <= 5);
	}
	File(dir).remove(true);
}


void DataTest::testSQLChannelArchiveNotConnected()
{
	using Poco::Data::Test::TestStatementImpl;

	// The archive strategy works on the database: it runs with each batch while
	// the channel is connected, and not while the channel writes to local files.
	Path dir(TemporaryFile::tempName());
	dir.makeDirectory();
	{
		AutoPtr<SQLChannel> pChannel = new SQLChannel("test", "cs", "TestSQLChannel");
		pChannel->setProperty("directory", dir.toString());
		pChannel->setProperty("keep", "1 seconds");
		{
			// connected: the archive statements run with the batch; the first one is held
			ExecutionHold hold;
			Message msg("DataTest", "database entry", Message::PRIO_INFORMATION);
			pChannel->log(msg);
			assertTrue (TestStatementImpl::waitUntilHeld(10000));
			// set while the logging thread is held, so that it reads the new value after the hold
			pChannel->setProperty("connector", "");
		}
		Stopwatch sw;
		sw.start();
		while (pChannel->logged() == 0 && sw.elapsedSeconds() < 10)
			Thread::sleep(10);
		const std::size_t logged = pChannel->logged();
		assertTrue (logged > 0);

		// not connected: the batch goes to a local file, without the archive statements
		ExecutionHold hold;
		Message msg("DataTest", "file entry", Message::PRIO_INFORMATION);
		pChannel->log(msg);
		sw.restart();
		while (pChannel->logged() == logged && sw.elapsedSeconds() < 10)
			Thread::sleep(10);
		assertTrue (pChannel->logged() > logged);
		assertTrue (!TestStatementImpl::waitUntilHeld(0));
	}
	const std::string content = contentOf(dir);
	File(dir).remove(true);
	assertTrue (content.find("file entry") != std::string::npos);
}


void DataTest::testSQLChannelDrainFallback()
{
	// When the channel stops, it writes what is left to local files if the
	// database cannot take it, also when the connector itself cannot be found.
	Path dir(TemporaryFile::tempName());
	dir.makeDirectory();
	{
		AutoPtr<SQLChannel> pChannel = new SQLChannel("no such connector", "cs", "TestSQLChannel");
		pChannel->setProperty("directory", dir.toString());
		Message msg("DataTest", "entry without a connector", Message::PRIO_INFORMATION);
		pChannel->log(msg);
	}
	const std::string content = contentOf(dir);
	File(dir).remove(true);
	assertTrue (content.find("entry without a connector") != std::string::npos);

	// The directory for local files is gone: the channel gives up on the entries
	// when it is destroyed. With "throw" set, the errors of the failed writes
	// reach the logging thread, which logs them as entries of their own.
	for (const std::string& throwErrors: {"false"s, "true"s})
	{
		Path gone(TemporaryFile::tempName());
		gone.makeDirectory();
		{
			AutoPtr<SQLChannel> pChannel = new SQLChannel("test", Poco::Data::Test::Connector::UNREACHABLE, "TestSQLChannel");
			pChannel->setProperty("directory", gone.toString());
			pChannel->setProperty("throw", throwErrors);
			File(gone).remove(true);
			Message msg("DataTest", "entry with nowhere to go", Message::PRIO_INFORMATION);
			pChannel->log(msg);
		}
		assertTrue (!File(gone).exists());
	}
}


void DataTest::testSQLChannelDrainRejected()
{
	// The logging thread holds the entries back until a batch of minBatch entries
	// is full. When the channel stops and the database rejects the entries held
	// back, the channel writes them to local files.
	Path dir(TemporaryFile::tempName());
	dir.makeDirectory();
	std::size_t queued = 0;
	std::size_t logged = 0;
	{
		AutoPtr<SQLChannel> pChannel = new SQLChannel("test", Poco::Data::Test::Connector::REJECTING, "TestSQLChannel", "T_POCO_LOG", 1000, 10);
		pChannel->setProperty("directory", dir.toString());
		pChannel->setProperty("flush", "0");
		for (int i = 0; i < 3; ++i)
		{
			Message msg("DataTest", Poco::format("held back entry %d", i), Message::PRIO_INFORMATION);
			pChannel->log(msg);
		}
		// the logging thread has taken the entries and holds them back
		queued = pChannel->wait(10000);
		logged = pChannel->logged();
	}
	const std::string content = contentOf(dir);
	File(dir).remove(true);

	assertEqual (0, queued);
	assertEqual (0, logged);
	for (int i = 0; i < 3; ++i)
		assertTrue (content.find(Poco::format("held back entry %d", i)) != std::string::npos);
}


void DataTest::testArchiveStrategyOpenReleasesStatements()
{
	// open() lets the statements of the previous session go: a strategy that
	// uses them without creating them again gets NullPointerException.
	TestArchiveStrategy strategy;
	(void) strategy.countStatement();
	strategy.open();
	try { (void) strategy.countStatement(); fail ("must fail"); }
	catch (NullPointerException&) { }

	// archive() creates them again, on the new session
	strategy.archive();
	assertTrue (strategy.countStatement().done());
}


void DataTest::setUp()
{
}


void DataTest::tearDown()
{
}


CppUnit::Test* DataTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("DataTest");

	CppUnit_addTest(pSuite, DataTest, testSession);
	CppUnit_addTest(pSuite, DataTest, testStatementFormatting);
	CppUnit_addTest(pSuite, DataTest, testStatementMoveAssignment);
	CppUnit_addTest(pSuite, DataTest, testStatementSwap);
	CppUnit_addTest(pSuite, DataTest, testFeatures);
	CppUnit_addTest(pSuite, DataTest, testProperties);
	CppUnit_addTest(pSuite, DataTest, testLOB);
	CppUnit_addTest(pSuite, DataTest, testCLOB);
	CppUnit_addTest(pSuite, DataTest, testCLOBStreams);
	CppUnit_addTest(pSuite, DataTest, testColumnVector);
	CppUnit_addTest(pSuite, DataTest, testColumnVectorBool);
	CppUnit_addTest(pSuite, DataTest, testColumnDeque);
	CppUnit_addTest(pSuite, DataTest, testColumnList);
	CppUnit_addTest(pSuite, DataTest, testRow);
	CppUnit_addTest(pSuite, DataTest, testRowSort);
	CppUnit_addTest(pSuite, DataTest, testSimpleRowFormatter);
	CppUnit_addTest(pSuite, DataTest, testSimpleRowFormatterSwap);
	CppUnit_addTest(pSuite, DataTest, testJSONRowFormatter);
	CppUnit_addTest(pSuite, DataTest, testDateAndTime);
	CppUnit_addTest(pSuite, DataTest, testExternalBindingAndExtraction);
	CppUnit_addTest(pSuite, DataTest, testTranscode);
	CppUnit_addTest(pSuite, DataTest, testSQLParse);
	CppUnit_addTest(pSuite, DataTest, testSQLChannel);
	CppUnit_addTest(pSuite, DataTest, testNullableExtract);
	CppUnit_addTest(pSuite, DataTest, testTransactionAutoCommit);
	CppUnit_addTest(pSuite, DataTest, testExecuteDirectNotImplemented);
	CppUnit_addTest(pSuite, DataTest, testSessionNotConnected);
	CppUnit_addTest(pSuite, DataTest, testSessionNullImpl);
	CppUnit_addTest(pSuite, DataTest, testRecordSetMovedFrom);
	CppUnit_addTest(pSuite, DataTest, testRecordSetMoveFilter);
	CppUnit_addTest(pSuite, DataTest, testNotConnectedMessage);
	CppUnit_addTest(pSuite, DataTest, testTransactionNotConnected);
	CppUnit_addTest(pSuite, DataTest, testTransactorFailure);
	CppUnit_addTest(pSuite, DataTest, testStatementAsyncKeepsImpl);
	CppUnit_addTest(pSuite, DataTest, testSQLChannelUnreachable);
	CppUnit_addTest(pSuite, DataTest, testArchiveStrategyReopen);
	CppUnit_addTest(pSuite, DataTest, testStatementAsyncError);
	CppUnit_addTest(pSuite, DataTest, testStatementWaitErrorType);
	CppUnit_addTest(pSuite, DataTest, testStatementSetAsyncMovedFrom);
	CppUnit_addTest(pSuite, DataTest, testTransactionNotConnectedLog);
	CppUnit_addTest(pSuite, DataTest, testTransactorFailureLog);
	CppUnit_addTest(pSuite, DataTest, testSQLChannelReconnectNotDelayed);
	CppUnit_addTest(pSuite, DataTest, testSQLChannelWaitStopped);
	CppUnit_addTest(pSuite, DataTest, testSQLChannelBackOff);
	CppUnit_addTest(pSuite, DataTest, testSQLChannelArchiveNotConnected);
	CppUnit_addTest(pSuite, DataTest, testSQLChannelDrainFallback);
	CppUnit_addTest(pSuite, DataTest, testSQLChannelDrainRejected);
	CppUnit_addTest(pSuite, DataTest, testArchiveStrategyOpenReleasesStatements);

	return pSuite;
}
