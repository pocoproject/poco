//
// FileProgressDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_FileProgress_INCLUDED
#define TypeDeserializer_ONVIF_FileProgress_INCLUDED


#include "ONVIF/FileProgress.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::FileProgress>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::FileProgress& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::FileProgress& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"FileName"s,"Progress"s};
		bool ret = false;
		std::string gen_fileName;
		ret = TypeDeserializer<std::string>::deserialize(REMOTING__NAMES[0], true, deser, gen_fileName);
		if (ret) value.setFileName(gen_fileName);
		float gen_progress;
		ret = TypeDeserializer<float>::deserialize(REMOTING__NAMES[1], true, deser, gen_progress);
		if (ret) value.setProgress(gen_progress);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_FileProgress_INCLUDED

