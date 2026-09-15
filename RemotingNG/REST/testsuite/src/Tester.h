//
// Tester.h
//
// Definition of the Tester class.
//
// Copyright (c) 2016, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties, 
// copied or duplicated in any form, in whole or in part.
//


#ifndef Tester_INCLUDED
#define Tester_INCLUDED


#include "Poco/Poco.h"
#include <map>


//@ serialize
struct Object
{
	std::string data;
};


//@ remote
//@ path="/tester/{id}"
class Tester
{
public:
	enum SpecialID
	{
		ID_SET_COOKIE = 0x0C00C1E,
		ID_VERIFY_COOKIE = 0x1C00C1E,
		ID_FAULT = 0x0BADBEEF
	};
	
	//@ $id={in=path}
	Object put(int id, const Object& object);

	//@ $id={in=path}
	Object post(int id, const Object& object);
	
	//@ $id={in=path}
	//@ $options={in=query, mandatory=false}
	Object get(int id, const std::string& options = "none");
	
	//@ $id={in=path}
	void delete_(int id);
	
protected:
	void setCookie();
	bool verifyCookie();

private:
	std::map<int, std::string> _data;
};


//@ remote
//@ path="/authtester/{id}"
//@ authenticated
class AuthTester
{
public:
	//@ $id={in=path}
	//@ permission="perm1"
	Object post(int id, const Object& object);
	
	//@ $id={in=path}
	//@ $options={in=query, mandatory=false}
	Object get(int id, const std::string& options = "none");
};


#endif // Tester_INCLUDED
