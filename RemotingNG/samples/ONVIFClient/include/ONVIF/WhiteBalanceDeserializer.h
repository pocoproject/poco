//
// WhiteBalanceDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_WhiteBalance_INCLUDED
#define TypeDeserializer_ONVIF_WhiteBalance_INCLUDED


#include "ONVIF/WhiteBalance.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::WhiteBalance>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::WhiteBalance& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::WhiteBalance& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Mode"s,"CrGain"s,"CbGain"s};
		bool ret = false;
		std::string gen_mode;
		ret = TypeDeserializer<std::string>::deserialize(REMOTING__NAMES[0], true, deser, gen_mode);
		if (ret) value.setMode(gen_mode);
		float gen_crGain;
		ret = TypeDeserializer<float>::deserialize(REMOTING__NAMES[1], true, deser, gen_crGain);
		if (ret) value.setCrGain(gen_crGain);
		float gen_cbGain;
		ret = TypeDeserializer<float>::deserialize(REMOTING__NAMES[2], true, deser, gen_cbGain);
		if (ret) value.setCbGain(gen_cbGain);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_WhiteBalance_INCLUDED

