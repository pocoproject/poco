//
// PolylineArrayConfigurationDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_PolylineArrayConfiguration_INCLUDED
#define TypeDeserializer_ONVIF_PolylineArrayConfiguration_INCLUDED


#include "ONVIF/PolylineArrayConfiguration.h"
#include "ONVIF/PolylineArrayDeserializer.h"
#include "ONVIF/PolylineArraySerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::PolylineArrayConfiguration>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::PolylineArrayConfiguration& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::PolylineArrayConfiguration& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"PolylineArray"s};
		bool ret = false;
		ONVIF::PolylineArray gen_polylineArray;
		ret = TypeDeserializer<ONVIF::PolylineArray>::deserialize(REMOTING__NAMES[0], true, deser, gen_polylineArray);
		if (ret) value.setPolylineArray(gen_polylineArray);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_PolylineArrayConfiguration_INCLUDED

