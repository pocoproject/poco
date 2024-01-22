//
// SQLExecutor.h
//
// Definition of the SQLExecutor class.
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.,
// Aleph ONE Software Engineering d.o.o., and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef DataTest_SQLExecutor_INCLUDED
#define DataTest_SQLExecutor_INCLUDED


#include "Poco/Data/Test/DataTest.h"
#include "Poco/Data/Session.h"
#include "Poco/Data/BulkExtraction.h"
#include "Poco/Data/BulkBinding.h"
#include "Poco/NumberFormatter.h"
#include "Poco/String.h"
#include "Poco/Exception.h"
#include <iostream>
#include <string_view>


namespace Poco {
namespace Data {
namespace Test {


#define poco_data_using_keywords using Poco::Data::Keywords::now; \
		using Poco::Data::Keywords::into; \
		using Poco::Data::Keywords::use; \
		using Poco::Data::Keywords::bulk; \
		using Poco::Data::Keywords::limit; \
		using Poco::Data::DataException; \
		using Poco::Data::CLOB;


class DataTest_API SQLExecutor: public CppUnit::TestCase
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

	SQLExecutor(const std::string& name, Poco::Data::Session* pSession, Poco::Data::Session* pEncSession = nullptr, bool numberedPlaceHolders = false);
	~SQLExecutor();

	template <typename C>
	void connection(C& c, const std::string& connectString)
	{
		assertFalse (c.isConnected());
		// some drivers/DBMS do not distinguish, so we only test same values here
		assertTrue (c.connect(connectString, 10, 10));
		assertTrue (c.isConnected());
		try
		{
			assertTrue (c.getTimeout() == 10);
		}
		catch(const NotSupportedException&)
		{
			std::cout << "Getting session timeout not supported." << '\n';
		}

		try
		{
			assertTrue (c.getLoginTimeout() == 10);
		}
		catch(const NotSupportedException&)
		{
			std::cout << "Getting login timeout not supported." << '\n';
		}

		try
		{
			c.setTimeout(6);
		}
		catch(const NotSupportedException&)
		{
			std::cout << "Setting session timeout not supported." << '\n';
		}

		try
		{
			assertTrue (c.getTimeout() == 6);
		}
		catch(const NotSupportedException&)
		{
			std::cout << "Getting session timeout not supported." << '\n';
		}

		try
		{
			c.setLoginTimeout(11);
		}
		catch(const InvalidAccessException&)
		{
			std::cout << "Setting login timeout not supported." << '\n';
		}

		try
		{
			assertTrue (c.getLoginTimeout() == 11);
		}
		catch(const NotSupportedException&)
		{
			std::cout << "Getting login timeout not supported." << '\n';
		}

		assertTrue (c.disconnect());
		assertFalse (c.isConnected());
		assertTrue (c.connect(connectString));
		assertTrue (c.isConnected());
		try
		{
			c.connect();
			fail ("Connection attempt must fail when already connected");
		}
		catch(const Poco::InvalidAccessException&){}
		assertTrue (c.disconnect());
		assertFalse (c.isConnected());
		try
		{
			c.connect();
			fail ("Connection attempt with empty string must fail");
		}
		catch(const Poco::InvalidArgumentException&){}
	}

	void session(const std::string& connector, const std::string& connectString, int timeout);
	void sessionPool(const std::string& connector, const std::string& connectString, int minSessions, int maxSessions, int idleTime, int timeout);

	void execute(const std::string& sql);
		/// Execute a query.

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

	void doBulkPerformance(Poco::UInt32 size);

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
		poco_data_using_keywords;

		std::string funct = "blobContainer()";
		C1 lastName(size, "lastname");
		C1 firstName(size, "firstname");
		C1 address(size, "Address");
		C2 img(size, CLOB("0123456789", 10));
		int count = 0;
		try { session() << "INSERT INTO Person VALUES (?,?,?,?)", use(lastName), use(firstName), use(address), use(img), now; }
		catch(DataException& ce){ std::cout << ce.displayText() << std::endl; fail (funct, __LINE__, __FILE__); }
		try { session() << "SELECT COUNT(*) FROM Person", into(count), now; }
		catch(DataException& ce){ std::cout << ce.displayText() << std::endl; fail (funct, __LINE__, __FILE__); }
		assert (count == size);

		C2 res;
		try { session() << "SELECT Image FROM Person", into(res), now; }
		catch(DataException& ce){ std::cout << ce.displayText() << std::endl; fail (funct, __LINE__, __FILE__); }
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
	void rowIterator();
	void stdVectorBool();

	void asynchronous(int rowCount = 500);

	void any();
	void dynamicAny();

	void multipleResults(const std::string& sql =
		"SELECT * FROM Person WHERE Age = ?; "
		"SELECT Age FROM Person WHERE FirstName = 'Bart'; "
		"SELECT * FROM Person WHERE Age = ? OR Age = ? ORDER BY Age;");

	void sqlChannel(const std::string& connector, const std::string& connect);
	void sqlLogger(const std::string& connector, const std::string& connect);

	void autoCommit();
	void transactionIsolation();

	void sessionTransaction(const std::string& connector, const std::string& connect);
	void sessionTransactionNoAutoCommit(const std::string& connector, const std::string& connect);
	void transaction(const std::string& connector, const std::string& connect, bool readUncommitted = true);
	void transactor();
	void nullable();

	void unicode(const std::string& dbConnString);
	void encoding(const std::string& dbConnString);

	void reconnect();

	Poco::Data::Session& session(bool enc = false);
	void setTransactionIsolation(Poco::Data::Session& session, Poco::UInt32 ti);

private:
	static const std::string MULTI_INSERT;
	static const std::string MULTI_SELECT;

	Poco::Data::Session* _pSession;
	Poco::Data::Session* _pEncSession;
	bool _numberedPlaceHolders = false;

	std::string formatSQL(const std::string& s) const;
};


} } } // Poco::Data::Test


#endif // DataTest_SQLExecutor_INCLUDED
