//
// ActiveConnectionDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_ActiveConnection_INCLUDED
#define TypeDeserializer_ONVIF_ActiveConnection_INCLUDED


#include "ONVIF/ActiveConnection.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::ActiveConnection>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::ActiveConnection& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::ActiveConnection& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"CurrentBitrate"s,"CurrentFps"s};
		bool ret = false;
		float gen_currentBitrate;
		ret = TypeDeserializer<float>::deserialize(REMOTING__NAMES[0], true, deser, gen_currentBitrate);
		if (ret) value.setCurrentBitrate(gen_currentBitrate);
		float gen_currentFps;
		ret = TypeDeserializer<float>::deserialize(REMOTING__NAMES[1], true, deser, gen_currentFps);
		if (ret) value.setCurrentFps(gen_currentFps);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_ActiveConnection_INCLUDED

