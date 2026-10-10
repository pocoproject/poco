//
// RemoteUserDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_RemoteUser_INCLUDED
#define TypeDeserializer_ONVIF_RemoteUser_INCLUDED


#include "ONVIF/RemoteUser.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::RemoteUser>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::RemoteUser& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::RemoteUser& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Username"s,"Password"s,"UseDerivedPassword"s};
		bool ret = false;
		std::string gen_username;
		ret = TypeDeserializer<std::string>::deserialize(REMOTING__NAMES[0], true, deser, gen_username);
		if (ret) value.setUsername(gen_username);
		Poco::Optional<std::string> gen_password;
		ret = TypeDeserializer<Poco::Optional<std::string>>::deserialize(REMOTING__NAMES[1], false, deser, gen_password);
		if (ret) value.setPassword(gen_password);
		bool gen_useDerivedPassword;
		ret = TypeDeserializer<bool>::deserialize(REMOTING__NAMES[2], true, deser, gen_useDerivedPassword);
		if (ret) value.setUseDerivedPassword(gen_useDerivedPassword);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_RemoteUser_INCLUDED

