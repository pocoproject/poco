//
// ColorCovarianceSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_ColorCovariance_INCLUDED
#define TypeSerializer_ONVIF_ColorCovariance_INCLUDED


#include "ONVIF/ColorCovariance.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::ColorCovariance>
{
public:
	static void prepareSerializeAttributes(Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"XX"s,"YY"s,"ZZ"s,"XY"s,"XZ"s,"YZ"s,"Colorspace"s,"http://www.onvif.org/ver10/schema"s};
		ser.pushAttribute(REMOTING__NAMES[7], REMOTING__NAMES[0]);
		ser.pushAttribute(REMOTING__NAMES[7], REMOTING__NAMES[1]);
		ser.pushAttribute(REMOTING__NAMES[7], REMOTING__NAMES[2]);
		ser.pushAttribute(REMOTING__NAMES[7], REMOTING__NAMES[3]);
		ser.pushAttribute(REMOTING__NAMES[7], REMOTING__NAMES[4]);
		ser.pushAttribute(REMOTING__NAMES[7], REMOTING__NAMES[5]);
		ser.pushAttribute(REMOTING__NAMES[7], REMOTING__NAMES[6]);
	}

	static void serialize(const std::string& name, const ONVIF::ColorCovariance& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMESPACE("http://www.onvif.org/ver10/schema"s);
		ser.registerNamespace(REMOTING__NAMESPACE);
		prepareSerializeAttributes(ser);
		ser.serializeStructBegin(name);
		ser.pushProperty(SerializerBase::PROP_NAMESPACE, REMOTING__NAMESPACE);
		serializeAttributes(value, ser);
		serializeImpl(value, ser);
		ser.popProperty(SerializerBase::PROP_NAMESPACE);
		ser.serializeStructEnd(name);
	}

	static void serializeAttributes(const ONVIF::ColorCovariance& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"XX"s,"YY"s,"ZZ"s,"XY"s,"XZ"s,"YZ"s,"Colorspace"s};
		TypeSerializer<float>::serialize(REMOTING__NAMES[0], value.getXX(), ser);
		TypeSerializer<float>::serialize(REMOTING__NAMES[1], value.getYY(), ser);
		TypeSerializer<float>::serialize(REMOTING__NAMES[2], value.getZZ(), ser);
		TypeSerializer<Poco::Optional<float>>::serialize(REMOTING__NAMES[3], value.getXY(), ser);
		TypeSerializer<Poco::Optional<float>>::serialize(REMOTING__NAMES[4], value.getXZ(), ser);
		TypeSerializer<Poco::Optional<float>>::serialize(REMOTING__NAMES[5], value.getYZ(), ser);
		TypeSerializer<Poco::Optional<Poco::URI>>::serialize(REMOTING__NAMES[6], value.getColorspace(), ser);
	}

	static void serializeImpl(const ONVIF::ColorCovariance& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_ColorCovariance_INCLUDED

