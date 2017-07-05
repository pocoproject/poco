//
// SQLExecutor.h
//
// $Id: //poco/Main/Data/ODBC/testsuite/src/SQLExecutor.h#4 $
//
// Definition of the SQLExecutor class.
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef SQLExecutor_INCLUDED
#define SQLExecutor_INCLUDED


#include "Poco/Data/ODBC/ODBC.h"
#include "Poco/Data/ODBC/Utility.h"
#include "Poco/Data/ODBC/ODBCException.h"
#include "Poco/Data/Session.h"
#include "Poco/Data/BulkExtraction.h"
#include "Poco/Data/BulkBinding.h"
#include "Poco/Data/RecordSet.h"
#include "Poco/NumberFormatter.h"
#include "Poco/String.h"
#include "Poco/Exception.h"
#include <iostream>


#define poco_odbc_check_env(r, h) \
	if (!SQL_SUCCEEDED(r))	\
	{ \
		Poco::Data::ODBC::EnvironmentException ee(h); \
		std::cout << ee.toString() << std::endl; \
	} \
	assert (SQL_SUCCEEDED(r))


#define poco_odbc_check_dbc(r, h) \
	if (!SQL_SUCCEEDED(r))	\
	{ \
		Poco::Data::ODBC::ConnectionException ce(h); \
		std::cout << ce.toString() << std::endl; \
	} \
	assert (SQL_SUCCEEDED(r))


#define poco_odbc_check_stmt(r, h) \
	if (!SQL_SUCCEEDED(r))	\
	{ \
		Poco::Data::ODBC::StatementException se(h); \
		std::cout << se.toString() << std::endl; \
	} \
	assert (SQL_SUCCEEDED(r))


#define poco_odbc_check_desc(r, h) \
	if (!SQL_SUCCEEDED(r))	\
	{ \
		Poco::Data::ODBC::DescriptorException de(h); \
		std::cout << de.toString() << std::endl; \
	} \
	assert (SQL_SUCCEEDED(r))


#define poco_data_using_statements using Poco::Data::Keywords::now; \
		using Poco::Data::Keywords::into; \
		using Poco::Data::Keywords::use; \
		using Poco::Data::Keywords::bulk; \
		using Poco::Data::Keywords::limit; \
		using Poco::Data::CLOB; \
		using Poco::Data::ODBC::ConnectionException; \
		using Poco::Data::ODBC::StatementException

struct ExecUtil
{
	static std::string mangleTable(const std::string& name);

	static std::string person()
	{
		return mangleTable("Person");
	}

	static std::string strings()
	{
		return mangleTable("Strings");
	}

	static std::string ints()
	{
		return mangleTable("Ints");
	}

	static std::string tuples()
	{
		return mangleTable("Tuples");
	}

	static std::string vectors()
	{
		return mangleTable("Vectors");
	}

	static std::string anys()
	{
		return mangleTable("Anys");
	}

	static std::string nulltest()
	{
		return mangleTable("NullTest");
	}

	static std::string misctest()
	{
		return mangleTable("MiscTest");
	}

	static std::string nullabletest()
	{
		return mangleTable("NullableTest");
	}

	static std::string pocolog()
	{
		return mangleTable("POCO_LOG");
	}

	static std::string pocolog_a()
	{
		return mangleTable("POCO_LOG_A");
	}

	static std::string stored_func()
	{
		return mangleTable("storedFunc");
	}

	static std::string stored_proc()
	{
		return mangleTable("storedProc");
	}

	static std::string test_tbl()
	{
		return mangleTable("Test");
	}
};


class SQLExecutor: public CppUnit::TestCase
{
public:
	enum DataBinding
	{
		PB_IMMEDIATE,
		PB_AT_EXEC
	};
	
	enum DataExtraction
	{
		DE_MANUAL,
		DE_BOUND
	};

	SQLExecutor(const std::string& name, Poco::Data::Session* _pSession, const std::string& connInitSql = std::string(), const std::string& schemaName = std::string());
	~SQLExecutor();

	void execute(const std::string& sql);
		/// Execute a query.

	void bareboneODBCTest(const std::string& dbConnString,
		const std::string& tableCreateString,
		DataBinding bindMode, 
		DataExtraction extractMode,
		bool doTime=true,
		const std::string& blobPlaceholder="?");

	void bareboneODBCMultiResultTest(const std::string& dbConnString, 
		const std::string& tableCreateString, 
		SQLExecutor::DataBinding bindMode, 
		SQLExecutor::DataExtraction extractMode,
		const std::string& insert = MULTI_INSERT,
		const std::string& select = MULTI_SELECT);
		/// The above two functions use "bare bone" ODBC API calls  
		/// (i.e. calls are not "wrapped" in PocoData framework structures).
		/// The purpose of the functions is to verify that a driver behaves
		/// correctly as well as to determine its capabilities 
		/// (e.g. SQLGetData() restrictions relaxation policy, if any). 
		/// If these test pass, subsequent tests failures are likely ours.

	void zeroRows();
	void simpleAccess();
	void complexType();
	void complexTypeTuple();

	void simpleAccessVector();
	void complexTypeVector();
	void sharedPtrComplexTypeVector();
	void autoPtrComplexTypeVector();
	void insertVector();
	void insertEmptyVector();

	void simpleAccessList();
	void complexTypeList();
	void insertList();
	void insertEmptyList();

	void simpleAccessDeque();
	void complexTypeDeque();
	void insertDeque();
	void insertEmptyDeque();

	void affectedRows(const std::string& whereClause = "");

	void insertSingleBulk();
	void insertSingleBulkVec();

	void limits();
	void limitOnce();
	void limitPrepare();
	void limitZero();
	void prepare();
	void insertStatReuse();

	template <typename C1, typename C2, typename C3, typename C4, typename C5, typename C6>
	void doBulkWithBool(Poco::UInt32 size, const std::string& blobPlaceholder="?")
	{
		poco_data_using_statements;

		std::string funct = "doBulkWithBool()";
		C1 ints;
		C2 strings;
		C3 blobs;
		C4 floats;
		C5 dateTimes(size);
		C6 bools;
		
		for (int i = 0; i < size; ++i)
		{
			ints.push_back(i);
			strings.push_back(std::string("xyz" + Poco::NumberFormatter::format(i)));
			blobs.push_back(std::string("abc") + Poco::NumberFormatter::format(i));
			floats.push_back(i + .5);
			bools.push_back(0 == i % 2);
		}

		try 
		{
			session() << 
				Poco::format("INSERT INTO %s VALUES (?,%s,?,?,?,?)", ExecUtil::misctest(), blobPlaceholder), 
				use(strings), 
				use(blobs), 
				use(ints),
				use(floats),
				use(dateTimes),
				use(bools), now;
		} catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
		catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }

		try { session() << "DELETE FROM "<< ExecUtil::misctest(), now; }
		catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
		catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }

		try 
		{
			session() <<  
				Poco::format("INSERT INTO %s VALUES (?,%s,?,?,?,?)", ExecUtil::misctest(), blobPlaceholder),
				use(strings, bulk), 
				use(blobs, bulk), 
				use(ints, bulk),
				use(floats, bulk),
				use(dateTimes, bulk),
				use(bools, bulk), now;
		} catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
		catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }

		ints.clear();
		strings.clear();
		blobs.clear();
		floats.clear();
		dateTimes.clear();
		bools.clear();
		
		try 
		{ 
			session() << "SELECT * FROM "<< ExecUtil::misctest() <<" ORDER BY Third", 
				into(strings), 
				into(blobs), 
				into(ints), 
				into(floats),
				into(dateTimes),
				into(bools),
				now; 
		} catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
		catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }
		
		std::string number = Poco::NumberFormatter::format(size - 1);
		assert (size == ints.size());
		assert (0 == ints.front());
		assert (size - 1 == ints.back());
		assert (std::string("xyz0") == strings.front());
		assert (std::string("xyz") + number == strings.back());
		assert (CLOB("abc0") == blobs.front());
		CLOB blob("abc");
		blob.appendRaw(number.c_str(), number.size());
		assert (blob == blobs.back());
		assert (.5 == floats.front());
		assert (floats.size() - 1 + .5 == floats.back());
		assert (bools.front());
		assert (((0 == ((bools.size() - 1) % 2)) == bools.back()));

		ints.clear();

		try 
		{ 
			session() << "SELECT First FROM "<< ExecUtil::misctest(), into(ints, bulk(size)), limit(size+1), now; 
			fail ("must fail");
		}
		catch(Poco::InvalidArgumentException&){ }

		try 
		{ 
			session() << "SELECT First FROM "<< ExecUtil::misctest(), into(ints), bulk(size), now; 
			fail ("must fail");
		}
		catch(Poco::InvalidAccessException&){ }

		ints.clear();
		strings.clear();
		strings.resize(size);
		blobs.clear();
		floats.clear();
		floats.resize(size);
		dateTimes.clear();
		bools.clear();
		bools.resize(size);
		
		try 
		{ 
			session() << "SELECT First, Second, Third, Fourth, Fifth, Sixth FROM "<< ExecUtil::misctest() <<" ORDER BY Third", 
				into(strings, bulk),
				into(blobs, bulk(size)),
				into(ints, bulk(size)),
				into(floats, bulk),
				into(dateTimes, bulk(size)),
				into(bools, bulk),
				now; 
		} catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
		catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }

		assert (size == ints.size());
		assert (0 == ints.front());
		assert (size - 1 == ints.back());
		assert (std::string("xyz0") == strings.front());
		assert (std::string("xyz") + number == strings.back());
		assert (CLOB("abc0") == blobs.front());
		blob.assignRaw("abc", 3);
		blob.appendRaw(number.c_str(), number.size());
		assert (blob == blobs.back());
		assert (.5 == floats.front());
		assert (floats.size() - 1 + .5 == floats.back());
		assert (bools.front());
		assert (((0 == ((bools.size() - 1) % 2)) == bools.back()));
	}

	void doBulkPerformance(Poco::UInt32 size);

	template <typename C1, typename C2, typename C3, typename C4, typename C5>
	void doBulk(Poco::UInt32 size)
	{
		poco_data_using_statements;

		std::string funct = "doBulk()";
		C1 ints;
		C2 strings;
		C3 blobs;
		C4 floats;
		C5 dateTimes(size);
	
		for (int i = 0; i < size; ++i)
		{
			ints.push_back(i);
			strings.push_back(std::string("xyz" + Poco::NumberFormatter::format(i)));
			blobs.push_back(std::string("abc") + Poco::NumberFormatter::format(i));
			floats.push_back(i + .5);
		}

		try 
		{
			session() << "INSERT INTO "<< ExecUtil::misctest() <<" VALUES (?,?,?,?,?)", 
				use(strings), 
				use(blobs), 
				use(ints),
				use(floats),
				use(dateTimes), now;
		} catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
		catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }

		try { session() << "DELETE FROM "<< ExecUtil::misctest(), now; }
		catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
		catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }

		try 
		{
			session() << "INSERT INTO "<< ExecUtil::misctest() <<" VALUES (?,?,?,?,?)", 
				use(strings, bulk), 
				use(blobs, bulk), 
				use(ints, bulk),
				use(floats, bulk),
				use(dateTimes, bulk), now;
		} catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
		catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }
		
		ints.clear();
		strings.clear();
		blobs.clear();
		floats.clear();
		dateTimes.clear();

		try 
		{ 
			session() << "SELECT * FROM "<< ExecUtil::misctest() <<" ORDER BY First", 
				into(strings), 
				into(blobs), 
				into(ints), 
				into(floats),
				into(dateTimes),
				now; 
		} catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
		catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }
		
		std::string number = Poco::NumberFormatter::format(size - 1);
		assert (size == ints.size());
		assert (0 == ints.front());
		assert (size - 1 == ints.back());
		assert (std::string("xyz0") == strings.front());
		assert (std::string("xyz") + number == strings.back());
		assert (CLOB("abc0") == blobs.front());
		CLOB blob("abc");
		blob.appendRaw(number.c_str(), number.size());
		assert (blob == blobs.back());
		assert (.5 == floats.front());
		assert (floats.size() - 1 + .5 == floats.back());

		ints.clear();

		try 
		{ 
			session() << "SELECT First FROM "<< ExecUtil::misctest(), into(ints, bulk(size)), limit(size+1), now; 
			fail ("must fail");
		}
		catch(Poco::InvalidArgumentException&){ }

		try 
		{ 
			session() << "SELECT First FROM "<< ExecUtil::misctest(), into(ints), bulk(size), now; 
			fail ("must fail");
		}
		catch(Poco::InvalidAccessException&){ }

		ints.clear();
		strings.clear();
		blobs.clear();
		floats.clear();
		dateTimes.clear();
		
		try 
		{ 
			session() << "SELECT * FROM "<< ExecUtil::misctest() <<" ORDER BY First", 
				into(strings, bulk(size)),
				into(blobs, bulk(size)),
				into(ints, bulk(size)),
				into(floats, bulk(size)),
				into(dateTimes, bulk(size)),
				now; 
		} catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
		catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }
		
		assert (size == ints.size());
		assert (0 == ints.front());
		assert (size - 1 == ints.back());
		assert (std::string("xyz0") == strings.front());
		assert (std::string("xyz") + number == strings.back());
		assert (CLOB("abc0") == blobs.front());
		blob.assignRaw("abc", 3);
		blob.appendRaw(number.c_str(), number.size());
		assert (blob == blobs.back());
		assert (.5 == floats.front());
		assert (floats.size() - 1 + .5 == floats.back());
	}

	void setSimple();
	void setComplex();
	void setComplexUnique();
	void multiSetSimple();
	void multiSetComplex();
	void mapComplex();
	void mapComplexUnique();
	void multiMapComplex();
	void selectIntoSingle();
	void selectIntoSingleStep();
	void selectIntoSingleFail();
	void lowerLimitOk();
	void lowerLimitFail();
	void combinedLimits();
	void combinedIllegalLimits();
	void ranges();
	void illegalRange();
	void singleSelect();
	void emptyDB();

	void assertImpl(bool condition, const std::string& conditionExpression, long lineNumber, const std::string& fileName)
	{
		assertImplementation(condition, conditionExpression, lineNumber, fileName);
	}

	void failImpl(const std::string& message, long lineNumber, const std::string& fileName)
	{
		fail(message, lineNumber, fileName);
	}


	void blob(int bigSize = 1024, const std::string& blobPlaceholder = "?");

	template <typename C1, typename C2>
	void blobContainer(int size)
	{
		poco_data_using_statements;

		std::string funct = "blobContainer()";
		C1 lastName(size, "lastname");
		C1 firstName(size, "firstname");
		C1 address(size, "Address");
		C2 img(size, CLOB("0123456789", 10));
		int count = 0;
		try { session() << "INSERT INTO " << ExecUtil::person() << " VALUES (?,?,?,?)", use(lastName), use(firstName), use(address), use(img), now; }
		catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
		catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }
		try { session() << "SELECT COUNT(*) FROM " << ExecUtil::person(), into(count), now; }
		catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
		catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }
		assert (count == size);

		C2 res;
		try { session() << "SELECT Image FROM " << ExecUtil::person(), into(res), now; }
		catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
		catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }
		assert (res.size() == img.size());
		assert (res == img);
	}

	void blobStmt();

	void dateTime();
	void date();
	void time();
	void floats();
	void doubles();
	void tuples();
	void tupleVector();

	template <typename IntType =
#ifdef POCO_64_BIT
		Poco::Int64 IntType
#else
		Poco::Int32 IntType
#endif
	>
	void internalExtraction(IntType)
	{
		using Poco::Data::RecordSet;
		using Poco::Data::Column;
		using Poco::UTF16String;
		using Poco::BadCastException;
		using Poco::RangeException;

		std::string funct = "internalExtraction()";
		std::vector<Tuple<int, double, std::string> > v;
		v.push_back(Tuple<int, double, std::string>(1, 1.5, "3"));
		v.push_back(Tuple<int, double, std::string>(2, 2.5, "4"));
		v.push_back(Tuple<int, double, std::string>(3, 3.5, "5"));
		v.push_back(Tuple<int, double, std::string>(4, 4.5, "6"));

		try { session() << "INSERT INTO " << ExecUtil::vectors() << " VALUES (?,?,?)", use(v), now; }
		catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
		catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }

		try
		{
			Statement stmt = (session() << "SELECT * FROM " << ExecUtil::vectors() , now);
			RecordSet rset(stmt);

			assert (3 == rset.columnCount());
			assert (4 == rset.rowCount());

			int curVal = 3;
			do
			{
				assert (rset["str0"] == curVal);
				++curVal;
			} while (rset.moveNext());

			rset.moveFirst();
			assert (rset["str0"] == "3");
			rset.moveLast();
			assert(rset["str0"] == "6");

			RecordSet rset2(rset);
			assert (3 == rset2.columnCount());
			assert (4 == rset2.rowCount());

			IntType i;
			try {
				i = rset.value<IntType>(0, 0);
				assert(1 == i);
			}
			catch (Poco::BadCastException& ex)
			{
				std::cout << ex.displayText() << std::endl;
			}
			std::string s = rset.value(0,0).convert<std::string>();
			assert ("1" == s);

			IntType a = rset.value<IntType>(0,2);
			assert (3 == a);

			try
			{
				double d = rset.value<double>(1,1);
				assert (2.5 == d);
			}
			catch (BadCastException&)
			{
				float f = rset.value<float>(1,1);
				assert (2.5 == f);
			}

			try
			{
				s = rset.value<std::string>(2, 2);
			}
			catch (BadCastException&)
			{
				UTF16String us = rset.value<Poco::UTF16String>(2, 2);
				Poco::UnicodeConverter::convert(us, s);
			}
			assert("5" == s);

			i = rset.value("str0", 2);
			assert (5 == i);

			const Column<std::deque<IntType> >& col = rset.column<std::deque<IntType> >(0);
			Column<std::deque<IntType> >::Iterator it = col.begin();
			Column<std::deque<IntType> >::Iterator end = col.end();
			for (int i = 1; it != end; ++it, ++i)
				assert (*it == i);

			rset = (session() << "SELECT COUNT(*) AS cnt FROM " << ExecUtil::vectors(), now);

			//various results for COUNT(*) are received from different drivers
			try
			{
				//this is what most drivers will return
				int i = rset.value<int>(0,0);
				assert (4 == i);
			}
			catch(BadCastException&)
			{
				try
				{
					//this is for Oracle
					double i = rset.value<double>(0,0);
					assert (4 == int(i));
				}
				catch(BadCastException&)
				{
					//this is for PostgreSQL
					Poco::Int64 big = rset.value<Poco::Int64>(0,0);
					assert (4 == big);
				}
			}

			s = rset.value("cnt", 0).convert<std::string>();
			assert ("4" == s);

			try { rset.column<std::deque<IntType> >(100); fail ("must fail"); }
			catch (RangeException&) { }

			try { rset.value<std::string>(0,0); fail ("must fail"); }
			catch (BadCastException&) {	}

			stmt = (session() << "DELETE FROM " << ExecUtil::vectors(), now);
			rset = stmt;

			try { rset.column<std::deque<IntType> >(0); fail ("must fail"); }
			catch (RangeException&) { }
		}
		catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
		catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }
	}

	void filter(const std::string& query = 
		"SELECT * FROM " + ExecUtil::vectors() + " ORDER BY int0 ASC",
		const std::string& intFldName = "int0");

	void internalBulkExtraction();
	void internalBulkExtractionUTF16();

	void internalStorageType();
	void nulls(bool emptyStrIsSpace = false);
	void notNulls(const std::string& sqlState = "23502");
	void rowIterator();
	void stdVectorBool();

	void asynchronous(int rowCount = 500);
	
	void any();
	void dynamicAny();

	void multipleResults(const std::string& sql = 
		"SELECT * FROM " + ExecUtil::person() + " WHERE Age = ?; "
		"SELECT Age FROM " + ExecUtil::person() +" WHERE FirstName = 'Bart'; "
		"SELECT * FROM " + ExecUtil::person() + " WHERE Age = ? OR Age = ? ORDER BY Age;");

	void multipleResultsNoProj(const std::string& sql);

	void sqlChannel(const std::string& connect);
	void sqlLogger(const std::string& connect);

	void sessionTransaction(const std::string& connect);
	void transaction(const std::string& connect);
	void transactor();
	void nullable();

	void unicode(const std::string& dbConnString);

	void reconnect();

private:
	static const std::string MULTI_INSERT;
	static const std::string MULTI_SELECT;

	void setTransactionIsolation(Poco::Data::Session& session, Poco::UInt32 ti);
	std::string schemaTable(const std::string& tblName) const;

	Poco::Data::Session& session();
	Poco::Data::Session* _pSession;
	std::string _connInitSql;
	std::string _schemaName;
};


inline Poco::Data::Session& SQLExecutor::session()
{
	poco_check_ptr (_pSession);
	return *_pSession;
}


inline std::string SQLExecutor::schemaTable(const std::string& tblName) const
{
	return _schemaName.empty() ? tblName : _schemaName + "." + tblName;
}


#endif // SQLExecutor_INCLUDED
