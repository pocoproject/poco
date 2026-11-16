//
// MessageHeader.h
//
// Library: Net
// Package: Messages
// Module:  MessageHeader
//
// Definition of the MessageHeader class.
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Net_MessageHeader_INCLUDED
#define Net_MessageHeader_INCLUDED


#include "Poco/Net/Net.h"
#include "Poco/Net/NameValueCollection.h"
#include <ostream>
#include <istream>
#include <map>
#include <utility>
#include <vector>


namespace Poco::Net {


class Net_API MessageHeader: public NameValueCollection
	/// A collection of name-value pairs that are used in
	/// various internet protocols like HTTP and SMTP.
	///
	/// The name is case-insensitive.
	///
	/// There can be more than one name-value pair with the
	/// same name.
	///
	/// MessageHeader supports writing and reading the
	/// header data in RFC 2822 format.
	///
	/// The maximum number of fields can be restricted
	/// by calling setFieldLimit(). This is useful to
	/// defend against certain kinds of denial-of-service
	/// attacks. The limit is only enforced when parsing
	/// header fields from a stream, not when programmatically
	/// adding them. The default limit is 100.
	///
	/// The forms in which read() received the fields follow the
	/// fields only through the functions of MessageHeader, not
	/// through those of NameValueCollection.
{
public:
	MessageHeader();
		/// Creates the MessageHeader.

	MessageHeader(const MessageHeader& messageHeader);
		/// Creates the MessageHeader by copying
		/// another one.

	MessageHeader(MessageHeader&& messageHeader) noexcept;
		/// Creates the MessageHeader by moving
		/// another one.

	virtual ~MessageHeader();
		/// Destroys the MessageHeader.

	MessageHeader& operator = (const MessageHeader& messageHeader);
		/// Assigns the content of another MessageHeader.

	MessageHeader& operator = (MessageHeader&& messageHeader) noexcept;
		/// Moves the content of another MessageHeader.

	using NameValueCollection::erase;

	void erase(const std::string& name);
		/// Removes all fields with the given name and the
		/// forms in which read() received them.

	void clear();
		/// Removes all fields and the forms in which read()
		/// received them.

	void secureErase(const std::string& name);
		/// Overwrites and removes all fields with the given name and
		/// the forms in which read() received them.

	void secureErase(Iterator it);
		/// Overwrites and removes the field referenced by the given
		/// iterator and the form in which read() received it.

	void secureClear();
		/// Overwrites and removes all fields and the forms in which
		/// read() received them.

	using NameValueCollection::swap;

	void swap(MessageHeader& messageHeader) noexcept;
		/// Exchanges the content of two MessageHeader objects.

	virtual void write(std::ostream& ostr) const;
		/// Writes the message header to the given output stream.
		///
		/// The format is one name-value pair per line, with
		/// name and value separated by a colon and lines
		/// delimited by a carriage return and a linefeed
		/// character. See RFC 2822 for details.
		///
		/// A field that read() decoded is written as it was received,
		/// unless its value has been changed since.
		///
		/// Throws a MessageException, before anything is written,
		/// if a field name is empty or contains a character that is not
		/// printable ASCII, a space or a colon (RFC 5322), or a field
		/// value contains a control character other than a horizontal tab.
		/// A carriage return and a linefeed followed by spaces or horizontal
		/// tabs and then other text are allowed and fold the value.

	virtual void read(std::istream& istr);
		/// Reads the message header from the given input stream.
		///
		/// See write() for the expected format.
		/// Also supported is folding of field content, according
		/// to section 2.2.3 of RFC 2822.
		///
		/// Reading stops at the first empty line (a line only
		/// containing \r\n or \n), as well as at the end of
		/// the stream.
		///
		/// Some basic sanity checking of the input stream is
		/// performed. Lines without a colon are ignored, but
		/// count towards the field limit.
		///
		/// Throws a MessageException if the input stream is
		/// malformed.

	void setAutoDecode(bool convert);
		/// Enables or disables automatic conversion of HTTP header values
		/// when reading HTTP header.
		///
		/// RFC 2047 encoded words are decoded where RFC 2047, section 5,
		/// allows them: in the whole value of an unstructured field, such as
		/// Subject or an X- field, and in the display names and comments of
		/// an address field, such as From or To. A decoded display name that
		/// contains specials is stored as a quoted-string, so that the value
		/// keeps its mailboxes. Other structured fields, such as Content-Type,
		/// Date or Message-ID, are not decoded.
		///
		/// A value whose decoded form contains a control character other
		/// than a horizontal tab is stored undecoded.

	[[nodiscard]] bool getAutoDecode() const;
		/// Returns true if automatic conversion of HTTP header values
		/// when reading HTTP header.

	void setStrictParsing(bool strict);
		/// Enables or disables strict parsing in read().
		///
		/// In strict mode, read() throws a MessageException if a field
		/// name is not a token as defined in RFC 9110, and removes only
		/// spaces and horizontal tabs before and after a field value.
		///
		/// Strict parsing is disabled by default. The HTTP server
		/// enables it for requests.

	[[nodiscard]] bool getStrictParsing() const;
		/// Returns true if strict parsing is enabled.

	[[nodiscard]] std::string getDecoded(const std::string& name) const;
		/// Get decoded header value. It does conversion if it was not
		/// automatically converted when reading, in the fields and parts
		/// of fields that automatic conversion decodes (see setAutoDecode()).
		///
		/// A value that automatic conversion left undecoded
		/// (see setAutoDecode()) is returned unchanged.

	[[nodiscard]] std::string getDecoded(const std::string& name, const std::string& defaultValue) const;
		/// Get decoded header value. It does conversion if it was not
		/// automatically converted when reading.
		/// Default value is returned if the name does not exist.
		///
		/// A value that automatic conversion left undecoded
		/// (see setAutoDecode()) is returned unchanged.

	[[nodiscard]] int getFieldLimit() const;
		/// Returns the maximum number of header fields
		/// allowed.
		///
		/// See setFieldLimit() for more information.

	void setFieldLimit(int limit);
		/// Sets the maximum number of header fields
		/// allowed. This limit is used to defend certain
		/// kinds of denial-of-service attacks.
		/// Specify 0 for unlimited (not recommended).
		///
		/// The default limit is 100.

	[[nodiscard]] int getNameLengthLimit() const;
		/// Returns the maximum length of a field name.
		///
		/// See setNameLengthLimit() for more information.

	void setNameLengthLimit(int limit);
		/// Sets the maximum length of a field name.
		///
		/// The default limit is 256.

	[[nodiscard]] int getValueLengthLimit() const;
		/// Returns the maximum length of a field value.
		///
		/// See setValueLengthLimit() for more information.

	void setValueLengthLimit(int limit);
		/// Sets the maximum length of a field value.
		///
		/// The default limit is 8192.

	[[nodiscard]] bool hasToken(const std::string& fieldName, const std::string& token) const;
		/// Returns true if the field with the given fieldName contains
		/// the given token. Tokens in a header field are expected to be
		/// comma-separated and are case insensitive.

	static void splitElements(const std::string& s, std::vector<std::string>& elements, bool ignoreEmpty = true);
		/// Splits the given string into separate elements. Elements are expected
		/// to be separated by commas.
		///
		/// For example, the string
		///   text/plain; q=0.5, text/html, text/x-dvi; q=0.8
		/// is split into the elements
		///   text/plain; q=0.5
		///   text/html
		///   text/x-dvi; q=0.8
		///
		/// Commas enclosed in double quotes do not split elements.
		///
		/// If ignoreEmpty is true, empty elements are not returned.

	static void splitParameters(const std::string& s, std::string& value, NameValueCollection& parameters);
		/// Splits the given string into a value and a collection of parameters.
		/// Parameters are expected to be separated by semicolons.
		///
		/// Enclosing quotes of parameter values are removed.
		///
		/// For example, the string
		///   multipart/mixed; boundary="MIME_boundary_01234567"
		/// is split into the value
		///   multipart/mixed
		/// and the parameter
		///   boundary -> MIME_boundary_01234567
		///
		/// RFC 2047 encoded words in the value of a name or filename
		/// parameter are decoded to UTF-8, unless the decoded value contains
		/// a control character other than a horizontal tab. RFC 2047,
		/// section 5, does not allow encoded words there, but many mail
		/// clients write file names so. Other parameters are not changed.

	static void splitParameters(const std::string::const_iterator& begin, const std::string::const_iterator& end, NameValueCollection& parameters);
		/// Splits the given string into a collection of parameters.
		/// Parameters are expected to be separated by semicolons.
		///
		/// Enclosing quotes of parameter values are removed. Parameter
		/// values are not decoded, so that opaque values such as cookies
		/// stay as they are.

	static void quote(const std::string& value, std::string& result, bool allowSpace = false);
		/// Checks if the value must be quoted. If so, the value is
		/// appended to result, enclosed in double-quotes.
		/// Otherwise, the value is appended to result as-is.

	static void decodeRFC2047(const std::string& ins, std::string& outs, const std::string& toCharset = "UTF-8");
		/// Decodes a single RFC 2047 encoded word, with or without the enclosing
		/// "=?" and "?=", and converts it to toCharset. If ins is not a
		/// well-formed encoded word with the encoding B or Q, or does not decode,
		/// outs receives ins unchanged.

	[[nodiscard]] static std::string decodeWord(const std::string& text, const std::string& charset = "UTF-8");
		/// Decodes the RFC 2047 encoded words in text and converts them to the
		/// given charset. Text that is not a well-formed encoded word, or does
		/// not decode, is copied unchanged. White space is removed only between
		/// two decoded words.

protected:
	[[nodiscard]] static bool isValidFieldName(const std::string& name);
		/// Returns true if name is a token as defined in RFC 9110.

	[[nodiscard]] static bool isInvalidValueChar(char c);
		/// Returns true if c is a control character other than
		/// a horizontal tab.

	static void checkFields(const NameValueCollection& fields, bool allowFolding);
		/// Throws a MessageException if a field cannot be written.
		/// If allowFolding is true, the rules of write() apply; otherwise
		/// those of HTTPMessage::write().

	static void checkFieldValue(const std::string& name, const std::string& value, bool allowFolding);
		/// Throws a MessageException if value cannot be written as the
		/// value of the field with the given name; see checkFields().

	static void writeFields(const NameValueCollection& fields, std::ostream& ostr);
		/// Writes the fields without checking them; see checkFields().

	[[nodiscard]] bool isReceived(const std::string& name, const std::string& value) const;
		/// Returns true if read() stored the field with the given name and value
		/// and the value was decoded or contains non-ASCII characters, so that
		/// write() writes the field as it was received.

private:
	void eraseReceived(const std::string& name, bool secure);

	enum Limits
		/// Limits for basic sanity checks when reading a header
	{
		DFL_NAME_LENGTH_LIMIT  = 256,
		DFL_VALUE_LENGTH_LIMIT = 8192,
		DFL_FIELD_LIMIT = 100
	};

	int _fieldLimit;
	int _nameLengthLimit;
	int _valueLengthLimit;
	bool _autoDecode;
	bool _decodedOnRead;
	bool _strictParsing;
	std::map<std::pair<std::string, std::string>, std::string> _receivedFields;
		/// The received values, by lowercase field name and stored value.

};


//
// inlines
//
inline void swap(MessageHeader& messageHeader1, MessageHeader& messageHeader2) noexcept
{
	messageHeader1.swap(messageHeader2);
}


} // namespace Poco::Net


#endif // Net_MessageHeader_INCLUDED
