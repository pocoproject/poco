//
// MoveOptions20Deserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_MoveOptions20_INCLUDED
#define TypeDeserializer_ONVIF_MoveOptions20_INCLUDED


#include "ONVIF/AbsoluteFocusOptionsDeserializer.h"
#include "ONVIF/AbsoluteFocusOptionsSerializer.h"
#include "ONVIF/ContinuousFocusOptionsDeserializer.h"
#include "ONVIF/ContinuousFocusOptionsSerializer.h"
#include "ONVIF/MoveOptions20.h"
#include "ONVIF/RelativeFocusOptions20Deserializer.h"
#include "ONVIF/RelativeFocusOptions20Serializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::MoveOptions20>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::MoveOptions20& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMESPACE("http://www.onvif.org/ver10/schema"s);
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

	static void deserializeImpl(Deserializer& deser, ONVIF::MoveOptions20& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Absolute"s,"Relative"s,"Continuous"s};
		bool ret = false;
		Poco::SharedPtr<ONVIF::AbsoluteFocusOptions> gen_absolute;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::AbsoluteFocusOptions>>::deserialize(REMOTING__NAMES[0], false, deser, gen_absolute);
		if (ret) value.setAbsolute(gen_absolute);
		Poco::SharedPtr<ONVIF::RelativeFocusOptions20> gen_relative;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::RelativeFocusOptions20>>::deserialize(REMOTING__NAMES[1], false, deser, gen_relative);
		if (ret) value.setRelative(gen_relative);
		Poco::SharedPtr<ONVIF::ContinuousFocusOptions> gen_continuous;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::ContinuousFocusOptions>>::deserialize(REMOTING__NAMES[2], false, deser, gen_continuous);
		if (ret) value.setContinuous(gen_continuous);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_MoveOptions20_INCLUDED

