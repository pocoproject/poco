//
// PizzaDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//
// Copyright (c) 2016-2020, Applied Informatics Software Engineering GmbH.
//


#ifndef TypeDeserializer_Pizzeria_Pizza_INCLUDED
#define TypeDeserializer_Pizzeria_Pizza_INCLUDED


#include "Pizzeria/ExtToppingDeserializer.h"
#include "Pizzeria/ExtToppingSerializer.h"
#include "Pizzeria/Pizza.h"
#include "Pizzeria/PizzaDetailsDeserializer.h"
#include "Pizzeria/PizzaDetailsSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<Pizzeria::Pizza>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, Pizzeria::Pizza& value)
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

	static void deserializeImpl(Deserializer& deser, Pizzeria::Pizza& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"name"s,"toppings"s,"basePrice"s,"details"s};
		bool ret = false;
		std::string gen_name;
		ret = TypeDeserializer<std::string>::deserialize(REMOTING__NAMES[0], true, deser, gen_name);
		if (ret) value.setName(gen_name);
		std::vector<Pizzeria::ExtTopping> gen_toppings;
		ret = TypeDeserializer<std::vector<Pizzeria::ExtTopping>>::deserialize(REMOTING__NAMES[1], true, deser, gen_toppings);
		if (ret) value.setToppings(gen_toppings);
		Poco::UInt32 gen_basePrice;
		ret = TypeDeserializer<Poco::UInt32>::deserialize(REMOTING__NAMES[2], true, deser, gen_basePrice);
		if (ret) value.setBasePrice(gen_basePrice);
		Pizzeria::Pizza::Details gen_details;
		ret = TypeDeserializer<Pizzeria::Pizza::Details>::deserialize(REMOTING__NAMES[3], true, deser, gen_details);
		if (ret) value.setDetails(gen_details);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_Pizzeria_Pizza_INCLUDED

