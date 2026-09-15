//
// TimeServiceEventSubscriber.h
//
// Package: Generated
// Module:  TimeServiceEventSubscriber
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//
// Copyright (c) 2006-2020, Applied Informatics Software Engineering GmbH.
//


#ifndef TimeServiceEventSubscriber_INCLUDED
#define TimeServiceEventSubscriber_INCLUDED


#include "Poco/RemotingNG/EventSubscriber.h"
#include "TimeServiceProxy.h"


namespace Services {


class TimeServiceEventSubscriber: public Poco::RemotingNG::EventSubscriber
{
public:
	TimeServiceEventSubscriber(const std::string& uri, TimeServiceProxy* pProxy);
		/// Creates a TimeServiceEventSubscriber.

	virtual ~TimeServiceEventSubscriber();
		/// Destroys the TimeServiceEventSubscriber.

	virtual const Poco::RemotingNG::Identifiable::TypeId& remoting__typeId() const;

private:
	void event__wakeUp(const Services::WakeUpEvent& data);

public:
	static const std::string DEFAULT_NS;
};


inline const Poco::RemotingNG::Identifiable::TypeId& TimeServiceEventSubscriber::remoting__typeId() const
{
	return ITimeService::remoting__typeId();
}


} // namespace Services


#endif // TimeServiceEventSubscriber_INCLUDED

