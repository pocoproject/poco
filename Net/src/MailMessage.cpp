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
#include "Poco/Net/MultipartWriter.h"
#include "Poco/Net/PartSource.h"
#include "Poco/Net/PartHandler.h"
#include "Poco/Net/StringPartSource.h"
#include "Poco/Net/QuotedPrintableEncoder.h"
#include "Poco/Net/QuotedPrintableDecoder.h"
#include "Poco/Net/NameValueCollection.h"
#include "Poco/Base64Encoder.h"
#include "Poco/Base64Decoder.h"
#include "Poco/StreamCopier.h"
#include "Poco/DateTimeFormat.h"
#include "Poco/DateTimeFormatter.h"
#include "Poco/DateTimeParser.h"
#include "Poco/String.h"
#include "Poco/Format.h"
#include "Poco/StringTokenizer.h"
#include "Poco/StreamCopier.h"
#include "Poco/NumberFormatter.h"
#include "Poco/TextEncoding.h"
#include "Poco/TextConverter.h"
#include "Poco/NumberParser.h"
#include "Poco/Ascii.h"
#include <sstream>


using Poco::Base64Encoder;
using Poco::Base64Decoder;
using Poco::StreamCopier;
using Poco::DateTimeFormat;
using Poco::DateTimeFormatter;
using Poco::DateTimeParser;
using Poco::StringTokenizer;
using Poco::TextEncoding;
using Poco::TextEncodingRegistry;
using Poco::TextConverter;
using Poco::NumberParser;
using Poco::Ascii;
using Poco::icompare;


namespace Poco {
namespace Net {


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

		~MultiPartHandler()
			/// Destroys string part handler.
		{
		}

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
					std::string enc = header[MailMessage::HEADER_CONTENT_TRANSFER_ENCODING];
					if (enc == MailMessage::CTE_8BIT)
						cte = MailMessage::ENCODING_8BIT;
					else if (enc == MailMessage::CTE_QUOTED_PRINTABLE)
						cte = MailMessage::ENCODING_QUOTED_PRINTABLE;
					else if (enc == MailMessage::CTE_BASE64)
						cte = MailMessage::ENCODING_BASE64;
				}

				std::string contentType = header.get(MailMessage::HEADER_CONTENT_TYPE, "text/plain; charset=us-ascii");
				std::string contentDisp = header.get(MailMessage::HEADER_CONTENT_DISPOSITION, "");
				std::string filename;
				if (!contentDisp.empty())
					filename = getParamFromHeader(contentDisp, "filename");
				if (filename.empty())
					filename = getParamFromHeader(contentType, "name");
				Poco::SharedPtr<PartSource> pPS = _pMsg->createPartStore(tmp, contentType, filename);
				poco_check_ptr (pPS);
				NameValueCollection::ConstIterator it = header.begin();
				NameValueCollection::ConstIterator end = header.end();
				bool added = false;
				for (; it != end; ++it)
				{
					if (!added && MailMessage::HEADER_CONTENT_DISPOSITION == it->first)
					{
						if (it->second == "inline")
							_pMsg->addContent(pPS, cte);
						else
							_pMsg->addAttachment(getPartName(header), pPS, cte);
						added = true;
					}

					pPS->headers().set(it->first, it->second);
				}

				if (contentDisp.empty())
				{
					_pMsg->addContent(pPS, cte);
				}
			}
		}

	private:
		std::string getParamFromHeader(const std::string& header, const std::string& param)
		{
			StringTokenizer st(header, ";=", StringTokenizer::TOK_IGNORE_EMPTY | StringTokenizer::TOK_TRIM);
			StringTokenizer::Iterator it = st.begin();
			StringTokenizer::Iterator end = st.end();
			for (; it != end; ++it) { if (icompare(*it, param) == 0) break; }
			if (it != end)
			{
				++it;
				if (it == end) return "";
				return *it;
			}
			return "";
		}

		std::string getPartName(const MessageHeader& header)
		{
			std::string ct = MailMessage::HEADER_CONTENT_TYPE;
			if (header.has(ct))
				return getParamFromHeader(header[ct], "name");
			return "";
		}

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

		~StringPartHandler()
			/// Destroys string part handler.
		{
		}

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


MailMessage::MailMessage(MailMessage&& other):
	_recipients(std::move(other._recipients)),
	_content(std::move(other._content)),
	_encoding(other._encoding),
	_boundary(std::move(other._boundary)),
	_pStoreFactory(other._pStoreFactory)
{
	other._recipients.clear();
	other._content.clear();
	other._boundary.clear();
	other._pStoreFactory = 0;
}


MailMessage& MailMessage::operator = (MailMessage&& other)
{
	if (&other != this)
	{
		_recipients = std::move(other._recipients);
		other._recipients.clear();
		_content = std::move(other._content);
		other._content.clear();
		_encoding = other._encoding;
		_boundary = std::move(other._boundary);
		other._boundary.clear();
		_pStoreFactory = other._pStoreFactory;
		other._pStoreFactory = 0;
	}
	return *this;
}


MailMessage::~MailMessage()
{
}


void MailMessage::addRecipient(const MailRecipient& recipient)
{
	_recipients.push_back(recipient);
}


void MailMessage::setRecipients(const Recipients& recipients)
{
	_recipients.assign(recipients.begin(), recipients.end());
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


void MailMessage::addPart(const std::string& name, const Poco::SharedPtr<PartSource>& pSource, ContentDisposition disposition, ContentTransferEncoding encoding)
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


void MailMessage::addContent(const Poco::SharedPtr<PartSource>& pSource, ContentTransferEncoding encoding)
{
	addPart("", pSource, CONTENT_INLINE, encoding);
}


void MailMessage::addAttachment(const std::string& name, const Poco::SharedPtr<PartSource>& pSource, ContentTransferEncoding encoding)
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
		StringPartHandler handler(_content);
		readPart(istr, *this, handler);
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
	setRecipientHeaders(header);
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


void MailMessage::makeMultipart()
{
	if (!isMultipart())
	{
		MediaType mediaType("multipart", "mixed");
		setContentType(mediaType);
	}
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
	for (PartVec::const_iterator it = _parts.begin(); it != _parts.end(); ++it)
	{
		writePart(writer, *it);
	}
	writer.close();
}


void MailMessage::writePart(MultipartWriter& writer, const Part& part)
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
	writer.nextPart(partHeader);
	writeEncoded(part.pSource->stream(), writer.stream(), part.encoding);
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
	MessageHeader::RecipientList recipients;
	MessageHeader::read(istr, &recipients);
	istr.get(); // \r
	if ('\n' == istr.peek()) istr.get(); // \n
	for (const auto& r : recipients) addRecipient(r);
}


void MailMessage::readMultipart(std::istream& istr, PartHandler& handler)
{
	MediaType contentType(getContentType());
	_boundary = contentType.getParameter("boundary");
	MultipartReader reader(istr, _boundary);
	while (reader.hasNextPart())
	{
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
		std::string::size_type pos = encoding.find(';');
		if (pos != std::string::npos)
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
	while (istr.good()) istr.get();
}


void MailMessage::setRecipientHeaders(MessageHeader& headers) const
{
	std::string to;
	std::string cc;
	std::string bcc;

	for (Recipients::const_iterator it = _recipients.begin(); it != _recipients.end(); ++it)
	{
		switch (it->getType())
		{
		case MailRecipient::PRIMARY_RECIPIENT:
			appendRecipient(*it, to);
			break;
		case MailRecipient::CC_RECIPIENT:
			appendRecipient(*it, cc);
			break;
		case MailRecipient::BCC_RECIPIENT:
			break;
		}
	}
	if (!to.empty()) headers.set(HEADER_TO, to);
	if (!cc.empty()) headers.set(HEADER_CC, cc);
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
	if (!realName.empty())
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


void encodeQ(std::string& encodedText, std::string::const_iterator it, std::string::size_type& lineLength)
{
	switch (*it)
	{
	case ' ':
		encodedText += '_';
		lineLength++;
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
		encodedText += '=';
		NumberFormatter::appendHex(encodedText, static_cast<unsigned>(static_cast<unsigned char>(*it)), 2);
		lineLength += 3;
		break;
	default:
		if (*it > 32 && *it < 127)
		{
			encodedText += *it;
			lineLength++;
		}
		else
		{
			encodedText += '=';
			NumberFormatter::appendHex(encodedText, static_cast<unsigned>(static_cast<unsigned char>(*it)), 2);
			lineLength += 3;
		}
	}
}


void startEncoding(std::string& encodedText, const std::string& charset, char encoding)
{
	encodedText += "=?";
	encodedText += charset;
	encodedText += '?';
	encodedText += encoding;
	encodedText += '?';
}


std::string MailMessage::encodeWord(const std::string& text, const std::string& charset, char encoding)
{
	if (encoding == 'q' || encoding == 'Q')
	{
		bool containsNonASCII = false;
		for (std::string::const_iterator it = text.begin(); it != text.end(); ++it)
		{
			if (static_cast<unsigned char>(*it) > 127)
			{
				containsNonASCII = true;
				break;
			}
		}
		if (!containsNonASCII) return text;
	}

	std::string encodedText;
	std::string::size_type lineLength = 0;
	if (encoding == 'q' || encoding == 'Q')
	{
		for (std::string::const_iterator it = text.begin(); it != text.end(); ++it)
		{
			if (lineLength == 0)
			{
				startEncoding(encodedText, charset, encoding);
				lineLength += charset.length() + 5;
			}
			encodeQ(encodedText, it, lineLength);
			if ((lineLength >= 64 &&
				(*it == ' ' || *it == '\t' || *it == '\r' || *it == '\n')) ||
				lineLength >= 72)
			{
				encodedText += "?=\r\n ";
				lineLength = 0;
			}
		}
	}
	else if (encoding == 'b' || encoding == 'B')
	{
		// to ensure we're under 75 chars, 4 padding chars are always predicted
		lineLength = 75 - (charset.length() + 5/*=??B?*/ + 2/*?=*/ + 4/*base64 padding*/);
		std::string::size_type pos = 0;
		size_t textLen = static_cast<size_t>(floor(lineLength * 3 / 4));
		std::ostringstream ostr;
		while (true)
		{
			Base64Encoder encoder(ostr);
			encoder.rdbuf()->setLineLength(static_cast<int>(lineLength));
			startEncoding(encodedText, charset, encoding);
			std::string line = text.substr(pos, textLen);
			encoder << line;
			encoder.close();
			encodedText.append(ostr.str());
			encodedText.append("?=");
			if (line.size() < textLen) break;
			ostr.str("");
			pos += textLen;
			encodedText.append("\r\n");
		}
		lineLength = 0;;
	}
	else
	{
		throw InvalidArgumentException(Poco::format("MailMessage::encodeWord: "
			"unknown encoding: %c", encoding));
	}

	if (lineLength > 0) encodedText += "?=";

	return encodedText;
}


void MailMessage::advanceToEncoded(const std::string& encoded, std::string& decoded, std::string::size_type& pos1, bool& isComment)
{
	bool spaceOnly = isComment; // flag to trim away spaces between encoded-word's
	auto it = encoded.begin();
	auto end = encoded.end();
	for (; it != end; ++it)
	{
		if (*it == '=')
		{
			if (++it != end && *it == '?')
			{
				if (spaceOnly) trimRightInPlace(decoded);
				return;
			}
		}
		else if (*it == '(') isComment = true;
		else if (*it == ')') isComment = false;
		if ((isComment) && (!Ascii::isSpace(*it))) spaceOnly = false;
		decoded.append(1, *it);
		++pos1;
	}
	pos1 = std::string::npos;
}


std::string MailMessage::decodeWord(const std::string& encoded, std::string toCharset)
{
	std::string encodedWord = replace(encoded, "?=\r\n=?", "?==?");
	bool toCharsetGiven = !toCharset.empty();
	std::string errMsg;
	const std::size_t notFound = std::string::npos;
	std::string decoded;
	std::string::size_type pos1 = 0, pos2 = 0;
	bool isComment = false;
	advanceToEncoded(encodedWord, decoded, pos1, isComment);
	if (pos1 != notFound)
	{
		getEncWordLimits(encodedWord, pos1, pos2, isComment);
		while ((pos1 != notFound) && (pos2 != notFound) && pos2 > pos1 + 2)
		{
			pos1 += 2;
			StringTokenizer st(encodedWord.substr(pos1, pos2 - pos1), "?");
			if (st.count() == 3)
			{
				std::string charset = st[0];
				if (!toCharsetGiven) toCharset = charset;
				if (st[1].size() > 1)
				{
					throw InvalidArgumentException(Poco::format("MailMessage::decodeWord: "
						"invalid encoding %s", st[1]));
				}
				char encoding = st[1][0];
				std::string encodedText = st[2];
				if (encodedText.find_first_of(" ?") != notFound)
				{
					throw InvalidArgumentException("MailMessage::decodeWord: "
						"forbidden characters found in encoded-word");
				}
				else if (encoding == 'q' || encoding == 'Q')
				{
					// no incomplete encoded characters allowed on single line
					std::string::size_type eqPos = encodedText.rfind('=');
					if (eqPos != notFound)
					{
						if ((eqPos + 2) >= encodedText.size())
						{
							throw InvalidArgumentException("MailMessage::decodeWord: "
								"incomplete encoded character found in encoded-word");
						}
					}
				}
				decoded.append(decodeWord(charset, encoding, encodedText, toCharset));
				pos1 = pos2 + 2;
				advanceToEncoded(encodedWord.substr(pos1), decoded, pos1, isComment);
				if (pos1 != notFound) getEncWordLimits(encodedWord, pos1, pos2, isComment);
			}
			else
			{
				throw InvalidArgumentException(Poco::format("MailMessage::decodeWord: "
					"invalid number of entries in encoded-word (expected 3, found %z)", st.count()));
			}
		}
	}
	else decoded = std::move(encodedWord);
	return decoded;
}


void MailMessage::getEncWordLimits(const std::string& encodedWord, std::string::size_type& pos1, std::string::size_type& pos2, bool isComment)
{
	const std::size_t notFound = std::string::npos;

	pos1 = encodedWord.find("=?", pos1); // beginning of encoded-word
	if (pos1 != notFound)
	{
		// must look sequentially for all '?' occurences because of a (valid) case like this:
		//   =?ISO-8859-1?q?=C4?=
		// where end would be prematurely found if we search for ?= only
		pos2 = encodedWord.find('?', pos1 + 2); // first '?'
		if (pos2 == notFound)  goto err;
		pos2 = encodedWord.find('?', pos2 + 1); // second '?'
		if (pos2 == notFound) goto err;
		pos2 = encodedWord.find("?=", pos2 + 1); // end of encoded-word
		if (pos2 == notFound) goto err;
		// before we leave, double-check for the next encoded-word end, to make sure
		// an illegal '?' was not sneaked in (eg. =?ISO-8859-1?q?=C4?=D6?=)
		if (((encodedWord.find("?=", pos2 + 1) != notFound &&
			encodedWord.find("=?", pos2 + 1) == notFound)) ||
			((encodedWord.find("=?", pos2 + 1) != notFound &&
			encodedWord.find("?=", pos2 + 1) == notFound))) goto err;
	}
	else goto err;

	// if encoded word is in a comment, then '(' and ')' are forbidden inside it
	if (isComment &&
		(notFound != encodedWord.substr(pos1, pos2 - pos1).find_first_of("()"))) goto err;

	return;

err:
	throw InvalidArgumentException("MailMessage::encodedWordLimits: invalid encoded word");
}


std::string MailMessage::decodeWord(const std::string& charset, char encoding,
	const std::string& text, const std::string& toCharset)
{
	const TextEncodingRegistry& registry = TextEncoding::registry();
	if (!registry.has(charset) || !registry.has(toCharset))
	{
		throw NotImplementedException(Poco::format("MailMessage::decodeWord: "
			"charset not supported: %s", charset));
	}
	TextEncoding* fromEnc = registry.find(charset);
	TextEncoding* toEnc = registry.find(toCharset);

	std::string decoded;
	switch (encoding)
	{
	case 'B': case 'b':
	{
		std::istringstream istr(text);
		Base64Decoder decoder(istr);
		int c = decoder.get();
		while (c != -1) { decoded.append(1, char(c)); c = decoder.get(); }
		break;
	}
	case 'Q': case 'q':
	{
		bool isWide = false;
		std::vector<char> wideChar;
		std::vector<unsigned char> wideCharSeq;
		for (const auto& c : text)
		{
			if (!Ascii::isPrintable(c) || c == '?' || c == ' ')
			{
				throw InvalidArgumentException("MailMessage::decodeWord: encoded-word must not contain "
					"non-printable characters, '? or SPACE");
			}
			if      (c == '_') decoded.append(1, ' ');
			else if (c == '=') isWide = true;
			else if (isWide)
			{
				wideChar.push_back(c);
				if (wideChar.size() % 2 == 0)
				{
					std::string wcStr(&wideChar[0], wideChar.size());
					unsigned char chr = NumberParser::parseHex(wcStr);
					wideCharSeq.push_back(chr);
					if (fromEnc->sequenceLength(&wideCharSeq[0], static_cast<int>(wideCharSeq.size())) > 0)
					{
						auto it = wideCharSeq.begin();
						auto end = wideCharSeq.end();
						for (; it != end; ++it)
						{
							decoded.append(1, static_cast<char>(*it));
						}
						wideChar.clear();
						wideCharSeq.clear();
						isWide = false;
					}
				}
			}
			else decoded.append(1, c);
		}
		break;
	}
	default:
		throw InvalidArgumentException(Poco::format("MailMessage::decodeWord: Unknown encoding: %c", encoding));
	}
	if (charset != toCharset)
	{
		TextConverter converter(*fromEnc, *toEnc);
		std::string converted;
		converter.convert(decoded, converted);
		return converted;
	}
	return decoded;
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
}


void MultipartSource::addPart(const std::string& name,
	const Poco::SharedPtr<PartSource>& pSource,
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
	for (MailMessage::PartVec::const_iterator it = _parts.begin(); it != _parts.end(); ++it)
	{
		MailMessage::writePart(writer, *it);
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


} } // namespace Poco::Net
