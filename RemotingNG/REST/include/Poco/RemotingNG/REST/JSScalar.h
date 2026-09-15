//
// JSScalar.h
//
// Library: RemotingNG/REST
// Package: JSValue
// Module:  JSScalar
//
// Definition of the JSScalar class.
//
// Copyright (c) 2010-2016, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef RemotingNG_REST_JSScalar_INCLUDED
#define RemotingNG_REST_JSScalar_INCLUDED


#include "Poco/RemotingNG/REST/JSValue.h"


namespace Poco {
namespace RemotingNG {
namespace REST {


class RemotingNGREST_API JSScalar: public JSValue
	/// This class represents a scalar JSON value.
	///
	/// Internal storage is always as std::string.
{
public:
	JSScalar(Type type = JS_STRING);
		/// Creates an empty JSScalar with the given type.
		
	JSScalar(const std::string& value, Type type = JS_STRING);
		/// Creates a JSScalar with the given value and type.

	// JSValue
	Type type() const;
	
	const std::string& stringValue() const;

protected:
	~JSScalar();
		/// Destroys the JSScalar.
	
private:
	std::string _value;
	Type _type;
};


} } } // namespace Poco::RemotingNG::REST


#endif // RemotingNG_REST_JSScalar_INCLUDED
