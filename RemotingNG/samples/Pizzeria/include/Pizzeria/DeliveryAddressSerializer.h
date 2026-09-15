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
		
		ser.serializeStructBegin(name);
		serializeImpl(value, ser);
		ser.serializeStructEnd(name);
	}

	static void serializeImpl(const Pizzeria::DeliveryAddress& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"personName"s,"streetName"s,"houseNo"s,"cityName"s,"zip"s,"state"s,"phoneNo"s,"details"s,""s};
		TypeSerializer<std::string>::serialize(REMOTING__NAMES[0], value.getPersonName(), ser);
		TypeSerializer<std::string>::serialize(REMOTING__NAMES[1], value.getStreet(), ser);
		TypeSerializer<Poco::Int16>::serialize(REMOTING__NAMES[2], value.getHouseNumber(), ser);
		TypeSerializer<std::string>::serialize(REMOTING__NAMES[3], value.getCityName(), ser);
		TypeSerializer<std::string>::serialize(REMOTING__NAMES[4], value.getZip(), ser);
		TypeSerializer<std::string>::serialize(REMOTING__NAMES[5], value.getState(), ser);
		TypeSerializer<std::string>::serialize(REMOTING__NAMES[6], value.getPhoneNumber(), ser);
		TypeSerializer<Pizzeria::DeliveryAddress::Details>::serialize(REMOTING__NAMES[7], value.getDetails(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_Pizzeria_DeliveryAddress_INCLUDED

