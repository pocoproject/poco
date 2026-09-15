//
// DescriptionSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_OASIS_WSRF_BF2_Description_INCLUDED
#define TypeSerializer_OASIS_WSRF_BF2_Description_INCLUDED


#include "OASIS/WSRF/BF2/Description.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<OASIS::WSRF::BF2::Description>
{
public:
	static void serialize(const std::string& name, const OASIS::WSRF::BF2::Description& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMESPACE("http://docs.oasis-open.org/wsrf/bf-2"s);
		ser.registerNamespace(REMOTING__NAMESPACE);
		ser.serializeStructBegin(name);
		ser.pushProperty(SerializerBase::PROP_NAMESPACE, REMOTING__NAMESPACE);
		serializeImpl(value, ser);
		ser.popProperty(SerializerBase::PROP_NAMESPACE);
		ser.serializeStructEnd(name);
	}

	static void serializeImpl(const OASIS::WSRF::BF2::Description& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"value"s,""s};
		TypeSerializer<Poco::Optional<std::string>>::serialize(REMOTING__NAMES[0], value.getValue(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_OASIS_WSRF_BF2_Description_INCLUDED

