//
// TimeServiceSkeleton.h
//
// Package: Generated
// Module:  TimeServiceSkeleton
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//
// Copyright (c) 2006-2020, Applied Informatics Software Engineering GmbH.
//


#ifndef TimeServiceSkeleton_INCLUDED
#define TimeServiceSkeleton_INCLUDED


#include "Poco/RemotingNG/Skeleton.h"
#include "TimeServiceRemoteObject.h"


namespace Services {


class TimeServiceSkeleton: public Poco::RemotingNG::Skeleton
{
public:
	TimeServiceSkeleton();
		/// Creates a TimeServiceSkeleton.

	virtual ~TimeServiceSkeleton();
		/// Destroys a TimeServiceSkeleton.

	virtual const Poco::RemotingNG::Identifiable::TypeId& remoting__typeId() const;

	static const std::string DEFAULT_NS;
};


inline const Poco::RemotingNG::Identifiable::TypeId& TimeServiceSkeleton::remoting__typeId() const
{
	return ITimeService::remoting__typeId();
}


} // namespace Services


#endif // TimeServiceSkeleton_INCLUDED

