//
// PaneLayoutOptionsDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_PaneLayoutOptions_INCLUDED
#define TypeDeserializer_ONVIF_PaneLayoutOptions_INCLUDED


#include "ONVIF/PaneLayoutOptions.h"
#include "ONVIF/PaneOptionExtensionDeserializer.h"
#include "ONVIF/PaneOptionExtensionSerializer.h"
#include "ONVIF/RectangleDeserializer.h"
#include "ONVIF/RectangleSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::PaneLayoutOptions>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::PaneLayoutOptions& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::PaneLayoutOptions& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Area"s,"Extension"s};
		bool ret = false;
		std::vector<ONVIF::Rectangle> gen_area;
		ret = TypeDeserializer<std::vector<ONVIF::Rectangle>>::deserialize(REMOTING__NAMES[0], true, deser, gen_area);
		if (ret) value.setArea(gen_area);
		Poco::SharedPtr<ONVIF::PaneOptionExtension> gen_extension;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::PaneOptionExtension>>::deserialize(REMOTING__NAMES[1], false, deser, gen_extension);
		if (ret) value.setExtension(gen_extension);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_PaneLayoutOptions_INCLUDED

