//
// ASN1Codec.h
//
// $Id: //poco/1.4/Foundation/include/Poco/ASN1Codec.h#1 $
//
// Library: Foundation
// Package: Streams
// Module:  ASN1Codec
//
// Definition of the ASN1Codec class.
//
// Copyright (c) 2010, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Foundation_ASN1Codec_INCLUDED
#define Foundation_ASN1Codec_INCLUDED


#include "Poco/Foundation.h"
#include "Poco/ASN1.h"
#include "Poco/SharedPtr.h"

#include <iostream>

namespace Poco {


class Foundation_API ASN1Codec
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


} // namespace Poco


#endif // Foundation_ASN1Codec_INCLUDED
