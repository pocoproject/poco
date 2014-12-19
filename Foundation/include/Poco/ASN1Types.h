//
// ASN1Types.h
//
// $Id: //poco/1.4/Foundation/include/Poco/ASN1Types.h#1 $
//
// Library: Foundation
// Package: Streams
// Module:  ASN1Types
//
// Definition of the ASN1Types namespace classes.
//
// Copyright (c) 2010, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Foundation_ASN1Types_INCLUDED
#define Foundation_ASN1Types_INCLUDED


#include "Poco/Foundation.h"
#include "Poco/ASN1.h"
#include "Poco/SharedPtr.h"

#include <vector>


namespace Poco {
namespace ASN1Types {


class Foundation_API Unknown : public ASN1
	/// Unknow ASN1 type.
	///
	/// Stores the raw value read from the stream.
{
public:

	Unknown(ASN1Type type);

	Unknown(ASN1Type type, const std::string &value);

	std::string toString() const;

	std::string typeName() const;

	const std::string &getValue() const;
		/// Returns the raw value as a string.

	void dump(std::ostream &stream, int ident) const;

protected:
	Poco::UInt32 getDataLength() const;
	void encodeData(Poco::BinaryWriter &stream) const;
	void decodeData(Poco::SharedPtr<ASN1Factory> factory, Poco::BinaryReader &stream, Poco::UInt32 length);
private:
	std::string _value;
};


class Foundation_API Null : public ASN1
	/// ASN1 Null Universal value.
{
public:
	Null();

	std::string toString() const;

	std::string typeName() const;

	void dump(std::ostream &stream, int ident) const;

protected:
	Poco::UInt32 getDataLength() const;
	void encodeData(Poco::BinaryWriter &stream) const;
	void decodeData(Poco::SharedPtr<ASN1Factory> factory, Poco::BinaryReader &stream, Poco::UInt32 length);
};


class Foundation_API Boolean : public ASN1
	/// ASN1 Boolean Universal value.
{
public:

	Boolean();

	Boolean(bool value);

	bool getValue() const;

	std::string toString() const;

	std::string typeName() const;

	void dump(std::ostream &stream, int ident) const;

protected:
	Poco::UInt32 getDataLength() const;
	void encodeData(Poco::BinaryWriter &stream) const;
	void decodeData(Poco::SharedPtr<ASN1Factory> factory, Poco::BinaryReader &stream, Poco::UInt32 length);
private:
	bool _value;
};


class Foundation_API Integer : public ASN1
{
	/// ASN1 Integer Universal value.
public:

	Integer();

	Integer(Poco::UInt32 value);

	Integer(ASN1Type type, Poco::UInt32 value);

	Poco::UInt32 getValue() const;

	std::string toString() const;

	std::string typeName() const;

	void dump(std::ostream &stream, int ident) const;

protected:
	Poco::UInt32 getDataLength() const;
	void encodeData(Poco::BinaryWriter &stream) const;
	void decodeData(Poco::SharedPtr<ASN1Factory> factory, Poco::BinaryReader &stream, Poco::UInt32 length);
private:
	Poco::UInt32 _value;
};

#ifdef POCO_HAVE_INT64

class Foundation_API Integer64 : public ASN1
{
	/// ASN1 Integer Universal value.
public:

	Integer64();

	Integer64(Poco::UInt64 value);

	Integer64(ASN1Type type, Poco::UInt64 value);

	Poco::UInt64 getValue() const;

	std::string toString() const;

	std::string typeName() const;

	void dump(std::ostream &stream, int ident) const;

protected:
	Poco::UInt32 getDataLength() const;
	void encodeData(Poco::BinaryWriter &stream) const;
	void decodeData(Poco::SharedPtr<ASN1Factory> factory, Poco::BinaryReader &stream, Poco::UInt32 length);
private:
	Poco::UInt64 _value;
};

#endif

class Foundation_API OctetString : public ASN1
	/// ASN1 OctetString Universal value.
{
public:

	OctetString();

	OctetString(const std::string &value);

	OctetString(ASN1Type type, const std::string &value = "");

	std::string getValue() const;

	std::string toString() const;

	std::string typeName() const;

	void dump(std::ostream &stream, int ident) const;

protected:
	Poco::UInt32 getDataLength() const;
	void encodeData(Poco::BinaryWriter &stream) const;
	void decodeData(Poco::SharedPtr<ASN1Factory> factory, Poco::BinaryReader &stream, Poco::UInt32 length);
private:
	std::string _value;
};


class Foundation_API ObjectIdentifier : public ASN1
	/// ASN1 Object Identifier Universal value.
{
public:

	ObjectIdentifier();

	ObjectIdentifier(const std::string &value);

	std::string toString() const;

	std::string typeName() const;

	void dump(std::ostream &stream, int ident) const;

protected:
	Poco::UInt32 getDataLength() const;
	void encodeData(Poco::BinaryWriter &stream) const;
	void decodeData(Poco::SharedPtr<ASN1Factory> factory, Poco::BinaryReader &stream, Poco::UInt32 length);
private:
	std::vector<Poco::UInt32> _value;
};


class SequenceData : public std::vector<ASN1::Ptr>
{
	/// Data for the Sequence type.
public:
	SequenceData();
	SequenceData(ASN1::Ptr v1, ASN1::Ptr v2 = NULL, ASN1::Ptr v3 = NULL);
};


class Sequence : public ASN1
	/// ASN1 Sequence Universal value.
{
public:

	Sequence();

	Sequence(ASN1Type type);

	Sequence(const SequenceData &value);

	Sequence(ASN1Type type, const SequenceData &value);

	const SequenceData &getSequenceData() const;

	std::string typeName() const;

	void dump(std::ostream &stream, int ident) const;

protected:
	ASN1::Ptr getDataType(int pos) const;
	Poco::UInt32 getDataLength() const;
	void encodeData(Poco::BinaryWriter &stream) const;
	void decodeData(Poco::SharedPtr<ASN1Factory> factory, Poco::BinaryReader &stream, Poco::UInt32 length);

	SequenceData _value;
};


} } // namespace Poco::ASN1Types


#endif // Foundation_ASN1Types_INCLUDED
