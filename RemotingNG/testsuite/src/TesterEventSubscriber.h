//
// TesterEventSubscriber.h
//
// Package: Generated
// Module:  TesterEventSubscriber
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TesterEventSubscriber_INCLUDED
#define TesterEventSubscriber_INCLUDED


#include "Poco/RemotingNG/EventSubscriber.h"
#include "TesterProxy.h"


class TesterEventSubscriber: public Poco::RemotingNG::EventSubscriber
{
public:
	TesterEventSubscriber(const std::string& uri, TesterProxy* pProxy);
		/// Creates a TesterEventSubscriber.

	virtual ~TesterEventSubscriber();
		/// Destroys the TesterEventSubscriber.

	virtual const Poco::RemotingNG::Identifiable::TypeId& remoting__typeId() const;

private:
	void event__testEnumEvent(Enum1& data);

	void event__testEvent(std::string& data);

	void event__testFilteredEvent(const int& data);

	void event__testOneWayEvent(std::string& data);

	void event__testScopedEnumEvent(ScopedEnum& data);

	void event__testVoidEvent();

public:
	static const std::string DEFAULT_NS;
};


inline const Poco::RemotingNG::Identifiable::TypeId& TesterEventSubscriber::remoting__typeId() const
{
	return ITester::remoting__typeId();
}




#endif // TesterEventSubscriber_INCLUDED

