//
// DeliveryAddressDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//
// Copyright (c) 2006-2020, Applied Informatics Software Engineering GmbH.
//


#ifndef TypeDeserializer_Pizzeria_DeliveryAddress_INCLUDED
#define TypeDeserializer_Pizzeria_DeliveryAddress_INCLUDED


#include "Pizzeria/DeliveryAddress.h"
#include "Pizzeria/DeliveryAddressDetailsDeserializer.h"
#include "Pizzeria/DeliveryAddressDetailsSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<Pizzeria::DeliveryAddress>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, Pizzeria::DeliveryAddress& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMESPACE("http://www.appinf.com/webservices/PizzaDeliveryService/"s);
		bool ret = deser.deserializeStructBegin(name, isMandatory);
		deser.pushProperty(SerializerBase::PROP_NAMESPACE, REMOTING__NAMESPACE);
		if (ret)
		{
			deserializeImpl(deser, value);
			deser.popProperty(SerializerBase::PROP_NAMESPACE);
			deser.deserializeStructEnd(name);
		}
		else deser.popProperty(SerializerBase::PROP_NAMESPACE);
		return ret;
	}

	static void deserializeImpl(Deserializer& deser, Pizzeria::DeliveryAddress& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"cityName"s,"details"s,"houseNo"s,"state"s,"streetName"s,"personName"s,"phoneNo"s,"zip"s};
		bool ret = false;
		std::string gen_cityName;
		ret = TypeDeserializer<std::string>::deserialize(REMOTING__NAMES[0], true, deser, gen_cityName);
		if (ret) value.setCityName(gen_cityName);
		Pizzeria::DeliveryAddressDetails gen_details;
		ret = TypeDeserializer<Pizzeria::DeliveryAddressDetails>::deserialize(REMOTING__NAMES[1], true, deser, gen_details);
		if (ret) value.setDetails(gen_details);
		Poco::Int16 gen_houseNo;
		ret = TypeDeserializer<Poco::Int16>::deserialize(REMOTING__NAMES[2], true, deser, gen_houseNo);
		if (ret) value.setHouseNo(gen_houseNo);
		std::string gen_state;
		ret = TypeDeserializer<std::string>::deserialize(REMOTING__NAMES[3], true, deser, gen_state);
		if (ret) value.setState(gen_state);
		std::string gen_streetName;
		ret = TypeDeserializer<std::string>::deserialize(REMOTING__NAMES[4], true, deser, gen_streetName);
		if (ret) value.setStreetName(gen_streetName);
		std::string gen_personName;
		ret = TypeDeserializer<std::string>::deserialize(REMOTING__NAMES[5], true, deser, gen_personName);
		if (ret) value.setPersonName(gen_personName);
		std::string gen_phoneNo;
		ret = TypeDeserializer<std::string>::deserialize(REMOTING__NAMES[6], true, deser, gen_phoneNo);
		if (ret) value.setPhoneNo(gen_phoneNo);
		std::string gen_zip;
		ret = TypeDeserializer<std::string>::deserialize(REMOTING__NAMES[7], true, deser, gen_zip);
		if (ret) value.setZip(gen_zip);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_Pizzeria_DeliveryAddress_INCLUDED

