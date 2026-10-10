//
// PTZPresetTourStatusDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_PTZPresetTourStatus_INCLUDED
#define TypeDeserializer_ONVIF_PTZPresetTourStatus_INCLUDED


#include "ONVIF/PTZPresetTourSpotDeserializer.h"
#include "ONVIF/PTZPresetTourSpotSerializer.h"
#include "ONVIF/PTZPresetTourStatus.h"
#include "ONVIF/PTZPresetTourStatusExtensionDeserializer.h"
#include "ONVIF/PTZPresetTourStatusExtensionSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::PTZPresetTourStatus>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::PTZPresetTourStatus& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::PTZPresetTourStatus& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"State"s,"CurrentTourSpot"s,"Extension"s};
		bool ret = false;
		std::string gen_state;
		ret = TypeDeserializer<std::string>::deserialize(REMOTING__NAMES[0], true, deser, gen_state);
		if (ret) value.setState(gen_state);
		Poco::SharedPtr<ONVIF::PTZPresetTourSpot> gen_currentTourSpot;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::PTZPresetTourSpot>>::deserialize(REMOTING__NAMES[1], false, deser, gen_currentTourSpot);
		if (ret) value.setCurrentTourSpot(gen_currentTourSpot);
		Poco::SharedPtr<ONVIF::PTZPresetTourStatusExtension> gen_extension;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::PTZPresetTourStatusExtension>>::deserialize(REMOTING__NAMES[2], false, deser, gen_extension);
		if (ret) value.setExtension(gen_extension);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_PTZPresetTourStatus_INCLUDED

