//
// IrCutFilterAutoAdjustmentDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_IrCutFilterAutoAdjustment_INCLUDED
#define TypeDeserializer_ONVIF_IrCutFilterAutoAdjustment_INCLUDED


#include "ONVIF/IrCutFilterAutoAdjustment.h"
#include "ONVIF/IrCutFilterAutoAdjustmentExtensionDeserializer.h"
#include "ONVIF/IrCutFilterAutoAdjustmentExtensionSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::IrCutFilterAutoAdjustment>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::IrCutFilterAutoAdjustment& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::IrCutFilterAutoAdjustment& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"BoundaryType"s,"BoundaryOffset"s,"ResponseTime"s,"Extension"s};
		bool ret = false;
		std::string gen_boundaryType;
		ret = TypeDeserializer<std::string>::deserialize(REMOTING__NAMES[0], true, deser, gen_boundaryType);
		if (ret) value.setBoundaryType(gen_boundaryType);
		Poco::Optional<float> gen_boundaryOffset;
		ret = TypeDeserializer<Poco::Optional<float>>::deserialize(REMOTING__NAMES[1], false, deser, gen_boundaryOffset);
		if (ret) value.setBoundaryOffset(gen_boundaryOffset);
		Poco::Optional<std::string> gen_responseTime;
		ret = TypeDeserializer<Poco::Optional<std::string>>::deserialize(REMOTING__NAMES[2], false, deser, gen_responseTime);
		if (ret) value.setResponseTime(gen_responseTime);
		Poco::SharedPtr<ONVIF::IrCutFilterAutoAdjustmentExtension> gen_extension;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::IrCutFilterAutoAdjustmentExtension>>::deserialize(REMOTING__NAMES[3], false, deser, gen_extension);
		if (ret) value.setExtension(gen_extension);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_IrCutFilterAutoAdjustment_INCLUDED

