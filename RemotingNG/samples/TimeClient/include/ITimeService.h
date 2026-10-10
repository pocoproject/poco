//
// ITimeService.h
//
// Package: Generated
// Module:  ITimeService
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//
// Copyright (c) 2006-2020, Applied Informatics Software Engineering GmbH.
//


#ifndef ITimeService_INCLUDED
#define ITimeService_INCLUDED


#include "Poco/AutoPtr.h"
#include "Poco/RefCountedObject.h"
#include "Poco/RemotingNG/Identifiable.h"
#include "TimeService.h"


namespace Services {


class ITimeService: public virtual Poco::RefCountedObject
{
public:
	using Ptr = Poco::AutoPtr<ITimeService>;

	ITimeService();
		/// Creates a ITimeService.

	virtual ~ITimeService();
		/// Destroys the ITimeService.

	virtual Poco::DateTime currentTime() const = 0;
		/// Returns the current date and time.

	static const Poco::RemotingNG::Identifiable::TypeId& remoting__typeId();
		/// Returns the TypeId of the class.

};


} // namespace Services


#endif // ITimeService_INCLUDED

