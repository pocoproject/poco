//
// PTZStatusFilterOptionsDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_PTZStatusFilterOptions_INCLUDED
#define TypeDeserializer_ONVIF_PTZStatusFilterOptions_INCLUDED


#include "ONVIF/PTZStatusFilterOptions.h"
#include "ONVIF/PTZStatusFilterOptionsExtensionDeserializer.h"
#include "ONVIF/PTZStatusFilterOptionsExtensionSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::PTZStatusFilterOptions>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::PTZStatusFilterOptions& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::PTZStatusFilterOptions& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"PanTiltStatusSupported"s,"ZoomStatusSupported"s,"PanTiltPositionSupported"s,"ZoomPositionSupported"s,"Extension"s};
		bool ret = false;
		bool gen_panTiltStatusSupported;
		ret = TypeDeserializer<bool>::deserialize(REMOTING__NAMES[0], true, deser, gen_panTiltStatusSupported);
		if (ret) value.setPanTiltStatusSupported(gen_panTiltStatusSupported);
		bool gen_zoomStatusSupported;
		ret = TypeDeserializer<bool>::deserialize(REMOTING__NAMES[1], true, deser, gen_zoomStatusSupported);
		if (ret) value.setZoomStatusSupported(gen_zoomStatusSupported);
		Poco::Optional<bool> gen_panTiltPositionSupported;
		ret = TypeDeserializer<Poco::Optional<bool>>::deserialize(REMOTING__NAMES[2], false, deser, gen_panTiltPositionSupported);
		if (ret) value.setPanTiltPositionSupported(gen_panTiltPositionSupported);
		Poco::Optional<bool> gen_zoomPositionSupported;
		ret = TypeDeserializer<Poco::Optional<bool>>::deserialize(REMOTING__NAMES[3], false, deser, gen_zoomPositionSupported);
		if (ret) value.setZoomPositionSupported(gen_zoomPositionSupported);
		Poco::SharedPtr<ONVIF::PTZStatusFilterOptionsExtension> gen_extension;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::PTZStatusFilterOptionsExtension>>::deserialize(REMOTING__NAMES[4], false, deser, gen_extension);
		if (ret) value.setExtension(gen_extension);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_PTZStatusFilterOptions_INCLUDED

