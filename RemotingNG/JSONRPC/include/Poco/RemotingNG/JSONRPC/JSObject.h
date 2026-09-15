//
// JSObject.h
//
// Library: RemotingNG/JSONRPC
// Package: JSValue
// Module:  JSObject
//
// Definition of the JSObject class.
//
// Copyright (c) 2010-2013, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties,
// copied or duplicated in any form, in whole or in part.
//


#ifndef RemotingNG_JSONRPC_JSObject_INCLUDED
#define RemotingNG_JSONRPC_JSObject_INCLUDED


#include "Poco/RemotingNG/JSONRPC/JSValue.h"
#include <map>


namespace Poco {
namespace RemotingNG {
namespace JSONRPC {


class RemotingNGJSONRPC_API JSObject: public JSValue
	/// This class represents a JSON Object.
	///
	/// Internal representation is a std::map
	/// from std::string to JSValue::Ptr.
	/// Element access is by name.
{
public:
	JSObject();
		/// Creates an empty JSObject.

	// JSValue
	Type type() const;
	std::size_t size() const;
	const Ptr& operator [] (const std::string& name) const;
	Ptr& operator [] (const std::string& name);
	bool has(const std::string& name) const;

protected:
	~JSObject();
		/// Destroys the JSObject.

private:
	using Map = std::map<std::string, JSValue::Ptr>;

	Map _map;
};


} } } // namespace Poco::RemotingNG::JSONRPC


#endif // RemotingNG_JSONRPC_JSObject_INCLUDED
