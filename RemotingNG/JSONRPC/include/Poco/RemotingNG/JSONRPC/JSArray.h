//
// JSArray.h
//
// Library: RemotingNG/JSONRPC
// Package: JSValue
// Module:  JSArray
//
// Definition of the JSArray class.
//
// Copyright (c) 2010-2013, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties,
// copied or duplicated in any form, in whole or in part.
//


#ifndef RemotingNG_JSONRPC_JSArray_INCLUDED
#define RemotingNG_JSONRPC_JSArray_INCLUDED


#include "Poco/RemotingNG/JSONRPC/JSValue.h"
#include <vector>


namespace Poco {
namespace RemotingNG {
namespace JSONRPC {


class RemotingNGJSONRPC_API JSArray: public JSValue
	/// This class represents a JSON Array.
	///
	/// Internal representation is a std::vector.
	/// Element access is by numeric index (0 .. size - 1).
	///
	/// The JSArray can be grown by accessing the size-th
	/// element.
{
public:
	JSArray();
		/// Creates an empty JSArray.

	// JSValue
	Type type() const;
	std::size_t size() const;
	const Ptr& operator [] (std::size_t index) const;
	Ptr& operator [] (std::size_t index);

protected:
	~JSArray();
		/// Destroys the JSArray.

private:
	using Vector = std::vector<JSValue::Ptr>;

	Vector _vector;
};


} } } // namespace Poco::RemotingNG::JSONRPC


#endif // RemotingNG_JSONRPC_JSArray_INCLUDED
