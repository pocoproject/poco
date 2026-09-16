//
// ColorClusterSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_ColorCluster_INCLUDED
#define TypeSerializer_ONVIF_ColorCluster_INCLUDED


#include "ONVIF/ColorCluster.h"
#include "ONVIF/ColorCovarianceDeserializer.h"
#include "ONVIF/ColorCovarianceSerializer.h"
#include "ONVIF/ColorDeserializer.h"
#include "ONVIF/ColorSerializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::ColorCluster>
{
public:
	static void serialize(const std::string& name, const ONVIF::ColorCluster& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMESPACE("http://www.onvif.org/ver10/schema"s);
		ser.registerNamespace(REMOTING__NAMESPACE);
		ser.serializeStructBegin(name);
		ser.pushProperty(SerializerBase::PROP_NAMESPACE, REMOTING__NAMESPACE);
		serializeImpl(value, ser);
		ser.popProperty(SerializerBase::PROP_NAMESPACE);
		ser.serializeStructEnd(name);
	}

	static void serializeImpl(const ONVIF::ColorCluster& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Color"s,"Weight"s,"Covariance"s,""s};
		TypeSerializer<ONVIF::Color>::serialize(REMOTING__NAMES[0], value.getColor(), ser);
		TypeSerializer<Poco::Optional<float>>::serialize(REMOTING__NAMES[1], value.getWeight(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::ColorCovariance>>::serialize(REMOTING__NAMES[2], value.getCovariance(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_ColorCluster_INCLUDED

