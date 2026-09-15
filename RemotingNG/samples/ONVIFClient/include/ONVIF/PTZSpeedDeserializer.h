//
// PTZSpeedDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_PTZSpeed_INCLUDED
#define TypeDeserializer_ONVIF_PTZSpeed_INCLUDED


#include "ONVIF/PTZSpeed.h"
#include "ONVIF/Vector1DDeserializer.h"
#include "ONVIF/Vector1DSerializer.h"
#include "ONVIF/Vector2DDeserializer.h"
#include "ONVIF/Vector2DSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::PTZSpeed>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::PTZSpeed& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::PTZSpeed& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"PanTilt"s,"Zoom"s};
		bool ret = false;
		Poco::SharedPtr<ONVIF::Vector2D> gen_panTilt;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::Vector2D>>::deserialize(REMOTING__NAMES[0], false, deser, gen_panTilt);
		if (ret) value.setPanTilt(gen_panTilt);
		Poco::SharedPtr<ONVIF::Vector1D> gen_zoom;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::Vector1D>>::deserialize(REMOTING__NAMES[1], false, deser, gen_zoom);
		if (ret) value.setZoom(gen_zoom);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_PTZSpeed_INCLUDED

