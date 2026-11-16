//
// MessageHeader.cpp
//
// Library: Net
// Package: Messages
// Module:  MessageHeader
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Net/MessageHeader.h"
#include "Poco/Net/NetException.h"
#include "HeaderField.h"
#include "Poco/String.h"
#include "Poco/Ascii.h"
#include "Poco/TextConverter.h"
#include "Poco/TextEncoding.h"
#include "Poco/Base64Decoder.h"
#include <algorithm>
#include <charconv>
#include <sstream>
#include <string_view>
#include <utility>


namespace Poco::Net {


namespace
{
	// RFC 2047 encoded words are a MIME construct. Decoding them in the headers
	// that decide the message framing would let an encoded value become
	// "chunked" after a proxy has framed the message on the undecoded one. The
	// names are spelled out rather than taken from HTTPMessage to keep this
	// class free of a dependency on the HTTP layer above it.
	bool isFramingHeader(const std::string& name)
	{
		return Poco::icompare(name, "Transfer-Encoding") == 0 ||
			Poco::icompare(name, "Content-Length") == 0;
	}


	bool isValidBase64(std::string_view s)
	{
		if (s.empty() || s.size() % 4 != 0) return false;
		const std::size_t dataLength = s.find_last_not_of('=') + 1;
		if (s.size() - dataLength > 2) return false;
		return std::all_of(s.begin(), s.begin() + dataLength,
			[](char c) { return Poco::Ascii::isAlphaNumeric(c) || c == '+' || c == '/'; });
	}


	bool decodeEncodedWord(const HeaderField::EncodedWord& word, const std::string& toCharset, std::string& decoded)
	{
		std::string bytes;
		if (word.encoding == 'B')
		{
			if (!isValidBase64(word.text)) return false;
			std::istringstream istr{std::string(word.text)};
			Base64Decoder decoder(istr);
			for (char c; decoder.get(c);) bytes += c;
		}
		else
		{
			std::size_t i = 0;
			while (i < word.text.size())
			{
				const char c = word.text[i++];
				if (c == '_')
				{
					bytes += ' ';
				}
				else if (c == '=')
				{
					if (word.text.size() - i < 2) return false;
					const char* first = word.text.data() + i;
					unsigned int value = 0;
					if (std::from_chars(first, first + 2, value, 16).ptr != first + 2) return false;
					bytes += static_cast<char>(value);
					i += 2;
				}
				else
				{
					bytes += c;
				}
			}
		}

		std::string result;
		const std::string charset(word.charset);
		if (Poco::icompare(charset, toCharset) != 0)
		{
			try
			{
				TextEncoding& enc = TextEncoding::byName(charset);
				TextEncoding& dec = TextEncoding::byName(toCharset);
				TextConverter converter(enc, dec);
				converter.convert(bytes, result);
			}
			catch (...)
			{
				// Unsupported or unknown encoding; fall back to raw decoded text
				result = bytes;
			}
		}
		else
		{
			result = std::move(bytes);
		}
		decoded = std::move(result);
		return true;
	}


	std::pair<std::string, std::string> receivedKey(const std::string& name, const std::string& value)
	{
		return {Poco::toLower(name), value};
	}


	std::string decodeField(const std::string& name, const std::string& value)
	{
		if (value.find("=?") == std::string::npos) return value;
		const HeaderField::Syntax syntax = HeaderField::syntax(name);
		if (syntax == HeaderField::Syntax::ADDRESS_LIST) return HeaderField::decodeAddressList(value, "UTF-8");
		if (syntax == HeaderField::Syntax::STRUCTURED) return value;
		return MessageHeader::decodeWord(value);
	}


	template <typename Node>
	void secureClearReceived(Node& node)
		/// Overwrites the stored value and the received form of an extracted received field.
	{
		Poco::secureClear(node.key().second);
		Poco::secureClear(node.mapped());
	}
}



MessageHeader::MessageHeader():
	_fieldLimit(DFL_FIELD_LIMIT),
	_nameLengthLimit(DFL_NAME_LENGTH_LIMIT),
	_valueLengthLimit(DFL_VALUE_LENGTH_LIMIT),
	_autoDecode(true),
	_decodedOnRead(false),
	_strictParsing(false)
{
}


MessageHeader::MessageHeader(const MessageHeader& messageHeader):
	NameValueCollection(messageHeader),
	_fieldLimit(messageHeader._fieldLimit),
	_nameLengthLimit(messageHeader._nameLengthLimit),
	_valueLengthLimit(messageHeader._valueLengthLimit),
	_autoDecode(messageHeader._autoDecode),
	_decodedOnRead(messageHeader._decodedOnRead),
	_strictParsing(messageHeader._strictParsing),
	_receivedFields(messageHeader._receivedFields)
{
}


MessageHeader::MessageHeader(MessageHeader&& messageHeader) noexcept:
	NameValueCollection(std::move(messageHeader)),
	_fieldLimit(messageHeader._fieldLimit),
	_nameLengthLimit(messageHeader._nameLengthLimit),
	_valueLengthLimit(messageHeader._valueLengthLimit),
	_autoDecode(messageHeader._autoDecode),
	_decodedOnRead(messageHeader._decodedOnRead),
	_strictParsing(messageHeader._strictParsing),
	_receivedFields(std::exchange(messageHeader._receivedFields, {}))
{
}


MessageHeader::~MessageHeader()
{
}


MessageHeader& MessageHeader::operator = (const MessageHeader& messageHeader)
{
	NameValueCollection::operator = (messageHeader);
	_fieldLimit = messageHeader._fieldLimit;
	_nameLengthLimit = messageHeader._nameLengthLimit;
	_valueLengthLimit = messageHeader._valueLengthLimit;
	_autoDecode = messageHeader._autoDecode;
	_decodedOnRead = messageHeader._decodedOnRead;
	_strictParsing = messageHeader._strictParsing;
	_receivedFields = messageHeader._receivedFields;
	return *this;
}


MessageHeader& MessageHeader::operator = (MessageHeader&& messageHeader) noexcept
{
	if (&messageHeader != this)
	{
		// Swapping and clearing leaves the source empty, as the move constructor does.
		swap(messageHeader);
		messageHeader.clear();
	}
	return *this;
}


void MessageHeader::erase(const std::string& name)
{
	NameValueCollection::erase(name);
	eraseReceived(name, false);
}


void MessageHeader::clear()
{
	NameValueCollection::clear();
	_receivedFields.clear();
}


void MessageHeader::secureErase(const std::string& name)
{
	NameValueCollection::secureErase(name);
	eraseReceived(name, true);
}


void MessageHeader::secureErase(Iterator it)
{
	if (it != end())
	{
		auto key = receivedKey(it->first, it->second);
		auto node = _receivedFields.extract(key);
		if (!node.empty()) secureClearReceived(node);
		// The lookup key holds a copy of the value.
		Poco::secureClear(key.second);
	}
	NameValueCollection::secureErase(it);
}


void MessageHeader::secureClear()
{
	NameValueCollection::secureClear();
	while (!_receivedFields.empty())
	{
		auto node = _receivedFields.extract(_receivedFields.begin());
		secureClearReceived(node);
	}
}


void MessageHeader::swap(MessageHeader& messageHeader) noexcept
{
	NameValueCollection::swap(messageHeader);
	std::swap(_fieldLimit, messageHeader._fieldLimit);
	std::swap(_nameLengthLimit, messageHeader._nameLengthLimit);
	std::swap(_valueLengthLimit, messageHeader._valueLengthLimit);
	std::swap(_autoDecode, messageHeader._autoDecode);
	std::swap(_decodedOnRead, messageHeader._decodedOnRead);
	std::swap(_strictParsing, messageHeader._strictParsing);
	_receivedFields.swap(messageHeader._receivedFields);
}


void MessageHeader::write(std::ostream& ostr) const
{
	if (_receivedFields.empty())
	{
		checkFields(*this, true);
		writeFields(*this, ostr);
		return;
	}
	NameValueCollection fields(*this);
	for (auto& [name, value]: fields)
	{
		if (const auto it = _receivedFields.find(receivedKey(name, value)); it != _receivedFields.end())
			value = it->second;
	}
	checkFields(fields, true);
	writeFields(fields, ostr);
}


void MessageHeader::read(std::istream& istr)
{
	static constexpr int eof = std::char_traits<char>::eof();
	std::streambuf& buf = *istr.rdbuf();

	std::string name;
	std::string value;
	name.reserve(32);
	value.reserve(64);
	int ch = buf.sbumpc();
	int fields = 0;
	while (ch != eof && ch != '\r' && ch != '\n')
	{
		if (_fieldLimit > 0 && fields == _fieldLimit)
			throw MessageException("Too many header fields");
		name.clear();
		value.clear();
		while (ch != eof && ch != ':' && ch != '\n' && static_cast<int>(name.length()) < _nameLengthLimit)
		{
			name += ch;
			ch = buf.sbumpc();
		}
		if (ch == '\n') { ch = buf.sbumpc(); ++fields; continue; } // ignore invalid header lines
		if (ch != ':') throw MessageException("Field name too long/no colon found");
		// White space or a control character in a name would hide a header
		// such as "Content-Length : 5" from a lookup by name, while a front
		// end may still frame the message on it.
		if (_strictParsing && !isValidFieldName(name))
			throw MessageException("Invalid character in header field name");
		ch = buf.sbumpc(); // skip ':'
		if (_strictParsing)
		{
			int skipped = 0;
			while (ch == ' ' || ch == '\t')
			{
				if (++skipped > _valueLengthLimit) throw MessageException("Field value too long");
				ch = buf.sbumpc();
			}
		}
		else
			while (ch != eof && Poco::Ascii::isSpace(ch) && ch != '\r' && ch != '\n') ch = buf.sbumpc();
		while (ch != eof && ch != '\r' && ch != '\n' && static_cast<int>(value.length()) < _valueLengthLimit)
		{
			value += ch;
			ch = buf.sbumpc();
		}
		if (ch == '\r') ch = buf.sbumpc();
		if (ch == '\n')
			ch = buf.sbumpc();
		else if (ch != eof)
			throw MessageException("Field value too long/no CRLF found");
		while (ch == ' ' || ch == '\t') // folding
		{
			while (ch != eof && ch != '\r' && ch != '\n' && static_cast<int>(value.length()) < _valueLengthLimit)
			{
				value += ch;
				ch = buf.sbumpc();
			}
			if (ch == '\r') ch = buf.sbumpc();
			if (ch == '\n')
				ch = buf.sbumpc();
			else if (ch != eof)
				throw MessageException("Folded field value too long/no CRLF found");
		}

		// TODO: Add to the if below?
		if (_strictParsing)
			value.erase(value.find_last_not_of(" \t") + 1);
		else
			Poco::trimRightInPlace(value);

		const std::string decoded = (_autoDecode && !isFramingHeader(name)) ? decodeField(name, value) : value;
		// A decoded CR or LF would split the field when the value is written again.
		const std::string& stored = std::any_of(decoded.begin(), decoded.end(), isInvalidValueChar) ? value : decoded;
		// write() writes a decoded field as received, and MailMessage::write() does not encode received text.
		if (stored != value || !std::all_of(stored.begin(), stored.end(), Poco::Ascii::isAscii))
			_receivedFields.emplace(receivedKey(name, stored), value);
		add(name, stored);

		++fields;
	}
	// Save the state of the auto decode at the time of reading.
	_decodedOnRead = _autoDecode;
	if (istr.good() && ch != eof)
		istr.putback(ch);
}


void MessageHeader::setAutoDecode(bool decode)
{
	_autoDecode = decode;
}


bool MessageHeader::getAutoDecode() const
{
	return _autoDecode;
}


void MessageHeader::setStrictParsing(bool strict)
{
	_strictParsing = strict;
}


bool MessageHeader::getStrictParsing() const
{
	return _strictParsing;
}


std::string MessageHeader::getDecoded(const std::string& name) const
{
	const auto& value { get(name) };
	if (_decodedOnRead)
	{
		// Already decoded, just return the value
		return value;
	}
	return decodeField(name, value);
}


std::string MessageHeader::getDecoded(const std::string& name, const std::string& defaultValue) const
{
	if (!has(name))
	{
		return defaultValue;
	}
	return getDecoded(name);
}


int MessageHeader::getFieldLimit() const
{
	return _fieldLimit;
}


void MessageHeader::setFieldLimit(int limit)
{
	poco_assert (limit >= 0);

	_fieldLimit = limit;
}


int MessageHeader::getNameLengthLimit() const
{
	return _nameLengthLimit;
}

void MessageHeader::setNameLengthLimit(int limit)
{
	poco_assert(limit >= 0);

	_nameLengthLimit = limit;
}


int MessageHeader::getValueLengthLimit() const
{
	return _valueLengthLimit;
}

void MessageHeader::setValueLengthLimit(int limit)
{
	poco_assert(limit >= 0);

	_valueLengthLimit = limit;
}


bool MessageHeader::hasToken(const std::string& fieldName, const std::string& token) const
{
	std::string field = get(fieldName, "");
	std::vector<std::string> tokens;
	splitElements(field, tokens, true);
	for (const auto& t: tokens)
	{
		if (Poco::icompare(t, token) == 0)
			return true;
	}
	return false;
}


void MessageHeader::splitElements(const std::string& s, std::vector<std::string>& elements, bool ignoreEmpty)
{
	elements.clear();
	std::string::const_iterator it  = s.begin();
	std::string::const_iterator end = s.end();
	std::string elem;
	elem.reserve(64);
	while (it != end)
	{
		if (*it == '"')
		{
			elem += *it++;
			while (it != end && *it != '"')
			{
				if (*it == '\\')
				{
					++it;
					if (it != end) elem += *it++;
				}
				else elem += *it++;
			}
			if (it != end) elem += *it++;
		}
		else if (*it == '\\')
		{
			++it;
			if (it != end) elem += *it++;
		}
		else if (*it == ',')
		{
			Poco::trimInPlace(elem);
			if (!ignoreEmpty || !elem.empty())
				elements.push_back(elem);
			elem.clear();
			++it;
		}
		else elem += *it++;
	}
	if (!elem.empty())
	{
		Poco::trimInPlace(elem);
		if (!ignoreEmpty || !elem.empty())
			elements.push_back(elem);
	}
}


void MessageHeader::splitParameters(const std::string& s, std::string& value, NameValueCollection& parameters)
{
	value.clear();
	parameters.clear();
	std::string::const_iterator it  = s.begin();
	std::string::const_iterator end = s.end();
	while (it != end && Poco::Ascii::isSpace(*it)) ++it;
	while (it != end && *it != ';') value += *it++;
	Poco::trimRightInPlace(value);
	if (it != end) ++it;
	splitParameters(it, end, parameters);
	for (auto& [name, parameter]: parameters)
	{
		if (Poco::icompare(name, "name") == 0 || Poco::icompare(name, "filename") == 0)
			HeaderField::decodeParameter(parameter);
	}
}


void MessageHeader::splitParameters(const std::string::const_iterator& begin, const std::string::const_iterator& end, NameValueCollection& parameters)
{
	std::string pname;
	std::string pvalue;
	pname.reserve(32);
	pvalue.reserve(64);
	std::string::const_iterator it = begin;
	while (it != end)
	{
		pname.clear();
		pvalue.clear();
		while (it != end && Poco::Ascii::isSpace(*it)) ++it;
		while (it != end && *it != '=' && *it != ';') pname += *it++;
		Poco::trimRightInPlace(pname);
		if (it != end && *it != ';') ++it;
		while (it != end && Poco::Ascii::isSpace(*it)) ++it;
		while (it != end && *it != ';')
		{
			if (*it == '"')
			{
				++it;
				while (it != end && *it != '"')
				{
					if (*it == '\\')
					{
						++it;
						if (it != end) pvalue += *it++;
					}
					else pvalue += *it++;
				}
				if (it != end) ++it;
			}
			else if (*it == '\\')
			{
				++it;
				if (it != end) pvalue += *it++;
			}
			else pvalue += *it++;
		}
		Poco::trimRightInPlace(pvalue);
		if (!pname.empty()) parameters.add(pname, pvalue);
		if (it != end) ++it;
	}
}


void MessageHeader::quote(const std::string& value, std::string& result, bool allowSpace)
{
	bool mustQuote = false;
	for (std::string::const_iterator it = value.begin(); !mustQuote && it != value.end(); ++it)
	{
		if (!Poco::Ascii::isAlphaNumeric(*it) && *it != '.' && *it != '_' && *it != '-' && !(Poco::Ascii::isSpace(*it) && allowSpace))
			mustQuote = true;
	}
	if (mustQuote) result += '"';
	for (const char c: value)
	{
		// A '"' or '\' always makes the value quoted, and is written as a quoted-pair (RFC 5322).
		if (c == '"' || c == '\\') result += '\\';
		result += c;
	}
	if (mustQuote) result += '"';
}


void MessageHeader::decodeRFC2047(const std::string& ins, std::string& outs, const std::string& toCharset)
{
	std::string_view word(ins);
	if (word.size() >= 4 && word.starts_with("=?") && word.ends_with("?="))
		word = word.substr(2, word.size() - 4);
	HeaderField::EncodedWord parts;
	std::string decoded;
	if (HeaderField::parseEncodedWord(word, parts) && decodeEncodedWord(parts, toCharset, decoded))
		outs = std::move(decoded);
	else
		outs = ins;
}


std::string MessageHeader::decodeWord(const std::string& text, const std::string& charset)
{
	std::string outs;
	std::size_t cur = 0;
	bool prevDecoded = false;
	std::size_t pos = 0;
	while ((pos = text.find("=?", cur)) != std::string::npos)
	{
		// The encoded text cannot contain '?', so the word ends at the third '?'
		// after "=?". This keeps the scan linear for values with many "=?" and no "?=".
		std::size_t end = pos + 1;
		for (int i = 0; i < 3 && end != std::string::npos; ++i)
			end = text.find('?', end + 1);
		HeaderField::EncodedWord parts;
		std::string decoded;
		const bool isDecoded = end != std::string::npos && end + 1 < text.size() && text[end + 1] == '='
			&& HeaderField::parseEncodedWord(std::string_view(text).substr(pos + 2, end - pos - 2), parts)
			&& decodeEncodedWord(parts, charset, decoded);
		if (!isDecoded)
		{
			outs.append(text, cur, pos + 2 - cur);
			cur = pos + 2;
			prevDecoded = false;
			continue;
		}
		const std::string_view between(text.data() + cur, pos - cur);
		// RFC 2047, section 6.2: white space between two decoded words is removed.
		if (!prevDecoded || between.find_first_not_of(" \t\r\n") != std::string_view::npos)
			outs.append(between);
		outs.append(decoded);
		prevDecoded = true;
		cur = end + 2;
	}
	outs.append(text, cur);
	return outs;
}


bool MessageHeader::isValidFieldName(const std::string& name)
{
	static constexpr std::string_view tokenPunct("!#$%&'*+-.^_`|~");
	return !name.empty() && std::all_of(name.begin(), name.end(),
		[](char c) { return Poco::Ascii::isAlphaNumeric(c) || tokenPunct.find(c) != std::string_view::npos; });
}


bool MessageHeader::isInvalidValueChar(char c)
{
	return HeaderField::isInvalidValueChar(c);
}


void MessageHeader::checkFields(const NameValueCollection& fields, bool allowFolding)
{
	const auto isFieldTextChar = [](char c) { return Poco::Ascii::isPrintable(c) && c != ' ' && c != ':'; };
	for (const auto& [name, value]: fields)
	{
		const bool validName = allowFolding
			? !name.empty() && std::all_of(name.begin(), name.end(), isFieldTextChar)
			: isValidFieldName(name);
		if (!validName)
			throw MessageException("Invalid header field name");
		checkFieldValue(name, value, allowFolding);
	}
}


void MessageHeader::checkFieldValue(const std::string& name, const std::string& value, bool allowFolding)
{
	for (std::string::size_type i = 0; i < value.size(); ++i)
	{
		if (!isInvalidValueChar(value[i])) continue;
		// CR LF folds the value, as MailMessage::encodeWord() writes it, only when
		// white space and then other text follow; any other CR or LF would start
		// a new field or, as a line of white space only, end the header.
		const std::string::size_type text = value.find_first_not_of(" \t", i + 2);
		const bool isFold = allowFolding && value.compare(i, 2, "\r\n") == 0
			&& text != std::string::npos && text > i + 2
			&& value[text] != '\r' && value[text] != '\n';
		if (isFold)
			++i;
		else
			throw MessageException("Invalid character in header field value", name);
	}
}


void MessageHeader::writeFields(const NameValueCollection& fields, std::ostream& ostr)
{
	for (const auto& [name, value]: fields)
		ostr << name << ": " << value << "\r\n";
}


void MessageHeader::eraseReceived(const std::string& name, bool secure)
{
	if (_receivedFields.empty()) return;
	const std::string key = Poco::toLower(name);
	auto it = _receivedFields.lower_bound({key, std::string()});
	while (it != _receivedFields.end() && it->first.first == key)
	{
		auto node = _receivedFields.extract(it++);
		if (secure) secureClearReceived(node);
	}
}


bool MessageHeader::isReceived(const std::string& name, const std::string& value) const
{
	return _receivedFields.contains(receivedKey(name, value));
}


} // namespace Poco::Net
