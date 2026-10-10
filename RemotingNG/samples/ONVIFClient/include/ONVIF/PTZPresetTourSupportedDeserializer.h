//
// PTZPresetTourSupportedDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_PTZPresetTourSupported_INCLUDED
#define TypeDeserializer_ONVIF_PTZPresetTourSupported_INCLUDED


#include "ONVIF/PTZPresetTourSupported.h"
#include "ONVIF/PTZPresetTourSupportedExtensionDeserializer.h"
#include "ONVIF/PTZPresetTourSupportedExtensionSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::PTZPresetTourSupported>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::PTZPresetTourSupported& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::PTZPresetTourSupported& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"MaximumNumberOfPresetTours"s,"PTZPresetTourOperation"s,"Extension"s};
		bool ret = false;
		int gen_maximumNumberOfPresetTours;
		ret = TypeDeserializer<int>::deserialize(REMOTING__NAMES[0], true, deser, gen_maximumNumberOfPresetTours);
		if (ret) value.setMaximumNumberOfPresetTours(gen_maximumNumberOfPresetTours);
		std::vector<std::string> gen_pTZPresetTourOperation;
		ret = TypeDeserializer<std::vector<std::string>>::deserialize(REMOTING__NAMES[1], false, deser, gen_pTZPresetTourOperation);
		if (ret) value.setPTZPresetTourOperation(gen_pTZPresetTourOperation);
		Poco::SharedPtr<ONVIF::PTZPresetTourSupportedExtension> gen_extension;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::PTZPresetTourSupportedExtension>>::deserialize(REMOTING__NAMES[2], false, deser, gen_extension);
		if (ret) value.setExtension(gen_extension);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_PTZPresetTourSupported_INCLUDED

