//
// LayoutOptionsDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_LayoutOptions_INCLUDED
#define TypeDeserializer_ONVIF_LayoutOptions_INCLUDED


#include "ONVIF/LayoutOptions.h"
#include "ONVIF/LayoutOptionsExtensionDeserializer.h"
#include "ONVIF/LayoutOptionsExtensionSerializer.h"
#include "ONVIF/PaneLayoutOptionsDeserializer.h"
#include "ONVIF/PaneLayoutOptionsSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::LayoutOptions>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::LayoutOptions& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::LayoutOptions& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"PaneLayoutOptions"s,"Extension"s};
		bool ret = false;
		std::vector<ONVIF::PaneLayoutOptions> gen_paneLayoutOptions;
		ret = TypeDeserializer<std::vector<ONVIF::PaneLayoutOptions>>::deserialize(REMOTING__NAMES[0], true, deser, gen_paneLayoutOptions);
		if (ret) value.setPaneLayoutOptions(gen_paneLayoutOptions);
		Poco::SharedPtr<ONVIF::LayoutOptionsExtension> gen_extension;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::LayoutOptionsExtension>>::deserialize(REMOTING__NAMES[1], false, deser, gen_extension);
		if (ret) value.setExtension(gen_extension);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_LayoutOptions_INCLUDED

