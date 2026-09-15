//
// StorageReferencePathDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_StorageReferencePath_INCLUDED
#define TypeDeserializer_ONVIF_StorageReferencePath_INCLUDED


#include "ONVIF/StorageReferencePath.h"
#include "ONVIF/StorageReferencePathExtensionDeserializer.h"
#include "ONVIF/StorageReferencePathExtensionSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::StorageReferencePath>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::StorageReferencePath& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::StorageReferencePath& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"StorageToken"s,"RelativePath"s,"Extension"s};
		bool ret = false;
		std::string gen_storageToken;
		ret = TypeDeserializer<std::string>::deserialize(REMOTING__NAMES[0], true, deser, gen_storageToken);
		if (ret) value.setStorageToken(gen_storageToken);
		Poco::Optional<std::string> gen_relativePath;
		ret = TypeDeserializer<Poco::Optional<std::string>>::deserialize(REMOTING__NAMES[1], false, deser, gen_relativePath);
		if (ret) value.setRelativePath(gen_relativePath);
		Poco::SharedPtr<ONVIF::StorageReferencePathExtension> gen_extension;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::StorageReferencePathExtension>>::deserialize(REMOTING__NAMES[2], false, deser, gen_extension);
		if (ret) value.setExtension(gen_extension);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_StorageReferencePath_INCLUDED

