//
// TimeServiceEventDispatcher.h
//
// Package: Generated
// Module:  TimeServiceEventDispatcher
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//
// Copyright (c) 2006-2020, Applied Informatics Software Engineering GmbH.
//


#ifndef TimeServiceEventDispatcher_INCLUDED
#define TimeServiceEventDispatcher_INCLUDED


#include "ITimeService.h"
#include "Poco/RemotingNG/EventDispatcher.h"


namespace Services {


class TimeServiceEventDispatcher: public Poco::RemotingNG::EventDispatcher
{
public:
	TimeServiceEventDispatcher(ITimeService* pInterface, const Poco::RemotingNG::Identifiable::ObjectId& objectId, const std::string& protocol);
		/// Creates a TimeServiceEventDispatcher.

	virtual ~TimeServiceEventDispatcher();
		/// Destroys the TimeServiceEventDispatcher.

	void event__wakeUp(const void* pSender, const Services::WakeUpEvent& data);

	virtual const Poco::RemotingNG::Identifiable::TypeId& remoting__typeId() const;

private:
	void event__wakeUpImpl(const std::string& subscriberURI, const Services::WakeUpEvent& data);

	static const std::string DEFAULT_NS;
	Poco::RemotingNG::Identifiable::ObjectId _objectId;
	ITimeService* _pInterface;
};


inline const Poco::RemotingNG::Identifiable::TypeId& TimeServiceEventDispatcher::remoting__typeId() const
{
	return ITimeService::remoting__typeId();
}


} // namespace Services


#endif // TimeServiceEventDispatcher_INCLUDED

