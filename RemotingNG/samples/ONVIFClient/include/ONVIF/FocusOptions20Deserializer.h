//
// FocusOptions20Deserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_FocusOptions20_INCLUDED
#define TypeDeserializer_ONVIF_FocusOptions20_INCLUDED


#include "ONVIF/FloatRangeDeserializer.h"
#include "ONVIF/FloatRangeSerializer.h"
#include "ONVIF/FocusOptions20.h"
#include "ONVIF/FocusOptions20ExtensionDeserializer.h"
#include "ONVIF/FocusOptions20ExtensionSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::FocusOptions20>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::FocusOptions20& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::FocusOptions20& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"AutoFocusModes"s,"DefaultSpeed"s,"NearLimit"s,"FarLimit"s,"Extension"s};
		bool ret = false;
		std::vector<std::string> gen_autoFocusModes;
		ret = TypeDeserializer<std::vector<std::string>>::deserialize(REMOTING__NAMES[0], false, deser, gen_autoFocusModes);
		if (ret) value.setAutoFocusModes(gen_autoFocusModes);
		Poco::SharedPtr<ONVIF::FloatRange> gen_defaultSpeed;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::FloatRange>>::deserialize(REMOTING__NAMES[1], false, deser, gen_defaultSpeed);
		if (ret) value.setDefaultSpeed(gen_defaultSpeed);
		Poco::SharedPtr<ONVIF::FloatRange> gen_nearLimit;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::FloatRange>>::deserialize(REMOTING__NAMES[2], false, deser, gen_nearLimit);
		if (ret) value.setNearLimit(gen_nearLimit);
		Poco::SharedPtr<ONVIF::FloatRange> gen_farLimit;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::FloatRange>>::deserialize(REMOTING__NAMES[3], false, deser, gen_farLimit);
		if (ret) value.setFarLimit(gen_farLimit);
		Poco::SharedPtr<ONVIF::FocusOptions20Extension> gen_extension;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::FocusOptions20Extension>>::deserialize(REMOTING__NAMES[4], false, deser, gen_extension);
		if (ret) value.setExtension(gen_extension);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_FocusOptions20_INCLUDED

