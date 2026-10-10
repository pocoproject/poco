//
// FindEventResultListSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_FindEventResultList_INCLUDED
#define TypeSerializer_ONVIF_FindEventResultList_INCLUDED


#include "ONVIF/FindEventResultDeserializer.h"
#include "ONVIF/FindEventResultList.h"
#include "ONVIF/FindEventResultSerializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::FindEventResultList>
{
public:
	static void serialize(const std::string& name, const ONVIF::FindEventResultList& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMESPACE("http://www.onvif.org/ver10/schema"s);
		ser.registerNamespace(REMOTING__NAMESPACE);
		ser.serializeStructBegin(name);
		ser.pushProperty(SerializerBase::PROP_NAMESPACE, REMOTING__NAMESPACE);
		serializeImpl(value, ser);
		ser.popProperty(SerializerBase::PROP_NAMESPACE);
		ser.serializeStructEnd(name);
	}

	static void serializeImpl(const ONVIF::FindEventResultList& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"SearchState"s,"Result"s,""s};
		TypeSerializer<std::string>::serialize(REMOTING__NAMES[0], value.getSearchState(), ser);
		TypeSerializer<std::vector<ONVIF::FindEventResult>>::serialize(REMOTING__NAMES[1], value.getResult(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_FindEventResultList_INCLUDED

