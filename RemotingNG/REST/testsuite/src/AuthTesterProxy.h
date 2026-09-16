//
// AuthTesterProxy.h
//
// Package: Generated
// Module:  AuthTesterProxy
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef AuthTesterProxy_INCLUDED
#define AuthTesterProxy_INCLUDED


#include "IAuthTester.h"
#include "Poco/RemotingNG/Proxy.h"


class AuthTesterProxy: public IAuthTester, public Poco::RemotingNG::Proxy
{
public:
	using Ptr = Poco::AutoPtr<AuthTesterProxy>;

	AuthTesterProxy(const Poco::RemotingNG::Identifiable::ObjectId& oid);
		/// Creates a AuthTesterProxy.

	virtual ~AuthTesterProxy();
		/// Destroys the AuthTesterProxy.

	Object get(int id, const std::string& options = std::string("none"));

	Object post(int id, const Object& object);

	virtual const Poco::RemotingNG::Identifiable::TypeId& remoting__typeId() const;

private:
	static const std::string DEFAULT_NS;
	mutable Object _getRet;
	mutable Object _postRet;
};


inline const Poco::RemotingNG::Identifiable::TypeId& AuthTesterProxy::remoting__typeId() const
{
	return IAuthTester::remoting__typeId();
}




#endif // AuthTesterProxy_INCLUDED

