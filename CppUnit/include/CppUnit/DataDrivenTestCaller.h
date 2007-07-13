//
// DataDrivenTestCaller.h
//
// $Id: //poco/Main/CppUnit/include/CppUnit/DataDrivenTestCaller.h#3 $
//


#ifndef CppUnit_DataDrivenTestCaller_INCLUDED
#define CppUnit_DataDrivenTestCaller_INCLUDED


#include "CppUnit/CppUnit.h"
#include "CppUnit/TestCase.h"
#include "CppUnit/TestSuite.h"
#include "CppUnit/CppUnitException.h"
#include <iostream>
#include <fstream>
#include "Poco/File.h"
#include "Poco/Path.h"
#include "Poco/Exception.h"
#include <algorithm>
#include <functional>
#include <map>


namespace CppUnit {

/*
 * A data driven test case defines the fixture to run a single test with varying input streams. 
 * A data driven test is assumed to take an input stream and write its result to an output stream
 * which can then be compared to a reference output.
 * To do data driven testing
 * 1) implement a subclass of TestCase
 * 2) define instance variables that store the state of the fixture
 * 3) initialize the fixture state by overriding setUp
 * 4) clean-up after a test by overriding tearDown.
 *
 * Each test runs in its own fixture so there
 * can be no side effects among test runs.
 *
 * Each test method must implement the following method signature:
 * 
 *     void dotest(std::istream& in, std::ostream& out)
 *
 * Add the test in the suite() method of your test case with the CppUnit_addDataTest macro:
 *     
 *     CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("WebWidgetsTest");
 *     CppUnit_addDataTest(pSuite, WebWidgetsTest, doTest, "tests/", "temp/");
 *
 * CppUnit_addDataTest expects 5 input parameters:
 *  - the first is always a pointer to the suite where the tests are added
 *  - the second parameter is the name of your test class
 *  - the third parameter is the test method
 *  - the fourth parameter contains the directory where the tests are stored. Note that
 *  we assume a certain directory structure inside the tests folder. 
 *  Input files are stored in the subfolder tests/data, the reference results are stored
 *  in the subfolder tests/results. The matching of reference to input data happens automatically via the file
 *  base name. For each input a separate test is inserted with the base name as the name of the test.
 *  - the last parameter defines where we should generate temporary files
 * 
 * Note that the test framework guarantees that input and output stream for the test
 * method are valid and open. It will also perform the comparision (i.e. does the produced output matches
 * the reference output) and close the input and outout streams.
 *
 */
template <class Fixture>
class DataDrivenTestCaller: public TestCase
{
	REFERENCEOBJECT (DataDrivenTestCaller)

	typedef void (Fixture::*TestMethod)(std::istream& in, std::ostream& out);

public:
	DataDrivenTestCaller(const std::string& name, TestMethod test, const Poco::File& in, const Poco::File& out, const Poco::File& expectedResult):
		TestCase(name), 
		_test(test),
		_fixture(new Fixture(name)),
		_in(in),
		_out(out),
		_result(expectedResult)
	{
	}

protected:
	void runTest()
	{
		std::ifstream in(_in.path().c_str(), std::ios::binary);
		if (!in)
			throw Poco::FileNotFoundException("Failed to open file " + _in.path());

		std::ofstream out(_out.path().c_str(), std::ios::binary);
		if (!out)
			throw Poco::CreateFileException("Failed to open file for writing " + _out.path());
		try
		{
			(_fixture.get()->*_test)(in, out);
		}
		catch(Poco::Exception&)
		{
			// maybe an exception is wanted
			// compare with ref file
			if (_result.exists() && _result.getSize() > 0)
			{
				in.close();
				out.close();
				throw;
			}
		}
		in.close();
		out.close();
		// now compare 
		std::ifstream inRes(_out.path().c_str(), std::ios::binary);
		std::ifstream refRes(_result.path().c_str(), std::ios::binary);
		if (!inRes || !refRes)
			throw Poco::FileNotFoundException("Can't compare with reference file. Reference result missing or file not created " + _out.path());
		while (!inRes.eof())
		{
			int c = inRes.get();
			int c2 = refRes.get();
			if (c != c2)
				throw Poco::DataException("Created file does not match reference file" + _out.path());
		}
	}

	void setUp()
	{
		_fixture.get()->setUp();
	}

	void tearDown()
	{
		_fixture.get()->tearDown();
	}

public:
	static void addDirectoryTest(TestSuite* pSuite, typename DataDrivenTestCaller::TestMethod method, const std::string& rootDir, const Poco::Path& tempD)
		/// rootDir contains the name of the root directory (not a path). We will start searching for the root dir in the
		/// current directory and go up until we are at the root level
		/// The root dir must contain a subfolder /data and /results. tempDir will be used to store temporary files
	{
		Poco::Path dir(rootDir);
		std::string cwd(Poco::Path::current());
		Poco::Path cwdPath(cwd);
		Poco::Path p(cwdPath, dir);
		p.makeFile();
		bool found = false;
		while (!found && p.depth() > 0)
		{
			Poco::File f(p);
			if (f.exists())
				found = true;
			else
				p.popDirectory();
		}
		if (!found)
			return;

		Poco::Path data(p, "data");
		data.makeDirectory();
		Poco::File dataDir(data);
		if (!dataDir.exists())
			return;
		Poco::Path results(p, "results");
		results.makeDirectory();
		Poco::File resDir(results);
		if (!resDir.exists())
			return;
		Poco::Path temp(tempD);
		Poco::File tempDir(temp);
		temp.makeDirectory();
		tempDir.createDirectories();
		std::vector<std::string> inputFiles;
		dataDir.list(inputFiles);
		std::vector<std::string> referenceFiles;
		resDir.list(referenceFiles);
		std::sort(inputFiles.begin(), inputFiles.end(), std::less<std::string>());
		std::sort(referenceFiles.begin(), referenceFiles.end(), std::less<std::string>());
		typedef std::pair<Poco::File, Poco::File> InputRef;
		std::map<std::string, InputRef> pairs;
		// now map input to reference via basename
		std::vector<std::string>::const_iterator itIn = inputFiles.begin();
		std::vector<std::string>::const_iterator itInEnd = inputFiles.end();
		for (; itIn != itInEnd; ++itIn)
		{
			std::cout << *itIn << std::endl;
			Poco::Path inTmp(data, *itIn);
			Poco::File inFTmp(inTmp);
			if (!inTmp.getBaseName().empty())
				pairs.insert(std::make_pair(inTmp.getBaseName(), std::make_pair(inFTmp, Poco::File())));
		}

		std::vector<std::string>::const_iterator itRef = referenceFiles.begin();
		std::vector<std::string>::const_iterator itRefEnd = referenceFiles.end();
		for (; itRef != itRefEnd; ++itRef)
		{
			Poco::Path refTmp(results, *itRef);
			Poco::File refFTmp(refTmp);
			std::map<std::string, InputRef>::iterator it = pairs.find(refTmp.getBaseName());
			if (it != pairs.end())
				it->second.second = refFTmp;
		}

		std::map<std::string, InputRef>::const_iterator itP = pairs.begin();
		std::map<std::string, InputRef>::const_iterator itPEnd = pairs.end();
		for (; itP != itPEnd; ++itP)
		{
			std::string fileName;
			
			if (!itP->second.second.path().empty())
			{
				Poco::Path refTmp(itP->second.second.path());
				fileName = refTmp.getFileName();
			}
			else
				fileName = itP->first + ".tmp";
			Poco::Path outTmp(temp, fileName);
			Poco::File outFTmp(outTmp);
			pSuite->addTest(new DataDrivenTestCaller(itP->first, method, itP->second.first, outFTmp, itP->second.second));
		}
	}
private:
	TestMethod             _test;
	std::auto_ptr<Fixture> _fixture;
	Poco::File             _in;
	Poco::File             _out;
	Poco::File             _result;
};


} // namespace CppUnit


#define CppUnit_addDataTest(suite, cls, mth, rootDir, tempDir) \
	CppUnit::DataDrivenTestCaller<cls>::addDirectoryTest(suite, &cls::mth, rootDir, tempDir)

#endif // DataDrivenTestCaller_INCLUDED
