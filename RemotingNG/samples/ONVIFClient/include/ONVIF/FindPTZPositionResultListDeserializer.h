//
// FindPTZPositionResultListDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_FindPTZPositionResultList_INCLUDED
#define TypeDeserializer_ONVIF_FindPTZPositionResultList_INCLUDED


#include "ONVIF/FindPTZPositionResultDeserializer.h"
#include "ONVIF/FindPTZPositionResultList.h"
#include "ONVIF/FindPTZPositionResultSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::FindPTZPositionResultList>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::FindPTZPositionResultList& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::FindPTZPositionResultList& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"SearchState"s,"Result"s};
		bool ret = false;
		std::string gen_searchState;
		ret = TypeDeserializer<std::string>::deserialize(REMOTING__NAMES[0], true, deser, gen_searchState);
		if (ret) value.setSearchState(gen_searchState);
		std::vector<ONVIF::FindPTZPositionResult> gen_result;
		ret = TypeDeserializer<std::vector<ONVIF::FindPTZPositionResult>>::deserialize(REMOTING__NAMES[1], false, deser, gen_result);
		if (ret) value.setResult(gen_result);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_FindPTZPositionResultList_INCLUDED

