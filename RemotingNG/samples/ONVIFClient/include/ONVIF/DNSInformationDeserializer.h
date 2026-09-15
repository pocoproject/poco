//
// DNSInformationDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_DNSInformation_INCLUDED
#define TypeDeserializer_ONVIF_DNSInformation_INCLUDED


#include "ONVIF/DNSInformation.h"
#include "ONVIF/DNSInformationExtensionDeserializer.h"
#include "ONVIF/DNSInformationExtensionSerializer.h"
#include "ONVIF/IPAddressDeserializer.h"
#include "ONVIF/IPAddressSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::DNSInformation>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::DNSInformation& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::DNSInformation& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"FromDHCP"s,"SearchDomain"s,"DNSFromDHCP"s,"DNSManual"s,"Extension"s};
		bool ret = false;
		bool gen_fromDHCP;
		ret = TypeDeserializer<bool>::deserialize(REMOTING__NAMES[0], true, deser, gen_fromDHCP);
		if (ret) value.setFromDHCP(gen_fromDHCP);
		std::vector<std::string> gen_searchDomain;
		ret = TypeDeserializer<std::vector<std::string>>::deserialize(REMOTING__NAMES[1], false, deser, gen_searchDomain);
		if (ret) value.setSearchDomain(gen_searchDomain);
		std::vector<ONVIF::IPAddress> gen_dNSFromDHCP;
		ret = TypeDeserializer<std::vector<ONVIF::IPAddress>>::deserialize(REMOTING__NAMES[2], false, deser, gen_dNSFromDHCP);
		if (ret) value.setDNSFromDHCP(gen_dNSFromDHCP);
		std::vector<ONVIF::IPAddress> gen_dNSManual;
		ret = TypeDeserializer<std::vector<ONVIF::IPAddress>>::deserialize(REMOTING__NAMES[3], false, deser, gen_dNSManual);
		if (ret) value.setDNSManual(gen_dNSManual);
		Poco::SharedPtr<ONVIF::DNSInformationExtension> gen_extension;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::DNSInformationExtension>>::deserialize(REMOTING__NAMES[4], false, deser, gen_extension);
		if (ret) value.setExtension(gen_extension);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_DNSInformation_INCLUDED

