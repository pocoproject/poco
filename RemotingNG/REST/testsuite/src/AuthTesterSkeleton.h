//
// AuthTesterSkeleton.h
//
// Package: Generated
// Module:  AuthTesterSkeleton
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef AuthTesterSkeleton_INCLUDED
#define AuthTesterSkeleton_INCLUDED


#include "AuthTesterRemoteObject.h"
#include "Poco/RemotingNG/Skeleton.h"


class AuthTesterSkeleton: public Poco::RemotingNG::Skeleton
{
public:
	AuthTesterSkeleton();
		/// Creates a AuthTesterSkeleton.

	virtual ~AuthTesterSkeleton();
		/// Destroys a AuthTesterSkeleton.

	virtual const Poco::RemotingNG::Identifiable::TypeId& remoting__typeId() const;

	static const std::string DEFAULT_NS;
};


inline const Poco::RemotingNG::Identifiable::TypeId& AuthTesterSkeleton::remoting__typeId() const
{
	return IAuthTester::remoting__typeId();
}




#endif // AuthTesterSkeleton_INCLUDED

