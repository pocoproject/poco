//
// TesterProxy.h
//
// Package: Generated
// Module:  TesterProxy
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TesterProxy_INCLUDED
#define TesterProxy_INCLUDED


#include "ITester.h"
#include "Poco/RemotingNG/EventListener.h"
#include "Poco/RemotingNG/EventSubscriber.h"
#include "Poco/RemotingNG/Proxy.h"


class TesterProxy: public ITester, public Poco::RemotingNG::Proxy
{
public:
	using Ptr = Poco::AutoPtr<TesterProxy>;

	TesterProxy(const Poco::RemotingNG::Identifiable::ObjectId& oid);
		/// Creates a TesterProxy.

	virtual ~TesterProxy();
		/// Destroys the TesterProxy.

	void fireTestEvent(const std::string& arg);

	void fireTestFilteredEvent(int arg);

	void fireTestOneWayEvent(const std::string& arg);

	void fireTestVoidEvent();

	virtual std::string remoting__enableEvents(Poco::RemotingNG::Listener::Ptr pListener, bool enable = bool(true));

	virtual const Poco::RemotingNG::Identifiable::TypeId& remoting__typeId() const;

	void testArgNamespace(const Struct3& s31, Struct3& s32);

	Poco::Array<int, 4> testArray(const Poco::Array<int, 4>& arr);

	Class1 testClass11(const Class1& c1);

	void testClass12(Class1& c1);

	void testClass13(const Class1& c1, Class1& c2);

	Enum1 testEnum11(Enum1 e1);

	void testEnum12(Enum1& e1);

	void testEnum13(Enum1 e1, Enum1& e2);

	Struct1::Enum2 testEnum21(Struct1::Enum2 e1);

	void testEnum22(Struct1::Enum2& e1);

	void testEnum23(Struct1::Enum2 e1, Struct1::Enum2& e2);

	int testInt1(int i);

	void testInt2(int& i);

	void testInt3(int i1, int& i2);

	Poco::Int64 testNestedType(Poco::Int64 arg);

	void testOneWay(const std::string& arg);

	std::string testOneWayResult();

	void testPermission1();

	void testPermission2();

	Poco::Optional<std::string> testPocoOptional(const Poco::Optional<std::string>& value);

	Poco::SharedPtr<Struct1> testPtr(Poco::SharedPtr<Struct1> p1);

	std::array<int, 4> testStdArray(const std::array<int, 4>& arr);

	std::shared_ptr<Struct1> testStdSharedPtr(const std::shared_ptr<Struct1>& pStruct1);

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

	Struct6 testStruct6(const Struct6& s1);

	Struct7 testStruct7(const Struct7& s1);

	Struct8 testStruct8(const Struct8& value);

	void testSynchronized();

	void testSynchronizedProxy();

	void testSynchronizedRemote();

private:
	static const std::string DEFAULT_NS;
	Poco::RemotingNG::EventListener::Ptr _pEventListener;
	Poco::RemotingNG::EventSubscriber::Ptr _pEventSubscriber;
	mutable Poco::Array<int, 4> _testArrayRet;
	mutable Class1 _testClass11Ret;
	mutable Enum1 _testEnum11Ret;
	mutable Struct1::Enum2 _testEnum21Ret;
	mutable int _testInt1Ret;
	mutable Poco::Int64 _testNestedTypeRet;
	mutable std::string _testOneWayResultRet;
	mutable Poco::Optional<std::string> _testPocoOptionalRet;
	mutable Poco::SharedPtr<Struct1> _testPtrRet;
	mutable std::array<int, 4> _testStdArrayRet;
	mutable std::shared_ptr<Struct1> _testStdSharedPtrRet;
	mutable Struct1 _testStruct11Ret;
	mutable std::vector<Struct1> _testStruct1Vec1Ret;
	mutable Struct2 _testStruct21Ret;
	mutable Struct3 _testStruct31Ret;
	mutable Struct4 _testStruct41Ret;
	mutable Struct5 _testStruct51Ret;
	mutable Struct6 _testStruct6Ret;
	mutable Struct7 _testStruct7Ret;
	mutable Struct8 _testStruct8Ret;
};


inline const Poco::RemotingNG::Identifiable::TypeId& TesterProxy::remoting__typeId() const
{
	return ITester::remoting__typeId();
}




#endif // TesterProxy_INCLUDED

