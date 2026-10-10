//
// TesterEventDispatcher.h
//
// Package: Generated
// Module:  TesterEventDispatcher
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TesterEventDispatcher_INCLUDED
#define TesterEventDispatcher_INCLUDED


#include "ITester.h"
#include "Poco/RemotingNG/EventDispatcher.h"


class TesterEventDispatcher: public Poco::RemotingNG::EventDispatcher
{
public:
	TesterEventDispatcher(ITester* pInterface, const Poco::RemotingNG::Identifiable::ObjectId& objectId, const std::string& protocol);
		/// Creates a TesterEventDispatcher.

	virtual ~TesterEventDispatcher();
		/// Destroys the TesterEventDispatcher.

	void event__testEnumEvent(const void* pSender, Enum1& data);

	void event__testEvent(const void* pSender, std::string& data);

	void event__testFilteredEvent(const void* pSender, const int& data);

	void event__testOneWayEvent(const void* pSender, std::string& data);

	void event__testScopedEnumEvent(const void* pSender, ScopedEnum& data);

	void event__testVoidEvent(const void* pSender);

	virtual const Poco::RemotingNG::Identifiable::TypeId& remoting__typeId() const;

private:
	void event__testEnumEventImpl(const std::string& subscriberURI, Enum1& data);

	void event__testEventImpl(const std::string& subscriberURI, std::string& data);

	void event__testFilteredEventImpl(const std::string& subscriberURI, const int& data);

	void event__testOneWayEventImpl(const std::string& subscriberURI, std::string& data);

	void event__testScopedEnumEventImpl(const std::string& subscriberURI, ScopedEnum& data);

	void event__testVoidEventImpl(const std::string& subscriberURI);

	static const std::string DEFAULT_NS;
	Poco::RemotingNG::Identifiable::ObjectId _objectId;
	ITester* _pInterface;
};


inline const Poco::RemotingNG::Identifiable::TypeId& TesterEventDispatcher::remoting__typeId() const
{
	return ITester::remoting__typeId();
}




#endif // TesterEventDispatcher_INCLUDED

