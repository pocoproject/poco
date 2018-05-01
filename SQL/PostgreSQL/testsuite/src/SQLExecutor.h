//
// SQLExecutor.h
//
// Copyright (c) 2015, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef SQLExecutor_PostgreSQL_INCLUDED
#define SQLExecutor_PostgreSQL_INCLUDED


#include "Poco/SQL/PostgreSQL/PostgreSQL.h"
#include "Poco/SQL/Session.h"

#include <libpq-fe.h>

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

	SQLExecutor(const std::string& name, Poco::SQL::Session* _pSession);
	~SQLExecutor();

	void oidPostgreSQLTest(std::string host, std::string user, std::string pwd, std::string db, std::string port, const char* tableCreateString, const Oid anOIDArray[]);
		/// This function verifies the PostgreSQL Column type OIDs are consistent between releases
	
	void barebonePostgreSQLTest(std::string host, std::string user, std::string pwd, std::string db, std::string port, const char* tableCreateString);
		/// This function uses "bare bone"  API calls (i.e. calls are not
		/// "wrapped" in PocoSQL framework structures).
		/// The purpose of the function is to verify that driver behaves
		/// correctly. If this test passes, subsequent tests failures are likely ours.

	void simpleAccess();
	void complexType();
	void simpleAccessVector();
	void complexTypeVector();
	void insertVector();
	void insertEmptyVector();

	void insertSingleBulk();
	void insertSingleBulkVec();

	void limits();
	void limitOnce();
	void limitPrepare();
	void limitZero();
	void prepare();

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

	void blob(unsigned int bigSize = 1024000); // size value will have to go into an int in the end
	void clobStmt();
	void blobStmt();
	void dateTime();
	void date();
	void time();
	void unsignedInts();
	void floats();
	void doubles();
	void tuples();
	void tupleVector();

	void internalExtraction();
	void doNull();

	void sessionTransaction(const std::string& connect);
	void transaction(const std::string& connect);

	void reconnect();

private:
	void setTransactionIsolation(Poco::SQL::Session& session, Poco::UInt32 ti);

	Poco::SQL::Session* _pSession;
};


#endif // SQLExecutor_PostgreSQL_INCLUDED
