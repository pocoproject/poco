//
// IrCutFilterAutoAdjustmentOptionsDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_IrCutFilterAutoAdjustmentOptions_INCLUDED
#define TypeDeserializer_ONVIF_IrCutFilterAutoAdjustmentOptions_INCLUDED


#include "ONVIF/DurationRangeDeserializer.h"
#include "ONVIF/DurationRangeSerializer.h"
#include "ONVIF/IrCutFilterAutoAdjustmentOptions.h"
#include "ONVIF/IrCutFilterAutoAdjustmentOptionsExtensionDeserializer.h"
#include "ONVIF/IrCutFilterAutoAdjustmentOptionsExtensionSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::IrCutFilterAutoAdjustmentOptions>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::IrCutFilterAutoAdjustmentOptions& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::IrCutFilterAutoAdjustmentOptions& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"BoundaryType"s,"BoundaryOffset"s,"ResponseTimeRange"s,"Extension"s};
		bool ret = false;
		std::vector<std::string> gen_boundaryType;
		ret = TypeDeserializer<std::vector<std::string>>::deserialize(REMOTING__NAMES[0], true, deser, gen_boundaryType);
		if (ret) value.setBoundaryType(gen_boundaryType);
		Poco::Optional<bool> gen_boundaryOffset;
		ret = TypeDeserializer<Poco::Optional<bool>>::deserialize(REMOTING__NAMES[1], false, deser, gen_boundaryOffset);
		if (ret) value.setBoundaryOffset(gen_boundaryOffset);
		Poco::SharedPtr<ONVIF::DurationRange> gen_responseTimeRange;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::DurationRange>>::deserialize(REMOTING__NAMES[2], false, deser, gen_responseTimeRange);
		if (ret) value.setResponseTimeRange(gen_responseTimeRange);
		Poco::SharedPtr<ONVIF::IrCutFilterAutoAdjustmentOptionsExtension> gen_extension;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::IrCutFilterAutoAdjustmentOptionsExtension>>::deserialize(REMOTING__NAMES[3], false, deser, gen_extension);
		if (ret) value.setExtension(gen_extension);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_IrCutFilterAutoAdjustmentOptions_INCLUDED

