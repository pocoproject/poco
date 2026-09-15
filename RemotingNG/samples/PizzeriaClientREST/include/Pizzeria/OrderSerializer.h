//
// OrderSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//
// Copyright (c) 2016-2020, Applied Informatics Software Engineering GmbH.
//


#ifndef TypeSerializer_Pizzeria_Order_INCLUDED
#define TypeSerializer_Pizzeria_Order_INCLUDED


#include "Pizzeria/DeliveryAddressDeserializer.h"
#include "Pizzeria/DeliveryAddressSerializer.h"
#include "Pizzeria/Order.h"
#include "Pizzeria/PizzaDeserializer.h"
#include "Pizzeria/PizzaSerializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<Pizzeria::Order>
{
public:
	static void serialize(const std::string& name, const Pizzeria::Order& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		ser.serializeStructBegin(name);
		serializeImpl(value, ser);
		ser.serializeStructEnd(name);
	}

	static void serializeImpl(const Pizzeria::Order& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"deliveryAddress"s,"deliveryTime"s,"orderNumber"s,"pizza"s,""s};
		TypeSerializer<Pizzeria::DeliveryAddress >::serialize(REMOTING__NAMES[0], value.deliveryAddress, ser);
		TypeSerializer<Poco::Nullable < Poco::DateTime > >::serialize(REMOTING__NAMES[1], value.deliveryTime, ser);
		TypeSerializer<Poco::Optional < int > >::serialize(REMOTING__NAMES[2], value.orderNumber, ser);
		TypeSerializer<Pizzeria::Pizza >::serialize(REMOTING__NAMES[3], value.pizza, ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_Pizzeria_Order_INCLUDED

