//
// ImagingOptions20Extension3Serializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_ImagingOptions20Extension3_INCLUDED
#define TypeSerializer_ONVIF_ImagingOptions20Extension3_INCLUDED


#include "ONVIF/DefoggingOptionsDeserializer.h"
#include "ONVIF/DefoggingOptionsSerializer.h"
#include "ONVIF/ImagingOptions20Extension3.h"
#include "ONVIF/ImagingOptions20Extension4Deserializer.h"
#include "ONVIF/ImagingOptions20Extension4Serializer.h"
#include "ONVIF/NoiseReductionOptionsDeserializer.h"
#include "ONVIF/NoiseReductionOptionsSerializer.h"
#include "ONVIF/ToneCompensationOptionsDeserializer.h"
#include "ONVIF/ToneCompensationOptionsSerializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::ImagingOptions20Extension3>
{
public:
	static void serialize(const std::string& name, const ONVIF::ImagingOptions20Extension3& value, Serializer& ser)
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

	static void serializeImpl(const ONVIF::ImagingOptions20Extension3& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"ToneCompensationOptions"s,"DefoggingOptions"s,"NoiseReductionOptions"s,"Extension"s,""s};
		TypeSerializer<Poco::SharedPtr<ONVIF::ToneCompensationOptions>>::serialize(REMOTING__NAMES[0], value.getToneCompensationOptions(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::DefoggingOptions>>::serialize(REMOTING__NAMES[1], value.getDefoggingOptions(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::NoiseReductionOptions>>::serialize(REMOTING__NAMES[2], value.getNoiseReductionOptions(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::ImagingOptions20Extension4>>::serialize(REMOTING__NAMES[3], value.getExtension(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_ImagingOptions20Extension3_INCLUDED

