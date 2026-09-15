//
// AudioClassDescriptorSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_AudioClassDescriptor_INCLUDED
#define TypeSerializer_ONVIF_AudioClassDescriptor_INCLUDED


#include "ONVIF/AudioClassCandidateDeserializer.h"
#include "ONVIF/AudioClassCandidateSerializer.h"
#include "ONVIF/AudioClassDescriptor.h"
#include "ONVIF/AudioClassDescriptorExtensionDeserializer.h"
#include "ONVIF/AudioClassDescriptorExtensionSerializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::AudioClassDescriptor>
{
public:
	static void serialize(const std::string& name, const ONVIF::AudioClassDescriptor& value, Serializer& ser)
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

	static void serializeImpl(const ONVIF::AudioClassDescriptor& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"ClassCandidate"s,"Extension"s,""s};
		TypeSerializer<std::vector<ONVIF::AudioClassCandidate>>::serialize(REMOTING__NAMES[0], value.getClassCandidate(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::AudioClassDescriptorExtension>>::serialize(REMOTING__NAMES[1], value.getExtension(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_AudioClassDescriptor_INCLUDED

