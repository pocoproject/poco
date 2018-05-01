//
// SQLExecutor.h
//
// Definition of the SQLExecutor class.
//
// Copyright (c) 2008, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef SQLExecutor_INCLUDED
#define SQLExecutor_INCLUDED


#include "Poco/SQL/MySQL/MySQL.h"
#include "Poco/SQL/Session.h"


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

	void bareboneMySQLTest(const std::string& host, const std::string& user, const std::string& pwd, const std::string& db, const std::string& port, const char* tableCreateString);
		/// This function uses "bare bone" MySQL API calls (i.e. calls are not
		/// "wrapped" in PocoData framework structures).
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

	void blob(unsigned int bigSize = ~0);
	void blobStmt();
	void longText();
	void dateTime();
	void date();
	void time();
	void unsignedInts();
	void floats();
	void doubles();
	void any();
	void dynamicAny();
	void tuples();
	void tupleVector();

	void stdTuples();
	void stdTupleVector();

	void internalExtraction();
	void doNull();

	void sessionTransaction(const std::string& connect);
	void transaction(const std::string& connect);

	void reconnect();

private:
	void setTransactionIsolation(Poco::SQL::Session& session, Poco::UInt32 ti);

	Poco::SQL::Session* _pSession;
};


#endif // SQLExecutor_INCLUDED
