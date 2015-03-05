//
// SNMPClientTypes.h
//
// $Id: //poco/1.4/NetEx/include/Poco/Net/SNMPClientTypes.h#1 $
//
// Library: NetEx
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


#ifndef NetEx_SNMPClientTypes_INCLUDED
#define NetEx_SNMPClientTypes_INCLUDED


#include "Poco/Net/NetEx.h"
#include "Poco/ASN/ASN1.h"
#include "Poco/RefCountedObject.h"


#include <vector>


namespace Poco {
namespace Net {
namespace SNMPTypes {


class NetEx_API VarBind : public Poco::RefCountedObject
	/// VarBind SNMP type.
{
public:

	typedef Poco::AutoPtr<VarBind> Ptr;

	VarBind();

	VarBind(ASN::ASN1::Ptr data);

	VarBind(const std::string oid, ASN::ASN1::Ptr data);

	const std::string &oid() const;

	void setOid(const std::string &value);

	ASN::ASN1::Ptr value() const;

	void setValue(ASN::ASN1::Ptr value);

	void decode(ASN::ASN1::Ptr data);

	ASN::ASN1::Ptr encode();

private:
	std::string _oid;
	ASN::ASN1::Ptr _value;
};


class NetEx_API VarBindList : public Poco::RefCountedObject
{
	/// VarBindList SNMP type.
public:

	typedef std::vector<VarBind::Ptr> list_t;

	VarBindList();

	VarBindList(ASN::ASN1::Ptr data);

	void clear();

	void add(VarBind::Ptr value);

	list_t &list();

	void decode(ASN::ASN1::Ptr data);

	ASN::ASN1::Ptr encode();

private:
	list_t _list;
};


class NetEx_API PDU : public Poco::RefCountedObject
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

	PDU(ASN::ASN1::Ptr data);

	const ASN::ASN1Type &type() const;

	void setType(const ASN::ASN1Type &value);

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

	void decode(ASN::ASN1::Ptr data);

	ASN::ASN1::Ptr encode();

private:
	ASN::ASN1Type _type;
	int _requestid;
	int _error;
	int _errorindex;
	VarBindList _varbindlist;
};


class NetEx_API SNMPMessage : public Poco::RefCountedObject
	/// Message SNMP type.
{
public:

	typedef Poco::AutoPtr<SNMPMessage> Ptr;

	SNMPMessage();

	SNMPMessage(ASN::ASN1::Ptr data);

	int version() const;

	void setVersion(int value);

	const std::string &community() const;

	void setCommunity(const std::string &value);

	PDU &pdu();

	void decode(ASN::ASN1::Ptr data);

	ASN::ASN1::Ptr encode();

private:
	int _version;
	std::string _community;
	PDU _pdu;
};


} } } // namespace Poco::Net::SNMPTypes


#endif // NetEx_SNMPClientTypes_INCLUDED
