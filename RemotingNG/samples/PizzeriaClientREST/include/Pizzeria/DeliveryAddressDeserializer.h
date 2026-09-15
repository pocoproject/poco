//
// DeliveryAddressDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//
// Copyright (c) 2016-2020, Applied Informatics Software Engineering GmbH.
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
		
		bool ret = deser.deserializeStructBegin(name, isMandatory);
		if (ret)
		{
			deserializeImpl(deser, value);
			deser.deserializeStructEnd(name);
		}
		return ret;
	}

	static void deserializeImpl(Deserializer& deser, Pizzeria::DeliveryAddress& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"cityName"s,"details"s,"houseNo"s,"state"s,"streetName"s,"personName"s,"phoneNo"s,"zip"s};
		bool ret = false;
		std::string gen_cityName;
		ret = TypeDeserializer<std::string >::deserialize(REMOTING__NAMES[0], true, deser, gen_cityName);
		if (ret) value.setCityName(gen_cityName);
		Pizzeria::DeliveryAddress::Details gen_details;
		ret = TypeDeserializer<Pizzeria::DeliveryAddress::Details >::deserialize(REMOTING__NAMES[1], false, deser, gen_details);
		if (ret) value.setDetails(gen_details);
		Poco::Int16 gen_housenumber;
		ret = TypeDeserializer<Poco::Int16 >::deserialize(REMOTING__NAMES[2], true, deser, gen_housenumber);
		if (ret) value.setHouseNumber(gen_housenumber);
		std::string gen_state;
		ret = TypeDeserializer<std::string >::deserialize(REMOTING__NAMES[3], true, deser, gen_state);
		if (ret) value.setState(gen_state);
		std::string genm_Street;
		ret = TypeDeserializer<std::string >::deserialize(REMOTING__NAMES[4], true, deser, genm_Street);
		if (ret) value.setStreet(genm_Street);
		std::string genm_personName;
		ret = TypeDeserializer<std::string >::deserialize(REMOTING__NAMES[5], true, deser, genm_personName);
		if (ret) value.setPersonName(genm_personName);
		std::string genphoneNumber;
		ret = TypeDeserializer<std::string >::deserialize(REMOTING__NAMES[6], false, deser, genphoneNumber);
		if (ret) value.setPhoneNumber(genphoneNumber);
		std::string genzip_;
		ret = TypeDeserializer<std::string >::deserialize(REMOTING__NAMES[7], true, deser, genzip_);
		if (ret) value.setZip(genzip_);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_Pizzeria_DeliveryAddress_INCLUDED

