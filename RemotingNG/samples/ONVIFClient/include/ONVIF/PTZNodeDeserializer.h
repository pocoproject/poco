//
// PTZNodeDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_PTZNode_INCLUDED
#define TypeDeserializer_ONVIF_PTZNode_INCLUDED


#include "ONVIF/PTZNode.h"
#include "ONVIF/PTZNodeExtensionDeserializer.h"
#include "ONVIF/PTZNodeExtensionSerializer.h"
#include "ONVIF/PTZSpacesDeserializer.h"
#include "ONVIF/PTZSpacesSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::PTZNode>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::PTZNode& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMESPACE("http://www.onvif.org/ver10/schema"s);
		prepareDeserializeAttributes(deser);
		bool ret = deser.deserializeStructBegin(name, isMandatory);
		deser.pushProperty(SerializerBase::PROP_NAMESPACE, REMOTING__NAMESPACE);
		if (ret)
		{
			deserializeAttributes(deser, value);
			deserializeImpl(deser, value);
			deser.popProperty(SerializerBase::PROP_NAMESPACE);
			deser.deserializeStructEnd(name);
		}
		else deser.popProperty(SerializerBase::PROP_NAMESPACE);
		return ret;
	}

	static void deserializeAttributes(Deserializer& deser, ONVIF::PTZNode& value)
	{
		// ONVIF::DeviceEntity
		{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES__ONVIF__DEVICEENTITY[] = {"token"s};
		bool ret = false;
		std::string gen_token;
		ret = TypeDeserializer<std::string>::deserialize(REMOTING__NAMES__ONVIF__DEVICEENTITY[0], true, deser, gen_token);
		if (ret) value.setToken(gen_token);
		}
		
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"FixedHomePosition"s,"GeoMove"s};
		bool ret = false;
		Poco::Optional<bool> gen_fixedHomePosition;
		ret = TypeDeserializer<Poco::Optional<bool>>::deserialize(REMOTING__NAMES[0], false, deser, gen_fixedHomePosition);
		if (ret) value.setFixedHomePosition(gen_fixedHomePosition);
		Poco::Optional<bool> gen_geoMove;
		ret = TypeDeserializer<Poco::Optional<bool>>::deserialize(REMOTING__NAMES[1], false, deser, gen_geoMove);
		if (ret) value.setGeoMove(gen_geoMove);
	}

	static void deserializeImpl(Deserializer& deser, ONVIF::PTZNode& value)
	{
		using namespace std::string_literals;
		
		// ONVIF::DeviceEntity
		{
		}
		
		static const std::string REMOTING__NAMES[] = {"Name"s,"SupportedPTZSpaces"s,"MaximumNumberOfPresets"s,"HomeSupported"s,"AuxiliaryCommands"s,"Extension"s};
		bool ret = false;
		Poco::Optional<std::string> gen_name;
		ret = TypeDeserializer<Poco::Optional<std::string>>::deserialize(REMOTING__NAMES[0], false, deser, gen_name);
		if (ret) value.setName(gen_name);
		ONVIF::PTZSpaces gen_supportedPTZSpaces;
		ret = TypeDeserializer<ONVIF::PTZSpaces>::deserialize(REMOTING__NAMES[1], true, deser, gen_supportedPTZSpaces);
		if (ret) value.setSupportedPTZSpaces(gen_supportedPTZSpaces);
		int gen_maximumNumberOfPresets;
		ret = TypeDeserializer<int>::deserialize(REMOTING__NAMES[2], true, deser, gen_maximumNumberOfPresets);
		if (ret) value.setMaximumNumberOfPresets(gen_maximumNumberOfPresets);
		bool gen_homeSupported;
		ret = TypeDeserializer<bool>::deserialize(REMOTING__NAMES[3], true, deser, gen_homeSupported);
		if (ret) value.setHomeSupported(gen_homeSupported);
		std::vector<std::string> gen_auxiliaryCommands;
		ret = TypeDeserializer<std::vector<std::string>>::deserialize(REMOTING__NAMES[4], false, deser, gen_auxiliaryCommands);
		if (ret) value.setAuxiliaryCommands(gen_auxiliaryCommands);
		Poco::SharedPtr<ONVIF::PTZNodeExtension> gen_extension;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::PTZNodeExtension>>::deserialize(REMOTING__NAMES[5], false, deser, gen_extension);
		if (ret) value.setExtension(gen_extension);
	}

	static void prepareDeserializeAttributes(Deserializer& deser)
	{
		// ONVIF::DeviceEntity
		{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES__ONVIF__DEVICEENTITY[] = {"token"s,"http://www.onvif.org/ver10/schema"s};
		deser.pushAttribute(REMOTING__NAMES__ONVIF__DEVICEENTITY[1], REMOTING__NAMES__ONVIF__DEVICEENTITY[0], true);
		}
		
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"FixedHomePosition"s,"GeoMove"s,"http://www.onvif.org/ver10/schema"s};
		deser.pushAttribute(REMOTING__NAMES[2], REMOTING__NAMES[0], false);
		deser.pushAttribute(REMOTING__NAMES[2], REMOTING__NAMES[1], false);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_PTZNode_INCLUDED

