//
// JSValue.h
//
// Library: RemotingNG/JSONRPC
// Package: JSValue
// Module:  JSValue
//
// Definition of the JSValue class.
//
// Copyright (c) 2010-2013, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties,
// copied or duplicated in any form, in whole or in part.
//


#ifndef RemotingNG_JSONRPC_JSValue_INCLUDED
#define RemotingNG_JSONRPC_JSValue_INCLUDED


#include "Poco/RemotingNG/JSONRPC/JSONRPC.h"
#include "Poco/RefCountedObject.h"
#include "Poco/NumberParser.h"
#include "Poco/AutoPtr.h"
#include <cstddef>


namespace Poco {
namespace RemotingNG {
namespace JSONRPC {


class RemotingNGJSONRPC_API JSValue: public Poco::RefCountedObject
	/// This class represents a generic JavaScript object.
	///
	/// Subclasses implement scalar values, arrays and objects.
{
public:
	using Ptr = Poco::AutoPtr<JSValue>;

	enum Type
	{
		JS_OBJECT,
		JS_ARRAY,
		JS_STRING,
		JS_NUMBER,
		JS_BOOL
	};

	virtual Type type() const = 0;
		/// Returns the type of the object.

	virtual std::size_t size() const;
		/// Returns the size of the object.

	virtual const std::string& stringValue() const;
		/// Returns the object's value as string.

	Poco::Int32 asInt32() const;
		/// Returns the object's value as Int32.

	Poco::Int64 asInt64() const;
		/// Returns the object's value as Int64.

	Poco::UInt32 asUInt32() const;
		/// Returns the object's value as UInt32.

	Poco::UInt64 asUInt64() const;
		/// Returns the object's value as UInt64.

	double asDouble() const;
		/// Returns the object's value as double.

	bool asBool() const;
		/// Returns the object's value as boolean.

	virtual const Ptr& operator [] (const std::string& name) const;
	virtual Ptr& operator [] (const std::string& name);

	virtual const Ptr& operator [] (std::size_t index) const;
	virtual Ptr& operator [] (std::size_t index);

	virtual bool has(const std::string& name) const;
};


//
// inlines
//
inline Poco::Int32 JSValue::asInt32() const
{
	return Poco::NumberParser::parse(stringValue());
}


inline Poco::Int64 JSValue::asInt64() const
{
	return Poco::NumberParser::parse64(stringValue());
}


inline Poco::UInt32 JSValue::asUInt32() const
{
	return Poco::NumberParser::parseUnsigned(stringValue());
}


inline Poco::UInt64 JSValue::asUInt64() const
{
	return Poco::NumberParser::parseUnsigned64(stringValue());
}


inline double JSValue::asDouble() const
{
	return Poco::NumberParser::parseFloat(stringValue());
}


inline bool JSValue::asBool() const
{
	const std::string& value = stringValue();
	if (value == "true")
		return true;
	else if (value == "false")
		return false;
	else throw Poco::SyntaxException("not a boolean value");
}


} } } // namespace Poco::RemotingNG::JSONRPC


#endif // RemotingNG_JSONRPC_JSValue_INCLUDED
