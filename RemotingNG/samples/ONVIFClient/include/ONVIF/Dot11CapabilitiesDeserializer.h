//
// Dot11CapabilitiesDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_Dot11Capabilities_INCLUDED
#define TypeDeserializer_ONVIF_Dot11Capabilities_INCLUDED


#include "ONVIF/Dot11Capabilities.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::Dot11Capabilities>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::Dot11Capabilities& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::Dot11Capabilities& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"TKIP"s,"ScanAvailableNetworks"s,"MultipleConfiguration"s,"AdHocStationMode"s,"WEP"s};
		bool ret = false;
		bool gen_tKIP;
		ret = TypeDeserializer<bool>::deserialize(REMOTING__NAMES[0], true, deser, gen_tKIP);
		if (ret) value.setTKIP(gen_tKIP);
		bool gen_scanAvailableNetworks;
		ret = TypeDeserializer<bool>::deserialize(REMOTING__NAMES[1], true, deser, gen_scanAvailableNetworks);
		if (ret) value.setScanAvailableNetworks(gen_scanAvailableNetworks);
		bool gen_multipleConfiguration;
		ret = TypeDeserializer<bool>::deserialize(REMOTING__NAMES[2], true, deser, gen_multipleConfiguration);
		if (ret) value.setMultipleConfiguration(gen_multipleConfiguration);
		bool gen_adHocStationMode;
		ret = TypeDeserializer<bool>::deserialize(REMOTING__NAMES[3], true, deser, gen_adHocStationMode);
		if (ret) value.setAdHocStationMode(gen_adHocStationMode);
		bool gen_wEP;
		ret = TypeDeserializer<bool>::deserialize(REMOTING__NAMES[4], true, deser, gen_wEP);
		if (ret) value.setWEP(gen_wEP);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_Dot11Capabilities_INCLUDED

