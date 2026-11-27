//
// MailMessage.cpp
//
// Library: Net
// Package: Mail
// Module:  MailMessage
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Net/MailMessage.h"
#include "Poco/Net/MediaType.h"
#include "Poco/Net/MultipartReader.h"
#include "Poco/Net/NetException.h"
#include "Poco/Net/MultipartWriter.h"
#include "Poco/Net/PartSource.h"
#include "Poco/Net/PartHandler.h"
#include "Poco/Net/StringPartSource.h"
#include "Poco/Net/QuotedPrintableEncoder.h"
#include "Poco/Net/QuotedPrintableDecoder.h"
#include "Poco/Net/NameValueCollection.h"
#include "HeaderField.h"
#include "Poco/Ascii.h"
#include "Poco/Base64Encoder.h"
#include "Poco/Base64Decoder.h"
#include "Poco/StreamCopier.h"
#include "Poco/DateTimeFormat.h"
#include "Poco/DateTimeFormatter.h"
#include "Poco/DateTimeParser.h"
#include "Poco/String.h"
#include "Poco/StreamCopier.h"
#include "Poco/NumberFormatter.h"
#include "Poco/UTF8String.h"
#include <algorithm>
#include <limits>
#include <sstream>
#include <string_view>
#include <utility>


using Poco::Base64Encoder;
using Poco::Base64Decoder;
using Poco::StreamCopier;
using Poco::DateTimeFormat;
using Poco::DateTimeFormatter;
using Poco::DateTimeParser;
using Poco::icompare;


namespace Poco::Net {


namespace
{
	class MultiPartHandler: public PartHandler
		/// This is a default part handler for multipart messages, used when there
		/// is no external handler provided to he MailMessage. This handler
		/// will handle all types of message parts, including attachments.
	{
	public:
		MultiPartHandler(MailMessage* pMsg): _pMsg(pMsg)
			/// Creates multi part handler.
			/// The pMsg pointer points to the calling MailMessage
			/// and will be used to properly populate it, so the
			/// message content could be written out unmodified
			/// in its entirety, including attachments.
		{
		}

		~MultiPartHandler() = default;
			/// Destroys multi part handler.

		void handlePart(const MessageHeader& header, std::istream& stream)
			/// Handles a part. If message pointer was provided at construction time,
			/// the message pointed to will be properly populated so it could be written
			/// back out at a later point in time.
		{
			std::string tmp;
			Poco::StreamCopier::copyToString(stream, tmp);
			if (_pMsg)
			{
				MailMessage::ContentTransferEncoding cte = MailMessage::ENCODING_7BIT;
				if (header.has(MailMessage::HEADER_CONTENT_TRANSFER_ENCODING))
				{
					const std::string& enc = header[MailMessage::HEADER_CONTENT_TRANSFER_ENCODING];
					if (enc == MailMessage::CTE_8BIT)
						cte = MailMessage::ENCODING_8BIT;
					else if (enc == MailMessage::CTE_QUOTED_PRINTABLE)
						cte = MailMessage::ENCODING_QUOTED_PRINTABLE;
					else if (enc == MailMessage::CTE_BASE64)
						cte = MailMessage::ENCODING_BASE64;
				}

				// RFC 2045, section 5.2: a part without Content-Type is US-ASCII plain text.
				static const std::string DEFAULT_CONTENT_TYPE("text/plain; charset=us-ascii");
				const std::string contentType = header.get(MailMessage::HEADER_CONTENT_TYPE, DEFAULT_CONTENT_TYPE);
				std::string mediaType;
				NameValueCollection typeParams;
				MessageHeader::splitParameters(contentType, mediaType, typeParams);
				const std::string contentDisp = header.get(MailMessage::HEADER_CONTENT_DISPOSITION, MailMessage::EMPTY_HEADER);
				std::string disposition;
				NameValueCollection dispParams;
				MessageHeader::splitParameters(contentDisp, disposition, dispParams);
				const std::string filename = dispParams.get("filename", typeParams.get("name", MailMessage::EMPTY_HEADER));
				PartSource* pPS = _pMsg->createPartStore(tmp, contentType, filename);
				poco_check_ptr (pPS);

				if (disposition.empty() || icompare(disposition, "inline") == 0)
					_pMsg->addContent(pPS, cte);
				else
					_pMsg->addAttachment(typeParams.get("name", filename), pPS, cte);

				for (const auto& [key, value] : header)
				{
					pPS->headers().set(key, value);
				}
			}
		}

	private:
		MailMessage* _pMsg;
	};


	class StringPartHandler: public PartHandler
		/// This is a default part handler, used when there is no
		/// external handler provided to the MailMessage. This handler
		/// handles only single-part messages.
	{
	public:
		StringPartHandler(std::string& content): _str(content)
			/// Creates string part handler.
			/// The content parameter represents the part content.
		{
		}

		~StringPartHandler() = default;
			/// Destroys string part handler.

		void handlePart(const MessageHeader& header, std::istream& stream)
			/// Handles a part.
		{
			std::string tmp;
			Poco::StreamCopier::copyToString(stream, tmp);
			_str.append(tmp);
		}

	private:
		std::string& _str;
	};


	constexpr std::string::size_type MAX_ENCODED_WORD_LENGTH = 75; // RFC 2047, section 2
	constexpr std::string::size_type MAX_UTF8_CHARACTER_LENGTH = 4;
	constexpr unsigned char UTF8_CONTINUATION_MASK = 0xC0;
	constexpr unsigned char UTF8_CONTINUATION_BITS = 0x80;
	constexpr std::string_view ENCODED_WORD_END("?=");
	// Encoded words of the "Q" encoding end after white space from this length on, or at
	// the second length, as earlier versions wrote them.
	constexpr std::string::size_type Q_WORD_LENGTH_BREAK_AT_SPACE = 64;
	constexpr std::string::size_type Q_WORD_LENGTH_BREAK = 72;


	std::string::size_type characterLength(const std::string& text, std::string::size_type pos, bool isUTF8)
		/// Returns the number of bytes of the character at pos: in UTF-8 the
		/// first byte and the continuation bytes after it, otherwise one.
	{
		std::string::size_type length = 1;
		while (isUTF8 && length < MAX_UTF8_CHARACTER_LENGTH && pos + length < text.size()
			&& (static_cast<unsigned char>(text[pos + length]) & UTF8_CONTINUATION_MASK) == UTF8_CONTINUATION_BITS)
			++length;
		return length;
	}


	void appendQ(char ch, std::string& encodedText)
		/// Appends a byte in the "Q" encoding. The specials of RFC 5322 are encoded
		/// too, so that an encoded word can be a display name.
	{
		switch (ch)
		{
		case ' ':
			encodedText += '_';
			break;
		case '=':
		case '?':
		case '_':
		case '(':
		case ')':
		case '[':
		case ']':
		case '<':
		case '>':
		case ',':
		case ';':
		case ':':
		case '.':
		case '@':
		case '"':
		case '\\':
			encodedText += '=';
			NumberFormatter::appendHex(encodedText, static_cast<unsigned>(static_cast<unsigned char>(ch)), 2);
			break;
		default:
			if (ch > 32 && ch < 127)
			{
				encodedText += ch;
			}
			else
			{
				encodedText += '=';
				NumberFormatter::appendHex(encodedText, static_cast<unsigned>(static_cast<unsigned char>(ch)), 2);
			}
		}
	}


	std::string encodeQ(const std::string& text, const std::string& charset)
	{
		const bool isUTF8 = icompare(charset, "UTF-8") == 0;
		const std::string wordStart = "=?" + charset + "?q?";
		std::string encodedText;
		std::string::size_type lineLength = 0;
		std::string::size_type pos = 0;
		while (pos < text.size())
		{
			const std::string::size_type length = characterLength(text, pos, isUTF8);
			std::string character;
			for (std::string::size_type i = pos; i < pos + length; ++i) appendQ(text[i], character);
			if (lineLength > 0 && lineLength + character.size() + ENCODED_WORD_END.size() > MAX_ENCODED_WORD_LENGTH)
			{
				encodedText += "?=\r\n ";
				lineLength = 0;
			}
			if (lineLength == 0)
			{
				encodedText += wordStart;
				lineLength = wordStart.size();
			}
			encodedText += character;
			lineLength += character.size();
			const char ch = text[pos];
			pos += length;
			const bool isSpace = ch == ' ' || ch == '\t' || ch == '\r' || ch == '\n';
			if (pos < text.size() && ((lineLength >= Q_WORD_LENGTH_BREAK_AT_SPACE && isSpace) || lineLength >= Q_WORD_LENGTH_BREAK))
			{
				encodedText += "?=\r\n ";
				lineLength = 0;
			}
		}
		encodedText += ENCODED_WORD_END;
		return encodedText;
	}


	std::string encodeB(const std::string& text, const std::string& charset)
	{
		const bool isUTF8 = icompare(charset, "UTF-8") == 0;
		const std::string wordStart = "=?" + charset + "?b?";
		const std::string::size_type overhead = wordStart.size() + ENCODED_WORD_END.size();
		// Base64 writes 4 characters for every 3 bytes.
		const std::string::size_type maxBytes = overhead < MAX_ENCODED_WORD_LENGTH ? (MAX_ENCODED_WORD_LENGTH - overhead) / 4 * 3 : 0;
		std::string encodedText;
		std::string::size_type pos = 0;
		while (pos < text.size())
		{
			std::string::size_type length = characterLength(text, pos, isUTF8);
			while (pos + length < text.size())
			{
				const std::string::size_type next = characterLength(text, pos + length, isUTF8);
				if (length + next > maxBytes) break;
				length += next;
			}
			std::ostringstream ostr;
			Base64Encoder encoder(ostr);
			encoder.rdbuf()->setLineLength(0);
			encoder.write(text.data() + pos, static_cast<std::streamsize>(length));
			encoder.close();
			if (!encodedText.empty()) encodedText += "\r\n ";
			encodedText += wordStart;
			encodedText += ostr.str();
			encodedText += ENCODED_WORD_END;
			pos += length;
		}
		return encodedText;
	}
}


const std::string MailMessage::HEADER_SUBJECT("Subject");
const std::string MailMessage::HEADER_FROM("From");
const std::string MailMessage::HEADER_TO("To");
const std::string MailMessage::HEADER_CC("CC");
const std::string MailMessage::HEADER_BCC("BCC");
const std::string MailMessage::HEADER_DATE("Date");
const std::string MailMessage::HEADER_CONTENT_TYPE("Content-Type");
const std::string MailMessage::HEADER_CONTENT_TRANSFER_ENCODING("Content-Transfer-Encoding");
const std::string MailMessage::HEADER_CONTENT_DISPOSITION("Content-Disposition");
const std::string MailMessage::HEADER_CONTENT_ID("Content-ID");
const std::string MailMessage::HEADER_MIME_VERSION("Mime-Version");
const std::string MailMessage::EMPTY_HEADER;
const std::string MailMessage::TEXT_PLAIN("text/plain");
const std::string MailMessage::CTE_7BIT("7bit");
const std::string MailMessage::CTE_8BIT("8bit");
const std::string MailMessage::CTE_QUOTED_PRINTABLE("quoted-printable");
const std::string MailMessage::CTE_BASE64("base64");


MailMessage::MailMessage(PartStoreFactory* pStoreFactory):
	_encoding(),
	_pStoreFactory(pStoreFactory)
{
	Poco::Timestamp now;
	setDate(now);
	setContentType("text/plain");
}


MailMessage::MailMessage(MailMessage&& message) noexcept:
	MessageHeader(std::move(message)),
	_recipients(std::exchange(message._recipients, {})),
	_parts(std::exchange(message._parts, {})),
	_content(std::exchange(message._content, {})),
	_encoding(message._encoding),
	_boundary(std::exchange(message._boundary, {})),
	_pStoreFactory(std::exchange(message._pStoreFactory, nullptr)),
	_isMultipart(std::exchange(message._isMultipart, false)),
	_recipientsChanged(std::exchange(message._recipientsChanged, false))
{
}


MailMessage& MailMessage::operator = (MailMessage&& message) noexcept
{
	if (&message != this)
	{
		MessageHeader::operator = (std::move(message));
		for (const auto& part: _parts)
		{
			delete part.pSource;
		}
		_recipients = std::exchange(message._recipients, {});
		_parts = std::exchange(message._parts, {});
		_content = std::exchange(message._content, {});
		_encoding = message._encoding;
		_boundary = std::exchange(message._boundary, {});
		_pStoreFactory = std::exchange(message._pStoreFactory, nullptr);
		_isMultipart = std::exchange(message._isMultipart, false);
		_recipientsChanged = std::exchange(message._recipientsChanged, false);
	}
	return *this;
}


MailMessage::~MailMessage()
{
	for (auto& part: _parts)
	{
		delete part.pSource;
	}
}


void MailMessage::addRecipient(const MailRecipient& recipient)
{
	_recipients.push_back(recipient);
	_recipientsChanged = true;
}


void MailMessage::setRecipients(const Recipients& recipients)
{
	_recipients.assign(recipients.begin(), recipients.end());
	_recipientsChanged = true;
}


void MailMessage::setSender(const std::string& sender)
{
	set(HEADER_FROM, sender);
}


const std::string& MailMessage::getSender() const
{
	if (has(HEADER_FROM))
		return get(HEADER_FROM);
	else
		return EMPTY_HEADER;
}


void MailMessage::setSubject(const std::string& subject)
{
	set(HEADER_SUBJECT, subject);
}


const std::string& MailMessage::getSubject() const
{
	if (has(HEADER_SUBJECT))
		return get(HEADER_SUBJECT);
	else
		return EMPTY_HEADER;
}


void MailMessage::setContent(const std::string& content, ContentTransferEncoding encoding)
{
	_content  = content;
	_encoding = encoding;
	set(HEADER_CONTENT_TRANSFER_ENCODING, contentTransferEncodingToString(encoding));
}


void MailMessage::setContentType(const std::string& mediaType)
{
	set(HEADER_CONTENT_TYPE, mediaType);
}


void MailMessage::setContentType(const MediaType& mediaType)
{
	setContentType(mediaType.toString());
}


const std::string& MailMessage::getContentType() const
{
	if (has(HEADER_CONTENT_TYPE))
		return get(HEADER_CONTENT_TYPE);
	else
		return TEXT_PLAIN;
}


void MailMessage::setDate(const Poco::Timestamp& dateTime)
{
	set(HEADER_DATE, DateTimeFormatter::format(dateTime, DateTimeFormat::RFC1123_FORMAT));
}


Poco::Timestamp MailMessage::getDate() const
{
	const std::string& dateTime = get(HEADER_DATE);
	int tzd;
	return DateTimeParser::parse(dateTime, tzd).timestamp();
}


bool MailMessage::isMultipart() const
{
	MediaType mediaType = getContentType();
	return mediaType.matches("multipart");
}


void MailMessage::addPart(const std::string& name, PartSource* pSource, ContentDisposition disposition, ContentTransferEncoding encoding)
{
	poco_check_ptr (pSource);

	makeMultipart();
	Part part;
	part.name        = name;
	part.pSource     = pSource;
	part.disposition = disposition;
	part.encoding    = encoding;
	_parts.push_back(part);
}


void MailMessage::addContent(PartSource* pSource, ContentTransferEncoding encoding)
{
	addPart("", pSource, CONTENT_INLINE, encoding);
}


void MailMessage::addAttachment(const std::string& name, PartSource* pSource, ContentTransferEncoding encoding)
{
	addPart(name, pSource, CONTENT_ATTACHMENT, encoding);
}


void MailMessage::read(std::istream& istr, PartHandler& handler)
{
	readHeader(istr);
	if (isMultipart())
	{
		readMultipart(istr, handler);
	}
	else
	{
		StringPartHandler stringHandler(_content);
		readPart(istr, *this, stringHandler);
	}
}


void MailMessage::read(std::istream& istr)
{
	readHeader(istr);
	if (isMultipart())
	{
		MultiPartHandler handler(this);
		readMultipart(istr, handler);
	}
	else
	{
		StringPartHandler handler(_content);
		readPart(istr, *this, handler);
	}
}


void MailMessage::write(std::ostream& ostr) const
{
	MessageHeader header(*this);
	// Every recipient receives the written header, so a Bcc field would disclose the blind copies.
	header.erase(HEADER_BCC);
	if (_recipientsChanged) setRecipientHeaders(header);
	// A control character must be rejected before encoding would hide it in an encoded word.
	checkFields(header, true);
	encodeFields(header);
	if (isMultipart())
	{
		writeMultipart(header, ostr);
	}
	else
	{
		writeHeader(header, ostr);
		std::istringstream istr(_content);
		writeEncoded(istr, ostr, _encoding);
	}
}


void MailMessage::validateHeaders() const
{
	MessageHeader header(*this);
	header.erase(HEADER_BCC);
	if (_recipientsChanged) setRecipientHeaders(header);
	checkFields(header, true);
	encodeFields(header);
	for (const auto& part: _parts)
	{
		checkFields(createPartHeader(part), true);
	}
}


void MailMessage::makeMultipart()
{
	if (_isMultipart) return;

	if (!isMultipart())
	{
		MediaType mediaType("multipart", "mixed");
		setContentType(mediaType);
	}
	_isMultipart = true;
}


void MailMessage::writeHeader(const MessageHeader& header, std::ostream& ostr) const
{
	header.write(ostr);
	ostr << "\r\n";
}


void MailMessage::writeMultipart(MessageHeader& header, std::ostream& ostr) const
{
	if (_boundary.empty()) _boundary = MultipartWriter::createBoundary();
	MediaType mediaType(getContentType());
	mediaType.setParameter("boundary", _boundary);
	header.set(HEADER_CONTENT_TYPE, mediaType.toString());
	header.set(HEADER_MIME_VERSION, "1.0");
	writeHeader(header, ostr);

	MultipartWriter writer(ostr, _boundary);
	for (const auto& part: _parts)
	{
		writePart(writer, part);
	}
	writer.close();
}


void MailMessage::writePart(MultipartWriter& writer, const Part& part)
{
	writer.nextPart(createPartHeader(part));
	writeEncoded(part.pSource->stream(), writer.stream(), part.encoding);
}


MessageHeader MailMessage::createPartHeader(const Part& part)
{
	MessageHeader partHeader(part.pSource->headers());
	MediaType mediaType(part.pSource->mediaType());
	if (!part.name.empty())
		mediaType.setParameter("name", part.name);
	partHeader.set(HEADER_CONTENT_TYPE, mediaType.toString());
	partHeader.set(HEADER_CONTENT_TRANSFER_ENCODING, contentTransferEncodingToString(part.encoding));
	std::string disposition;
	if (part.disposition == CONTENT_ATTACHMENT)
	{
		disposition = "attachment";
		const std::string& filename = part.pSource->filename();
		if (!filename.empty())
		{
			disposition.append("; filename=");
			quote(filename, disposition);
		}
	}
	else disposition = "inline";
	partHeader.set(HEADER_CONTENT_DISPOSITION, disposition);
	return partHeader;
}


void MailMessage::writeEncoded(std::istream& istr, std::ostream& ostr, ContentTransferEncoding encoding)
{
	switch (encoding)
	{
	case ENCODING_7BIT:
	case ENCODING_8BIT:
		StreamCopier::copyStream(istr, ostr);
		break;
	case ENCODING_QUOTED_PRINTABLE:
		{
			QuotedPrintableEncoder encoder(ostr);
			StreamCopier::copyStream(istr, encoder);
			encoder.close();
		}
		break;
	case ENCODING_BASE64:
		{
			Base64Encoder encoder(ostr);
			StreamCopier::copyStream(istr, encoder);
			encoder.close();
		}
		break;
	}
}


void MailMessage::readHeader(std::istream& istr)
{
	clear();
	_recipients.clear();
	MessageHeader::read(istr);
	istr.get(); // \r
	if ('\n' == istr.peek()) istr.get(); // \n

	// Decoding keeps the mailboxes of an address list, so the recipients are parsed from the stored values.
	for (const auto& [name, value]: *this)
	{
		if (icompare(name, HEADER_TO) == 0)
			HeaderField::parseAddressList(value, MailRecipient::PRIMARY_RECIPIENT, _recipients);
		else if (icompare(name, HEADER_CC) == 0)
			HeaderField::parseAddressList(value, MailRecipient::CC_RECIPIENT, _recipients);
		else if (icompare(name, HEADER_BCC) == 0)
			HeaderField::parseAddressList(value, MailRecipient::BCC_RECIPIENT, _recipients);
	}
	_recipientsChanged = false;
}


void MailMessage::readMultipart(std::istream& istr, PartHandler& handler)
{
	MediaType contentType(getContentType());
	_boundary = contentType.getParameter("boundary");
	if (_boundary.empty())
		throw MultipartException("Missing multipart boundary");

	MultipartReader reader(istr, _boundary);
	int partCount = 0;
	while (reader.hasNextPart())
	{
		if (++partCount > MAX_PARTS)
			throw MultipartException("Too many MIME parts");
		MessageHeader partHeader;
		reader.nextPart(partHeader);
		readPart(reader.stream(), partHeader, handler);
	}
}


void MailMessage::readPart(std::istream& istr, const MessageHeader& header, PartHandler& handler)
{
	std::string encoding;
	if (header.has(HEADER_CONTENT_TRANSFER_ENCODING))
	{
		encoding = header.get(HEADER_CONTENT_TRANSFER_ENCODING);
		// get rid of a parameter if one is set
		if (auto pos = encoding.find(';'); pos != std::string::npos)
			encoding.resize(pos);
	}
	if (icompare(encoding, CTE_QUOTED_PRINTABLE) == 0)
	{
		QuotedPrintableDecoder decoder(istr);
		handlePart(decoder, header, handler);
		_encoding = ENCODING_QUOTED_PRINTABLE;
	}
	else if (icompare(encoding, CTE_BASE64) == 0)
	{
		Base64Decoder decoder(istr);
		handlePart(decoder, header, handler);
		_encoding = ENCODING_BASE64;
	}
	else
	{
		if (icompare(encoding, CTE_7BIT) == 0)
			_encoding = ENCODING_7BIT;
		else if (icompare(encoding, CTE_8BIT) == 0)
			_encoding = ENCODING_8BIT;

		handlePart(istr, header, handler);
	}
}


void MailMessage::handlePart(std::istream& istr, const MessageHeader& header, PartHandler& handler)
{
	handler.handlePart(header, istr);
	// Read remaining characters from stream in case
	// the handler failed to read the complete stream.
	istr.ignore(std::numeric_limits<std::streamsize>::max());
}


void MailMessage::setRecipientHeaders(MessageHeader& headers) const
{
	std::string to;
	std::string cc;
	std::string bcc;

	for (const auto& rec: _recipients)
	{
		switch (rec.getType())
		{
		case MailRecipient::PRIMARY_RECIPIENT:
			// A control character must be rejected before encoding would hide it in an encoded word.
			checkFieldValue(HEADER_TO, rec.getRealName(), true);
			appendRecipient(rec, to);
			break;
		case MailRecipient::CC_RECIPIENT:
			checkFieldValue(HEADER_CC, rec.getRealName(), true);
			appendRecipient(rec, cc);
			break;
		case MailRecipient::BCC_RECIPIENT:
			break;
		}
	}
	// erase() removes every field of that name, also a second one from a read message.
	if (!to.empty())
	{
		headers.erase(HEADER_TO);
		headers.set(HEADER_TO, to);
	}
	if (!cc.empty())
	{
		headers.erase(HEADER_CC);
		headers.set(HEADER_CC, cc);
	}
}


void MailMessage::encodeFields(MessageHeader& header) const
{
	for (auto& [name, value]: header)
	{
		const HeaderField::Syntax syntax = HeaderField::syntax(name);
		if (syntax == HeaderField::Syntax::STRUCTURED || isReceived(name, value)
			|| std::all_of(value.begin(), value.end(), Poco::Ascii::isAscii) || !Poco::UTF8::isValid(value))
			continue;
		// Every CR LF in a checked value folds it; encoded, it would become text.
		const std::string text = Poco::replace(value, "\r\n", "");
		value = syntax == HeaderField::Syntax::ADDRESS_LIST ? HeaderField::encodeAddressList(text) : encodeWord(text, "UTF-8", WordEncoding::AUTO);
	}
}


const std::string& MailMessage::contentTransferEncodingToString(ContentTransferEncoding encoding)
{
	switch (encoding)
	{
	case ENCODING_7BIT:
		return CTE_7BIT;
	case ENCODING_8BIT:
		return CTE_8BIT;
	case ENCODING_QUOTED_PRINTABLE:
		return CTE_QUOTED_PRINTABLE;
	case ENCODING_BASE64:
		return CTE_BASE64;
	default:
		poco_bugcheck();
	}
	return CTE_7BIT;
}


int MailMessage::lineLength(const std::string& str)
{
	int n = 0;
	std::string::const_reverse_iterator it  = str.rbegin();
	std::string::const_reverse_iterator end = str.rend();
	while (it != end && *it != '\n') { ++n; ++it; }
	return n;
}


void MailMessage::appendRecipient(const MailRecipient& recipient, std::string& str)
{
	if (!str.empty()) str.append(", ");
	const std::string& realName = recipient.getRealName();
	const std::string& address  = recipient.getAddress();
	std::string rec;
	if (!std::all_of(realName.begin(), realName.end(), Poco::Ascii::isAscii) && Poco::UTF8::isValid(realName))
	{
		rec = encodeWord(Poco::replace(realName, "\r\n", ""), "UTF-8", WordEncoding::AUTO);
		rec.append(" ");
	}
	else if (!realName.empty())
	{
		quote(realName, rec, true);
		rec.append(" ");
	}
	rec.append("<");
	rec.append(address);
	rec.append(">");
	if (lineLength(str) + rec.length() > 70) str.append("\r\n\t");
	str.append(rec);
}


std::string MailMessage::encodeWord(const std::string& text, const std::string& charset, WordEncoding encoding)
{
	if (std::none_of(text.begin(), text.end(),
		[](char ch) { return static_cast<unsigned char>(ch) > 127; }))
		return text;

	if (encoding == WordEncoding::Q) return encodeQ(text, charset);
	if (encoding == WordEncoding::B) return encodeB(text, charset);
	std::string q = encodeQ(text, charset);
	std::string b = encodeB(text, charset);
	return b.size() < q.size() ? b : q;
}


PartSource* MailMessage::createPartStore(const std::string& content, const std::string& mediaType, const std::string& filename)
{
	if (!_pStoreFactory) return new StringPartSource(content, mediaType, filename);
	else return _pStoreFactory->createPartStore(content, mediaType, filename);
}


MultipartSource::MultipartSource(const std::string& contentType):
	PartSource(contentTypeWithBoundary(contentType))
{
}


MultipartSource::~MultipartSource()
{
	for (auto& part: _parts)
	{
		delete part.pSource;
	}
}


void MultipartSource::addPart(const std::string& name,
	PartSource* pSource,
	MailMessage::ContentDisposition disposition,
	MailMessage::ContentTransferEncoding encoding)
{
	MailMessage::Part part;
	part.name        = name;
	part.pSource     = pSource;
	part.disposition = disposition;
	part.encoding    = encoding;
	_parts.push_back(part);
}


std::istream& MultipartSource::stream()
{
	MediaType mt(mediaType());
	std::string boundary = mt.getParameter("boundary");

	MultipartWriter writer(_content, boundary);
	for (const auto& part: _parts)
	{
		MailMessage::writePart(writer, part);
	}
	writer.close();

	return _content;
}


std::string MultipartSource::contentTypeWithBoundary(const std::string& contentType)
{
	MediaType mediaType(contentType);
	mediaType.setParameter("boundary", MultipartWriter::createBoundary());
	return mediaType.toString();
}


} // namespace Poco::Net
