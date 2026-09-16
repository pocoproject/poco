//
// CellLayoutDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_CellLayout_INCLUDED
#define TypeDeserializer_ONVIF_CellLayout_INCLUDED


#include "ONVIF/CellLayout.h"
#include "ONVIF/TransformationDeserializer.h"
#include "ONVIF/TransformationSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::CellLayout>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::CellLayout& value)
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

	static void deserializeAttributes(Deserializer& deser, ONVIF::CellLayout& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Columns"s,"Rows"s};
		bool ret = false;
		int gen_columns;
		ret = TypeDeserializer<int>::deserialize(REMOTING__NAMES[0], true, deser, gen_columns);
		if (ret) value.setColumns(gen_columns);
		int gen_rows;
		ret = TypeDeserializer<int>::deserialize(REMOTING__NAMES[1], true, deser, gen_rows);
		if (ret) value.setRows(gen_rows);
	}

	static void deserializeImpl(Deserializer& deser, ONVIF::CellLayout& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Transformation"s};
		bool ret = false;
		ONVIF::Transformation gen_transformation;
		ret = TypeDeserializer<ONVIF::Transformation>::deserialize(REMOTING__NAMES[0], true, deser, gen_transformation);
		if (ret) value.setTransformation(gen_transformation);
	}

	static void prepareDeserializeAttributes(Deserializer& deser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Columns"s,"Rows"s,"http://www.onvif.org/ver10/schema"s};
		deser.pushAttribute(REMOTING__NAMES[2], REMOTING__NAMES[0], true);
		deser.pushAttribute(REMOTING__NAMES[2], REMOTING__NAMES[1], true);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_CellLayout_INCLUDED

