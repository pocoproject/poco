//
// AttachmentDataDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_AttachmentData_INCLUDED
#define TypeDeserializer_ONVIF_AttachmentData_INCLUDED


#include "ONVIF/AttachmentData.h"
#include "Poco/RemotingNG/TypeDeserializer.h"
#include "W3/XOP/IncludeDeserializer.h"
#include "W3/XOP/IncludeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::AttachmentData>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::AttachmentData& value)
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

	static void deserializeAttributes(Deserializer& deser, ONVIF::AttachmentData& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"contentType"s};
		bool ret = false;
		Poco::Optional<std::string> gen_contentType;
		ret = TypeDeserializer<Poco::Optional<std::string>>::deserialize(REMOTING__NAMES[0], false, deser, gen_contentType);
		if (ret) value.setContentType(gen_contentType);
	}

	static void deserializeImpl(Deserializer& deser, ONVIF::AttachmentData& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Include"s};
		bool ret = false;
		W3::XOP::Include gen_include;
		ret = TypeDeserializer<W3::XOP::Include>::deserialize(REMOTING__NAMES[0], true, deser, gen_include);
		if (ret) value.setInclude(gen_include);
	}

	static void prepareDeserializeAttributes(Deserializer& deser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"contentType"s,"http://www.onvif.org/ver10/schema"s,"http://www.w3.org/2005/05/xmlmime"s};
		deser.pushAttribute(REMOTING__NAMES[2], REMOTING__NAMES[0], false);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_AttachmentData_INCLUDED

