//
// TestResult.cpp
//


#if defined(__clang__) || defined (__GNUC__)
	#define HAVE_CXXABI_H
#endif


#include "CppUnit/TestResult.h"
#ifdef HAVE_CXXABI_H
#include <cxxabi.h>
#include <cstdlib>
#endif


namespace CppUnit {


// Destroys a test result
TestResult::~TestResult()
{
	std::vector<TestFailure*>::iterator it;

	for (it = _errors.begin(); it != _errors.end(); ++it)
		delete *it;

	for (it = _failures.begin(); it != _failures.end(); ++it)
		delete *it;

	delete _syncObject;
}


std::string TestResult::demangle(const char* typeName)
{
	std::string result;
#ifdef HAVE_CXXABI_H
	int status;
	char* demangled = abi::__cxa_demangle(typeName, nullptr, nullptr, &status);
	if (demangled)
	{
		result = demangled;
		std::free(demangled);
	}
	else
	{
		result = typeName;
	}
#else
	result = typeName;
#endif
	return result;
}


} // namespace CppUnit
