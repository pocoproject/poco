//
// NTPInformationDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_NTPInformation_INCLUDED
#define TypeDeserializer_ONVIF_NTPInformation_INCLUDED


#include "ONVIF/NTPInformation.h"
#include "ONVIF/NTPInformationExtensionDeserializer.h"
#include "ONVIF/NTPInformationExtensionSerializer.h"
#include "ONVIF/NetworkHostDeserializer.h"
#include "ONVIF/NetworkHostSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::NTPInformation>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::NTPInformation& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::NTPInformation& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"FromDHCP"s,"NTPFromDHCP"s,"NTPManual"s,"Extension"s};
		bool ret = false;
		bool gen_fromDHCP;
		ret = TypeDeserializer<bool>::deserialize(REMOTING__NAMES[0], true, deser, gen_fromDHCP);
		if (ret) value.setFromDHCP(gen_fromDHCP);
		std::vector<ONVIF::NetworkHost> gen_nTPFromDHCP;
		ret = TypeDeserializer<std::vector<ONVIF::NetworkHost>>::deserialize(REMOTING__NAMES[1], false, deser, gen_nTPFromDHCP);
		if (ret) value.setNTPFromDHCP(gen_nTPFromDHCP);
		std::vector<ONVIF::NetworkHost> gen_nTPManual;
		ret = TypeDeserializer<std::vector<ONVIF::NetworkHost>>::deserialize(REMOTING__NAMES[2], false, deser, gen_nTPManual);
		if (ret) value.setNTPManual(gen_nTPManual);
		Poco::SharedPtr<ONVIF::NTPInformationExtension> gen_extension;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::NTPInformationExtension>>::deserialize(REMOTING__NAMES[3], false, deser, gen_extension);
		if (ret) value.setExtension(gen_extension);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_NTPInformation_INCLUDED

