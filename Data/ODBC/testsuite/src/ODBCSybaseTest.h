#ifndef ODBCSybaseTest_INCLUDED
#define ODBCSybaseTest_INCLUDED


#include "Poco/Data/ODBC/ODBC.h"
#include "ODBCTest.h"

class SybaseODBC : public ODBCTest {
public:
    SybaseODBC(const std::string& name);
    static CppUnit::Test* suite();

    virtual void testBareboneODBC();
    virtual void testBulkPerformance();
private:
    void dropObject(const std::string& type, const std::string& tableName);
    void recreateNullableTable();
    void recreatePersonTable();
    void recreatePersonBLOBTable();
    void recreatePersonDateTable();
    void recreatePersonTimeTable();
    void recreatePersonDateTimeTable();
    void recreateStringsTable();
    void recreateIntsTable();
    void recreateFloatsTable();
    void recreateTuplesTable();
    void recreateVectorsTable();
    void recreateAnysTable();
    void recreateNullsTable(const std::string& notNull = "");
    void recreateMiscTable();
    void recreateLogTable();
    void testStoredProcedure();
    void testStoredProcedureDynamicAny();
    void testStoredProcedureAny();
    void testTransaction();

    void doMiscTable(bool haveSecCol);
    void doPersonTable(const std::string& lnAttr = "");

    static ODBCTest::SessionPtr  _pSession;
    static ODBCTest::ExecPtr     _pExecutor;
    static std::string _driver;
    static std::string _dsn;
    static std::string _uid;
    static std::string _pwd;
    static std::string _connectString;
};


#endif
