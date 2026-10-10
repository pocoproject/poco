//
// DeliveryAddressDetailsDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//
// Copyright (c) 2006-2020, Applied Informatics Software Engineering GmbH.
//


#ifndef TypeDeserializer_Pizzeria_DeliveryAddressDetails_INCLUDED
#define TypeDeserializer_Pizzeria_DeliveryAddressDetails_INCLUDED


#include "Pizzeria/DeliveryAddressDetails.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<Pizzeria::DeliveryAddressDetails>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, Pizzeria::DeliveryAddressDetails& value)
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

	static void deserializeImpl(Deserializer& deser, Pizzeria::DeliveryAddressDetails& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"milesAway"s};
		bool ret = false;
		double gen_milesAway;
		ret = TypeDeserializer<double>::deserialize(REMOTING__NAMES[0], true, deser, gen_milesAway);
		if (ret) value.setMilesAway(gen_milesAway);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_Pizzeria_DeliveryAddressDetails_INCLUDED

