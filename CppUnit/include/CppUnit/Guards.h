//
// Guards.h
//
// $Id: //poco/Main/CppUnit/include/CppUnit/Guards.h#5 $
//


#ifndef CppUnit_Guards_INCLUDED
#define CppUnit_Guards_INCLUDED


// Prevent copy construction and assignment for a class
#define REFERENCEOBJECT(className) \
private: \
	className(const className& other); \
	className& operator = (const className& other);


#endif // CppUnit_Guards_INCLUDED
