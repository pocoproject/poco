//
// ASN1Codec.h
//
// $Id: //poco/1.4/ASN/include/Poco/ASN/ASN1Codec.h#1 $
//
// Library: ASN
// Package: ASN
// Module:  ASN1Codec
//
// Definition of the ASN1Codec class.
//
// Copyright (c) 2010, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef ASN_ASN1Codec_INCLUDED
#define ASN_ASN1Codec_INCLUDED


#include "Poco/ASN/ASN.h"
#include "Poco/ASN/ASN1.h"
#include "Poco/SharedPtr.h"

#include <iostream>

namespace Poco {
namespace ASN {

class ASN_API ASN1Codec
	/// This class encodes and decodes ASN.1 streams.
{
public:

	ASN1Codec();
		/// Constructor that uses the default ASN1 factory.

	ASN1Codec(Poco::SharedPtr<ASN1Factory> factory);
		/// Constructs passing a custom ASN1 factory.

	void setFactory(Poco::SharedPtr<ASN1Factory> factory);
		/// Sets the factory to be used.

	void encode(ASN1::Ptr data, std::ostream &stream);
		/// Encodes the ASN1 data to the stream.

	ASN1::Ptr decode(std::istream &stream);
		/// Decodes the ASN1 data from the stream.

private:
	Poco::SharedPtr<ASN1Factory> _factory;
};


} } // namespace Poco::ASN


#endif // ASN_ASN1Codec_INCLUDED
