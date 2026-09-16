//
// ToppingSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//
// Copyright (c) 2006-2020, Applied Informatics Software Engineering GmbH.
//


#ifndef TypeSerializer_Pizzeria_Topping_INCLUDED
#define TypeSerializer_Pizzeria_Topping_INCLUDED


#include "Pizzeria/Topping.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<Pizzeria::Topping>
{
public:
	static void serialize(const std::string& name, const Pizzeria::Topping& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMESPACE("http://www.appinf.com/webservices/PizzaDeliveryService/"s);
		ser.registerNamespace(REMOTING__NAMESPACE);
		ser.serializeStructBegin(name);
		ser.pushProperty(SerializerBase::PROP_NAMESPACE, REMOTING__NAMESPACE);
		serializeImpl(value, ser);
		ser.popProperty(SerializerBase::PROP_NAMESPACE);
		ser.serializeStructEnd(name);
	}

	static void serializeImpl(const Pizzeria::Topping& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"name"s,"price"s,""s};
		TypeSerializer<std::string>::serialize(REMOTING__NAMES[0], value.getName(), ser);
		TypeSerializer<Poco::UInt32>::serialize(REMOTING__NAMES[1], value.getPrice(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_Pizzeria_Topping_INCLUDED

