//
// TesterProxy.h
//
// Package: Generated
// Module:  TesterProxy
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TesterProxy_INCLUDED
#define TesterProxy_INCLUDED


#include "ITester.h"
#include "Poco/RemotingNG/Proxy.h"


class TesterProxy: public ITester, public Poco::RemotingNG::Proxy
{
public:
	using Ptr = Poco::AutoPtr<TesterProxy>;

	TesterProxy(const Poco::RemotingNG::Identifiable::ObjectId& oid);
		/// Creates a TesterProxy.

	virtual ~TesterProxy();
		/// Destroys the TesterProxy.

	void delete_(int id);

	Object get(int id, const std::string& options = std::string("none"));

	Object post(int id, const Object& object);

	Object put(int id, const Object& object);

	virtual const Poco::RemotingNG::Identifiable::TypeId& remoting__typeId() const;

private:
	static const std::string DEFAULT_NS;
	mutable Object _getRet;
	mutable Object _postRet;
	mutable Object _putRet;
};


inline const Poco::RemotingNG::Identifiable::TypeId& TesterProxy::remoting__typeId() const
{
	return ITester::remoting__typeId();
}




#endif // TesterProxy_INCLUDED

