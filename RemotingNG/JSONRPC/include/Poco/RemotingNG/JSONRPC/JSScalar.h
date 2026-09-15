//
// JSScalar.h
//
// Library: RemotingNG/JSONRPC
// Package: JSValue
// Module:  JSScalar
//
// Definition of the JSScalar class.
//
// Copyright (c) 2010-2013, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef RemotingNG_JSONRPC_JSScalar_INCLUDED
#define RemotingNG_JSONRPC_JSScalar_INCLUDED


#include "Poco/RemotingNG/JSONRPC/JSValue.h"


namespace Poco {
namespace RemotingNG {
namespace JSONRPC {


class RemotingNGJSONRPC_API JSScalar: public JSValue
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


} } } // namespace Poco::RemotingNG::JSONRPC


#endif // RemotingNG_JSONRPC_JSScalar_INCLUDED
