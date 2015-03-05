//
// ASN1Types.h
//
// $Id: //poco/1.4/ASN/include/Poco/ASN/ASN1Types.h#1 $
//
// Library: ASN
// Package: ASN
// Module:  ASN1Types
//
// Definition of the ASN1Types namespace classes.
//
// Copyright (c) 2010, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef ASN_ASN1Types_INCLUDED
#define ASN_ASN1Types_INCLUDED


#include "Poco/ASN/ASN.h"
#include "Poco/ASN/ASN1.h"
#include "Poco/SharedPtr.h"

#include <vector>


namespace Poco {
namespace ASN {
namespace ASN1Types {


class ASN_API Unknown : public ASN1
	/// Unknown ASN1 type.
	///
	/// Stores the raw value read from the stream.
{
public:
	Unknown(ASN1Type type);
		/// Construct using the passed type.

	Unknown(ASN1Type type, const std::string &value);
		/// Construct using the passed type and value.

	std::string toString() const;
		/// Returns an user-friendly string representation of the data.

	std::string typeName() const;
		/// Returns a string representation of the type name.

	const std::string &getValue() const;
		/// Returns the raw value as a string.

	void dump(std::ostream &stream, int ident) const;
		/// Dumps the data in indented format.

protected:
	Poco::UInt32 getDataLength() const;
	void encodeData(Poco::BinaryWriter &stream) const;
	void decodeData(Poco::SharedPtr<ASN1Factory> factory, Poco::BinaryReader &stream, Poco::UInt32 length);

private:
	std::string _value;
};


class ASN_API Null : public ASN1
	/// ASN1 Null Universal value.
{
public:
	Null();
		/// Default constructor.

	std::string toString() const;
		/// Returns an user-friendly string representation of the data.

	std::string typeName() const;
		/// Returns a string representation of the type name.

	void dump(std::ostream &stream, int ident) const;
		/// Dumps the data in indented format.

protected:
	Poco::UInt32 getDataLength() const;
	void encodeData(Poco::BinaryWriter &stream) const;
	void decodeData(Poco::SharedPtr<ASN1Factory> factory, Poco::BinaryReader &stream, Poco::UInt32 length);
};


class ASN_API Boolean : public ASN1
	/// ASN1 Boolean Universal value.
{
public:
	Boolean();
		/// Default constructor.

	Boolean(bool value);
		/// Construct using the passed value.

	bool getValue() const;
		/// Returns the contained value.

	std::string toString() const;
		/// Returns an user-friendly string representation of the data.

	std::string typeName() const;
		/// Returns a string representation of the type name.

	void dump(std::ostream &stream, int ident) const;
		/// Dumps the data in indented format.

protected:
	Poco::UInt32 getDataLength() const;
	void encodeData(Poco::BinaryWriter &stream) const;
	void decodeData(Poco::SharedPtr<ASN1Factory> factory, Poco::BinaryReader &stream, Poco::UInt32 length);

private:
	bool _value;
};


class ASN_API Integer : public ASN1
	/// ASN1 Integer Universal value.
{
public:
	Integer();
		/// Default constructor.

	Integer(Poco::UInt32 value);
		/// Construct using the passed value.

	Integer(ASN1Type type, Poco::UInt32 value);
		/// Construct using the passed type and value.

	Poco::UInt32 getValue() const;
		/// Returns the contained value.

	std::string toString() const;
		/// Returns an user-friendly string representation of the data.

	std::string typeName() const;
		/// Returns a string representation of the type name.

	void dump(std::ostream &stream, int ident) const;
		/// Dumps the data in indented format.

protected:
	Poco::UInt32 getDataLength() const;
	void encodeData(Poco::BinaryWriter &stream) const;
	void decodeData(Poco::SharedPtr<ASN1Factory> factory, Poco::BinaryReader &stream, Poco::UInt32 length);

private:
	Poco::UInt32 _value;
};

#ifdef POCO_HAVE_INT64

class ASN_API Integer64 : public ASN1
	/// ASN1 Integer Universal value.
{
public:
	Integer64();
		/// Default constructor.

	Integer64(Poco::UInt64 value);
		/// Construct using the passed value.

	Integer64(ASN1Type type, Poco::UInt64 value);
		/// Construct using the passed type and value.

	Poco::UInt64 getValue() const;
		/// Returns the contained value.

	std::string toString() const;
		/// Returns an user-friendly string representation of the data.

	std::string typeName() const;
		/// Returns a string representation of the type name.

	void dump(std::ostream &stream, int ident) const;
		/// Dumps the data in indented format.

protected:
	Poco::UInt32 getDataLength() const;
	void encodeData(Poco::BinaryWriter &stream) const;
	void decodeData(Poco::SharedPtr<ASN1Factory> factory, Poco::BinaryReader &stream, Poco::UInt32 length);

private:
	Poco::UInt64 _value;
};

#endif

class ASN_API OctetString : public ASN1
	/// ASN1 OctetString Universal value.
{
public:
	OctetString();
		/// Default constructor.

	OctetString(const std::string &value);
		/// Construct using the passed value.

	OctetString(ASN1Type type, const std::string &value = "");
		/// Construct using the passed type and value.

	std::string getValue() const;
		/// Returns the contained value.

	std::string toString() const;
		/// Returns an user-friendly string representation of the data.

	std::string typeName() const;
		/// Returns a string representation of the type name.

	void dump(std::ostream &stream, int ident) const;
		/// Dumps the data in indented format.

protected:
	Poco::UInt32 getDataLength() const;
	void encodeData(Poco::BinaryWriter &stream) const;
	void decodeData(Poco::SharedPtr<ASN1Factory> factory, Poco::BinaryReader &stream, Poco::UInt32 length);

private:
	std::string _value;
};


class ASN_API ObjectIdentifier : public ASN1
	/// ASN1 Object Identifier Universal value.
{
public:
	ObjectIdentifier();
		/// Default constructor.

	ObjectIdentifier(const std::string &value);
		/// Construct using the passed value.

	std::string toString() const;
		/// Returns an user-friendly string representation of the data.

	std::string typeName() const;
		/// Returns a string representation of the type name.

	void dump(std::ostream &stream, int ident) const;
		/// Dumps the data in indented format.

protected:
	Poco::UInt32 getDataLength() const;
	void encodeData(Poco::BinaryWriter &stream) const;
	void decodeData(Poco::SharedPtr<ASN1Factory> factory, Poco::BinaryReader &stream, Poco::UInt32 length);

private:
	std::vector<Poco::UInt32> _value;
};


class ASN_API SequenceData : public std::vector<ASN1::Ptr>
	/// Data for the Sequence type.
{
public:
	SequenceData();
		/// Default constructor.

	SequenceData(ASN1::Ptr v1, ASN1::Ptr v2 = NULL, ASN1::Ptr v3 = NULL);
		/// Construct adding 3 values.

};


class ASN_API Sequence : public ASN1
	/// ASN1 Sequence Universal value.
{
public:
	Sequence();
		/// Default constructor.

	Sequence(ASN1Type type);
		/// Construct using the passed type.

	Sequence(const SequenceData &value);
		/// Construct using the passed value.

	Sequence(ASN1Type type, const SequenceData &value);
		/// Construct using the passed type and value.

	const SequenceData &getSequenceData() const;
		/// Returns the contained value.

	std::string typeName() const;
		/// Returns a string representation of the type name.

	void dump(std::ostream &stream, int ident) const;
		/// Dumps the data in indented format.

protected:
	ASN1::Ptr getDataType(int pos) const;
	Poco::UInt32 getDataLength() const;
	void encodeData(Poco::BinaryWriter &stream) const;
	void decodeData(Poco::SharedPtr<ASN1Factory> factory, Poco::BinaryReader &stream, Poco::UInt32 length);

private:
	SequenceData _value;
};


} } } // namespace Poco::ASN::ASN1Types


#endif // ASN_ASN1Types_INCLUDED
