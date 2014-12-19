//
// ASN1Codec.cpp
//
// $Id: //poco/1.4/Foundation/src/ASN1Codec.cpp#1 $
//
// Library: Foundation
// Package: Streams
// Module:  ASN1Codec
//
// Copyright (c) 2010, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/ASN1Codec.h"
#include "Poco/ASN1FactoryDefault.h"
#include "Poco/BinaryReader.h"
#include "Poco/BinaryWriter.h"


namespace Poco {


ASN1Codec::ASN1Codec() : _factory(new ASN1FactoryDefault)
{

}


ASN1Codec::ASN1Codec(Poco::SharedPtr<ASN1Factory> factory) : _factory(factory)
{
	if (factory.isNull())
		throw InvalidArgumentException("Factory is required");
}


void ASN1Codec::setFactory(Poco::SharedPtr<ASN1Factory> factory)
{
	if (factory.isNull())
		throw InvalidArgumentException("Factory is required");

	_factory = factory;
}


void ASN1Codec::encode(ASN1::Ptr data, std::ostream &stream)
{
	Poco::BinaryWriter swrite(stream);
	data->encode(swrite);
}


ASN1::Ptr ASN1Codec::decode(std::istream &stream)
{
	ASN1::Ptr ret;
	Poco::BinaryReader sread(stream);

	ASN1Type tp;
	tp.decodeData(sread);

	ret = _factory->create(tp);
	ret->decode(_factory, sread);

	return ret;
}


} // namespace Poco
