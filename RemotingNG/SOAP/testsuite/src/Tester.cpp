//
// Tester.cpp
//
// Copyright (c) 2009-2011, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties, 
// copied or duplicated in any form, in whole or in part.
//


#include "Tester.h"
#include "Poco/Exception.h"
#include "Poco/RemotingNG/Context.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Net/HTTPCookie.h"


const int Class1::A_CONSTANT(1);


int Tester::testInt1(int i)
{
	return i;
}


void Tester::testInt2(int& i)
{
	i = 2112;
}


void Tester::testInt3(int i1, int& i2)
{
	i2 = i1;
}


Enum1 Tester::testEnum11(Enum1 e1)
{
	return e1;
}


void Tester::testEnum12(Enum1& e1)
{
	e1 = VALUE_2;
}


void Tester::testEnum13(Enum1 e1, Enum1& e2)
{
	e2 = e1;
}


Struct1::Enum2 Tester::testEnum21(Struct1::Enum2 e1)
{
	return e1;
}


void Tester::testEnum22(Struct1::Enum2& e1)
{
	e1 = Struct1::VALUE_2;
}


void Tester::testEnum23(Struct1::Enum2 e1, Struct1::Enum2& e2)
{
	e2 = e1;
}


Struct1 Tester::testStruct11(const Struct1& s1)
{
	return s1;
}


void Tester::testStruct12(Struct1& s1)
{
}


void Tester::testStruct13(const Struct1& s1, Struct1& s2)
{
	s2 = s1;
}


Struct2 Tester::testStruct21(const Struct2& s1)
{
	return s1;
}


void Tester::testStruct22(Struct2& s1)
{
}


void Tester::testStruct23(const Struct2& s1, Struct2& s2)
{
	s2 = s1;
}


Struct3 Tester::testStruct31(const Struct3& s1)
{
	return s1;
}


void Tester::testStruct32(Struct3& s1)
{
}


void Tester::testStruct33(const Struct3& s1, Struct3& s2)
{
	s2 = s1;
}


Struct4 Tester::testStruct41(const Struct4& s1)
{
	return s1;
}


void Tester::testStruct42(Struct4& s1)
{
}


void Tester::testStruct43(const Struct4& s1, Struct4& s2)
{
	s2 = s1;
}


Struct5 Tester::testStruct51(const Struct5& s1)
{
	return s1;
}


void Tester::testStruct52(Struct5& s1)
{
}


void Tester::testStruct53(const Struct5& s1, Struct5& s2)
{
	s2 = s1;
}


Class1 Tester::testClass11(const Class1& c1)
{
	return c1;
}


void Tester::testClass12(Class1& c1)
{
}


void Tester::testClass13(const Class1& c1, Class1& c2)
{
	c2 = c1;
}


Struct1::Ptr Tester::testPtr(Struct1::Ptr p1)
{
	return p1;
}


Struct1Vec Tester::testStruct1Vec1(const Struct1Vec& vec)
{
	return vec;
}


void Tester::testStruct1Vec2(Struct1Vec& vec)
{
}


void Tester::testStruct1Vec3(const Struct1Vec& vec1, Struct1Vec& vec2)
{
	vec2 = vec1;
}


void Tester::testOneWay(const std::string& arg)
{
	_oneWayResult = arg;
}


std::string Tester::testOneWayResult()
{
	return _oneWayResult;
}


Poco::Timestamp::TimeVal Tester::testNestedType(Poco::Timestamp::TimeVal arg)
{
	return arg;
}


void Tester::testSynchronized()
{
}


void Tester::testSynchronizedProxy()
{
}


void Tester::testSynchronizedRemote()
{
}


void Tester::testFault()
{
	throw Poco::ApplicationException("Something went wrong");
}


void Tester::setCookie()
{
	Poco::RemotingNG::Context::Ptr pContext = Poco::RemotingNG::Context::get();
	Poco::Net::HTTPServerResponse* pResponse = pContext->getValue<Poco::Net::HTTPServerResponse*>("httpResponse");
	Poco::Net::HTTPCookie cookie("testcookie", "testvalue");
	cookie.setMaxAge(5);
	pResponse->addCookie(cookie);
}


bool Tester::verifyCookie()
{
	Poco::RemotingNG::Context::Ptr pContext = Poco::RemotingNG::Context::get();
	Poco::Net::HTTPServerRequest* pRequest = pContext->getValue<Poco::Net::HTTPServerRequest*>("httpRequest");
	Poco::Net::NameValueCollection cookies;
	pRequest->getCookies(cookies);
	return cookies.get("testcookie", "") == "testvalue";
}


void Tester::testAuthenticated()
{
}


void Tester::testPermission()
{
}
