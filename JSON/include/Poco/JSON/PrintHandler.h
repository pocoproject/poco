//
// PrintHandler.h
//
// Library: JSON
// Package: JSON
// Module:  PrintHandler
//
// Definition of the PrintHandler class.
//
// Copyright (c) 2012, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef JSON_PrintHandler_INCLUDED
#define JSON_PrintHandler_INCLUDED


#include "Poco/JSON/JSON.h"
#include "Poco/JSON/Handler.h"
#include "Poco/JSONString.h"


namespace Poco {
namespace JSON {


class JSON_API PrintHandler: public Handler
	/// PrintHandler formats and prints the JSON object
	/// to either user-provided std::ostream or standard output.
	/// If indent is zero, the output is a condensed JSON string,
	/// otherwise, the proper indentation is applied to elements.
{
public:
	using Ptr = SharedPtr<PrintHandler>;

	static const unsigned JSON_PRINT_FLAT = 0;

	PrintHandler(unsigned indent = 0, int options = Poco::JSON_WRAP_STRINGS);
		/// Creates the PrintHandler.

	PrintHandler(std::ostream& out, unsigned indent = 0, int options = Poco::JSON_WRAP_STRINGS);
		/// Creates the PrintHandler.

	~PrintHandler() override;
		/// Destroys the PrintHandler.

	void reset() override;
		/// Resets the handler state.

	void startObject() override;
		/// The parser has read a '{'; a new object is started.
		/// If indent is greater than zero, a newline will be appended.

	void endObject() override;
		/// The parser has read a '}'; the object is closed.

	void startArray() override;
		/// The parser has read a [; a new array will be started.
		/// If indent is greater than zero, a newline will be appended.

	void endArray() override;
		/// The parser has read a ]; the array is closed.

	void key(const std::string& k) override;
		/// A key of an object is read; it will be written to the output,
		/// followed by a ':'. If indent is greater than zero, the colon
		/// is padded by a space before and after.

	void null() override;
		/// A null value is read; "null" will be written to the output.

	void value(int v) override;
		/// An integer value is read.

	void value(unsigned v) override;
		/// An unsigned value is read. This will only be triggered if the
		/// value cannot fit into a signed int.

#if defined(POCO_HAVE_INT64)
	void value(Int64 v) override;
		/// A 64-bit integer value is read; it will be written to the output.

	void value(UInt64 v) override;
		/// An unsigned 64-bit integer value is read; it will be written to the output.
#endif

	void value(const std::string& value) override;
		/// A string value is read; it will be formatted and written to the output.

	void value(double d) override;
		/// A double value is read; it will be written to the output.

	void value(bool b) override;
		/// A boolean value is read; it will be written to the output.

	void comma();
		/// A comma is read; it will be written to the output as "true" or "false".

	void setIndent(unsigned indent);
		/// Sets indentation.

private:
	const char* endLine() const;
	unsigned indent();
	bool printFlat() const;
	void arrayValue();
	bool array() const;

	std::ostream& _out;
	unsigned      _indent;
	std::string   _tab;
	int           _array;
	bool          _objStart;
	int           _options;
};


//
// inlines
//
inline void PrintHandler::setIndent(unsigned indent)
{
	_indent = indent;
}


inline bool PrintHandler::array() const
{
	return _array > 0;
}


} } // namespace Poco::JSON


#endif // JSON_PrintHandler_INCLUDED
