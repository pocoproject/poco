//
// HeaderField.h
//
// Library: Net
// Package: Messages
// Module:  HeaderField
//
// Definition of the HeaderField class.
//
// Copyright (c) 2026, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Net_HeaderField_INCLUDED
#define Net_HeaderField_INCLUDED


#include "Poco/Net/Net.h"
#include "Poco/Net/MailRecipient.h"
#include <string>
#include <string_view>
#include <vector>


namespace Poco::Net {


class HeaderField
	/// The syntax of RFC 5322 and MIME header fields as far as RFC 2047
	/// encoded words depend on it, for MessageHeader and MailMessage.
	///
	/// RFC 2047, section 5, allows encoded words only in unstructured text
	/// and in the phrases and comments of structured fields.
{
public:
	enum class Syntax
	{
		UNSTRUCTURED, /// text; encoded words may appear anywhere
		ADDRESS_LIST, /// encoded words only in display names and comments
		STRUCTURED    /// no encoded words
	};

	struct EncodedWord
		/// The parts of an RFC 2047 encoded word.
	{
		std::string_view charset;
		char encoding = 0; /// 'B' or 'Q'
		std::string_view text;
	};

	HeaderField() = delete;

	[[nodiscard]] static Syntax syntax(const std::string& name);
		/// Returns the syntax of the field with the given name. A field
		/// that is not known as structured, such as an X- field, is
		/// unstructured.

	static void parseAddressList(std::string_view list, MailRecipient::RecipientType type, std::vector<MailRecipient>& recipients);
		/// Appends a recipient of the given type for every mailbox of an RFC 5322
		/// address list, including the members of groups. A mailbox that cannot be
		/// parsed is skipped.

	[[nodiscard]] static std::string decodeAddressList(std::string_view list, const std::string& charset);
		/// Returns the address list with the encoded words in its display names and
		/// comments decoded and converted to charset. A decoded display name that
		/// contains specials is written as a quoted-string, and a decoded '(', ')' or
		/// '\' in a comment as a quoted-pair, so that the result has the mailboxes of
		/// the given list.

	[[nodiscard]] static bool parseEncodedWord(std::string_view word, EncodedWord& parts);
		/// Splits word, an RFC 2047 encoded word without the enclosing "=?" and "?=",
		/// into its parts. Returns false if word is not a well-formed encoded word
		/// with the encoding B or Q.

	static void decodeParameter(std::string& value);
		/// Decodes the RFC 2047 encoded words in the value of a MIME parameter, such as
		/// a file name. RFC 2047, section 5, does not allow them there, but many mail
		/// clients write them. A value whose decoded form contains a control character
		/// other than a horizontal tab is left unchanged.

	[[nodiscard]] static bool isInvalidValueChar(char c);
		/// Returns true if c is a control character other than
		/// a horizontal tab.

	[[nodiscard]] static std::string encodeAddressList(std::string_view list);
		/// Returns the address list with every display name of a mailbox or a group
		/// that contains non-ASCII characters replaced by RFC 2047 encoded words of
		/// its text, which must be UTF-8 (see MailMessage::encodeWord()).
		///
		/// Throws a MessageException if a mailbox that cannot be parsed contains
		/// non-ASCII characters.
};


} // namespace Poco::Net


#endif // Net_HeaderField_INCLUDED
