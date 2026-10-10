//
// ScopeDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_Scope_INCLUDED
#define TypeDeserializer_ONVIF_Scope_INCLUDED


#include "ONVIF/Scope.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::Scope>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::Scope& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::Scope& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"ScopeDef"s,"ScopeItem"s};
		bool ret = false;
		std::string gen_scopeDef;
		ret = TypeDeserializer<std::string>::deserialize(REMOTING__NAMES[0], true, deser, gen_scopeDef);
		if (ret) value.setScopeDef(gen_scopeDef);
		Poco::URI gen_scopeItem;
		ret = TypeDeserializer<Poco::URI>::deserialize(REMOTING__NAMES[1], true, deser, gen_scopeItem);
		if (ret) value.setScopeItem(gen_scopeItem);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_Scope_INCLUDED

