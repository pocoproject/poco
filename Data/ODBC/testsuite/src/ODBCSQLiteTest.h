//
// ODBCSQLiteTest.h
//
// Definition of the ODBCSQLiteTest class.
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef ODBCSQLiteTest_INCLUDED
#define ODBCSQLiteTest_INCLUDED


#include "Poco/Data/ODBC/ODBC.h"
#include "Poco/Data/LOB.h"
#include "ODBCTest.h"
#include <string>
#include <vector>


class ODBCSQLiteTest: public ODBCTest
	/// SQLite3 ODBC test class
	/// Tested:
	///
	/// Driver		|	DB			| OS
	/// ------------+---------------+------------------------------------------
	///	00.70.00.00	| SQLite 3.*	| MS Windows XP Professional x64 v.2003/SP1
{
public:
	ODBCSQLiteTest(const std::string& name);
	~ODBCSQLiteTest() override;

	void testBareboneODBC() override;
	void testAffectedRows() override;
	void testNull() override;
	void testBoundLOBCapacity();
	void testBoundLOBBulkCapacity();
	void testBoundLOBBulkNull();
	void testBoundLOBBulkOverflow();
	void testBoundBLOBBulk();
	void testBoundStringBulk();
	void testBoundStringBulkOverflow();
	void testBulkThenScalarLOB();
	void testAnyUntypedColumn();

	static CppUnit::Test* suite();

private:
	std::size_t probeColumnLength(const std::string& query, bool character);
		/// Returns RecordSet::columnLength(0) for query, probed on a statement
		/// without into(). Asserts that column 0 is FDT_BLOB (character false) or
		/// FDT_STRING/FDT_WSTRING (character true) and that 1 < length < 1024,
		/// the default maxFieldSize. Call it while the table is empty.

	template <typename L>
	void checkBoundLOB(const std::string& query, const L& expected, std::size_t capacity);
		/// Selects the single row of query into a scalar L; expects expected if it
		/// fits into capacity bytes, the bound buffer DataException otherwise.

	template <typename C>
	void checkBoundLOBBulk(const std::string& query, const std::vector<Poco::Data::CLOB>& expected);
		/// Selects all rows of query with bulk(expected.size()) into a container C
		/// of LOBs pre-filled with a sentinel; expects the bytes of expected, empty
		/// for NULL rows, and the sentinel unchanged.

	template <typename C>
	void checkBoundLOBBulkRejected(const std::string& query, Poco::UInt32 rows, std::size_t capacity);
		/// Selects all rows of query with bulk(rows) into a container C of LOBs;
		/// expects the bound buffer DataException for a value of capacity + 1 bytes.

	void dropObject(const std::string& type, const std::string& name) override;
	void recreateNullableTable() override;
	void recreatePersonTable() override;
	void recreatePersonBLOBTable() override;
	void recreatePersonDateTimeTable() override;
	void recreateStringsTable() override;
	void recreateIntsTable() override;
	void recreateFloatsTable() override;
	void recreateTuplesTable() override;
	void recreateVectorsTable() override;
	void recreateAnysTable() override;
	void recreateNullsTable(const std::string& notNull = "") override;
	void recreateMiscTable() override;
	void recreateLogTable() override;

	static ODBCTest::SessionPtr _pSession;
	static ODBCTest::ExecPtr    _pExecutor;
	static std::string          _driver;
	static std::string          _dsn;
	static std::string          _uid;
	static std::string          _pwd;
	static std::string          _connectString;
};


#endif // ODBCSQLiteTest_INCLUDED
