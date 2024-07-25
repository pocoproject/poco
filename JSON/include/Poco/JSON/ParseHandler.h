//
// ParseHandler.h
//
// Library: JSON
// Package: JSON
// Module:  ParseHandler
//
// Definition of the ParseHandler class.
//
// Copyright (c) 2012, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef JSON_ParseHandler_INCLUDED
#define JSON_ParseHandler_INCLUDED


#include "Poco/JSON/Handler.h"
#include <stack>


namespace Poco {
namespace JSON {


class JSON_API ParseHandler: public Handler
	/// ParseHandler is the default handler for the JSON Parser.
	///
	/// This handler will construct an Object or Array based
	/// on the handlers called by the Parser.
{
public:
	ParseHandler(bool preserveObjectOrder = false);
		/// Creates the ParseHandler.
		///
		/// If preserveObjectOrder is true, the order of properties
		/// inside objects is preserved. Otherwise, items
		/// will be sorted by keys.

	~ParseHandler() override;
		/// Destroys the ParseHandler.

	void reset() override;
		/// Resets the handler state.

	void startObject() override;
		/// Handles a '{'; a new object is started.

	void endObject() override;
		/// Handles a '}'; the object is closed.

	void startArray() override;
		/// Handles a '['; a new array is started.

	void endArray() override;
		/// Handles a ']'; the array is closed.

	void key(const std::string& k) override;
		/// A key is read

	Dynamic::Var asVar() const override;
		/// Returns the result of the parser (an object or an array).

	void value(int v) override;
		/// An integer value is read

	void value(unsigned v) override;
		/// An unsigned value is read. This will only be triggered if the
		/// value cannot fit into a signed int.

#if defined(POCO_HAVE_INT64)
	void value(Int64 v) override;
		/// A 64-bit integer value is read

	void value(UInt64 v) override;
		/// An unsigned 64-bit integer value is read. This will only be
		/// triggered if the value cannot fit into a signed 64-bit integer.
#endif

	void value(const std::string& s) override;
		/// A string value is read.

	void value(double d) override;
		/// A double value is read.

	void value(bool b) override;
		/// A boolean value is read.

	void null() override;
		/// A null value is read.

private:
	void setValue(const Poco::Dynamic::Var& value);
	using Stack = std::stack<Dynamic::Var>;

	Stack        _stack;
	std::string  _key;
	Dynamic::Var _result;
	bool         _preserveObjectOrder;
};


//
// inlines
//
inline Dynamic::Var ParseHandler::asVar() const
{
	return _result;
}


inline void ParseHandler::value(int v)
{
	setValue(v);
}


inline void ParseHandler::value(unsigned v)
{
	setValue(v);
}


#if defined(POCO_HAVE_INT64)
inline void ParseHandler::value(Int64 v)
{
	setValue(v);
}


inline void ParseHandler::value(UInt64 v)
{
	setValue(v);
}
#endif


inline void ParseHandler::value(const std::string& s)
{
	setValue(s);
}


inline void ParseHandler::value(double d)
{
	setValue(d);
}


inline void ParseHandler::value(bool b)
{
	setValue(b);
}


inline void ParseHandler::null()
{
	Poco::Dynamic::Var empty;
	setValue(empty);
}


} } // namespace Poco::JSON


#endif // JSON_ParseHandler_INCLUDED
