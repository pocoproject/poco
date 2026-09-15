//
// FocusMoveDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_FocusMove_INCLUDED
#define TypeDeserializer_ONVIF_FocusMove_INCLUDED


#include "ONVIF/AbsoluteFocusDeserializer.h"
#include "ONVIF/AbsoluteFocusSerializer.h"
#include "ONVIF/ContinuousFocusDeserializer.h"
#include "ONVIF/ContinuousFocusSerializer.h"
#include "ONVIF/FocusMove.h"
#include "ONVIF/RelativeFocusDeserializer.h"
#include "ONVIF/RelativeFocusSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::FocusMove>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::FocusMove& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::FocusMove& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Absolute"s,"Relative"s,"Continuous"s};
		bool ret = false;
		Poco::SharedPtr<ONVIF::AbsoluteFocus> gen_absolute;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::AbsoluteFocus>>::deserialize(REMOTING__NAMES[0], false, deser, gen_absolute);
		if (ret) value.setAbsolute(gen_absolute);
		Poco::SharedPtr<ONVIF::RelativeFocus> gen_relative;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::RelativeFocus>>::deserialize(REMOTING__NAMES[1], false, deser, gen_relative);
		if (ret) value.setRelative(gen_relative);
		Poco::SharedPtr<ONVIF::ContinuousFocus> gen_continuous;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::ContinuousFocus>>::deserialize(REMOTING__NAMES[2], false, deser, gen_continuous);
		if (ret) value.setContinuous(gen_continuous);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_FocusMove_INCLUDED

