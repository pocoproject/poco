//
// ITester.h
//
// Package: Generated
// Module:  ITester
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef ITester_INCLUDED
#define ITester_INCLUDED


#include "Poco/AutoPtr.h"
#include "Poco/RefCountedObject.h"
#include "Poco/RemotingNG/Identifiable.h"
#include "Tester.h"


class ITester: public virtual Poco::RefCountedObject
{
public:
	using Ptr = Poco::AutoPtr<ITester>;

	ITester();
		/// Creates a ITester.

	virtual ~ITester();
		/// Destroys the ITester.

	virtual void delete_(int id) = 0;

	virtual Object get(int id, const std::string& options = std::string("none")) = 0;

	virtual Object post(int id, const Object& object) = 0;

	virtual Object put(int id, const Object& object) = 0;

	static const Poco::RemotingNG::Identifiable::TypeId& remoting__typeId();
		/// Returns the TypeId of the class.

};




#endif // ITester_INCLUDED

