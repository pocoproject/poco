//
// ITester.h
//
// Package: Generated
// Module:  ITester
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef ITester_INCLUDED
#define ITester_INCLUDED


#include "Poco/AutoPtr.h"
#include "Poco/RefCountedObject.h"
#include "Poco/RemotingNG/Identifiable.h"
#include "Poco/RemotingNG/Listener.h"
#include "Tester.h"


class ITester: public virtual Poco::RefCountedObject
{
public:
	using Ptr = Poco::AutoPtr<ITester>;

	ITester();
		/// Creates a ITester.

	virtual ~ITester();
		/// Destroys the ITester.

	virtual void fireTestEvent(const std::string& arg) = 0;

	virtual void fireTestFilteredEvent(int arg) = 0;

	virtual void fireTestOneWayEvent(const std::string& arg) = 0;

	virtual void fireTestVoidEvent() = 0;

	virtual std::string remoting__enableEvents(Poco::RemotingNG::Listener::Ptr pListener, bool enable = bool(true)) = 0;
		/// Enable or disable delivery of remote events.
		///
		/// The given Listener instance must implement the Poco::RemotingNG::EventListener
		/// interface, otherwise this method will fail with a RemotingException.
		///
		/// This method is only used with Proxy objects; calling this method on a
		/// RemoteObject will do nothing.

	static const Poco::RemotingNG::Identifiable::TypeId& remoting__typeId();
		/// Returns the TypeId of the class.

	virtual void testArgNamespace(const Struct3& s31, Struct3& s32) = 0;

	virtual Poco::Array<int, 4> testArray(const Poco::Array<int, 4>& arr) = 0;

	virtual Class1 testClass11(const Class1& c1) = 0;

	virtual void testClass12(Class1& c1) = 0;

	virtual void testClass13(const Class1& c1, Class1& c2) = 0;

	virtual Enum1 testEnum11(Enum1 e1) = 0;

	virtual void testEnum12(Enum1& e1) = 0;

	virtual void testEnum13(Enum1 e1, Enum1& e2) = 0;

	virtual Struct1::Enum2 testEnum21(Struct1::Enum2 e1) = 0;

	virtual void testEnum22(Struct1::Enum2& e1) = 0;

	virtual void testEnum23(Struct1::Enum2 e1, Struct1::Enum2& e2) = 0;

	virtual int testInt1(int i) = 0;

	virtual void testInt2(int& i) = 0;

	virtual void testInt3(int i1, int& i2) = 0;

	virtual Poco::Int64 testNestedType(Poco::Int64 arg) = 0;

	virtual void testOneWay(const std::string& arg) = 0;

	virtual std::string testOneWayResult() = 0;

	virtual void testPermission1() = 0;

	virtual void testPermission2() = 0;

	virtual Poco::Optional<std::string> testPocoOptional(const Poco::Optional<std::string>& value) = 0;

	virtual Poco::SharedPtr<Struct1> testPtr(Poco::SharedPtr<Struct1> p1) = 0;

	virtual std::array<int, 4> testStdArray(const std::array<int, 4>& arr) = 0;

	virtual std::shared_ptr<Struct1> testStdSharedPtr(const std::shared_ptr<Struct1>& pStruct1) = 0;

	virtual Struct1 testStruct11(const Struct1& s1) = 0;

	virtual void testStruct12(Struct1& s1) = 0;

	virtual void testStruct13(const Struct1& s1, Struct1& s2) = 0;

	virtual std::vector<Struct1> testStruct1Vec1(const std::vector<Struct1>& vec) = 0;

	virtual void testStruct1Vec2(std::vector<Struct1>& vec) = 0;

	virtual void testStruct1Vec3(const std::vector<Struct1>& vec1, std::vector<Struct1>& vec2) = 0;

	virtual Struct2 testStruct21(const Struct2& s1) = 0;

	virtual void testStruct22(Struct2& s1) = 0;

	virtual void testStruct23(const Struct2& s1, Struct2& s2) = 0;

	virtual Struct3 testStruct31(const Struct3& s1) = 0;

	virtual void testStruct32(Struct3& s1) = 0;

	virtual void testStruct33(const Struct3& s1, Struct3& s2) = 0;

	virtual Struct4 testStruct41(const Struct4& s1) = 0;

	virtual void testStruct42(Struct4& s1) = 0;

	virtual void testStruct43(const Struct4& s1, Struct4& s2) = 0;

	virtual Struct5 testStruct51(const Struct5& s1) = 0;

	virtual void testStruct52(Struct5& s1) = 0;

	virtual void testStruct53(const Struct5& s1, Struct5& s2) = 0;

	virtual Struct6 testStruct6(const Struct6& s1) = 0;

	virtual Struct7 testStruct7(const Struct7& s1) = 0;

	virtual Struct8 testStruct8(const Struct8& value) = 0;

	virtual void testSynchronized() = 0;

	virtual void testSynchronizedProxy() = 0;

	virtual void testSynchronizedRemote() = 0;

	Poco::BasicEvent<Enum1> testEnumEvent;
	Poco::BasicEvent<std::string> testEvent;
	Poco::BasicEvent<const int> testFilteredEvent;
	Poco::BasicEvent<std::string> testOneWayEvent;
	Poco::BasicEvent<ScopedEnum> testScopedEnumEvent;
	Poco::BasicEvent<void> testVoidEvent;
};




#endif // ITester_INCLUDED

