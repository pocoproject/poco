//
// RemotingTest.cpp
//
// Copyright (c) 2006-2014, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// License: Applied Informatics Software and Source Code License Agreement
//


#include "RemotingTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/RemotingNG/Transport.h"
#include "Poco/RemotingNG/TransportFactory.h"
#include "Poco/RemotingNG/TransportFactoryManager.h"
#include "Poco/RemotingNG/ServerTransport.h"
#include "Poco/RemotingNG/EventListener.h"
#include "Poco/RemotingNG/BinarySerializer.h"
#include "Poco/RemotingNG/BinaryDeserializer.h"
#include "Poco/RemotingNG/Skeleton.h"
#include "Poco/RemotingNG/EventSubscriber.h"
#include "Poco/RemotingNG/EventDispatcher.h"
#include "Poco/RemotingNG/Authorizer.h"
#include "Poco/RemotingNG/ORB.h"
#include "Poco/SharedPtr.h"
#include "Poco/NumberFormatter.h"
#include "Poco/DateTimeFormatter.h"
#include "Poco/Delegate.h"
#include "Tester.h"
#include "TesterServerHelper.h"
#include "TesterClientHelper.h"
#include "TesterRemoteObject.h"
#include "TesterProxy.h"
#include <sstream>


namespace
{
	class MockServerTransport: public Poco::RemotingNG::ServerTransport
	{
	public:
		MockServerTransport(std::istream& istr, std::ostream& ostr, Poco::RemotingNG::Authorizer::Ptr pAuthorizer):
			_istr(istr),
			_ostr(ostr),
			_pAuthorizer(pAuthorizer)
		{
		}

		bool authorize(const std::string& method, const std::string& permission)
		{
			if (_pAuthorizer)
			{
				return _pAuthorizer->authorize(method, permission);
			}
			return false;
		}

		Poco::RemotingNG::Deserializer& beginRequest()
		{
			_deserializer.setup(_istr);
			return _deserializer;
		}

		Poco::RemotingNG::Serializer& sendReply(Poco::RemotingNG::SerializerBase::MessageType messageType)
		{
			_serializer.setup(_ostr);
			return _serializer;
		}

		void endRequest()
		{
		}

	private:
		std::istream& _istr;
		std::ostream& _ostr;
		Poco::RemotingNG::BinarySerializer _serializer;
		Poco::RemotingNG::BinaryDeserializer _deserializer;
		Poco::RemotingNG::Authorizer::Ptr _pAuthorizer;
	};

	class MockListener: public Poco::RemotingNG::EventListener
	{
	public:
		using Ptr = Poco::AutoPtr<MockListener>;

		MockListener(const std::string& protocol, const std::string& endPoint):
			Poco::RemotingNG::EventListener(endPoint),
			_protocol(protocol),
			_running(false)
		{
		}

		MockListener(const std::string& protocol, const std::string& endPoint, MockListener::Ptr pServerListener):
			Poco::RemotingNG::EventListener(endPoint),
			_protocol(protocol),
			_running(false),
			_pServerListener(pServerListener)
		{
		}

		~MockListener()
		{
		}

		bool running() const
		{
			return _running;
		}

		void invoke(const std::string& typeId, const std::string& oid, std::istream& istr, std::ostream& ostr)
		{
			MockServerTransport transport(istr, ostr, getAuthorizer());
			std::string objectPath(_protocol);
			objectPath += '/';
			objectPath += endPoint();
			objectPath += '/';
			objectPath += typeId;
			objectPath += '/';
			objectPath += oid;
			if (!Poco::RemotingNG::ORB::instance().invoke(objectPath, transport))
			{
				if (_pEventSubscriber)
				{
					_pEventSubscriber->invoke(transport, 0);
				}
				else
				{
					Poco::RemotingNG::UnknownObjectException exc(objectPath);
					transport.sendReply(Poco::RemotingNG::SerializerBase::MESSAGE_FAULT).serializeFaultMessage("", exc);
					transport.endRequest();
				}
			}
		}

		void registerSubscriber(const std::string& objectURI, const std::string& subscriberURI)
		{
			Poco::RemotingNG::EventDispatcher::Ptr pEventDispatcher = Poco::RemotingNG::ORB::instance().findEventDispatcher(objectURI, "evmock");
			pEventDispatcher->subscribe(subscriberURI, subscriberURI);
		}

		void unregisterSubscriber(const std::string& objectURI, const std::string& subscriberURI)
		{
			Poco::RemotingNG::EventDispatcher::Ptr pEventDispatcher = Poco::RemotingNG::ORB::instance().findEventDispatcher(objectURI, "evmock");
			pEventDispatcher->unsubscribe(subscriberURI);
		}

		// Listener
		void start()
		{
			_running = true;
		}

		void stop()
		{
			_running = false;
		}

		const std::string& protocol() const
		{
			return _protocol;
		}

		std::string createURI(const Poco::RemotingNG::Identifiable::TypeId& typeId, const Poco::RemotingNG::Identifiable::ObjectId& objectId)
		{
			std::string uri("mock://localhost/");
			uri += _protocol;
			uri += '/';
			uri += typeId;
			uri += '/';
			uri += objectId;
			return uri;
		}

		bool handlesURI(const std::string& uri)
		{
			return false;
		}

		void registerObject(Poco::RemotingNG::RemoteObject::Ptr pRemoteObject, Poco::RemotingNG::Skeleton::Ptr pSkeleton)
		{
		}

		void unregisterObject(Poco::RemotingNG::RemoteObject::Ptr pRemoteObject)
		{
		}

		// EventListener
		std::string subscribeToEvents(Poco::RemotingNG::EventSubscriber::Ptr pEventSubscriber)
		{
			_pEventSubscriber = pEventSubscriber;
			std::string subscriberURI = createURI("RemotingNG::EventSubscriber", "TheEventSubscriber");
			_pServerListener->registerSubscriber(pEventSubscriber->uri(), subscriberURI);
			return subscriberURI;
		}

		void unsubscribeFromEvents(Poco::RemotingNG::EventSubscriber::Ptr pEventSubscriber)
		{
			std::string subscriberURI = createURI("RemotingNG::EventSubscriber", "TheEventSubscriber");
			_pServerListener->unregisterSubscriber(pEventSubscriber->uri(), subscriberURI);
			_pEventSubscriber = 0;
		}

	private:
		std::string _protocol;
		bool _running;
		MockListener::Ptr _pServerListener;
		Poco::RemotingNG::EventSubscriber::Ptr _pEventSubscriber;

		friend class MockTransport;
	};


	class MockTransport: public Poco::RemotingNG::Transport
	{
	public:
		MockTransport():
			_connected(false)
		{
		}

		MockTransport(MockListener::Ptr pListener):
			_pListener(pListener),
			_connected(false)
		{
		}

		~MockTransport()
		{
		}

		const std::string& endPoint() const
		{
			return _pListener->endPoint();
		}

		void connect(const std::string& endPoint)
		{
			_connected = true;
		}

		void disconnect()
		{
			_connected = false;
		}

		bool connected() const
		{
			return _connected;
		}

		Poco::RemotingNG::Serializer& beginMessage(const Poco::RemotingNG::Identifiable::ObjectId& oid, const Poco::RemotingNG::Identifiable::TypeId& tid, const std::string& messageName, Poco::RemotingNG::SerializerBase::MessageType messageType)
		{
			return beginRequest(oid, tid, messageName, messageType);
		}

		void sendMessage(const Poco::RemotingNG::Identifiable::ObjectId& oid, const Poco::RemotingNG::Identifiable::TypeId& tid, const std::string& messageName, Poco::RemotingNG::SerializerBase::MessageType messageType)
		{
			sendRequest(oid, tid, messageName, messageType);
		}

		Poco::RemotingNG::Serializer& beginRequest(const Poco::RemotingNG::Identifiable::ObjectId& oid, const Poco::RemotingNG::Identifiable::TypeId& tid, const std::string& messageName, Poco::RemotingNG::SerializerBase::MessageType messageType)
		{
			_pRequestStream = new std::stringstream;
			_serializer.setup(*_pRequestStream);
			return _serializer;
		}

		Poco::RemotingNG::Deserializer& sendRequest(const Poco::RemotingNG::Identifiable::ObjectId& oid, const Poco::RemotingNG::Identifiable::TypeId& tid, const std::string& messageName, Poco::RemotingNG::SerializerBase::MessageType messageType)
		{
			_pResponseStream = new std::stringstream;
			_pListener->invoke(tid, oid, *_pRequestStream, *_pResponseStream);
			_deserializer.setup(*_pResponseStream);
			_pRequestStream = 0;
			return _deserializer;
		}

		void endRequest()
		{
			_pResponseStream = 0;
		}

	private:
		MockListener::Ptr _pListener;
		bool _connected;
		Poco::RemotingNG::BinarySerializer _serializer;
		Poco::RemotingNG::BinaryDeserializer _deserializer;
		Poco::SharedPtr<std::stringstream> _pRequestStream;
		Poco::SharedPtr<std::stringstream> _pResponseStream;
	};


	class TestTransportFactory: public Poco::RemotingNG::TransportFactory
	{
	public:
		TestTransportFactory(MockListener::Ptr pListener):
			_pListener(pListener)
		{
		}

		Poco::RemotingNG::Transport* createTransport()
		{
			return new MockTransport(_pListener);
		}

		static void registerFactory(MockListener::Ptr pListener, const std::string& protocol)
		{
			Poco::RemotingNG::TransportFactoryManager::instance().registerFactory(protocol, new TestTransportFactory(pListener));
		}

		static void unregisterFactory(const std::string& protocol)
		{
			Poco::RemotingNG::TransportFactoryManager::instance().unregisterFactory(protocol);
		}

	private:
		MockListener::Ptr _pListener;
	};

	class MockAuthorizer: public Poco::RemotingNG::Authorizer
	{
	public:
		MockAuthorizer(const std::string& permission):
			_permission(permission)
		{
		}

		~MockAuthorizer()
		{
		}

		bool authorize(const std::string& method, const std::string& permission)
		{
			return permission == _permission;
		}

	private:
		std::string _permission;
	};
}


bool operator == (const Struct1& s1, const Struct1& s2)
{
	return s1.aString == s2.aString
	    && s1.anInt   == s2.anInt
	    && s1.aDouble == s2.aDouble
	    && s1.anEnum  == s2.anEnum
	    && s1.anEnum2 == s2.anEnum2;
}


bool operator == (const Struct1::Ptr s1, const Struct1::Ptr s2)
{
	return *s1 == *s2;
}


bool operator == (const Struct2& s1, const Struct2& s2)
{
	bool equal = s1.anURI == s2.anURI
	    && s1.aDateTime == s2.aDateTime
	    && s1.aLocalDateTime == s2.aLocalDateTime
	    && s1.aTimestamp == s2.aTimestamp
	    && s1.anUUID == s2.anUUID;

	if (!equal) return false;
	if (s1.aVector.size() != s2.aVector.size()) return false;
	for (int i = 0; i < s1.aVector.size(); i++)
	{
		if (!(s1.aVector[i] == s2.aVector[i])) return false;
	}
	if (s1.anotherVector.size() != s2.anotherVector.size()) return false;
	for (int i = 0; i < s1.anotherVector.size(); i++)
	{
		if (!s1.anotherVector[i].isNull() && !s2.anotherVector[i].isNull())
		{
			if (!(*s1.anotherVector[i] == *s2.anotherVector[i])) return false;
		}
	}
	return true;
}


bool operator == (const Struct3& s1, const Struct3& s2)
{
	bool equal = s1.aCharVector == s2.aCharVector
	          && s1.aSet == s2.aSet
	          && s1.aMultiSet == s2.aMultiSet;
	if (!equal) return false;
	if (s1.aComplexVector.size() != s2.aComplexVector.size()) return false;
	for (int i = 0; i < s1.aComplexVector.size(); i++)
	{
		if (!(s1.aComplexVector[i] == s2.aComplexVector[i])) return false;
	}
	return true;
}


bool operator == (const Struct4& s1, const Struct4& s2)
{
	bool equal = s1.value == s2.value;
	if (!equal) return false;
	if (s1.vec.size() != s2.vec.size()) return false;
	for (int i = 0; i < s1.vec.size(); i++)
	{
		if (!(s1.vec[i] == s2.vec[i])) return false;
	}
	if (s1.ptr.isNull() != s2.ptr.isNull()) return false;
	if (s1.ptr && s2.ptr && !(*s1.ptr == *s2.ptr)) return false;
	return true;
}


RemotingTest::RemotingTest(const std::string& name):
	CppUnit::TestCase(name),
	_filteredArg(0)
{
}


RemotingTest::~RemotingTest()
{
}


void RemotingTest::setUp()
{
	MockListener::Ptr pListener = new MockListener("mock", "endpoint");
	_listener = Poco::RemotingNG::ORB::instance().registerListener(pListener);
	TestTransportFactory::registerFactory(pListener, "mock");
	TestTransportFactory::registerFactory(pListener, "MOCK");
	_objectURI = TesterServerHelper::registerObject(new Tester, "TheTester", _listener);
	_pListener = pListener;
}


void RemotingTest::tearDown()
{
	TesterServerHelper::unregisterObject(_objectURI);
	TestTransportFactory::unregisterFactory("mock");
	TestTransportFactory::unregisterFactory("MOCK");
	Poco::RemotingNG::ORB::instance().unregisterListener(_listener);
	_pListener = 0;
}


void RemotingTest::testListeners()
{
	Poco::RemotingNG::ORB::ListenerVec listeners = Poco::RemotingNG::ORB::instance().listeners();
	assert (listeners.size() == 1);
}


void RemotingTest::testRegistration()
{
	assert (_objectURI == "mock://localhost/mock/Tester/TheTester");
}


void RemotingTest::testFindObject()
{
	ITester::Ptr pTester = TesterClientHelper::find(_objectURI);

	Poco::AutoPtr<TesterRemoteObject> pRO = pTester.cast<TesterRemoteObject>();
	assert (!pRO.isNull());

	Poco::AutoPtr<TesterProxy> pProxy = pTester.cast<TesterProxy>();
	assert (pProxy.isNull());

	pTester = TesterClientHelper::find("MOCK://localhost/MOCK/Tester/TheTester");

	pRO = pTester.cast<TesterRemoteObject>();
	assert (pRO.isNull());

	pProxy = pTester.cast<TesterProxy>();
	assert (!pProxy.isNull());
}


void RemotingTest::testInt()
{
	ITester::Ptr pTester = TesterClientHelper::find("MOCK://localhost/MOCK/Tester/TheTester");
	testInt(pTester);
}


void RemotingTest::testEnum1()
{
	ITester::Ptr pTester = TesterClientHelper::find("MOCK://localhost/MOCK/Tester/TheTester");
	testEnum1(pTester);
}


void RemotingTest::testEnum2()
{
	ITester::Ptr pTester = TesterClientHelper::find("MOCK://localhost/MOCK/Tester/TheTester");
	testEnum2(pTester);
}


void RemotingTest::testNested()
{
	ITester::Ptr pTester = TesterClientHelper::find("MOCK://localhost/MOCK/Tester/TheTester");
	testNested(pTester);
}


void RemotingTest::testStruct1()
{
	ITester::Ptr pTester = TesterClientHelper::find("MOCK://localhost/MOCK/Tester/TheTester");
	testStruct1(pTester);
}


void RemotingTest::testStruct2()
{
	ITester::Ptr pTester = TesterClientHelper::find("MOCK://localhost/MOCK/Tester/TheTester");
	testStruct2(pTester);
}


void RemotingTest::testStruct3()
{
	ITester::Ptr pTester = TesterClientHelper::find("MOCK://localhost/MOCK/Tester/TheTester");
	testStruct3(pTester);
}


void RemotingTest::testStruct4()
{
	ITester::Ptr pTester = TesterClientHelper::find("MOCK://localhost/MOCK/Tester/TheTester");
	testStruct4(pTester);
}


void RemotingTest::testStruct5()
{
	ITester::Ptr pTester = TesterClientHelper::find("MOCK://localhost/MOCK/Tester/TheTester");
	testStruct5(pTester);
}


void RemotingTest::testStruct6()
{
	ITester::Ptr pTester = TesterClientHelper::find("MOCK://localhost/MOCK/Tester/TheTester");
	testStruct6(pTester);
}


void RemotingTest::testArray()
{
	ITester::Ptr pTester = TesterClientHelper::find("MOCK://localhost/MOCK/Tester/TheTester");
	testArray(pTester);
}


void RemotingTest::testStruct7()
{
	ITester::Ptr pTester = TesterClientHelper::find("MOCK://localhost/MOCK/Tester/TheTester");
	testStruct7(pTester);
}


void RemotingTest::testStdArray()
{
	ITester::Ptr pTester = TesterClientHelper::find("MOCK://localhost/MOCK/Tester/TheTester");
	testStdArray(pTester);
}


void RemotingTest::testOptional()
{
	ITester::Ptr pTester = TesterClientHelper::find("MOCK://localhost/MOCK/Tester/TheTester");
	testOptional(pTester);
}


void RemotingTest::testStruct8()
{
	ITester::Ptr pTester = TesterClientHelper::find("MOCK://localhost/MOCK/Tester/TheTester");
	testStruct8(pTester);
}


void RemotingTest::testClass1()
{
	ITester::Ptr pTester = TesterClientHelper::find("MOCK://localhost/MOCK/Tester/TheTester");
	testClass1(pTester);
}


void RemotingTest::testPtr()
{
	ITester::Ptr pTester = TesterClientHelper::find("MOCK://localhost/MOCK/Tester/TheTester");
	testPtr(pTester);
}


void RemotingTest::testStruct1Vec()
{
	ITester::Ptr pTester = TesterClientHelper::find("MOCK://localhost/MOCK/Tester/TheTester");
	testStruct1Vec(pTester);
}


void RemotingTest::testOneWay()
{
	ITester::Ptr pTester = TesterClientHelper::find("MOCK://localhost/MOCK/Tester/TheTester");
	pTester->testOneWay("s3cr3t");
	std::string result = pTester->testOneWayResult();
	assert (result == "s3cr3t");
}


void RemotingTest::testEvent()
{
	MockListener::Ptr pEventListener = new MockListener("evmock", "endpoint2", _pListener.cast<MockListener>());
	std::string elEndpoint = Poco::RemotingNG::ORB::instance().registerListener(pEventListener);
	TestTransportFactory::registerFactory(pEventListener, "evmock");
	TesterServerHelper::enableEvents(_objectURI, "evmock");

	// We must use the exact same URI as registered on the server ORB,
	// so we have to manually build a Proxy, otherwise the ORB would
	// just return a RemoteObject.
	Poco::AutoPtr<TesterProxy> pTester = new TesterProxy("TheTester");
	pTester->remoting__connect("mock", "mock://localhost/mock/Tester/TheTester");
	pTester->remoting__enableEvents(pEventListener);

	pTester->testEvent += Poco::delegate(this, &RemotingTest::onEvent);
	_eventArg.clear();
	pTester->fireTestEvent("s3cr3t");
	assert (_eventArg == "s3cr3t");

	TestTransportFactory::unregisterFactory("evmock");
	Poco::RemotingNG::ORB::instance().unregisterListener(elEndpoint);
}


void RemotingTest::testOneWayEvent()
{
	MockListener::Ptr pEventListener = new MockListener("evmock", "endpoint2", _pListener.cast<MockListener>());
	std::string elEndpoint = Poco::RemotingNG::ORB::instance().registerListener(pEventListener);
	TestTransportFactory::registerFactory(pEventListener, "evmock");
	TesterServerHelper::enableEvents(_objectURI, "evmock");

	// We must use the exact same URI as registered on the server ORB,
	// so we have to manually build a Proxy, otherwise the ORB would
	// just return a RemoteObject.
	Poco::AutoPtr<TesterProxy> pTester = new TesterProxy("TheTester");
	pTester->remoting__connect("mock", "mock://localhost/mock/Tester/TheTester");
	pTester->remoting__enableEvents(pEventListener);

	pTester->testOneWayEvent += Poco::delegate(this, &RemotingTest::onEvent);
	_eventArg.clear();
	pTester->fireTestOneWayEvent("s3cr3t");
	assert (_eventArg == "s3cr3t");

	TestTransportFactory::unregisterFactory("evmock");
	Poco::RemotingNG::ORB::instance().unregisterListener(elEndpoint);
}


void RemotingTest::testVoidEvent()
{
	MockListener::Ptr pEventListener = new MockListener("evmock", "endpoint2", _pListener.cast<MockListener>());
	std::string elEndpoint = Poco::RemotingNG::ORB::instance().registerListener(pEventListener);
	TestTransportFactory::registerFactory(pEventListener, "evmock");
	TesterServerHelper::enableEvents(_objectURI, "evmock");

	// We must use the exact same URI as registered on the server ORB,
	// so we have to manually build a Proxy, otherwise the ORB would
	// just return a RemoteObject.
	Poco::AutoPtr<TesterProxy> pTester = new TesterProxy("TheTester");
	pTester->remoting__connect("mock", "mock://localhost/mock/Tester/TheTester");
	pTester->remoting__enableEvents(pEventListener);

	pTester->testVoidEvent += Poco::delegate(this, &RemotingTest::onVoidEvent);
	_eventArg.clear();
	pTester->fireTestVoidEvent();
	assert (_eventArg == "FIRED");

	TestTransportFactory::unregisterFactory("evmock");
	Poco::RemotingNG::ORB::instance().unregisterListener(elEndpoint);
}


void RemotingTest::testFilteredEvent()
{
	MockListener::Ptr pEventListener = new MockListener("evmock", "endpoint2", _pListener.cast<MockListener>());
	std::string elEndpoint = Poco::RemotingNG::ORB::instance().registerListener(pEventListener);
	TestTransportFactory::registerFactory(pEventListener, "evmock");
	TesterServerHelper::enableEvents(_objectURI, "evmock");

	// We must use the exact same URI as registered on the server ORB,
	// so we have to manually build a Proxy, otherwise the ORB would
	// just return a RemoteObject.
	Poco::AutoPtr<TesterProxy> pTester = new TesterProxy("TheTester");
	pTester->remoting__connect("mock", "mock://localhost/mock/Tester/TheTester");
	std::string subscriberURI = pTester->remoting__enableEvents(pEventListener);

	Poco::RemotingNG::EventDispatcher::Ptr pED = Poco::RemotingNG::ORB::instance().findEventDispatcher("mock://localhost/mock/Tester/TheTester", "evmock");
	pED->setEventFilter<int>(subscriberURI, "testFilteredEvent", new Poco::RemotingNG::IsGreaterThanOrEqualToFilter<int>(100));

	pTester->testFilteredEvent += Poco::delegate(this, &RemotingTest::onFilteredEvent);
	_filteredArg = 0;
	pTester->fireTestFilteredEvent(10);
	assert (_filteredArg == 0);
	pTester->fireTestFilteredEvent(100);
	assert (_filteredArg == 100);

	pED->removeEventFilter(subscriberURI, "testFilteredEvent");

	pTester->fireTestFilteredEvent(1);
	assert (_filteredArg == 1);
	pTester->fireTestFilteredEvent(2);
	assert (_filteredArg == 2);

	TestTransportFactory::unregisterFactory("evmock");
	Poco::RemotingNG::ORB::instance().unregisterListener(elEndpoint);
}


void RemotingTest::testPermissions()
{
	ITester::Ptr pTester = TesterClientHelper::find("MOCK://localhost/MOCK/Tester/TheTester");
	testPermissions(pTester);
}


void RemotingTest::testInt(ITester::Ptr pTester)
{
	int i = pTester->testInt1(42);
	assert (i == 42);

	pTester->testInt2(i);
	assert (i == 2112);

	int i2(0);
	pTester->testInt3(55, i2);
	assert (i2 == 55);
}


void RemotingTest::testEnum1(ITester::Ptr pTester)
{
	Enum1 e = pTester->testEnum11(VALUE_1);
	assert (e == VALUE_1);

	pTester->testEnum12(e);
	assert (e == VALUE_2);

	pTester->testEnum13(VALUE_3, e);
	assert (e == VALUE_3);
}


void RemotingTest::testEnum2(ITester::Ptr pTester)
{
	Struct1::Enum2 e = pTester->testEnum21(Struct1::VALUE_1);
	assert (e == Struct1::VALUE_1);

	pTester->testEnum22(e);
	assert (e == Struct1::VALUE_2);

	pTester->testEnum23(Struct1::VALUE_3, e);
	assert (e == Struct1::VALUE_3);
}


void RemotingTest::testNested(ITester::Ptr pTester)
{
	Poco::Timestamp::TimeVal tv1 = 12345678;
	Poco::Timestamp::TimeVal tv2 = pTester->testNestedType(tv1);
	assert (tv1 == tv2);
}


void RemotingTest::testStruct1(ITester::Ptr pTester)
{
	Struct1 s1;
	s1.aString = "foobar";
	s1.anInt = 2112;
	s1.aDouble = 0.5;
	s1.anEnum = VALUE_1;
	s1.anEnum2 = Struct1::VALUE_2;

	Struct1 s2 = pTester->testStruct11(s1);
	assert (s2 == s1);

	pTester->testStruct12(s2);
	assert (s2 == s1);

	Struct1 s3;
	pTester->testStruct13(s1, s3);
	assert (s3 == s1);
}


void RemotingTest::testStruct2(ITester::Ptr pTester)
{
	Struct2 s21;
	for (int i = 0; i < 10; i++)
	{
		Struct1 s1;
		s1.aString = "foobar" + Poco::NumberFormatter::format(i);
		s1.anInt = i;
		s1.aDouble = i + 0.5;
		s1.anEnum = VALUE_1;
		s1.anEnum2 = Struct1::VALUE_2;
		s21.aVector.push_back(s1);
	}

	for (int i = 0; i < 9; i++)
	{
		Struct1::Ptr p1 = new Struct1;
		p1->aString = "foobar" + Poco::NumberFormatter::format(i);
		p1->anInt = i;
		p1->aDouble = i + 0.5;
		p1->anEnum = VALUE_1;
		p1->anEnum2 = Struct1::VALUE_2;
		s21.anotherVector.push_back(p1);
	}
	s21.anotherVector.push_back(Struct1::Ptr());
	s21.anURI = "http://www.appinf.com";
	s21.anUUID.parse("01234567-ABCD-EF00-1234-0123456789AB");
	s21.aDateTime.assign(2009, 11, 9, 13, 23, 23, 200);
	s21.aLocalDateTime.assign(7200, 2009, 11, 9, 13, 23, 23, 200, 0);
	s21.aTimestamp = s21.aDateTime.timestamp();

	Struct2 s22 = pTester->testStruct21(s21);
	assert (s22 == s21);

	pTester->testStruct22(s22);
	assert (s22 == s21);

	Struct2 s23;
	assert (!(s23 == s21));
	pTester->testStruct23(s21, s23);
	assert (s23 == s21);
}


void RemotingTest::testStruct3(ITester::Ptr pTester)
{
	Struct3 s31;
	Struct2 s21;
	for (int i = 0; i < 10; i++)
	{
		Struct1 s1;
		s1.aString = "foobar" + Poco::NumberFormatter::format(i);
		s1.anInt = i;
		s1.aDouble = i + 0.5;
		s1.anEnum = VALUE_1;
		s1.anEnum2 = Struct1::VALUE_2;
		s21.aVector.push_back(s1);
	}

	for (int i = 0; i < 9; i++)
	{
		Struct1::Ptr p1 = new Struct1;
		p1->aString = "foobar" + Poco::NumberFormatter::format(i);
		p1->anInt = i;
		p1->aDouble = i + 0.5;
		p1->anEnum = VALUE_1;
		p1->anEnum2 = Struct1::VALUE_2;
		s21.anotherVector.push_back(p1);
	}
	s21.anotherVector.push_back(Struct1::Ptr());
	s21.anURI = "http://www.appinf.com";
	s21.anUUID.parse("01234567-ABCD-EF00-1234-0123456789AB");
	s21.aDateTime.assign(2009, 11, 9, 13, 23, 23, 200);
	s21.aLocalDateTime.assign(7200, 2009, 11, 9, 13, 23, 23, 200, 0);
	s21.aTimestamp = s21.aDateTime.timestamp();

	for (char c = 'A'; c <= 'Z'; c++)
		s31.aCharVector.push_back(c);
	s31.aComplexVector.push_back(s21);
	s31.aComplexVector.push_back(s21);
	s31.aSet.insert(1);
	s31.aSet.insert(2);
	s31.aSet.insert(3);
	s31.aMultiSet.insert(1);
	s31.aMultiSet.insert(2);
	s31.aMultiSet.insert(3);

	Struct3 s32 = pTester->testStruct31(s31);
	assert (s32 == s31);

	pTester->testStruct32(s32);
	assert (s32 == s31);

	Struct3 s33;
	assert (!(s33 == s31));
	pTester->testStruct33(s31, s33);
	assert (s33 == s31);
}


void RemotingTest::testStruct4(ITester::Ptr pTester)
{
	Struct4 s41;
	s41.value = "baz";
	Struct4 s42;
	s42.value = "foo";
	Struct4 s43;
	s43.value = "bar";
	s41.vec.push_back(s42);
	s41.vec.push_back(s43);

	Struct4 s44 = pTester->testStruct41(s41);
	assert (s44 == s41);

	pTester->testStruct42(s44);
	assert (s44 == s41);

	Struct4 s45;
	assert (!(s45 == s41));
	pTester->testStruct43(s41, s45);
	assert (s41 == s45);

	s41.ptr = new Struct4;

	s44 = pTester->testStruct41(s41);
	assert (s44 == s41);

	pTester->testStruct42(s44);
	assert (s44 == s41);

	Struct4 s46;
	assert (!(s46 == s41));
	pTester->testStruct43(s41, s46);
	assert (s41 == s46);
}


void RemotingTest::testStruct5(ITester::Ptr pTester)
{
	Struct5 s51;
	s51.tv = 12345678;

	Struct5 s52 = pTester->testStruct51(s51);
	assert (s52.tv == s51.tv);

	pTester->testStruct52(s52);
	assert (s52.tv == s51.tv);

	Struct5 s53;
	pTester->testStruct53(s51, s53);
	assert (s51.tv == s53.tv);
}


void RemotingTest::testStruct6(ITester::Ptr pTester)
{
	Struct6 s61;
	s61.arr[0] = 0;
	s61.arr[1] = 1;
	s61.arr[2] = 2;
	s61.arr[3] = 3;

	Struct6 s62 = pTester->testStruct6(s61);

	assert (s61.arr == s62.arr);
}


void RemotingTest::testArray(ITester::Ptr pTester)
{
	Poco::Array<int, 4> arr1;
	arr1[0] = 0;
	arr1[1] = 1;
	arr1[2] = 2;
	arr1[3] = 3;

	Poco::Array<int, 4> arr2 = pTester->testArray(arr1);

	assert (arr1 == arr2);
}


void RemotingTest::testStruct7(ITester::Ptr pTester)
{
	Struct7 s71;
	s71.arr[0] = 0;
	s71.arr[1] = 1;
	s71.arr[2] = 2;
	s71.arr[3] = 3;

	Struct7 s72 = pTester->testStruct7(s71);

	assert (s71.arr == s72.arr);
}


void RemotingTest::testStdArray(ITester::Ptr pTester)
{
	std::array<int, 4> arr1;
	arr1[0] = 0;
	arr1[1] = 1;
	arr1[2] = 2;
	arr1[3] = 3;

	std::array<int, 4> arr2 = pTester->testStdArray(arr1);

	assert (arr1 == arr2);
}


void RemotingTest::testOptional(ITester::Ptr pTester)
{
	{
		Poco::Optional<std::string> value("test");
		Poco::Optional<std::string> res = pTester->testPocoOptional(value);
		assert (res.isSpecified());
		assert (res.value() == value.value());

		value.clear();
		res.clear();

		res = pTester->testPocoOptional(value);
		assert (!res.isSpecified());
	}

#if __cplusplus >= 201703L && defined(TEST_STD_OPTIONAL)
	{
		std::optional<std::string> value("test");
		std::optional<std::string> res = pTester->testStdOptional(value);
		assert (res.has_value());
		assert (res.value() == value.value());

		value.reset();
		res.reset();

		res = pTester->testStdOptional(value);
		assert (!res.has_value());
	}
#endif
}


void RemotingTest::testStruct8(ITester::Ptr pTester)
{
	Struct8 value;
	value.pocoOptional = "test";

#if __cplusplus >= 201703L && defined(TEST_STD_OPTIONAL)
	value.stdOptional = "test2";
#endif

	Struct8 res = pTester->testStruct8(value);
	assert (res.pocoOptional.isSpecified());
	assert (res.pocoOptional.value() == value.pocoOptional.value());

#if __cplusplus >= 201703L && defined(TEST_STD_OPTIONAL)
	assert (res.stdOptional.has_value());
	assert (res.stdOptional.value() == value.stdOptional.value());
#endif
}


void RemotingTest::testClass1(ITester::Ptr pTester)
{
	Class1 c1;
	c1.setAString("foobar");
	c1.setAnInt(42);
	c1.setAnEnum(VALUE_2);

	Class1 c2 = pTester->testClass11(c1);
	assert (c2 == c1);

	pTester->testClass12(c2);
	assert (c2 == c1);

	Class1 c3;
	pTester->testClass13(c1, c3);
	assert (c3 == c1);
}


void RemotingTest::testPtr(ITester::Ptr pTester)
{
	Struct1::Ptr p1 = new Struct1;
	p1->aString = "foobar";
	p1->anInt = 2112;
	p1->aDouble = 0.5;
	p1->anEnum = VALUE_1;
	p1->anEnum2 = Struct1::VALUE_2;

	Struct1::Ptr p2 = pTester->testPtr(p1);
	assert (*p2 == *p1);
}


void RemotingTest::testStruct1Vec(ITester::Ptr pTester)
{
	Struct1Vec vec1;
	for (int i = 0; i < 10; i++)
	{
		Struct1 s1;
		s1.aString = "foobar" + Poco::NumberFormatter::format(i);
		s1.anInt = i;
		s1.aDouble = i + 0.5;
		s1.anEnum = VALUE_1;
		s1.anEnum2 = Struct1::VALUE_2;
		vec1.push_back(s1);
	}

	Struct1Vec vec2 = pTester->testStruct1Vec1(vec1);
	assert (vec2.size() == vec1.size());
	for (int i = 0; i < vec1.size(); i++)
	{
		assert (vec2[i] == vec1[i]);
	}

	pTester->testStruct1Vec2(vec2);
	assert (vec2.size() == vec1.size());
	for (int i = 0; i < vec1.size(); i++)
	{
		assert (vec2[i] == vec1[i]);
	}

	Struct1Vec vec3;
	pTester->testStruct1Vec3(vec1, vec3);
	assert (vec2.size() == vec1.size());
	for (int i = 0; i < vec1.size(); i++)
	{
		assert (vec2[i] == vec1[i]);
	}
}


void RemotingTest::testPermissions(ITester::Ptr pTester)
{
	try
	{
		pTester->testPermission1();
		fail("no authorizer - must throw");
	}
	catch (Poco::RemotingNG::RemoteException&)
	{
	}

	_pListener->setAuthorizer(new MockAuthorizer("perm1"));
	pTester->testPermission1();

	try
	{
		pTester->testPermission2();
		fail("no permission - must throw");
	}
	catch (Poco::RemotingNG::RemoteException&)
	{
	}

	_pListener->setAuthorizer(new MockAuthorizer("perm2"));
	pTester->testPermission2();
}


void RemotingTest::onEvent(const void* pSender, std::string& arg)
{
	_eventArg = arg;
}


void RemotingTest::onVoidEvent(const void* pSender)
{
	_eventArg = "FIRED";
}


void RemotingTest::onFilteredEvent(const int& arg)
{
	_filteredArg = arg;
}


CppUnit::Test* RemotingTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("RemotingTest");

	CppUnit_addTest(pSuite, RemotingTest, testListeners);
	CppUnit_addTest(pSuite, RemotingTest, testRegistration);
	CppUnit_addTest(pSuite, RemotingTest, testFindObject);
	CppUnit_addTest(pSuite, RemotingTest, testInt);
	CppUnit_addTest(pSuite, RemotingTest, testEnum1);
	CppUnit_addTest(pSuite, RemotingTest, testEnum2);
	CppUnit_addTest(pSuite, RemotingTest, testNested);
	CppUnit_addTest(pSuite, RemotingTest, testStruct1);
	CppUnit_addTest(pSuite, RemotingTest, testStruct2);
	CppUnit_addTest(pSuite, RemotingTest, testStruct3);
	CppUnit_addTest(pSuite, RemotingTest, testStruct4);
	CppUnit_addTest(pSuite, RemotingTest, testStruct5);
	CppUnit_addTest(pSuite, RemotingTest, testStruct6);
	CppUnit_addTest(pSuite, RemotingTest, testArray);
	CppUnit_addTest(pSuite, RemotingTest, testStruct7);
	CppUnit_addTest(pSuite, RemotingTest, testStdArray);
	CppUnit_addTest(pSuite, RemotingTest, testOptional);
	CppUnit_addTest(pSuite, RemotingTest, testStruct8);
	CppUnit_addTest(pSuite, RemotingTest, testClass1);
	CppUnit_addTest(pSuite, RemotingTest, testPtr);
	CppUnit_addTest(pSuite, RemotingTest, testStruct1Vec);
	CppUnit_addTest(pSuite, RemotingTest, testOneWay);
	CppUnit_addTest(pSuite, RemotingTest, testEvent);
	CppUnit_addTest(pSuite, RemotingTest, testOneWayEvent);
	CppUnit_addTest(pSuite, RemotingTest, testVoidEvent);
	CppUnit_addTest(pSuite, RemotingTest, testFilteredEvent);
	CppUnit_addTest(pSuite, RemotingTest, testPermissions);

	return pSuite;
}
