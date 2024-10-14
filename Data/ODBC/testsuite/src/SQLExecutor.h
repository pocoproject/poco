//
// SQLExecutor.h
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
#include "Poco/Data/Test/SQLExecutor.h"
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
		std::cout << se.displayText() << std::endl; \
	} \
	assert (SQL_SUCCEEDED(r))


#define poco_odbc_check_desc(r, h) \
	if (!SQL_SUCCEEDED(r))	\
	{ \
		Poco::Data::ODBC::DescriptorException de(h); \
		std::cout << de.displayText() << std::endl; \
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

	SQLExecutor(const std::string& name, Poco::Data::Session* pSession, Poco::Data::Session* pEncSession = 0);
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
		const std::string& select = MULTI_SELECT,
		const std::string& procCreateString = "");

	void bareboneODBCStoredFuncTest(const std::string& dbConnString,
		const std::string& tableCreateString,
		const std::string& procExecuteString,
		SQLExecutor::DataBinding bindMode,
		SQLExecutor::DataExtraction extractMode);
		/// The above three functions use "bare bone" ODBC API calls
		/// (i.e. calls are not "wrapped" in PocoData framework structures).
		/// The purpose of the functions is to verify that a driver behaves
		/// correctly as well as to determine its capabilities
		/// (e.g. SQLGetData() restrictions relaxation policy, if any).
		/// If these test pass, subsequent tests failures are likely ours.

	void connection(const std::string& connectString);
	void session(const std::string& connectString, int timeout);
	void sessionPool(const std::string& connectString,
		int minSessions, int maxSessions, int idleTime, int timeout);

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
				Poco::format("INSERT INTO MiscTest VALUES (?,%s,?,?,?,?)", blobPlaceholder),
				use(strings),
				use(blobs),
				use(ints),
				use(floats),
				use(dateTimes),
				use(bools), now;
		} catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
		catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }

		try { session() << "DELETE FROM MiscTest", now; }
		catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
		catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }

		try
		{
			session() <<
				Poco::format("INSERT INTO MiscTest VALUES (?,%s,?,?,?,?)", blobPlaceholder),
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
			session() << "SELECT * FROM MiscTest ORDER BY Third",
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
			session() << "SELECT First FROM MiscTest", into(ints, bulk(size)), limit(size+1), now;
			fail ("must fail");
		}
		catch(Poco::InvalidArgumentException&){ }

		try
		{
			session() << "SELECT First FROM MiscTest", into(ints), bulk(size), now;
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
			session() << "SELECT First, Second, Third, Fourth, Fifth, Sixth FROM MiscTest ORDER BY Third",
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
			session() << "INSERT INTO MiscTest VALUES (?,?,?,?,?)",
				use(strings),
				use(blobs),
				use(ints),
				use(floats),
				use(dateTimes), now;
		} catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
		catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }

		try { session() << "DELETE FROM MiscTest", now; }
		catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
		catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }

		try
		{
			session() << "INSERT INTO MiscTest VALUES (?,?,?,?,?)",
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
			session() << "SELECT * FROM MiscTest ORDER BY First",
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
			session() << "SELECT First FROM MiscTest", into(ints, bulk(size)), limit(size+1), now;
			fail ("must fail");
		}
		catch(Poco::InvalidArgumentException&){ }

		try
		{
			session() << "SELECT First FROM MiscTest", into(ints), bulk(size), now;
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
			session() << "SELECT * FROM MiscTest ORDER BY First",
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
		try { session() << "INSERT INTO Person VALUES (?,?,?,?)", use(lastName), use(firstName), use(address), use(img), now; }
		catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
		catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }
		try { session() << "SELECT COUNT(*) FROM Person", into(count), now; }
		catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
		catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }
		assert (count == size);

		C2 res;
		try { session() << "SELECT Image FROM Person", into(res), now; }
		catch(ConnectionException& ce){ std::cout << ce.toString() << std::endl; fail (funct); }
		catch(StatementException& se){ std::cout << se.toString() << std::endl; fail (funct); }
		assert (res.size() == img.size());
		assert (res == img);
	}

	void blobStmt();
	void recordSet();

	void dateTime();
	void date();
	void time();
	void floats();
	void doubles();
	void uuids();
	void tuples();
	void tupleVector();

	void internalExtraction();
	void filter(const std::string& query =
		"SELECT * FROM Vectors ORDER BY int0 ASC",
		const std::string& intFldName = "int0");

	void internalBulkExtraction();
	void internalBulkExtractionUTF16();
	void internalStorageType();
	void nulls();
	void notNulls(const std::vector<std::string>& sqlStates = {std::string("23502")});
	void rowIterator();
	void stdVectorBool();

	void asynchronous(int rowCount = 500);

	void any();
	void dynamicAny();

	void multipleResults(const std::string& sql =
		"SELECT * FROM Person WHERE Age = ?; "
		"SELECT Age FROM Person WHERE FirstName = 'Bart'; "
		"SELECT * FROM Person WHERE Age = ? OR Age = ? ORDER BY Age;");

	void sqlChannel(const std::string& connect);
	void sqlLogger(const std::string& connect);

	void autoCommit(const std::string& connect);
	void transactionIsolation();

	void sessionTransaction(const std::string& connect);
	void sessionTransactionNoAutoCommit(const std::string& connect);
	void transaction(const std::string& connect, bool readUncommitted = true);
	void transactor();
	void nullable();

	void unicode(const std::string& dbConnString);
	void encoding(const std::string& dbConnString);

	void reconnect();

private:
	static const std::string MULTI_INSERT;
	static const std::string MULTI_SELECT;

	void setTransactionIsolation(Poco::Data::Session& session, Poco::UInt32 ti);

	Poco::Data::Session& session(bool enc = false);
	Poco::Data::Session* _pSession;
	Poco::Data::Session* _pEncSession;
	Poco::Data::Test::SQLExecutor _dataExecutor;
};


inline void SQLExecutor::sessionPool(const std::string& connectString, int minSessions, int maxSessions, int idleTime, int timeout)
{
	_dataExecutor.sessionPool("ODBC", connectString, minSessions, maxSessions, idleTime, timeout);
}


inline void SQLExecutor::zeroRows()
{
	_dataExecutor.zeroRows();
}


inline void SQLExecutor::simpleAccess()
{
	_dataExecutor.simpleAccess();
}


inline void SQLExecutor::complexType()
{
	_dataExecutor.complexType();
}


inline void SQLExecutor::complexTypeTuple()
{
	_dataExecutor.complexTypeTuple();
}


inline void SQLExecutor::simpleAccessVector()
{
	_dataExecutor.simpleAccessVector();
}


inline void SQLExecutor::complexTypeVector()
{
	_dataExecutor.complexTypeVector();
}


inline void SQLExecutor::sharedPtrComplexTypeVector()
{
	_dataExecutor.sharedPtrComplexTypeVector();
}


inline void SQLExecutor::autoPtrComplexTypeVector()
{
	_dataExecutor.autoPtrComplexTypeVector();
}


inline void SQLExecutor::insertVector()
{
	_dataExecutor.insertVector();
}


inline void SQLExecutor::insertEmptyVector()
{
	_dataExecutor.insertEmptyVector();
}


inline void SQLExecutor::simpleAccessList()
{
	_dataExecutor.simpleAccessList();
}


inline void SQLExecutor::complexTypeList()
{
	_dataExecutor.complexTypeList();
}


inline void SQLExecutor::insertList()
{
	_dataExecutor.insertList();
}


inline void SQLExecutor::insertEmptyList()
{
	_dataExecutor.insertEmptyList();
}


inline void SQLExecutor::simpleAccessDeque()
{
	_dataExecutor.simpleAccessDeque();
}


inline void SQLExecutor::complexTypeDeque()
{
	_dataExecutor.complexTypeDeque();
}


inline void SQLExecutor::insertDeque()
{
	_dataExecutor.insertDeque();
}


inline void SQLExecutor::insertEmptyDeque()
{
	_dataExecutor.insertEmptyDeque();
}


inline void SQLExecutor::affectedRows(const std::string& whereClause)
{
	_dataExecutor.affectedRows();
}


inline void SQLExecutor::insertSingleBulk()
{
	_dataExecutor.insertSingleBulk();
}


inline void SQLExecutor::floats()
{
	_dataExecutor.floats();
}


inline void SQLExecutor::doubles()
{
	_dataExecutor.doubles();
}


inline void SQLExecutor::uuids()
{
	_dataExecutor.uuids();
}


inline void SQLExecutor::insertSingleBulkVec()
{
	_dataExecutor.insertSingleBulkVec();
}


inline void SQLExecutor::limits()
{
	_dataExecutor.limits();
}


inline void SQLExecutor::limitZero()
{
	_dataExecutor.limitZero();
}


inline void SQLExecutor::limitOnce()
{
	_dataExecutor.limitOnce();
}


inline void SQLExecutor::limitPrepare()
{
	_dataExecutor.limitPrepare();
}


inline void SQLExecutor::prepare()
{
	_dataExecutor.prepare();
}


inline void SQLExecutor::doBulkPerformance(Poco::UInt32 size)
{
	_dataExecutor.doBulkPerformance(size);
}


inline void SQLExecutor::setSimple()
{
	_dataExecutor.setSimple();
}


inline void SQLExecutor::setComplex()
{
	_dataExecutor.setComplex();
}


inline void SQLExecutor::setComplexUnique()
{
	_dataExecutor.setComplexUnique();
}

inline void SQLExecutor::multiSetSimple()
{
	_dataExecutor.multiSetSimple();
}


inline void SQLExecutor::multiSetComplex()
{
	_dataExecutor.multiSetComplex();
}


inline void SQLExecutor::mapComplex()
{
	_dataExecutor.mapComplex();
}


inline void SQLExecutor::mapComplexUnique()
{
	_dataExecutor.mapComplexUnique();
}


inline void SQLExecutor::multiMapComplex()
{
	_dataExecutor.multiMapComplex();
}


inline void SQLExecutor::selectIntoSingle()
{
	_dataExecutor.selectIntoSingle();
}


inline void SQLExecutor::selectIntoSingleStep()
{
	_dataExecutor.selectIntoSingleStep();
}


inline void SQLExecutor::selectIntoSingleFail()
{
	_dataExecutor.selectIntoSingleFail();
}


inline void SQLExecutor::lowerLimitOk()
{
	_dataExecutor.lowerLimitOk();
}


inline void SQLExecutor::singleSelect()
{
	_dataExecutor.singleSelect();
}


inline void SQLExecutor::lowerLimitFail()
{
	_dataExecutor.lowerLimitFail();
}


inline void SQLExecutor::combinedLimits()
{
	_dataExecutor.combinedLimits();
}



inline void SQLExecutor::ranges()
{
	_dataExecutor.ranges();
}


inline void SQLExecutor::combinedIllegalLimits()
{
	_dataExecutor.combinedIllegalLimits();
}


inline void SQLExecutor::illegalRange()
{
	_dataExecutor.illegalRange();
}


inline void SQLExecutor::emptyDB()
{
	_dataExecutor.emptyDB();
}


inline void SQLExecutor::blob(int bigSize, const std::string& blobPlaceholder)
{
	_dataExecutor.blob(bigSize, blobPlaceholder);
}


inline void SQLExecutor::blobStmt()
{
	_dataExecutor.blobStmt();
}


inline void SQLExecutor::recordSet()
{
	_dataExecutor.recordSet();
}


inline void SQLExecutor::dateTime()
{
	_dataExecutor.dateTime();
}


inline void SQLExecutor::date()
{
	_dataExecutor.date();
}


inline void SQLExecutor::time()
{
	_dataExecutor.time();
}


inline void SQLExecutor::tuples()
{
	_dataExecutor.tuples();
}

inline void SQLExecutor::tupleVector()
{
	_dataExecutor.tupleVector();
}


inline void SQLExecutor::internalExtraction()
{
	_dataExecutor.internalExtraction();
}


inline void SQLExecutor::filter(const std::string& query, const std::string& intFldName)
{
	_dataExecutor.filter();
}


inline void SQLExecutor::internalBulkExtraction()
{
	_dataExecutor.internalBulkExtraction();
}


inline void SQLExecutor::internalBulkExtractionUTF16()
{
	_dataExecutor.internalBulkExtractionUTF16();
}


inline void SQLExecutor::internalStorageType()
{
	_dataExecutor.internalStorageType();
}


inline void SQLExecutor::nulls()
{
	_dataExecutor.nulls();
}


inline void SQLExecutor::rowIterator()
{
	_dataExecutor.rowIterator();
}


inline void SQLExecutor::stdVectorBool()
{
	_dataExecutor.stdVectorBool();
}


inline void SQLExecutor::asynchronous(int rowCount)
{
	_dataExecutor.asynchronous();
}


inline void SQLExecutor::dynamicAny()
{
	_dataExecutor.dynamicAny();
}


inline void SQLExecutor::multipleResults(const std::string& sql)
{
	_dataExecutor.multipleResults();
}


inline void SQLExecutor::sqlChannel(const std::string& connect)
{
	_dataExecutor.sqlChannel("odbc", connect);
}


inline void SQLExecutor::sqlLogger(const std::string& connect)
{
	_dataExecutor.sqlLogger("odbc", connect);
}


inline void SQLExecutor::setTransactionIsolation(Poco::Data::Session& session, Poco::UInt32 ti)
{
	_dataExecutor.setTransactionIsolation(session, ti);
}


inline void SQLExecutor::autoCommit(const std::string&)
{
	_dataExecutor.autoCommit();
}


inline void SQLExecutor::transactionIsolation()
{
	_dataExecutor.transactionIsolation();
}


inline void SQLExecutor::sessionTransaction(const std::string& connect)
{
	_dataExecutor.sessionTransaction("odbc", connect);
}


inline void SQLExecutor::sessionTransactionNoAutoCommit(const std::string& connect)
{
	_dataExecutor.sessionTransactionNoAutoCommit("odbc", connect);
}


inline void SQLExecutor::transaction(const std::string& connect, bool readUncommitted)
{
	_dataExecutor.transaction("odbc", connect, readUncommitted);
}


inline void SQLExecutor::transactor()
{
	_dataExecutor.transactor();
}


inline void SQLExecutor::nullable()
{
	_dataExecutor.nullable();
}


inline void SQLExecutor::reconnect()
{
	_dataExecutor.reconnect();
}


inline void SQLExecutor::unicode(const std::string& dbConnString)
{
	_dataExecutor.unicode(dbConnString);
}


inline void SQLExecutor::encoding(const std::string& dbConnString)
{
	_dataExecutor.encoding(dbConnString);
}


#endif // SQLExecutor_INCLUDED
