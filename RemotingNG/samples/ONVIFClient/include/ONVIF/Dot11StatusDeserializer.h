//
// Dot11StatusDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_Dot11Status_INCLUDED
#define TypeDeserializer_ONVIF_Dot11Status_INCLUDED


#include "ONVIF/Dot11Status.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::Dot11Status>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::Dot11Status& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::Dot11Status& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"SSID"s,"BSSID"s,"PairCipher"s,"GroupCipher"s,"SignalStrength"s,"ActiveConfigAlias"s};
		bool ret = false;
		std::string gen_sSID;
		ret = TypeDeserializer<std::string>::deserialize(REMOTING__NAMES[0], true, deser, gen_sSID);
		if (ret) value.setSSID(gen_sSID);
		Poco::Optional<std::string> gen_bSSID;
		ret = TypeDeserializer<Poco::Optional<std::string>>::deserialize(REMOTING__NAMES[1], false, deser, gen_bSSID);
		if (ret) value.setBSSID(gen_bSSID);
		Poco::Optional<std::string> gen_pairCipher;
		ret = TypeDeserializer<Poco::Optional<std::string>>::deserialize(REMOTING__NAMES[2], false, deser, gen_pairCipher);
		if (ret) value.setPairCipher(gen_pairCipher);
		Poco::Optional<std::string> gen_groupCipher;
		ret = TypeDeserializer<Poco::Optional<std::string>>::deserialize(REMOTING__NAMES[3], false, deser, gen_groupCipher);
		if (ret) value.setGroupCipher(gen_groupCipher);
		Poco::Optional<std::string> gen_signalStrength;
		ret = TypeDeserializer<Poco::Optional<std::string>>::deserialize(REMOTING__NAMES[4], false, deser, gen_signalStrength);
		if (ret) value.setSignalStrength(gen_signalStrength);
		std::string gen_activeConfigAlias;
		ret = TypeDeserializer<std::string>::deserialize(REMOTING__NAMES[5], true, deser, gen_activeConfigAlias);
		if (ret) value.setActiveConfigAlias(gen_activeConfigAlias);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_Dot11Status_INCLUDED

