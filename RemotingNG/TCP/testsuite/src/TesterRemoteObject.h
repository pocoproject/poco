//
// TesterRemoteObject.h
//
// Package: Generated
// Module:  TesterRemoteObject
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TesterRemoteObject_INCLUDED
#define TesterRemoteObject_INCLUDED


#include "ITester.h"
#include "Poco/RemotingNG/Identifiable.h"
#include "Poco/RemotingNG/RemoteObject.h"
#include "Poco/SharedPtr.h"


class TesterRemoteObject: public ITester, public Poco::RemotingNG::RemoteObject
{
public:
	using Ptr = Poco::AutoPtr<TesterRemoteObject>;

	TesterRemoteObject(const Poco::RemotingNG::Identifiable::ObjectId& oid, Poco::SharedPtr<Tester> pServiceObject);
		/// Creates a TesterRemoteObject.

	virtual ~TesterRemoteObject();
		/// Destroys the TesterRemoteObject.

	void fireTestEvent(const std::string& arg);

	void fireTestOneWayEvent(const std::string& arg);

	void fireTestVoidEvent();

	virtual std::string remoting__enableEvents(Poco::RemotingNG::Listener::Ptr pListener, bool enable = bool(true));

	virtual void remoting__enableRemoteEvents(const std::string& protocol);

	virtual bool remoting__hasEvents() const;

	virtual const Poco::RemotingNG::Identifiable::TypeId& remoting__typeId() const;

	void testAuthenticated();

	Class1 testClass11(const Class1& c1);

	void testClass12(Class1& c1);

	void testClass13(const Class1& c1, Class1& c2);

	Enum1 testEnum11(Enum1 e1);

	void testEnum12(Enum1& e1);

	void testEnum13(Enum1 e1, Enum1& e2);

	Struct1::Enum2 testEnum21(Struct1::Enum2 e1);

	void testEnum22(Struct1::Enum2& e1);

	void testEnum23(Struct1::Enum2 e1, Struct1::Enum2& e2);

	void testFault();

	int testInt1(int i);

	void testInt2(int& i);

	void testInt3(int i1, int& i2);

	Poco::Int64 testNestedType(Poco::Int64 arg);

	void testOneWay(const std::string& arg);

	std::string testOneWayResult();

	void testPermission();

	Poco::SharedPtr<Struct1> testPtr(Poco::SharedPtr<Struct1> p1);

	Struct1 testStruct11(const Struct1& s1);

	void testStruct12(Struct1& s1);

	void testStruct13(const Struct1& s1, Struct1& s2);

	std::vector<Struct1> testStruct1Vec1(const std::vector<Struct1>& vec);

	void testStruct1Vec2(std::vector<Struct1>& vec);

	void testStruct1Vec3(const std::vector<Struct1>& vec1, std::vector<Struct1>& vec2);

	Struct2 testStruct21(const Struct2& s1);

	void testStruct22(Struct2& s1);

	void testStruct23(const Struct2& s1, Struct2& s2);

	Struct3 testStruct31(const Struct3& s1);

	void testStruct32(Struct3& s1);

	void testStruct33(const Struct3& s1, Struct3& s2);

	Struct4 testStruct41(const Struct4& s1);

	void testStruct42(Struct4& s1);

	void testStruct43(const Struct4& s1, Struct4& s2);

	Struct5 testStruct51(const Struct5& s1);

	void testStruct52(Struct5& s1);

	void testStruct53(const Struct5& s1, Struct5& s2);

	void testSynchronized();

	void testSynchronizedProxy();

	void testSynchronizedRemote();

	void testTimeout();

protected:
	void event__testEvent(std::string& data);

	void event__testOneWayEvent(std::string& data);

	void event__testVoidEvent();

private:
	Poco::SharedPtr<Tester> _pServiceObject;
};


inline void TesterRemoteObject::fireTestEvent(const std::string& arg)
{
	_pServiceObject->fireTestEvent(arg);
}


inline void TesterRemoteObject::fireTestOneWayEvent(const std::string& arg)
{
	_pServiceObject->fireTestOneWayEvent(arg);
}


inline void TesterRemoteObject::fireTestVoidEvent()
{
	_pServiceObject->fireTestVoidEvent();
}


inline const Poco::RemotingNG::Identifiable::TypeId& TesterRemoteObject::remoting__typeId() const
{
	return ITester::remoting__typeId();
}


inline void TesterRemoteObject::testAuthenticated()
{
	_pServiceObject->testAuthenticated();
}


inline Class1 TesterRemoteObject::testClass11(const Class1& c1)
{
	return _pServiceObject->testClass11(c1);
}


inline void TesterRemoteObject::testClass12(Class1& c1)
{
	_pServiceObject->testClass12(c1);
}


inline void TesterRemoteObject::testClass13(const Class1& c1, Class1& c2)
{
	_pServiceObject->testClass13(c1, c2);
}


inline Enum1 TesterRemoteObject::testEnum11(Enum1 e1)
{
	return _pServiceObject->testEnum11(e1);
}


inline void TesterRemoteObject::testEnum12(Enum1& e1)
{
	_pServiceObject->testEnum12(e1);
}


inline void TesterRemoteObject::testEnum13(Enum1 e1, Enum1& e2)
{
	_pServiceObject->testEnum13(e1, e2);
}


inline Struct1::Enum2 TesterRemoteObject::testEnum21(Struct1::Enum2 e1)
{
	return _pServiceObject->testEnum21(e1);
}


inline void TesterRemoteObject::testEnum22(Struct1::Enum2& e1)
{
	_pServiceObject->testEnum22(e1);
}


inline void TesterRemoteObject::testEnum23(Struct1::Enum2 e1, Struct1::Enum2& e2)
{
	_pServiceObject->testEnum23(e1, e2);
}


inline void TesterRemoteObject::testFault()
{
	_pServiceObject->testFault();
}


inline int TesterRemoteObject::testInt1(int i)
{
	return _pServiceObject->testInt1(i);
}


inline void TesterRemoteObject::testInt2(int& i)
{
	_pServiceObject->testInt2(i);
}


inline void TesterRemoteObject::testInt3(int i1, int& i2)
{
	_pServiceObject->testInt3(i1, i2);
}


inline Poco::Int64 TesterRemoteObject::testNestedType(Poco::Int64 arg)
{
	return _pServiceObject->testNestedType(arg);
}


inline void TesterRemoteObject::testOneWay(const std::string& arg)
{
	_pServiceObject->testOneWay(arg);
}


inline std::string TesterRemoteObject::testOneWayResult()
{
	return _pServiceObject->testOneWayResult();
}


inline void TesterRemoteObject::testPermission()
{
	_pServiceObject->testPermission();
}


inline Poco::SharedPtr<Struct1> TesterRemoteObject::testPtr(Poco::SharedPtr<Struct1> p1)
{
	return _pServiceObject->testPtr(p1);
}


inline Struct1 TesterRemoteObject::testStruct11(const Struct1& s1)
{
	return _pServiceObject->testStruct11(s1);
}


inline void TesterRemoteObject::testStruct12(Struct1& s1)
{
	_pServiceObject->testStruct12(s1);
}


inline void TesterRemoteObject::testStruct13(const Struct1& s1, Struct1& s2)
{
	_pServiceObject->testStruct13(s1, s2);
}


inline std::vector<Struct1> TesterRemoteObject::testStruct1Vec1(const std::vector<Struct1>& vec)
{
	return _pServiceObject->testStruct1Vec1(vec);
}


inline void TesterRemoteObject::testStruct1Vec2(std::vector<Struct1>& vec)
{
	_pServiceObject->testStruct1Vec2(vec);
}


inline void TesterRemoteObject::testStruct1Vec3(const std::vector<Struct1>& vec1, std::vector<Struct1>& vec2)
{
	_pServiceObject->testStruct1Vec3(vec1, vec2);
}


inline Struct2 TesterRemoteObject::testStruct21(const Struct2& s1)
{
	return _pServiceObject->testStruct21(s1);
}


inline void TesterRemoteObject::testStruct22(Struct2& s1)
{
	_pServiceObject->testStruct22(s1);
}


inline void TesterRemoteObject::testStruct23(const Struct2& s1, Struct2& s2)
{
	_pServiceObject->testStruct23(s1, s2);
}


inline Struct3 TesterRemoteObject::testStruct31(const Struct3& s1)
{
	return _pServiceObject->testStruct31(s1);
}


inline void TesterRemoteObject::testStruct32(Struct3& s1)
{
	_pServiceObject->testStruct32(s1);
}


inline void TesterRemoteObject::testStruct33(const Struct3& s1, Struct3& s2)
{
	_pServiceObject->testStruct33(s1, s2);
}


inline Struct4 TesterRemoteObject::testStruct41(const Struct4& s1)
{
	return _pServiceObject->testStruct41(s1);
}


inline void TesterRemoteObject::testStruct42(Struct4& s1)
{
	_pServiceObject->testStruct42(s1);
}


inline void TesterRemoteObject::testStruct43(const Struct4& s1, Struct4& s2)
{
	_pServiceObject->testStruct43(s1, s2);
}


inline Struct5 TesterRemoteObject::testStruct51(const Struct5& s1)
{
	return _pServiceObject->testStruct51(s1);
}


inline void TesterRemoteObject::testStruct52(Struct5& s1)
{
	_pServiceObject->testStruct52(s1);
}


inline void TesterRemoteObject::testStruct53(const Struct5& s1, Struct5& s2)
{
	_pServiceObject->testStruct53(s1, s2);
}


inline void TesterRemoteObject::testSynchronized()
{
	Poco::FastMutex::ScopedLock lock(mutex());
	_pServiceObject->testSynchronized();
}


inline void TesterRemoteObject::testSynchronizedProxy()
{
	_pServiceObject->testSynchronizedProxy();
}


inline void TesterRemoteObject::testSynchronizedRemote()
{
	Poco::FastMutex::ScopedLock lock(mutex());
	_pServiceObject->testSynchronizedRemote();
}


inline void TesterRemoteObject::testTimeout()
{
	_pServiceObject->testTimeout();
}




#endif // TesterRemoteObject_INCLUDED

