//
// TesterSkeleton.h
//
// Package: Generated
// Module:  TesterSkeleton
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TesterSkeleton_INCLUDED
#define TesterSkeleton_INCLUDED


#include "Poco/RemotingNG/Skeleton.h"
#include "TesterRemoteObject.h"


class TesterSkeleton: public Poco::RemotingNG::Skeleton
{
public:
	TesterSkeleton();
		/// Creates a TesterSkeleton.

	virtual ~TesterSkeleton();
		/// Destroys a TesterSkeleton.

	virtual const Poco::RemotingNG::Identifiable::TypeId& remoting__typeId() const;

	static const std::string DEFAULT_NS;
};


inline const Poco::RemotingNG::Identifiable::TypeId& TesterSkeleton::remoting__typeId() const
{
	return ITester::remoting__typeId();
}




#endif // TesterSkeleton_INCLUDED

