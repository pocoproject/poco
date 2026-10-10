//
// LayoutDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_Layout_INCLUDED
#define TypeDeserializer_ONVIF_Layout_INCLUDED


#include "ONVIF/Layout.h"
#include "ONVIF/LayoutExtensionDeserializer.h"
#include "ONVIF/LayoutExtensionSerializer.h"
#include "ONVIF/PaneLayoutDeserializer.h"
#include "ONVIF/PaneLayoutSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::Layout>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::Layout& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::Layout& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"PaneLayout"s,"Extension"s};
		bool ret = false;
		std::vector<ONVIF::PaneLayout> gen_paneLayout;
		ret = TypeDeserializer<std::vector<ONVIF::PaneLayout>>::deserialize(REMOTING__NAMES[0], true, deser, gen_paneLayout);
		if (ret) value.setPaneLayout(gen_paneLayout);
		Poco::SharedPtr<ONVIF::LayoutExtension> gen_extension;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::LayoutExtension>>::deserialize(REMOTING__NAMES[1], false, deser, gen_extension);
		if (ret) value.setExtension(gen_extension);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_Layout_INCLUDED

