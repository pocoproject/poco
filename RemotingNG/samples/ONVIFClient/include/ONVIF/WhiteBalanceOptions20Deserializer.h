//
// WhiteBalanceOptions20Deserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_WhiteBalanceOptions20_INCLUDED
#define TypeDeserializer_ONVIF_WhiteBalanceOptions20_INCLUDED


#include "ONVIF/FloatRangeDeserializer.h"
#include "ONVIF/FloatRangeSerializer.h"
#include "ONVIF/WhiteBalanceOptions20.h"
#include "ONVIF/WhiteBalanceOptions20ExtensionDeserializer.h"
#include "ONVIF/WhiteBalanceOptions20ExtensionSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::WhiteBalanceOptions20>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::WhiteBalanceOptions20& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::WhiteBalanceOptions20& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Mode"s,"YrGain"s,"YbGain"s,"Extension"s};
		bool ret = false;
		std::vector<std::string> gen_mode;
		ret = TypeDeserializer<std::vector<std::string>>::deserialize(REMOTING__NAMES[0], true, deser, gen_mode);
		if (ret) value.setMode(gen_mode);
		Poco::SharedPtr<ONVIF::FloatRange> gen_yrGain;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::FloatRange>>::deserialize(REMOTING__NAMES[1], false, deser, gen_yrGain);
		if (ret) value.setYrGain(gen_yrGain);
		Poco::SharedPtr<ONVIF::FloatRange> gen_ybGain;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::FloatRange>>::deserialize(REMOTING__NAMES[2], false, deser, gen_ybGain);
		if (ret) value.setYbGain(gen_ybGain);
		Poco::SharedPtr<ONVIF::WhiteBalanceOptions20Extension> gen_extension;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::WhiteBalanceOptions20Extension>>::deserialize(REMOTING__NAMES[3], false, deser, gen_extension);
		if (ret) value.setExtension(gen_extension);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_WhiteBalanceOptions20_INCLUDED

