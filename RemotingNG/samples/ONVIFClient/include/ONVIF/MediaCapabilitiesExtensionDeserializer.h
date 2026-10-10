//
// MediaCapabilitiesExtensionDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_MediaCapabilitiesExtension_INCLUDED
#define TypeDeserializer_ONVIF_MediaCapabilitiesExtension_INCLUDED


#include "ONVIF/MediaCapabilitiesExtension.h"
#include "ONVIF/ProfileCapabilitiesDeserializer.h"
#include "ONVIF/ProfileCapabilitiesSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::MediaCapabilitiesExtension>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::MediaCapabilitiesExtension& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::MediaCapabilitiesExtension& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"ProfileCapabilities"s};
		bool ret = false;
		ONVIF::ProfileCapabilities gen_profileCapabilities;
		ret = TypeDeserializer<ONVIF::ProfileCapabilities>::deserialize(REMOTING__NAMES[0], true, deser, gen_profileCapabilities);
		if (ret) value.setProfileCapabilities(gen_profileCapabilities);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_MediaCapabilitiesExtension_INCLUDED

