//
// ExtToppingSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//
// Copyright (c) 2016-2020, Applied Informatics Software Engineering GmbH.
//


#ifndef TypeSerializer_Pizzeria_ExtTopping_INCLUDED
#define TypeSerializer_Pizzeria_ExtTopping_INCLUDED


#include "Pizzeria/Topping.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<Pizzeria::ExtTopping>
{
public:
	static void serialize(const std::string& name, const Pizzeria::ExtTopping& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		ser.serializeStructBegin(name);
		serializeImpl(value, ser);
		ser.serializeStructEnd(name);
	}

	static void serializeImpl(const Pizzeria::ExtTopping& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		// Pizzeria::Topping
		{
		static const std::string REMOTING__NAMES__PIZZERIA__TOPPING[] = {"name"s,"price"s,""s};
		TypeSerializer<std::string>::serialize(REMOTING__NAMES__PIZZERIA__TOPPING[0], value.getName(), ser);
		TypeSerializer<Poco::UInt32>::serialize(REMOTING__NAMES__PIZZERIA__TOPPING[1], value.getPrice(), ser);
		}
		
		static const std::string REMOTING__NAMES[] = {"addInfo"s,""s};
		TypeSerializer<std::string>::serialize(REMOTING__NAMES[0], value.getAddInfo(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_Pizzeria_ExtTopping_INCLUDED

