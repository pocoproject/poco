//
// ASN1Types.cpp
//
// $Id: //poco/1.4/Foundation/src/ASN1Types.cpp#1 $
//
// Library: Foundation
// Package: Streams
// Module:  ASN1Types
//
// Copyright (c) 2010, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/ASN1Types.h"
#include "Poco/Format.h"
#include "Poco/StringTokenizer.h"
#include "Poco/NumberParser.h"
#include "Poco/NumberFormatter.h"

#include <sstream>
#include <assert.h>


#define HIGH_BIT 0x80


namespace Poco {
namespace ASN1Types {


struct Whitespace
{
	Whitespace(int n)
		: n(n)
	{
	}
	int n;
};


std::ostream& operator<<(std::ostream& stream, const Whitespace &ws)
{
	for(int i = 0; i < ws.n; i++)
	{
		stream << "   ";
	}
	return stream;
}


///
/// Unknown
///


Unknown::Unknown(ASN1Type type) : ASN1(type)
{

}


Unknown::Unknown(ASN1Type type, const std::string &value) : ASN1(type), _value(value)
{

}


std::string Unknown::toString() const
{
	return Poco::format("[unknown type 0x%s size %?d]", Poco::NumberFormatter::formatHex(getType().rawValue()), _value.size());
}


std::string Unknown::typeName() const
{
	return Poco::format("UNKNOWN-0x%s", Poco::NumberFormatter::formatHex(getType().rawValue()));
}


const std::string &Unknown::getValue() const
{
	return _value;
}


Poco::UInt32 Unknown::getDataLength() const
{
	return _value.size();
}


void Unknown::encodeData(Poco::BinaryWriter &stream) const
{
	stream.writeRaw(_value);
}


void Unknown::decodeData(Poco::SharedPtr<ASN1Factory> factory, Poco::BinaryReader &stream, Poco::UInt32 length)
{
	if (length > 0)
		stream.readRaw(length, _value);
}


void Unknown::dump(std::ostream &stream, int ident) const
{
	stream << Whitespace(ident) << "UNKNOWN TYPE 0x" << Poco::NumberFormatter::formatHex(getType().rawValue()) << " " << _value.size() << " BYTES" << std::endl;
}


///
/// Null
///


Null::Null() : ASN1(ASN1Type(ASN1Type::Null, true))
{

}


std::string Null::toString() const
{
	return "null";
}


std::string Null::typeName() const
{
	return "NULL";
}


Poco::UInt32 Null::getDataLength() const
{
	return 0;
}


void Null::encodeData(Poco::BinaryWriter &stream) const
{

}


void Null::decodeData(Poco::SharedPtr<ASN1Factory> factory, Poco::BinaryReader &stream, Poco::UInt32 length)
{

}


void Null::dump(std::ostream &stream, int ident) const
{
	stream << Whitespace(ident) << "NULL " << std::endl;
}


///
/// Boolean
///


Boolean::Boolean() : ASN1(ASN1Type(ASN1Type::Boolean, true)), _value(false)
{

}


Boolean::Boolean(bool value) : ASN1(ASN1Type(ASN1Type::Boolean, true)), _value(value)
{

}


bool Boolean::getValue() const
{
	return _value;
}


std::string Boolean::toString() const
{
	return _value?"true":"false";
}


std::string Boolean::typeName() const
{
	return "BOOLEAN";
}


Poco::UInt32 Boolean::getDataLength() const
{
	return 1;
}


void Boolean::encodeData(Poco::BinaryWriter &stream) const
{
	if (_value)
		stream << (Poco::UInt8)0xff;
	else
		stream << (Poco::UInt8)0x00;
}


void Boolean::decodeData(Poco::SharedPtr<ASN1Factory> factory, Poco::BinaryReader &stream, Poco::UInt32 length)
{
	Poco::UInt8 byte = 0;
	if (length > 0)
		stream >> byte;
	_value = byte != 0;
}


void Boolean::dump(std::ostream &stream, int ident) const
{
	stream << Whitespace(ident) << "BOOLEAN " << toString() << std::endl;
}


///
/// Integer
///


Integer::Integer() : ASN1(ASN1Type(ASN1Type::Integer, true)), _value(0)
{

}


Integer::Integer(Poco::UInt32 value) : ASN1(ASN1Type(ASN1Type::Integer, true)), _value(value)
{

}


Integer::Integer(ASN1Type type, Poco::UInt32 value) : ASN1(type), _value(value)
{

}


Poco::UInt32 Integer::getValue() const
{
	return _value;
}


std::string Integer::toString() const
{
	return Poco::format("%?d", _value);
}


std::string Integer::typeName() const
{
	std::string seqtype;
	if (getType().universalValue() != ASN1Type::Integer)
		seqtype = Poco::format("-0x%s", Poco::NumberFormatter::formatHex(getType().rawValue()));

	return Poco::format("INTEGER%s", seqtype);
}


Poco::UInt32 Integer::getDataLength() const
{
	int value = _value;
	int valueSize = 0;

	do 
	{
		value >>= 7;
		valueSize++;
	} while (value != 0);

	return valueSize;
}


void Integer::encodeData(Poco::BinaryWriter &stream) const
{
	int integer = _value;
	int mask;
	int intsize = 4;

	/*
	 * Truncate "unnecessary" bytes off of the most significant end of this
	 * 2's complement integer.  There should be no sequence of 9
	 * consecutive 1's or 0's at the most significant end of the
	 * integer.
	 */
	mask = 0x1FF << ((8 * 3) - 1);
	/* mask is 0xFF800000 on a big-endian machine */
	while ((((integer & mask) == 0) || ((integer & mask) == mask))
		&& intsize > 1)
	{
		intsize--;
		integer <<= 8;
	}
	//encodeHeader(os, type, intsize);
	mask = 0xFF << (8 * 3);
	/* mask is 0xFF000000 on a big-endian machine */
	while ((intsize--) > 0)
	{
		stream << (char)( ((integer & mask) >> (8 * 3)) );
		integer <<= 8;
	}
}


void Integer::decodeData(Poco::SharedPtr<ASN1Factory> factory, Poco::BinaryReader &stream, Poco::UInt32 length)
{
	Poco::UInt8 bytesRead = 0;

	Poco::UInt8 byte = 0;
	stream >> byte;
	bytesRead++;

	_value = (byte & HIGH_BIT) == 0 ? 0 : -1;

	for (;;) 
	{
		_value <<= 8;
		_value |= byte;

		if (bytesRead >= length)
			break;

		stream >> byte;
		bytesRead++;
	}
}


void Integer::dump(std::ostream &stream, int ident) const
{
	stream << Whitespace(ident) << "INTEGER " << _value << std::endl;
}


#ifdef POCO_HAVE_INT64

///
/// Integer64
///


Integer64::Integer64() : ASN1(ASN1Type(ASN1Type::Integer, true)), _value(0)
{

}


Integer64::Integer64(Poco::UInt64 value) : ASN1(ASN1Type(ASN1Type::Integer, true)), _value(value)
{

}


Integer64::Integer64(ASN1Type type, Poco::UInt64 value) : ASN1(type), _value(value)
{

}


Poco::UInt64 Integer64::getValue() const
{
	return _value;
}


std::string Integer64::toString() const
{
	return Poco::format("%?d", _value);
}


std::string Integer64::typeName() const
{
	std::string seqtype;
	if (getType().universalValue() != ASN1Type::Integer)
		seqtype = Poco::format("-0x%s", Poco::NumberFormatter::formatHex(getType().rawValue()));

	return Poco::format("INTEGER64%s", seqtype);
}


Poco::UInt32 Integer64::getDataLength() const
{
	Poco::Int64 value = _value;
	int valueSize = 0;

	do 
	{
		value >>= 7;
		valueSize++;
	} while (value != 0);

    return valueSize;
}


void Integer64::encodeData(Poco::BinaryWriter &stream) const
{
	Poco::UInt64 integer = _value;
	int mask;
	int intsize = 4;

	/*
	 * Truncate "unnecessary" bytes off of the most significant end of this
	 * 2's complement integer.  There should be no sequence of 9
	 * consecutive 1's or 0's at the most significant end of the
	 * integer.
	 */
	mask = 0x1FF << ((8 * 3) - 1);
	/* mask is 0xFF800000 on a big-endian machine */
	while ((((integer & mask) == 0) || ((integer & mask) == mask))
		&& intsize > 1)
	{
		intsize--;
		integer <<= 8;
	}
	//encodeHeader(os, type, intsize);
	mask = 0xFF << (8 * 3);
	/* mask is 0xFF000000 on a big-endian machine */
	while ((intsize--) > 0)
	{
		stream << (char)( ((integer & mask) >> (8 * 3)) );
		integer <<= 8;
	}
}


void Integer64::decodeData(Poco::SharedPtr<ASN1Factory> factory, Poco::BinaryReader &stream, Poco::UInt32 length)
{
	Poco::UInt8 bytesRead = 0;

	Poco::UInt8 byte = 0;
	stream >> byte;
	bytesRead++;

	_value = (byte & HIGH_BIT) == 0 ? 0 : -1;

	for (;;) 
	{
		_value <<= 8;
		_value |= byte;

		if (bytesRead >= length)
			break;

		stream >> byte;
		bytesRead++;
	}
}


void Integer64::dump(std::ostream &stream, int ident) const
{
	stream << Whitespace(ident) << "INTEGER64 " << _value << std::endl;
}


#endif // POCO_HAVE_INT64


///
/// OctetString
///


OctetString::OctetString() : ASN1(ASN1Type(ASN1Type::OctetString, true)), _value()
{

}


OctetString::OctetString(const std::string &value) : ASN1(ASN1Type(ASN1Type::OctetString, true)), _value(value)
{

}


OctetString::OctetString(ASN1Type type, const std::string &value) : ASN1(type), _value(value)
{

}


std::string OctetString::getValue() const
{
	return _value;
}


std::string OctetString::toString() const
{
	return _value;
}


std::string OctetString::typeName() const
{
	std::string seqtype;
	if (getType().universalValue() != ASN1Type::OctetString)
		seqtype = Poco::format("-0x%s", Poco::NumberFormatter::formatHex(getType().rawValue()));

	return Poco::format("OCTETSTRING%s", seqtype);
}


Poco::UInt32 OctetString::getDataLength() const
{
	return _value.size();
}


void OctetString::encodeData(Poco::BinaryWriter &stream) const
{
	stream.writeRaw(_value);
}


void OctetString::decodeData(Poco::SharedPtr<ASN1Factory> factory, Poco::BinaryReader &stream, Poco::UInt32 length)
{
	stream.readRaw(length, _value);
}


void OctetString::dump(std::ostream &stream, int ident) const
{
	stream << Whitespace(ident) << "OCTETSTRING '" << _value << "'" << std::endl;
}


///
/// ObjectIdentifier
///


ObjectIdentifier::ObjectIdentifier() : ASN1(ASN1Type(ASN1Type::ObjectIdentifier, true)), _value()
{

}


ObjectIdentifier::ObjectIdentifier(const std::string &value) : ASN1(ASN1Type(ASN1Type::ObjectIdentifier, true)), _value()
{
	Poco::StringTokenizer tok(value, ".", Poco::StringTokenizer::TOK_IGNORE_EMPTY|Poco::StringTokenizer::TOK_TRIM);
	for (Poco::StringTokenizer::Iterator i=tok.begin(); i!=tok.end(); i++)
	{
		_value.push_back(Poco::NumberParser::parseUnsigned(*i));
	}
}


std::string ObjectIdentifier::toString() const
{
	std::stringstream str;
	for (std::vector<Poco::UInt32>::const_iterator i = _value.begin(); i!=_value.end(); i++)
	{
		if (i != _value.begin())
			str << ".";
		str << *i;
	}
	return str.str();
}


std::string ObjectIdentifier::typeName() const
{
	return "OBJECTIDENTIFIER";
}


Poco::UInt32 ObjectIdentifier::getDataLength() const
{
	Poco::UInt32 totalLength = 1;

	for (int i = 2; i < _value.size(); ++i) 
	{
		Poco::UInt32 value = _value.at(i);
		do 
		{
			value >>= 7;
			totalLength++;
		} while (value != 0);
	}

	return totalLength;
}


void ObjectIdentifier::encodeData(Poco::BinaryWriter &stream) const
{
	if (_value.size() < 2)
		throw DataException("Invalid Object Identifier");

	stream << (Poco::UInt8)(_value.at(0) * 40 + _value.at(1));

	for (int i = 2; i < _value.size(); ++i) 
	{
		std::string result;
		std::string tmp;
		Poco::UInt32 value = _value.at(i);

		if (value < 128) 
		{
			result.push_back((char)value);
		} 
		else 
		{
			Poco::UInt32 val = value;

			while (val != 0) 
			{
				Poco::UInt8 bval = (Poco::UInt8)(val & 0xFF);
				if ((bval & HIGH_BIT) != 0) 
				{
					bval = (Poco::UInt8)(bval & ~HIGH_BIT);
				}

				val >>= 7;
				tmp.push_back(bval);
			}

			// Now we need to reverse the bytes for the final encoding
			for (int i = tmp.length() - 1; i >= 0; i--) 
			{
				if (i > 0)
					result.push_back(tmp[i] | HIGH_BIT);
				else
					result.push_back(tmp[i]);
			}
		}

		stream.writeRaw(result);
	}
}


void ObjectIdentifier::decodeData(Poco::SharedPtr<ASN1Factory> factory, Poco::BinaryReader &stream, Poco::UInt32 length)
{
	Poco::UInt8 byte = 0;
	stream >> byte;

	_value.push_back(byte / 40);
	_value.push_back(byte % 40);

	Poco::UInt8 bytesRead = 1;
	while (bytesRead++ < length) 
	{
		Poco::UInt8 buffer = 0;
		Poco::UInt32 value = 0;
		stream >> buffer;

		if ((buffer & HIGH_BIT) == 0) 
		{
			value = buffer;
		} 
		else 
		{
			std::string tmp;
			for (;;) 
			{
				tmp.push_back(buffer & ~HIGH_BIT);
				if ((buffer & HIGH_BIT) == 0)
					break;

				stream >> buffer;
				bytesRead++;
			}

			for (int i = 0; i < tmp.size(); i++) 
			{
				value <<= 7;
				value |= tmp[i];
			}
		}

		_value.push_back(value);
	}
}


void ObjectIdentifier::dump(std::ostream &stream, int ident) const
{
	stream << Whitespace(ident) << "OID '" << toString() << "'" << std::endl;
}


///
/// SequenceData
///


SequenceData::SequenceData()
{

}

SequenceData::SequenceData(ASN1::Ptr v1, ASN1::Ptr v2, ASN1::Ptr v3)
{
	push_back(v1);
	if (!v2.isNull()) push_back(v2);
	if (!v3.isNull()) push_back(v3);
}


///
/// Sequence
///


Sequence::Sequence() : ASN1(ASN1Type(ASN1Type::Sequence, true))
{

}


Sequence::Sequence(ASN1Type type) : ASN1(type)
{

}


Sequence::Sequence(const SequenceData &value) : ASN1(ASN1Type(ASN1Type::Sequence, true))
{
	for (SequenceData::const_iterator i=value.begin(); i!=value.end(); i++)
	{
		_value.push_back(*i);
	}
}


Sequence::Sequence(ASN1Type type, const SequenceData &value) : ASN1(type)
{
	for (SequenceData::const_iterator i=value.begin(); i!=value.end(); i++)
	{
		_value.push_back(*i);
	}
}


const SequenceData &Sequence::getSequenceData() const
{
	return _value;
}


ASN1::Ptr Sequence::getDataType(int pos) const
{
    if (_value.size() < pos + 1)
		throw InvalidAccessException("No such element");
    return _value.at(pos);
}


Poco::UInt32 Sequence::getDataLength() const
{
    Poco::UInt32 totalDataLength = 0;
	for (SequenceData::const_iterator i=_value.begin(); i!=_value.end(); i++)
	{
        totalDataLength += (*i)->getLength();
	}
    return totalDataLength;
}


void Sequence::encodeData(Poco::BinaryWriter &stream) const
{
	for (SequenceData::const_iterator i=_value.begin(); i!=_value.end(); i++)
	{
		(*i)->encode(stream);
	}
}


void Sequence::decodeData(Poco::SharedPtr<ASN1Factory> factory, Poco::BinaryReader &stream, Poco::UInt32 length)
{
	Poco::UInt32 bytesRead = 0;
	while (!stream.eof() && bytesRead < length) 
	{
		ASN1Type tp;
		Poco::UInt8 typelen = tp.decodeData(stream);
		bytesRead += typelen;

		ASN1::Ptr asnType = factory->create(tp);
		Poco::UInt32 curBytesRead = asnType->decode(factory, stream);
		bytesRead += curBytesRead;

		_value.push_back(asnType);
	}
}


void Sequence::dump(std::ostream &stream, int ident) const
{
	std::string seqtype;
	if (getType().universalValue() != ASN1Type::Sequence)
		seqtype = Poco::format(" TYPE 0x%s", Poco::NumberFormatter::formatHex(getType().rawValue()));

	stream << Whitespace(ident) << "SEQUENCE BEGIN" << seqtype << " " << _value.size() << " ITEMS - SIZE " << getDataLength() << std::endl;
	for (SequenceData::const_iterator i=_value.begin(); i!=_value.end(); i++)
	{
		(*i)->dump(stream, ident+1);
	}
	stream << Whitespace(ident) << "SEQUENCE END" << seqtype << " " << _value.size() << " ITEMS" << std::endl;
}


std::string Sequence::typeName() const
{
	std::string seqtype;
	if (getType().universalValue() != ASN1Type::Sequence)
		seqtype = Poco::format("-0x%s", Poco::NumberFormatter::formatHex(getType().rawValue()));

	return Poco::format("SEQUENCE%s", seqtype);
}


} } // namespace Poco::ASN1Types
