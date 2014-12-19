//
// SNMPClientTypes.cpp
//
// $Id: //poco/1.4/Net/src/SNMPClientTypes.cpp#1 $
//
// Library: Net
// Package: SNMP
// Module:  SNMPClientTypes
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Net/SNMPClientTypes.h"
#include "Poco/Net/SNMPClientRawTypes.h"


namespace Poco {
namespace Net {
namespace SNMPTypes {


///
/// VarBind
///


VarBind::VarBind() : Poco::RefCountedObject()
{

}


VarBind::VarBind(ASN1::Ptr data) : Poco::RefCountedObject()
{
	decode(data);
}


VarBind::VarBind(const std::string oid, ASN1::Ptr value) :
	 Poco::RefCountedObject(), _oid(oid), _value(value)
{

}


const std::string &VarBind::oid() const
{
	return _oid;
}


void VarBind::setOid(const std::string &value)
{
	_oid = value;
}


ASN1::Ptr VarBind::value() const
{
	return _value;
}


void VarBind::setValue(ASN1::Ptr value)
{
	_value = value;
}


void VarBind::decode(ASN1::Ptr data)
{
	if (data->getType().universalValue() != ASN1Type::Sequence)
		throw Poco::DataException("Data must be ASN.1 sequence for VarBind");
	Poco::AutoPtr<Sequence> seq(data.cast<Sequence>());
	if (seq->getSequenceData().size() != 2)
		throw Poco::DataException("Data must be ASN.1 sequence with 2 values for VarBind");
	if (seq->getSequenceData()[0]->getType().universalValue() != ASN1Type::ObjectIdentifier)
		throw Poco::DataException("Invalid VarBind data format");

	_oid = seq->getSequenceData()[0].cast<ObjectIdentifier>()->toString();
	_value = seq->getSequenceData()[1];
}


ASN1::Ptr VarBind::encode()
{
	Poco::ASN1Types::SequenceData seqdata;
	seqdata.push_back(new ObjectIdentifier(_oid));
	seqdata.push_back(_value);
	return ASN1::Ptr(new Sequence(seqdata));
}


///
/// VarBindList
///


VarBindList::VarBindList() : Poco::RefCountedObject()
{

}


VarBindList::VarBindList(ASN1::Ptr data) : Poco::RefCountedObject()
{
	decode(data);
}


void VarBindList::clear()
{
	_list.clear();
}


void VarBindList::add(VarBind::Ptr value)
{
	_list.push_back(value);
}


VarBindList::list_t &VarBindList::list()
{
	return _list;
}


void VarBindList::decode(ASN1::Ptr data)
{
	_list.clear();

	if (data->getType().universalValue() != ASN1Type::Sequence)
		throw Poco::DataException("Data must be ASN.1 sequence for VarBindList");
	Poco::AutoPtr<Sequence> seq(data.cast<Sequence>());

	for (SequenceData::const_iterator i=seq->getSequenceData().begin(); i!=seq->getSequenceData().end(); i++)
	{
		_list.push_back(new VarBind(*i));
	}
}


ASN1::Ptr VarBindList::encode()
{
	Poco::ASN1Types::SequenceData seqdata;
	for (list_t::iterator i=_list.begin(); i!=_list.end(); i++)
	{
		seqdata.push_back((*i)->encode());
	}
	return ASN1::Ptr(new Sequence(seqdata));

}


///
/// PDU
///


PDU::PDU() : 
	Poco::RefCountedObject(), _type(ASN1Type(ASN1Type::Sequence, true)), _requestid(0), _error(0), _errorindex(0)
{

}


PDU::PDU(ASN1::Ptr data) : 
	Poco::RefCountedObject(), _type(ASN1Type(ASN1Type::Sequence, true)), _requestid(0), _error(0), _errorindex(0)
{
	decode(data);
}


const ASN1Type &PDU::type() const
{
	return _type;
}


void PDU::setType(const ASN1Type &value)
{
	_type = value;
}


int PDU::requestId() const
{
	return _requestid;
}


void PDU::setRequestId(int value)
{
	_requestid = value;
}


int PDU::error() const
{
	return _error;
}


void PDU::setError(int value)
{
	_error = value;
}


int PDU::errorIndex() const
{
	return _errorindex;
}


void PDU::setErrorIndex(int value)
{
	_errorindex = value;
}


int PDU::nonRepeaters() const
{
	return _error;
}


void PDU::setNonRepeaters(int value)
{
	_error = value;
}


int PDU::maxRepetitions() const
{
	return _errorindex;
}


void PDU::setMaxRepetitions(int value)
{
	_errorindex = value;
}


VarBindList &PDU::varBindList()
{
	return _varbindlist;
}


void PDU::decode(ASN1::Ptr data)
{
	_varbindlist.clear();

	Poco::AutoPtr<Sequence> seq(data.cast<Sequence>());
	if (seq.isNull())
		throw Poco::DataException("Data must be ASN.1 sequence for PDU");
	if (seq->getSequenceData().size() != 4)
		throw Poco::DataException("Data must be ASN.1 sequence with 4 values for PDU");
	
	if (seq->getSequenceData()[0]->getType().universalValue() != ASN1Type::Integer ||
		seq->getSequenceData()[1]->getType().universalValue() != ASN1Type::Integer ||
		seq->getSequenceData()[2]->getType().universalValue() != ASN1Type::Integer ||
		seq->getSequenceData()[3]->getType().universalValue() != ASN1Type::Sequence)
		throw Poco::DataException("Invalid VarBind data format");

	_type = seq->getType();
	_requestid = seq->getSequenceData()[0].cast<Integer>()->getValue();
	_error = seq->getSequenceData()[1].cast<Integer>()->getValue();
	_errorindex = seq->getSequenceData()[2].cast<Integer>()->getValue();
	_varbindlist.decode(seq->getSequenceData()[3]);
}


ASN1::Ptr PDU::encode()
{
	Poco::ASN1Types::SequenceData seqdata;
	seqdata.push_back(new Integer(_requestid));
	seqdata.push_back(new Integer(_error));
	seqdata.push_back(new Integer(_errorindex));
	seqdata.push_back(_varbindlist.encode());
	return ASN1::Ptr(new Sequence(_type, seqdata));
}


///
/// SNMPMessage
///


SNMPMessage::SNMPMessage() : Poco::RefCountedObject()
{

}


SNMPMessage::SNMPMessage(ASN1::Ptr data) : Poco::RefCountedObject()
{
	decode(data);
}


int SNMPMessage::version() const
{
	return _version;
}


void SNMPMessage::setVersion(int value)
{
	_version = value;
}

const std::string &SNMPMessage::community() const
{
	return _community;
}


void SNMPMessage::setCommunity(const std::string &value)
{
	_community = value;
}


PDU &SNMPMessage::pdu()
{
	return _pdu;
}


void SNMPMessage::decode(ASN1::Ptr data)
{
	if (data->getType().universalValue() != ASN1Type::Sequence)
		throw Poco::DataException("Data must be ASN.1 sequence for SNMPMessage");
	Poco::AutoPtr<Sequence> seq(data.cast<Sequence>());
	if (seq->getSequenceData().size() != 3)
		throw Poco::DataException("Data must be ASN.1 sequence with 3 values for SNMPMessage");
	
	if (seq->getSequenceData()[0]->getType().universalValue() != ASN1Type::Integer ||
		seq->getSequenceData()[1]->getType().universalValue() != ASN1Type::OctetString)
		throw Poco::DataException("Invalid SNMPMessage data format");

	_version = seq->getSequenceData()[0].cast<Integer>()->getValue();
	_community = seq->getSequenceData()[1].cast<OctetString>()->getValue();
	_pdu.decode(seq->getSequenceData()[2]);
}


ASN1::Ptr SNMPMessage::encode()
{
	Poco::ASN1Types::SequenceData seqdata;
	seqdata.push_back(new Integer(_version));
	seqdata.push_back(new OctetString(_community));
	seqdata.push_back(_pdu.encode());
	return ASN1::Ptr(new Sequence(seqdata));
}


} } } // namespace Poco::Net::SNMPTypes
