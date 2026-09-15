//
// TesterRemoteObject.h
//
// Package: Generated
// Module:  TesterRemoteObject
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TesterRemoteObject_INCLUDED
#define TesterRemoteObject_INCLUDED


#include "ITester.h"
#include "Poco/RemotingNG/Identifiable.h"
#include "Poco/RemotingNG/RemoteObject.h"
#include "Poco/SharedPtr.h"


class TesterRemoteObject: public ITester, public Poco::RemotingNG::RemoteObject
{
public:
	using Ptr = Poco::AutoPtr<TesterRemoteObject>;

	TesterRemoteObject(const Poco::RemotingNG::Identifiable::ObjectId& oid, Poco::SharedPtr<Tester> pServiceObject);
		/// Creates a TesterRemoteObject.

	virtual ~TesterRemoteObject();
		/// Destroys the TesterRemoteObject.

	void delete_(int id);

	Object get(int id, const std::string& options = std::string("none"));

	Object post(int id, const Object& object);

	Object put(int id, const Object& object);

	virtual const Poco::RemotingNG::Identifiable::TypeId& remoting__typeId() const;

private:
	Poco::SharedPtr<Tester> _pServiceObject;
};


inline void TesterRemoteObject::delete_(int id)
{
	_pServiceObject->delete_(id);
}


inline Object TesterRemoteObject::get(int id, const std::string& options)
{
	return _pServiceObject->get(id, options);
}


inline Object TesterRemoteObject::post(int id, const Object& object)
{
	return _pServiceObject->post(id, object);
}


inline Object TesterRemoteObject::put(int id, const Object& object)
{
	return _pServiceObject->put(id, object);
}


inline const Poco::RemotingNG::Identifiable::TypeId& TesterRemoteObject::remoting__typeId() const
{
	return ITester::remoting__typeId();
}




#endif // TesterRemoteObject_INCLUDED

