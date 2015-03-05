//
// ASN1FactoryDefault.h
//
// $Id: //poco/1.4/ASN/include/Poco/ASN/ASN1FactoryDefault.h#1 $
//
// Library: ASN
// Package: ASN
// Module:  ASN1FactoryDefault
//
// Definition of the ASN1FactoryDefault class.
//
// Copyright (c) 2010, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef ASN_ASN1FactoryDefault_INCLUDED
#define ASN_ASN1FactoryDefault_INCLUDED


#include "Poco/ASN/ASN.h"
#include "Poco/ASN/ASN1.h"


namespace Poco {
namespace ASN {


class ASN_API ASN1FactoryDefault : public ASN1Factory
	/// This class is a factory for default ASN1 types.
{
public:

	ASN1::Ptr create(ASN1Type type);
		/// Creates the ASN1 object using the passed type.

};


} } // namespace Poco::ASN


#endif // ASN_ASN1FactoryDefault_INCLUDED
