//
// CppUnitException.h
//
// $Id: //poco/1.1.0/CppUnit/include/CppUnit/CppUnitException.h#1 $
//


#ifndef CppUnit_CppUnitException_INCLUDED
#define CppUnit_CppUnitException_INCLUDED


#ifndef CppUnit_CppUnit_INCLUDED
#include "CppUnit/CppUnit.h"
#endif
#ifndef STD_EXCEPTION_INCLUDED
#include <exception>
#define STD_EXCEPTION_INCLUDED
#endif
#ifndef STD_STRING_INCLUDED
#include <string>
#define STD_STRING_INCLUDED
#endif


CppUnit_BEGIN


class CppUnit_API CppUnitException: public std::exception
	/// CppUnitException is an exception that serves
	/// descriptive strings through its what() method
{
public:
	CppUnitException(const std::string& message = "", 
	                 long lineNumber = CPPUNIT_UNKNOWNLINENUMBER, 
	                 const std::string& fileName = CPPUNIT_UNKNOWNFILENAME);
	CppUnitException(const CppUnitException& other);
	virtual ~CppUnitException() throw();

	CppUnitException& operator = (const CppUnitException& other);

	const char* what() const throw ();

	long lineNumber() const;
	const std::string& fileName() const;

	static const std::string CPPUNIT_UNKNOWNFILENAME;
	static const int CPPUNIT_UNKNOWNLINENUMBER;

private:
	std::string _message;
	long        _lineNumber;
	std::string _fileName;
};


inline CppUnitException::CppUnitException(const CppUnitException& other): exception (other)
{
    _message    = other._message;
    _lineNumber = other._lineNumber;
    _fileName   = other._fileName;
}

inline CppUnitException::CppUnitException (const std::string& message, long lineNumber, const std::string& fileName): _message(message), _lineNumber(lineNumber), _fileName(fileName)
{
}


inline CppUnitException::~CppUnitException () throw()
{
}


inline CppUnitException& CppUnitException::operator = (const CppUnitException& other)
{
	exception::operator= (other);

    if (&other != this)
    {
        _message    = other._message;
        _lineNumber = other._lineNumber;
        _fileName   = other._fileName;
    }
    return *this;
}


inline const char* CppUnitException::what() const throw ()
{
	return _message.c_str();
}


inline long CppUnitException::lineNumber() const
{
	return _lineNumber; 
}


// The file in which the error occurred
inline const std::string& CppUnitException::fileName() const
{
	return _fileName; 
}


CppUnit_END


#endif // CppUnit_CppUnitException_INCLUDED

