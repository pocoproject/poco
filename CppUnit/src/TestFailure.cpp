//
// TestFailure.cpp
//
// $Id: //poco/1.1.0/CppUnit/src/TestFailure.cpp#1 $
//


#include "CppUnit/TestFailure.h"
#include "CppUnit/Test.h"


CppUnit_BEGIN


// Returns a short description of the failure.
std::string TestFailure::toString()
{
	return _failedTest->toString () + ": " + _thrownException->what();
}


CppUnit_END
