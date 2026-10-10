//
// ZoomLimitsDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_ZoomLimits_INCLUDED
#define TypeDeserializer_ONVIF_ZoomLimits_INCLUDED


#include "ONVIF/Space1DDescriptionDeserializer.h"
#include "ONVIF/Space1DDescriptionSerializer.h"
#include "ONVIF/ZoomLimits.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::ZoomLimits>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::ZoomLimits& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::ZoomLimits& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Range"s};
		bool ret = false;
		ONVIF::Space1DDescription gen_range;
		ret = TypeDeserializer<ONVIF::Space1DDescription>::deserialize(REMOTING__NAMES[0], true, deser, gen_range);
		if (ret) value.setRange(gen_range);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_ZoomLimits_INCLUDED

