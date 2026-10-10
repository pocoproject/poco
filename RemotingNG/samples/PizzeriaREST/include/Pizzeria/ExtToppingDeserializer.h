//
// ExtToppingDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//
// Copyright (c) 2016-2020, Applied Informatics Software Engineering GmbH.
//


#ifndef TypeDeserializer_Pizzeria_ExtTopping_INCLUDED
#define TypeDeserializer_Pizzeria_ExtTopping_INCLUDED


#include "Pizzeria/Topping.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<Pizzeria::ExtTopping>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, Pizzeria::ExtTopping& value)
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

	static void deserializeImpl(Deserializer& deser, Pizzeria::ExtTopping& value)
	{
		using namespace std::string_literals;
		
		// Pizzeria::Topping
		{
		static const std::string REMOTING__NAMES__PIZZERIA__TOPPING[] = {"name"s,"price"s};
		bool ret = false;
		std::string gen_name;
		ret = TypeDeserializer<std::string>::deserialize(REMOTING__NAMES__PIZZERIA__TOPPING[0], true, deser, gen_name);
		if (ret) value.setName(gen_name);
		Poco::UInt32 gen_price;
		ret = TypeDeserializer<Poco::UInt32>::deserialize(REMOTING__NAMES__PIZZERIA__TOPPING[1], true, deser, gen_price);
		if (ret) value.setPrice(gen_price);
		}
		
		static const std::string REMOTING__NAMES[] = {"addInfo"s};
		bool ret = false;
		std::string gen_addInfo;
		ret = TypeDeserializer<std::string>::deserialize(REMOTING__NAMES[0], true, deser, gen_addInfo);
		if (ret) value.setAddInfo(gen_addInfo);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_Pizzeria_ExtTopping_INCLUDED

