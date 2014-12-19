//
// ASN1.cpp
//
// $Id: //poco/1.4/Foundation/src/ASN1.cpp#1 $
//
// Library: Foundation
// Package: Streams
// Module:  ASN1
//
// Copyright (c) 2010, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/ASN1.h"
#include "Poco/Format.h"
#include "Poco/NumberFormatter.h"


#define HIGH_BIT 0x80


namespace Poco {


///
/// ASN1Type
///


ASN1Type::ASN1Type() : _aclass(0), _primitive(true), _tag(0)
{

}


ASN1Type::ASN1Type(Poco::UInt8 aclass, bool primitive, Poco::UInt32 tag) : 
	_aclass(aclass), _primitive(primitive), _tag(tag)
{

}


ASN1Type::ASN1Type(UniversalType utype, bool universalMarker) : _aclass(0), _primitive(true), _tag(utype)
{
	if (utype == Sequence)
		_primitive = false;
}


ASN1Type::ASN1Type(Poco::UInt8 simpleclass)
{
	_aclass = (simpleclass&0xC0) >> 6; // first 2 bytes
	_primitive = ((simpleclass&0x20) >> 5) != 1; // 3rd byte
	_tag = (simpleclass&0x1F); // 5th to 8th byte

	if (_tag == 0x1F)
		throw Poco::DataException("Type is not a simple class");
}


Poco::UInt8 ASN1Type::aclass() const
{
	return _aclass;
}


bool ASN1Type::primitive() const
{
	return _primitive;
}


Poco::UInt32 ASN1Type::tag() const
{
	return _tag;
}


Poco::UInt8 ASN1Type::rawValue() const
{
	Poco::UInt8 t = 0;
	t |= _aclass << 6;
	t |= (_primitive?0:1) << 5;
	t |= _tag;

	if (_tag < 0x1f)
		t |= _tag;
	else
		t |= 0x1f;

	return t;
}


Poco::UInt8 ASN1Type::universalValue() const
{
	if (_aclass == Universal)
		return _tag;
	return 0xff;
}


Poco::UInt32 ASN1Type::getDataLength() const
{
	Poco::UInt32 len = 1;
	if (_tag >= 0x1f)
		len += ASN1::sizeBase128Int(_tag);
	return len;
}


void ASN1Type::encodeData(Poco::BinaryWriter &stream) const
{
	Poco::UInt8 t = rawValue();
	stream << t;

	if (_tag >= 0x1f)
		ASN1::writeBase128Int(_tag, stream);
}


Poco::UInt8 ASN1Type::decodeData(Poco::BinaryReader &stream)
{
	Poco::UInt8 length = 1;
	Poco::UInt8 type;
	stream >> type;

	Poco::UInt8 aclass = (type&0xC0) >> 6; // first 2 bytes
	Poco::UInt8 composite = (type&0x20) >> 5; // 3rd byte
	Poco::UInt32 tag = (type&0x1F); // 5th to 8th byte

	if (tag == 0x1F)
	{
		std::pair<Poco::UInt32, Poco::UInt32> pi = ASN1::parseBase128Int(stream);
		tag = pi.first;
		length += pi.second;
	}

	_aclass = aclass;
	_primitive = composite != 1;
	_tag = tag;

	return length;
}


///
/// ASN1
///


ASN1::ASN1(ASN1Type type) : _type(type)
{

}


ASN1::~ASN1()
{

}

	
const ASN1Type &ASN1::getType() const
{
	return _type;
}


void ASN1::encode(Poco::BinaryWriter &stream) const
{
	Poco::UInt32 dataLength = getDataLength();

	_type.encodeData(stream);

	// encode length
	if (dataLength > 127)
	{
		Poco::UInt8 numBytes = 1;
		Poco::UInt32 ll = dataLength;
		while (ll > 255)
		{
			numBytes++;
			ll >>= 8;
		}
		stream << (Poco::UInt8)(0x80 | numBytes);

		for (ll=numBytes; ll>0; ll--)
		{
			stream << (Poco::UInt8)(dataLength >> Poco::UInt32((ll-1)*8));
		}
	}
	else
		stream << (Poco::UInt8)dataLength;

	encodeData(stream);
}


Poco::UInt32 ASN1::decode(Poco::SharedPtr<ASN1Factory> factory, Poco::BinaryReader &stream)
{
	Poco::UInt32 length = 0;
	Poco::UInt8 dataLength = 0;
	Poco::UInt8 lengthSize = 1;
	stream >> dataLength;

	if ((dataLength & 0x80) == 0)
	{
		length = dataLength;
	}
	else
	{
		// Bottom 7 bits give the number of length bytes to follow.
		int numBytes = dataLength & 0x7f;
		if (numBytes == 0)
			throw DataFormatException("indefinite length found (not DER)");

		lengthSize += numBytes;

		for (int i=0; i < numBytes; i++)
		{
			stream >> dataLength;
			
			if (length >= 1<<23)
				throw DataFormatException("length too large");

			length <<= 8;
			length |= int(dataLength);
		}
	}

	decodeData(factory, stream, length);

	return lengthSize + length;
}


Poco::UInt32 ASN1::getLength() const
{
	Poco::UInt32 dataLength = getDataLength();
	Poco::UInt8 lengthSize = 1;

	if (dataLength > 127)
	{
		lengthSize++;
		while (dataLength > 255)
		{
			lengthSize++;
			dataLength >>= 8;
		}
	}

	return _type.getDataLength() + lengthSize + getDataLength();
}


std::string ASN1::toString() const
{
	return Poco::format("[item type 0x%s]", Poco::NumberFormatter::formatHex(_type.rawValue()));
}


std::string ASN1::typeName() const
{
	return Poco::format("0x%s", Poco::NumberFormatter::formatHex(_type.rawValue()));
}


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


void ASN1::dump(std::ostream &stream, int ident) const
{
	stream << Whitespace(ident) << "ASN.1 0x" << Poco::NumberFormatter::formatHex(_type.rawValue()) << " " << getDataLength() << " BYTES" << std::endl;
}


std::pair<Poco::UInt32, Poco::UInt32> ASN1::parseBase128Int(Poco::BinaryReader &stream)
{
	Poco::UInt8 b;
	std::pair<Poco::UInt32, Poco::UInt32> ret(0, 0);
	do
	{
		stream >> b;
		ret.second++;
		ret.first <<= 7;
		ret.first |= int(b & 0x7F);
	}
	while (!stream.eof() && (b&HIGH_BIT) != 0);

	return ret;
}


void ASN1::writeBase128Int(Poco::UInt32 value, Poco::BinaryWriter &stream)
{
	Poco::UInt32 l = 0;
	for (Poco::UInt32 i = value; i > 0; i >>= 7)
		l++;

	for (Poco::UInt32 i = l-1; i>=0; i--)
	{
		Poco::UInt8 o = Poco::UInt8(value >> Poco::UInt32(i*7));
		o &= 0x7f;
		if (i != 0)
			o |= 0x80;

		stream << o;
	}
}


Poco::UInt32 ASN1::sizeBase128Int(Poco::UInt32 value)
{
	Poco::UInt32 l = 0;
	for (Poco::UInt32 i = value; i > 0; i >>= 7)
		l++;

	return l;
}


} // namespace Poco
