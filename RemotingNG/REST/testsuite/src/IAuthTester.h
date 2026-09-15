//
// IAuthTester.h
//
// Package: Generated
// Module:  IAuthTester
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef IAuthTester_INCLUDED
#define IAuthTester_INCLUDED


#include "Poco/AutoPtr.h"
#include "Poco/RefCountedObject.h"
#include "Poco/RemotingNG/Identifiable.h"
#include "Tester.h"


class IAuthTester: public virtual Poco::RefCountedObject
{
public:
	using Ptr = Poco::AutoPtr<IAuthTester>;

	IAuthTester();
		/// Creates a IAuthTester.

	virtual ~IAuthTester();
		/// Destroys the IAuthTester.

	virtual Object get(int id, const std::string& options = std::string("none")) = 0;

	virtual Object post(int id, const Object& object) = 0;

	static const Poco::RemotingNG::Identifiable::TypeId& remoting__typeId();
		/// Returns the TypeId of the class.

};




#endif // IAuthTester_INCLUDED

