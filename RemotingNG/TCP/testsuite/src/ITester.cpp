//
// ITester.cpp
//
// Package: Generated
// Module:  ITester
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#include "ITester.h"


ITester::ITester():
	Poco::RefCountedObject(),
	testEvent(),
	testOneWayEvent(),
	testVoidEvent()
{
}


ITester::~ITester()
{
}


const Poco::RemotingNG::Identifiable::TypeId& ITester::remoting__typeId()
{
	static const std::string REMOTING__TYPE_ID("Tester");
	return REMOTING__TYPE_ID;
}


