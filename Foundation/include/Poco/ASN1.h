//
// ASN1.h
//
// $Id: //poco/1.4/Foundation/include/Poco/ASN1.h#1 $
//
// Library: Foundation
// Package: Streams
// Module:  ASN1
//
// Definition of the ASN1 class.
//
// Copyright (c) 2010, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Foundation_ASN1_INCLUDED
#define Foundation_ASN1_INCLUDED


#include "Poco/Foundation.h"
#include "Poco/AutoPtr.h"
#include "Poco/SharedPtr.h"
#include "Poco/RefCountedObject.h"
#include "Poco/BinaryReader.h"
#include "Poco/BinaryWriter.h"

#include <iostream>
#include <utility>


namespace Poco {


class ASN1Factory;


class Foundation_API ASN1Type
	/// This class encapsulates an ASN.1 type.
{
public:
	enum ClassType {
		Universal = 0x0,
		Application = 0x1,
		ContextSpecific = 0x2,
		Private = 0x3,
	};

	enum UniversalType {
		None = 0x00,
		Boolean = 0x01,
		Integer = 0x02,
		OctetString = 0x04,
		Null = 0x05,
		ObjectIdentifier = 0x06,
		Sequence = 0x10,
	};

	ASN1Type();
		/// Constructs the ASN1 type.

	ASN1Type(Poco::UInt8 aclass, bool primitive, Poco::UInt32 tag);
		/// Constructs the ASN1 type by headers.

	explicit ASN1Type(UniversalType utype, bool universalMarker);
		/// Constructs the ASN1 type by universal type.
		///
		/// UniversalMaker is not used, it is used for C++ disambiguation of the "simpleclass" constructor.

	ASN1Type(Poco::UInt8 simpleclass);
		/// Constructs the ASN1 type using a simple class (1 byte only).

	Poco::UInt8 aclass() const;
		/// Returns the ASN1 type class header.

	bool primitive() const;
		/// Returns the ASN1 type class primitive header.

	Poco::UInt32 tag() const;
		/// Returns the ASN1 type tag.

	Poco::UInt8 rawValue() const;
		/// Returns the raw header value (only the first byte).

	Poco::UInt8 universalValue() const;
		/// Returns the universal value (tag header excluding all other headers).

	Poco::UInt32 getDataLength() const;
		/// Returns the length in bytes of the type.

	void encodeData(Poco::BinaryWriter &stream) const;
		/// Encodes the type to the stream.

	Poco::UInt8 decodeData(Poco::BinaryReader &stream);
		/// Decodes the type from the stream.
		///
		/// Returns the number of bytes read.

private:
	Poco::UInt8 _aclass;
	bool _primitive;
	Poco::UInt32 _tag;
};


class Foundation_API ASN1 : public Poco::RefCountedObject
	/// This is the base class for ASN.1 types.
{
public:
	typedef Poco::AutoPtr<ASN1> Ptr;
		/// Helper smart pointer

	ASN1(ASN1Type type);
		/// Creates an ASN1 of the passed type.

	virtual ~ASN1();
		/// Destroys the ASN1 type.
	
	const ASN1Type &getType() const;
		/// Returns the ASN1 type passed on the constructor.

	void encode(Poco::BinaryWriter &stream) const;
		/// Encode the data to the stream.

	Poco::UInt32 decode(Poco::SharedPtr<ASN1Factory> factory, Poco::BinaryReader &stream);
		/// Decode the data from the stream.
		///
		///	Returns the length of the data, including the length header itself.

	Poco::UInt32 getLength() const;
		/// Returns the total length of the data, including type and length headers.

	virtual std::string toString() const;
		/// Returns an user-friendly string representation of the data.

	virtual std::string typeName() const;
		/// Returns a string representation of the type name.

	virtual void dump(std::ostream &stream, int ident = 0) const;
		/// Dumps the data in idented format.

	static std::pair<Poco::UInt32, Poco::UInt32> parseBase128Int(Poco::BinaryReader &stream);
		/// Parses a 128-bit integer from the stream.
		/// The first item of the return value is the value, the second the encoded length in bytes.

	static void writeBase128Int(Poco::UInt32 value, Poco::BinaryWriter &stream);
		/// Writes the value as a 128-bit encoded integer.

	static Poco::UInt32 sizeBase128Int(Poco::UInt32 value);
		/// Calculates the size in bytes that a 128-bit encoded integer would use.

protected:
	virtual Poco::UInt32 getDataLength() const = 0;
		/// Returns the length of the data, excluding type and length headers.

	virtual void encodeData(Poco::BinaryWriter &stream) const = 0;
		/// Encodes the data to the stream.

	virtual void decodeData(Poco::SharedPtr<ASN1Factory> factory, Poco::BinaryReader &stream, Poco::UInt32 length) = 0;
		/// Decodes the data from the stream, using the passed length.

private:
	ASN1Type _type;
};


class Foundation_API ASN1Factory
	/// This class is a factory for ASN1 objects.
{
public:
	typedef Poco::SharedPtr<ASN1Factory> Ptr;

	virtual ~ASN1Factory() {}
		/// Destructor

	virtual ASN1::Ptr create(ASN1Type type) = 0;
		/// Creates and ASN1 object using the passed type.

};


} // namespace Poco


#endif // Foundation_ASN1_INCLUDED
