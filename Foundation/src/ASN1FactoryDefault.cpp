//
// ASN1FactoryDefault.cpp
//
// $Id: //poco/1.4/Foundation/src/ASN1FactoryDefault.cpp#1 $
//
// Library: Foundation
// Package: Streams
// Module:  ASN1FactoryDefault
//
// Copyright (c) 2010, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/ASN1FactoryDefault.h"
#include "Poco/ASN1Types.h"


namespace Poco {


ASN1::Ptr ASN1FactoryDefault::create(ASN1Type type)
{
	if (type.aclass() == ASN1Type::Universal)
	{
		switch (type.tag())
		{
		case ASN1Type::None: return new ASN1Types::Null;
		case ASN1Type::Boolean: return new ASN1Types::Boolean;
		case ASN1Type::Integer: return new ASN1Types::Integer;
		case ASN1Type::OctetString: return new ASN1Types::OctetString;
		case ASN1Type::Null: return new ASN1Types::Null;
		case ASN1Type::ObjectIdentifier: return new ASN1Types::ObjectIdentifier;
		case ASN1Type::Sequence: return new ASN1Types::Sequence;
		};
	}

	return new ASN1Types::Unknown(type);
}


} // namespace Poco
