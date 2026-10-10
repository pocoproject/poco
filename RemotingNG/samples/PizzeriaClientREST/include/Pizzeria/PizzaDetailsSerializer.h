//
// PizzaDetailsSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//
// Copyright (c) 2016-2020, Applied Informatics Software Engineering GmbH.
//


#ifndef TypeSerializer_Pizzeria_Pizza_Details_INCLUDED
#define TypeSerializer_Pizzeria_Pizza_Details_INCLUDED


#include "Pizzeria/Pizza.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<Pizzeria::Pizza::Details>
{
public:
	static void serialize(const std::string& name, const Pizzeria::Pizza::Details& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		ser.serializeStructBegin(name);
		serializeImpl(value, ser);
		ser.serializeStructEnd(name);
	}

	static void serializeImpl(const Pizzeria::Pizza::Details& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"calories"s,"carboHydrates"s,"percentFat"s,""s};
		TypeSerializer<double >::serialize(REMOTING__NAMES[0], value.calories, ser);
		TypeSerializer<double >::serialize(REMOTING__NAMES[1], value.carboHydrates, ser);
		TypeSerializer<double >::serialize(REMOTING__NAMES[2], value.percentFat, ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_Pizzeria_Pizza_Details_INCLUDED

