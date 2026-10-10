//
// AuthTesterRemoteObject.h
//
// Package: Generated
// Module:  AuthTesterRemoteObject
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef AuthTesterRemoteObject_INCLUDED
#define AuthTesterRemoteObject_INCLUDED


#include "IAuthTester.h"
#include "Poco/RemotingNG/Identifiable.h"
#include "Poco/RemotingNG/RemoteObject.h"
#include "Poco/SharedPtr.h"


class AuthTesterRemoteObject: public IAuthTester, public Poco::RemotingNG::RemoteObject
{
public:
	using Ptr = Poco::AutoPtr<AuthTesterRemoteObject>;

	AuthTesterRemoteObject(const Poco::RemotingNG::Identifiable::ObjectId& oid, Poco::SharedPtr<AuthTester> pServiceObject);
		/// Creates a AuthTesterRemoteObject.

	virtual ~AuthTesterRemoteObject();
		/// Destroys the AuthTesterRemoteObject.

	Object get(int id, const std::string& options = std::string("none"));

	Object post(int id, const Object& object);

	virtual const Poco::RemotingNG::Identifiable::TypeId& remoting__typeId() const;

private:
	Poco::SharedPtr<AuthTester> _pServiceObject;
};


inline Object AuthTesterRemoteObject::get(int id, const std::string& options)
{
	return _pServiceObject->get(id, options);
}


inline Object AuthTesterRemoteObject::post(int id, const Object& object)
{
	return _pServiceObject->post(id, object);
}


inline const Poco::RemotingNG::Identifiable::TypeId& AuthTesterRemoteObject::remoting__typeId() const
{
	return IAuthTester::remoting__typeId();
}




#endif // AuthTesterRemoteObject_INCLUDED

