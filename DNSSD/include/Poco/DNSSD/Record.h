//
// Record.h
//
// $Id: //poco/1.7/DNSSD/include/Poco/DNSSD/Record.h#1 $
//
// Library: DNSSD
// Package: Core
// Module:  Record
//
// Definition of the Record class.
//
// Copyright (c) 2006-2024, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef DNSSD_Record_INCLUDED
#define DNSSD_Record_INCLUDED


#include "Poco/DNSSD/DNSSD.h"


namespace Poco {
namespace DNSSD {


class DNSSD_API Record
	/// Service stores the information found in a DNSSD record.
{
public:
	enum RecordType
	{
		RT_A         = 1,      /// Host address.
		RT_NS        = 2,      /// Authoritative server.
		RT_MD        = 3,      /// Mail destination.
		RT_MF        = 4,      /// Mail forwarder.
		RT_CNAME     = 5,      /// Canonical name.
		RT_SOA       = 6,      /// Start of authority zone.
		RT_MB        = 7,      /// Mailbox domain name.
		RT_MG        = 8,      /// Mail group member.
		RT_MR        = 9,      /// Mail rename name.
		RT_NULL      = 10,     /// Null resource record.
		RT_WKS       = 11,     /// Well known service.
		RT_PTR       = 12,     /// Domain name pointer.
		RT_HINFO     = 13,     /// Host information.
		RT_MINFO     = 14,     /// Mailbox information.
		RT_MX        = 15,     /// Mail routing information.
		RT_TXT       = 16,     /// One or more text strings (NOT "zero or more...").
		RT_RP        = 17,     /// Responsible person.
		RT_AFSDB     = 18,     /// AFS cell database.
		RT_X25       = 19,     /// X_25 calling address.
		RT_ISDN      = 20,     /// ISDN calling address.
		RT_RT        = 21,     /// Router.
		RT_NSAP      = 22,     /// NSAP address.
		RT_NSAP_PTR  = 23,     /// Reverse NSAP lookup (deprecated).
		RT_SIG       = 24,     /// Security signature.
		RT_KEY       = 25,     /// Security key.
		RT_PX        = 26,     /// X.400 mail mapping.
		RT_GPOS      = 27,     /// Geographical position (withdrawn).
		RT_AAAA      = 28,     /// IPv6 Address.
		RT_LOC       = 29,     /// Location Information.
		RT_NXT       = 30,     /// Next domain (security).
		RT_EID       = 31,     /// Endpoint identifier.
		RT_NIMLOC    = 32,     /// Nimrod Locator.
		RT_SRV       = 33,     /// Server Selection.
		RT_ATMA      = 34,     /// ATM Address
		RT_NAPTR     = 35,     /// Naming Authority PoinTeR
		RT_KX        = 36,     /// Key Exchange
		RT_CERT      = 37,     /// Certification record
		RT_A6        = 38,     /// IPv6 Address (deprecated)
		RT_DNAME     = 39,     /// Non-terminal DNAME (for IPv6)
		RT_SINK      = 40,     /// Kitchen sink (experimental)
		RT_OPT       = 41,     /// EDNS0 option (meta-RR)
		RT_APL       = 42,     /// Address Prefix List
		RT_DS        = 43,     /// Delegation Signer
		RT_SSHFP     = 44,     /// SSH Key Fingerprint
		RT_IPSECKEY  = 45,     /// IPSECKEY
		RT_RRSIG     = 46,     /// RRSIG
		RT_NSEC      = 47,     /// Denial of Existence
		RT_DNSKEY    = 48,     /// DNSKEY
		RT_DHCID     = 49,     /// DHCP Client Identifier
		RT_NSEC3     = 50,     /// Hashed Authenticated Denial of Existence
		RT_NSEC3PARAM= 51,     /// Hashed Authenticated Denial of Existence
		RT_HIP       = 55,     /// Host Identity Protocol

		RT_SPF       = 99,     /// Sender Policy Framework for E-Mail
		RT_UINFO     = 100,    /// IANA-Reserved
		RT_UID       = 101,    /// IANA-Reserved
		RT_GID       = 102,    /// IANA-Reserved
		RT_UNSPEC    = 103,    /// IANA-Reserved

		RT_TKEY      = 249,    /// Transaction key
		RT_TSIG      = 250,    /// Transaction signature.
		RT_IXFR      = 251,    /// Incremental zone transfer.
		RT_AXFR      = 252,    /// Transfer zone of authority.
		RT_MAILB     = 253,    /// Transfer mailbox records.
		RT_MAILA     = 254,    /// Transfer mail agent records.
		RT_ANY       = 255     /// Wildcard match.
	};

	enum RecordClass
	{
		RC_IN = 1              /// Internet
	};

	Record();
		/// Creates an empty Record.

	Record(Poco::Int32 networkInterface, const std::string& name, Poco::UInt16 type, Poco::UInt16 clazz, Poco::UInt16 length, const void* data, Poco::UInt32 ttl);
		/// Creates the Record using the given information.
		///
		///    - networkInterface specifies the index of the interface the record was discovered on.
		///    - name specifies the full domain name of the record.
		///    - type specifies the record's type (e.g., RT_PTR).
		///      Suitable values can be found in the RecordType enumeration.
		///    - clazz specifies the record's class (usually RC_IN).
		///    - length specifies the length in bytes of the record's data.
		///    - data points to the actual data. This pointer must be valid for the entire
		///      lifetime of the Record object, as it's content is not copied.
		///    - ttl specifies the time-to-live of the record in seconds.

	Record(const std::string& name, Poco::UInt16 type, Poco::UInt16 length, const void* data, Poco::UInt32 ttl = 0);
		/// Creates the Record using the given information.
		///
		///    - name specifies the full domain name of the record.
		///    - type specifies the record's type (e.g., RT_PTR).
		///      Suitable values can be found in the RecordType enumeration.
		///    - length specifies the length in bytes of the record's data.
		///    - data points to the actual data. This pointer must be valid for the entire
		///      lifetime of the Record object, as it's content is not copied.
		///    - ttl specifies the time-to-live of the record in seconds.
		///      If ttl is 0, the system will chose an appropriate value.
		///
		/// The record class is set to RT_IN.

	~Record();
		/// Destroys the Service.

	Poco::Int32 networkInterface() const;
		/// The id of the interface on which the remote service is running, or zero
		/// if no interface has been specified.

	const std::string& name() const;
		/// The full domain name of the record.

	Poco::UInt16 type() const;
		/// The record's type (e.g., RT_PTR).

	Poco::UInt16 clazz() const;
		/// The record's class, which is usually RC_IN.

	Poco::UInt16 length() const;
		/// The length of the record.

	const void* data() const;
		/// The actual data.

	Poco::UInt32 ttl() const;
		/// The time-to-live for the record in seconds.

private:
	Poco::Int32   _networkInterface;
	std::string   _name;
	Poco::UInt16  _type;
	Poco::UInt16  _clazz;
	Poco::UInt16  _length;
	const void*   _data;
	Poco::UInt32  _ttl;
};


//
// inlines
//
inline Poco::Int32 Record::networkInterface() const
{
	return _networkInterface;
}


inline const std::string& Record::name() const
{
	return _name;
}


inline Poco::UInt16 Record::type() const
{
	return _type;
}


inline Poco::UInt16 Record::clazz() const
{
	return _clazz;
}


inline Poco::UInt16 Record::length() const
{
	return _length;
}


inline const void* Record::data() const
{
	return _data;
}


inline Poco::UInt32 Record::ttl() const
{
	return _ttl;
}


} } // namespace Poco::DNSSD


#endif // DNSSD_Service_INCLUDED
