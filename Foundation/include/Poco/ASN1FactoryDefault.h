//
// ASN1FactoryDefault.h
//
// $Id: //poco/1.4/Foundation/include/Poco/ASN1FactoryDefault.h#1 $
//
// Library: Foundation
// Package: Streams
// Module:  ASN1FactoryDefault
//
// Definition of the ASN1FactoryDefault class.
//
// Copyright (c) 2010, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Foundation_ASN1FactoryDefault_INCLUDED
#define Foundation_ASN1FactoryDefault_INCLUDED


#include "Poco/Foundation.h"
#include "Poco/ASN1.h"


namespace Poco {


class Foundation_API ASN1FactoryDefault : public ASN1Factory
	/// This class is a factory for default ASN1 types.
{
public:

	ASN1::Ptr create(ASN1Type type);
		/// Creates the ASN1 object using the passed type.

};


} // namespace Poco


#endif // Foundation_ASN1FactoryDefault_INCLUDED
