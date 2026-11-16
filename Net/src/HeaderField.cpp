//
// HeaderField.cpp
//
// Library: Net
// Package: Messages
// Module:  HeaderField
//
// Copyright (c) 2026, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "HeaderField.h"
#include "Poco/Net/MailMessage.h"
#include "Poco/Net/MessageHeader.h"
#include "Poco/Net/NetException.h"
#include "Poco/Ascii.h"
#include "Poco/String.h"
#include <algorithm>
#include <cstddef>
#include <iterator>
#include <utility>


namespace Poco::Net {


namespace
{
	constexpr const char* ADDRESS_FIELDS[] = {
		"From", "Sender", "Reply-To", "To", "Cc", "Bcc",
		"Resent-From", "Resent-Sender", "Resent-To", "Resent-Cc", "Resent-Bcc",
		"Disposition-Notification-To", "Return-Receipt-To", "Mail-Followup-To", "Mail-Reply-To",
		"Errors-To", "Apparently-To", "Delivered-To", "X-Original-To"
	};


	constexpr const char* STRUCTURED_FIELDS[] = {
		"Date", "Message-ID", "In-Reply-To", "References", "Received", "Return-Path",
		"Resent-Date", "Resent-Message-ID",
		"MIME-Version", "Content-Type", "Content-Transfer-Encoding", "Content-ID", "Content-Disposition"
	};


	constexpr std::string_view SPECIALS("()<>[]:;@\\,.\""); // RFC 5322, section 3.2.3


	constexpr std::string_view ESPECIALS("()<>@,;:\"/[]?.="); // RFC 2047, section 2


	bool isValidCharset(std::string_view s)
	{
		return !s.empty() && std::all_of(s.begin(), s.end(),
			[](char c) { return Poco::Ascii::hasProperties(c, Poco::Ascii::ACP_GRAPH) && ESPECIALS.find(c) == std::string_view::npos; });
	}


	bool isValidEncodedText(std::string_view s)
	{
		return !s.empty() && std::all_of(s.begin(), s.end(),
			[](char c) { return Poco::Ascii::hasProperties(c, Poco::Ascii::ACP_GRAPH) && c != '?'; });
	}


	bool skipComment(std::string_view::const_iterator& it, std::string_view::const_iterator end)
		/// Skips a comment, which may be nested, starting at its opening parenthesis.
		/// Returns false if the comment is not terminated.
	{
		int depth = 0;
		while (it != end)
		{
			const char ch = *it++;
			if (ch == '\\')
			{
				if (it == end) return false;
				++it;
			}
			else if (ch == '(') ++depth;
			else if (ch == ')' && --depth == 0) return true;
		}
		return false;
	}


	bool readQuotedString(std::string_view::const_iterator& it, std::string_view::const_iterator end, std::string& text)
		/// Appends the content of a quoted string, starting at its opening quote, to text.
		/// Returns false if the quoted string is not terminated.
	{
		++it;
		while (it != end)
		{
			const char ch = *it++;
			if (ch == '"') return true;
			if (ch == '\\')
			{
				if (it == end) return false;
				text += *it++;
			}
			else text += ch;
		}
		return false;
	}


	bool skipDomainLiteral(std::string_view::const_iterator& it, std::string_view::const_iterator end)
		/// Skips a domain literal, starting at its opening bracket.
		/// Returns false if the domain literal is not terminated by ']'
		/// before another '['; it then ends before the next ','.
	{
		// dtext excludes '[' (RFC 5322, section 3.4.1). Ending a malformed literal
		// at ',' keeps it from taking in the next mailbox.
		const auto close = std::find_if(it + 1, end, [](char c) { return c == ']' || c == '['; });
		if (close != end && *close == ']')
		{
			it = close + 1;
			return true;
		}
		it = std::find(it + 1, end, ',');
		return false;
	}


	bool readAngleAddress(std::string_view::const_iterator& it, std::string_view::const_iterator end, std::string& address)
		/// Reads an address enclosed in angle brackets, starting at the opening bracket.
		/// Returns false if the address is not terminated by '>'.
	{
		// Stopping at ',' keeps an unterminated address from taking in the next mailbox;
		// a ',' in a domain literal or a quoted local part belongs to the address.
		auto close = it + 1;
		while (close != end && *close != '>' && *close != ',' && *close != '<')
		{
			if (*close == '[') skipDomainLiteral(close, end);
			else if (*close == '"')
			{
				std::string localPart;
				readQuotedString(close, end, localPart);
			}
			else ++close;
		}
		address.assign(it + 1, close);
		Poco::trimInPlace(address);
		const bool terminated = close != end && *close == '>';
		it = terminated ? close + 1 : close;
		return terminated;
	}


	struct Mailbox
	{
		std::string phrase;
		std::string addrSpec; /// the text without comments, with quoted strings as written
		std::string address;
		bool hasAngleAddress = false;
		bool hasAt = false; /// an '@' outside of quoted strings, comments and domain literals
		bool valid = true;
	};


	char readMailbox(std::string_view::const_iterator& it, std::string_view::const_iterator end, Mailbox& mailbox)
		/// Reads a mailbox up to the next ',', ';' or ':' outside of quoted strings,
		/// comments, domain literals and angle brackets. Returns that delimiter, or '\0' at the end.
	{
		while (it != end)
		{
			const char ch = *it;
			if (ch == ',' || ch == ';' || ch == ':')
			{
				++it;
				return ch;
			}
			const auto start = it;
			if (ch == '(')
			{
				if (!skipComment(it, end)) mailbox.valid = false;
			}
			else if (Poco::Ascii::isSpace(ch))
			{
				if (!mailbox.phrase.empty() && mailbox.phrase.back() != ' ') mailbox.phrase += ' ';
				++it;
			}
			else if (mailbox.hasAngleAddress)
			{
				mailbox.valid = false;
				++it;
			}
			else if (ch == '"')
			{
				if (!readQuotedString(it, end, mailbox.phrase)) mailbox.valid = false;
			}
			else if (ch == '<')
			{
				if (!readAngleAddress(it, end, mailbox.address)) mailbox.valid = false;
				mailbox.hasAngleAddress = true;
			}
			else if (ch == '[')
			{
				if (!skipDomainLiteral(it, end)) mailbox.valid = false;
				mailbox.phrase.append(start, it);
			}
			else
			{
				if (ch == '@') mailbox.hasAt = true;
				mailbox.phrase += ch;
				++it;
			}
			if (ch != '(') mailbox.addrSpec.append(start, it);
		}
		return '\0';
	}


	void addMailbox(Mailbox& mailbox, MailRecipient::RecipientType type, std::vector<MailRecipient>& recipients)
		/// Appends a recipient for the mailbox, unless it is not valid.
	{
		Poco::trimInPlace(mailbox.phrase);
		Poco::trimInPlace(mailbox.addrSpec);
		if (!mailbox.valid) return;
		if (mailbox.hasAngleAddress && !mailbox.address.empty())
			recipients.emplace_back(type, mailbox.address, mailbox.phrase);
		else if (!mailbox.hasAngleAddress && mailbox.hasAt
			&& std::none_of(mailbox.addrSpec.begin(), mailbox.addrSpec.end(), Poco::Ascii::isSpace))
			recipients.emplace_back(type, mailbox.addrSpec);
	}


	std::string_view::size_type findAngleAddress(std::string_view mailbox)
		/// Returns the position of the '<' that starts the angle address of a mailbox,
		/// or the size of the mailbox if it has none.
	{
		auto it = mailbox.begin();
		while (it != mailbox.end() && *it != '<')
		{
			if (*it == '"')
			{
				std::string text;
				readQuotedString(it, mailbox.end(), text);
			}
			else if (*it == '(') skipComment(it, mailbox.end());
			else if (*it == '[') skipDomainLiteral(it, mailbox.end());
			else ++it;
		}
		return static_cast<std::string_view::size_type>(it - mailbox.begin());
	}


	void appendComments(std::string_view displayName, std::string& out)
		/// Appends the comments of a display name without unterminated parts,
		/// each preceded by a space.
	{
		auto it = displayName.begin();
		const auto end = displayName.end();
		while (it != end)
		{
			const auto start = it;
			if (*it == '"')
			{
				std::string text;
				readQuotedString(it, end, text);
			}
			else if (*it == '[') skipDomainLiteral(it, end);
			else if (*it == '(')
			{
				skipComment(it, end);
				out += ' ';
				out.append(start, it);
			}
			else ++it;
		}
	}


	void appendDisplayName(std::string_view words, const std::string& charset, std::string& out)
		/// Appends words of a display name with their encoded words decoded.
	{
		// White space between words is collapsed as readMailbox() does, so that a
		// quoted display name gives the real name that the undecoded one gives.
		std::string text;
		for (const char ch: words)
		{
			if (!Poco::Ascii::isSpace(ch)) text += ch;
			else if (text.empty() || text.back() != ' ') text += ' ';
		}
		const std::string decoded = MessageHeader::decodeWord(text, charset);
		if (decoded == text)
		{
			out.append(words);
			return;
		}
		const auto first = decoded.find_first_not_of(' ');
		if (first == std::string::npos || decoded.find_first_of(SPECIALS, first) == std::string::npos)
		{
			out += decoded;
			return;
		}
		const auto last = decoded.find_last_not_of(' ') + 1;
		out.append(decoded, 0, first);
		out += '"';
		for (auto i = first; i < last; ++i)
		{
			if (decoded[i] == '"' || decoded[i] == '\\') out += '\\';
			out += decoded[i];
		}
		out += '"';
		out.append(decoded, last);
	}


	void appendCommentText(std::string_view text, const std::string& charset, std::string& out)
		/// Appends text of a comment between parentheses and quoted-pairs with its encoded words decoded.
	{
		for (const char ch: MessageHeader::decodeWord(std::string(text), charset))
		{
			if (ch == '(' || ch == ')' || ch == '\\') out += '\\';
			out += ch;
		}
	}


	void appendComment(std::string_view comment, const std::string& charset, std::string& out)
		/// Appends a terminated comment with the encoded words in its text decoded.
	{
		std::string_view::size_type textStart = 0;
		for (std::string_view::size_type pos = 0; pos < comment.size(); ++pos)
		{
			const char ch = comment[pos];
			if (ch != '(' && ch != ')' && ch != '\\') continue;
			appendCommentText(comment.substr(textStart, pos - textStart), charset, out);
			const std::string_view::size_type length = (ch == '\\' && pos + 1 < comment.size()) ? 2 : 1;
			out.append(comment.substr(pos, length));
			pos += length - 1;
			textStart = pos + 1;
		}
		appendCommentText(comment.substr(textStart), charset, out);
	}


	void appendDecoded(std::string_view text, bool isDisplayName, const std::string& charset, std::string& out)
		/// Appends a part of a mailbox with the encoded words in its comments decoded,
		/// and also those in its words outside of quoted strings if it is a display name.
	{
		auto it = text.begin();
		const auto end = text.end();
		while (it != end)
		{
			const auto start = it;
			if (*it == '"')
			{
				std::string content;
				readQuotedString(it, end, content);
				out.append(start, it);
			}
			else if (*it == '[')
			{
				skipDomainLiteral(it, end);
				out.append(start, it);
			}
			else if (*it == '(')
			{
				if (skipComment(it, end))
					appendComment(std::string_view(start, it), charset, out);
				else
					out.append(start, it);
			}
			else
			{
				it = std::find_if(it, end, [](char c) { return c == '"' || c == '(' || c == '['; });
				if (isDisplayName)
					appendDisplayName(std::string_view(start, it), charset, out);
				else
					out.append(start, it);
			}
		}
	}
}


HeaderField::Syntax HeaderField::syntax(const std::string& name)
{
	const auto isName = [&name](const char* field) { return Poco::icompare(name, field) == 0; };
	if (std::any_of(std::begin(ADDRESS_FIELDS), std::end(ADDRESS_FIELDS), isName))
		return Syntax::ADDRESS_LIST;
	if (std::any_of(std::begin(STRUCTURED_FIELDS), std::end(STRUCTURED_FIELDS), isName))
		return Syntax::STRUCTURED;
	return Syntax::UNSTRUCTURED;
}


void HeaderField::parseAddressList(std::string_view list, MailRecipient::RecipientType type, std::vector<MailRecipient>& recipients)
{
	bool inGroup = false;
	auto it = list.begin();
	const auto end = list.end();
	while (it != end)
	{
		Mailbox mailbox;
		char delimiter = readMailbox(it, end, mailbox);
		while (delimiter == ':' && (inGroup || mailbox.hasAngleAddress))
		{
			mailbox.valid = false;
			delimiter = readMailbox(it, end, mailbox);
		}
		if (delimiter == ':')
		{
			// The display name of a group is not a recipient.
			inGroup = true;
			continue;
		}
		// ';' ends a group; it also separates mailboxes in lists written by some clients.
		if (delimiter == ';') inGroup = false;
		addMailbox(mailbox, type, recipients);
	}
}


std::string HeaderField::decodeAddressList(std::string_view list, const std::string& charset)
{
	std::string result;
	auto it = list.begin();
	const auto end = list.end();
	while (it != end)
	{
		const auto start = it;
		Mailbox mailbox;
		const char delimiter = readMailbox(it, end, mailbox);
		std::string_view text(start, it);
		if (delimiter != '\0') text.remove_suffix(1);
		// Words are a display name before an angle address or the colon of a group;
		// otherwise they are an addr-spec, in which RFC 2047 allows no encoded words.
		const auto nameEnd = text.begin() + ((mailbox.hasAngleAddress || delimiter == ':') ? findAngleAddress(text) : 0);
		// The angle address is copied unchanged, as readMailbox() takes it without comments.
		auto addressEnd = nameEnd;
		if (addressEnd != text.end() && *addressEnd == '<')
		{
			std::string address;
			readAngleAddress(addressEnd, text.end(), address);
		}
		appendDecoded(std::string_view(text.begin(), nameEnd), true, charset, result);
		result.append(nameEnd, addressEnd);
		appendDecoded(std::string_view(addressEnd, text.end()), false, charset, result);
		if (delimiter != '\0') result += delimiter;
	}
	return result;
}


bool HeaderField::parseEncodedWord(std::string_view word, EncodedWord& parts)
{
	const std::size_t pos1 = word.find('?');
	if (pos1 == std::string_view::npos) return false;
	const std::size_t pos2 = word.find('?', pos1 + 1);
	if (pos2 == std::string_view::npos) return false;
	const std::string_view encoding = word.substr(pos1 + 1, pos2 - pos1 - 1);
	if (encoding.size() != 1) return false;
	parts.charset = word.substr(0, pos1);
	parts.encoding = static_cast<char>(Poco::Ascii::toUpper(encoding[0]));
	parts.text = word.substr(pos2 + 1);
	if (parts.encoding != 'B' && parts.encoding != 'Q') return false;
	return isValidCharset(parts.charset) && isValidEncodedText(parts.text);
}


void HeaderField::decodeParameter(std::string& value)
{
	if (value.find("=?") == std::string::npos) return;
	std::string decoded = MessageHeader::decodeWord(value);
	if (std::none_of(decoded.begin(), decoded.end(), isInvalidValueChar)) value = std::move(decoded);
}


bool HeaderField::isInvalidValueChar(char c)
{
	return c != '\t' && Poco::Ascii::hasProperties(c, Poco::Ascii::ACP_CONTROL);
}


std::string HeaderField::encodeAddressList(std::string_view list)
{
	std::string result;
	auto it = list.begin();
	const auto end = list.end();
	while (it != end)
	{
		const auto start = it;
		Mailbox mailbox;
		const char delimiter = readMailbox(it, end, mailbox);
		const std::string_view text(start, it);
		Poco::trimInPlace(mailbox.phrase);
		const bool hasDisplayName = mailbox.hasAngleAddress || delimiter == ':';
		if (!mailbox.valid && !std::all_of(text.begin(), text.end(), Poco::Ascii::isAscii))
			throw MessageException("Cannot encode the non-ASCII text of a mailbox that cannot be parsed", std::string(text));
		if (hasDisplayName && !std::all_of(mailbox.phrase.begin(), mailbox.phrase.end(), Poco::Ascii::isAscii))
		{
			const auto nameStart = std::min(text.find_first_not_of(" \t"), text.size());
			result.append(text.substr(0, nameStart));
			result += MailMessage::encodeWord(mailbox.phrase, "UTF-8", MailMessage::WordEncoding::AUTO);
			// The phrase has no comments; the mailbox is valid here, so they are terminated.
			const auto nameEnd = mailbox.hasAngleAddress ? findAngleAddress(text) : text.size() - 1;
			appendComments(text.substr(nameStart, nameEnd - nameStart), result);
			if (mailbox.hasAngleAddress)
			{
				result += ' ';
				result.append(text.substr(nameEnd));
			}
			else result += delimiter;
		}
		else result.append(text);
	}
	return result;
}


} // namespace Poco::Net
