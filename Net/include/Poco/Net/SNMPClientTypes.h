//
// SNMPClientTypes.h
//
// $Id: //poco/1.4/Net/include/Poco/Net/SNMPClientTypes.h#1 $
//
// Library: Net
// Package: SNMP
// Module:  SNMPClientRawTypes
//
// Definition of the SNMP client types.
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Net_SNMPClientTypes_INCLUDED
#define Net_SNMPClientTypes_INCLUDED


#include "Poco/Net/Net.h"
#include "Poco/ASN1.h"
#include "Poco/RefCountedObject.h"


#include <vector>


namespace Poco {
namespace Net {
namespace SNMPTypes {


class Net_API VarBind : public Poco::RefCountedObject
	/// VarBind SNMP type.
{
public:

	typedef Poco::AutoPtr<VarBind> Ptr;

	VarBind();

	VarBind(ASN1::Ptr data);

	VarBind(const std::string oid, ASN1::Ptr data);

	const std::string &oid() const;

	void setOid(const std::string &value);

	ASN1::Ptr value() const;

	void setValue(ASN1::Ptr value);

	void decode(ASN1::Ptr data);

	ASN1::Ptr encode();

private:
	std::string _oid;
	ASN1::Ptr _value;
};


class Net_API VarBindList : public Poco::RefCountedObject
{
	/// VarBindList SNMP type.
public:

	typedef std::vector<VarBind::Ptr> list_t;

	VarBindList();

	VarBindList(ASN1::Ptr data);

	void clear();

	void add(VarBind::Ptr value);

	list_t &list();

	void decode(ASN1::Ptr data);

	ASN1::Ptr encode();

private:
	list_t _list;
};


class Net_API PDU : public Poco::RefCountedObject
	/// PDU SNMP type.
{
public:
	enum Errors {
		E_NOERROR				= 0,
		E_TOOBIG				= 1,
		E_NOSUCHNAME			= 2,
		E_BADVALUE				= 3,
		E_READONLY				= 4,
		E_GENERR				= 5,
		E_NOACCESS				= 6,
		E_WRONGTYPE				= 7,
		E_WRONGLENGTH			= 8,
		E_WRONGENCODING			= 9,
		E_WRONGVALUE			= 10,
		E_NOCREATION			= 11,
		E_INCONSISTENTVALUE		= 12,
		E_RESOURCEUNAVAILABLE	= 13,
		E_COMMITFAILED			= 14,
		E_UNDOFAILED			= 15,
		E_AUTHORIZATIONERROR	= 16,
		E_NOTWRITABLE			= 17,
		E_INCONSISTENTNAME		= 18
	};

	PDU();

	PDU(ASN1::Ptr data);

	const ASN1Type &type() const;

	void setType(const ASN1Type &value);

	int requestId() const;

	void setRequestId(int value);

	int error() const;

	void setError(int value);

	int errorIndex() const;

	void setErrorIndex(int value);

	int nonRepeaters() const;

	void setNonRepeaters(int value);

	int maxRepetitions() const;

	void setMaxRepetitions(int value);

	VarBindList &varBindList();

	void decode(ASN1::Ptr data);

	ASN1::Ptr encode();

private:
	ASN1Type _type;
	int _requestid;
	int _error;
	int _errorindex;
	VarBindList _varbindlist;
};


class Net_API SNMPMessage : public Poco::RefCountedObject
	/// Message SNMP type.
{
public:

	typedef Poco::AutoPtr<SNMPMessage> Ptr;

	SNMPMessage();

	SNMPMessage(ASN1::Ptr data);

	int version() const;

	void setVersion(int value);

	const std::string &community() const;

	void setCommunity(const std::string &value);

	PDU &pdu();

	void decode(ASN1::Ptr data);

	ASN1::Ptr encode();

private:
	int _version;
	std::string _community;
	PDU _pdu;
};


} } } // namespace Poco::Net::SNMPTypes


#endif // Net_SNMPClientTypes_INCLUDED
