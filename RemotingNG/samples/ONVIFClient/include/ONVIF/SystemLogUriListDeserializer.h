//
// SystemLogUriListDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_SystemLogUriList_INCLUDED
#define TypeDeserializer_ONVIF_SystemLogUriList_INCLUDED


#include "ONVIF/SystemLogUriDeserializer.h"
#include "ONVIF/SystemLogUriList.h"
#include "ONVIF/SystemLogUriSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::SystemLogUriList>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::SystemLogUriList& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::SystemLogUriList& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"SystemLog"s};
		bool ret = false;
		std::vector<ONVIF::SystemLogUri> gen_systemLog;
		ret = TypeDeserializer<std::vector<ONVIF::SystemLogUri>>::deserialize(REMOTING__NAMES[0], false, deser, gen_systemLog);
		if (ret) value.setSystemLog(gen_systemLog);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_SystemLogUriList_INCLUDED

