//
// ArrayOfFileProgressDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_ArrayOfFileProgress_INCLUDED
#define TypeDeserializer_ONVIF_ArrayOfFileProgress_INCLUDED


#include "ONVIF/ArrayOfFileProgress.h"
#include "ONVIF/ArrayOfFileProgressExtensionDeserializer.h"
#include "ONVIF/ArrayOfFileProgressExtensionSerializer.h"
#include "ONVIF/FileProgressDeserializer.h"
#include "ONVIF/FileProgressSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::ArrayOfFileProgress>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::ArrayOfFileProgress& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::ArrayOfFileProgress& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"FileProgress"s,"Extension"s};
		bool ret = false;
		std::vector<ONVIF::FileProgress> gen_fileProgress;
		ret = TypeDeserializer<std::vector<ONVIF::FileProgress>>::deserialize(REMOTING__NAMES[0], false, deser, gen_fileProgress);
		if (ret) value.setFileProgress(gen_fileProgress);
		Poco::SharedPtr<ONVIF::ArrayOfFileProgressExtension> gen_extension;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::ArrayOfFileProgressExtension>>::deserialize(REMOTING__NAMES[1], false, deser, gen_extension);
		if (ret) value.setExtension(gen_extension);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_ArrayOfFileProgress_INCLUDED

