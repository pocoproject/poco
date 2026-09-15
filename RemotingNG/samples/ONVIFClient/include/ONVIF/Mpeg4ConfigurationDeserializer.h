//
// Mpeg4ConfigurationDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_Mpeg4Configuration_INCLUDED
#define TypeDeserializer_ONVIF_Mpeg4Configuration_INCLUDED


#include "ONVIF/Mpeg4Configuration.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::Mpeg4Configuration>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::Mpeg4Configuration& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::Mpeg4Configuration& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"GovLength"s,"Mpeg4Profile"s};
		bool ret = false;
		int gen_govLength;
		ret = TypeDeserializer<int>::deserialize(REMOTING__NAMES[0], true, deser, gen_govLength);
		if (ret) value.setGovLength(gen_govLength);
		std::string gen_mpeg4Profile;
		ret = TypeDeserializer<std::string>::deserialize(REMOTING__NAMES[1], true, deser, gen_mpeg4Profile);
		if (ret) value.setMpeg4Profile(gen_mpeg4Profile);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_Mpeg4Configuration_INCLUDED

