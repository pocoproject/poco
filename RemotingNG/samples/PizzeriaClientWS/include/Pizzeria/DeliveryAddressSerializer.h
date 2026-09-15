//
// DeliveryAddressSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//
// Copyright (c) 2006-2020, Applied Informatics Software Engineering GmbH.
//


#ifndef TypeSerializer_Pizzeria_DeliveryAddress_INCLUDED
#define TypeSerializer_Pizzeria_DeliveryAddress_INCLUDED


#include "Pizzeria/DeliveryAddress.h"
#include "Pizzeria/DeliveryAddressDetailsDeserializer.h"
#include "Pizzeria/DeliveryAddressDetailsSerializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<Pizzeria::DeliveryAddress>
{
public:
	static void serialize(const std::string& name, const Pizzeria::DeliveryAddress& value, Serializer& ser)
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

	static void serializeImpl(const Pizzeria::DeliveryAddress& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"cityName"s,"details"s,"houseNo"s,"state"s,"streetName"s,"personName"s,"phoneNo"s,"zip"s,""s};
		TypeSerializer<std::string>::serialize(REMOTING__NAMES[0], value.getCityName(), ser);
		TypeSerializer<Pizzeria::DeliveryAddressDetails>::serialize(REMOTING__NAMES[1], value.getDetails(), ser);
		TypeSerializer<Poco::Int16>::serialize(REMOTING__NAMES[2], value.getHouseNo(), ser);
		TypeSerializer<std::string>::serialize(REMOTING__NAMES[3], value.getState(), ser);
		TypeSerializer<std::string>::serialize(REMOTING__NAMES[4], value.getStreetName(), ser);
		TypeSerializer<std::string>::serialize(REMOTING__NAMES[5], value.getPersonName(), ser);
		TypeSerializer<std::string>::serialize(REMOTING__NAMES[6], value.getPhoneNo(), ser);
		TypeSerializer<std::string>::serialize(REMOTING__NAMES[7], value.getZip(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_Pizzeria_DeliveryAddress_INCLUDED

