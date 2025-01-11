//
// NTLMCredentials.cpp
//
// Library: Net
// Package: NTLM
// Module:	NTLMCredentials
//
// Copyright (c) 2019, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Net/NTLMCredentials.h"
#include "Poco/HMACEngine.h"
#include "Poco/MD4Engine.h"
#include "Poco/MD5Engine.h"
#include "Poco/DigestStream.h"
#include "Poco/StreamCopier.h"
#include "Poco/UTF8Encoding.h"
#include "Poco/UTF16Encoding.h"
#include "Poco/TextConverter.h"
#include "Poco/UTF8String.h"
#include "Poco/Random.h"
#include "Poco/Timestamp.h"
#include "Poco/MemoryStream.h"
#include "Poco/Base64Encoder.h"
#include "Poco/Base64Decoder.h"
#include <sstream>
#include <cstring>


namespace Poco {
namespace Net {


const std::string NTLMCredentials::NTLMSSP("NTLMSSP");


std::vector<unsigned char> NTLMCredentials::createNonce()
{
	Poco::MD5Engine md5;
	Poco::Random rnd;
	rnd.seed();

	Poco::UInt32 n = rnd.next();
	md5.update(&n, sizeof(n));

	Poco::Timestamp ts;
	md5.update(&ts, sizeof(ts));

	Poco::DigestEngine::Digest d = md5.digest();
	d.resize(8);

	return d;
}


Poco::UInt64 NTLMCredentials::createTimestamp()
{
	const Poco::UInt64 EPOCH_DELTA_SECONDS = 11644473600; // seconds between January 1, 1970 and January 1, 1601

	Poco::Timestamp now;
	Poco::UInt64 ts = now.epochMicroseconds();
	ts += EPOCH_DELTA_SECONDS*1000000; // since January 1, 1601
	ts *= 10; // tenths of a microsecond
	return ts;
}


std::vector<unsigned char> NTLMCredentials::createPasswordHash(const std::string& password)
{
	Poco::UTF8Encoding utf8;
	Poco::UTF16Encoding utf16(Poco::UTF16Encoding::LITTLE_ENDIAN_BYTE_ORDER);
	Poco::TextConverter converter(utf8, utf16);

	std::string utf16Password;
	converter.convert(password, utf16Password);

	Poco::MD4Engine md4;
	md4.update(utf16Password);
	return md4.digest();
}


std::vector<unsigned char> NTLMCredentials::createNTLMv2Hash(const std::string& username, const std::string& target, const std::string& password)
{
	Poco::UTF8Encoding utf8;
	Poco::UTF16Encoding utf16(Poco::UTF16Encoding::LITTLE_ENDIAN_BYTE_ORDER);
	Poco::TextConverter converter(utf8, utf16);

	std::vector<unsigned char> passwordHash = createPasswordHash(password);

	std::string userDomain = Poco::UTF8::toUpper(username);
	userDomain += target;

	std::string utf16UserDomain;
	converter.convert(userDomain, utf16UserDomain);

	std::string passwordHashString(reinterpret_cast<const char*>(&passwordHash[0]), passwordHash.size());
	Poco::HMACEngine<Poco::MD5Engine> hmac(passwordHashString);
	hmac.update(utf16UserDomain);
	return hmac.digest();
}


std::vector<unsigned char> NTLMCredentials::createLMv2Response(const std::vector<unsigned char>& ntlm2Hash, const std::vector<unsigned char>& challenge, const std::vector<unsigned char>& nonce)
{
	poco_assert (challenge.size() == 8);
	poco_assert (nonce.size() == 8);

	std::vector<unsigned char> lm2Response;

	std::string ntlm2HashString(reinterpret_cast<const char*>(&ntlm2Hash[0]), ntlm2Hash.size());
	Poco::HMACEngine<Poco::MD5Engine> hmac2(ntlm2HashString);
	hmac2.update(&challenge[0], challenge.size());
	hmac2.update(&nonce[0], nonce.size());
	lm2Response = hmac2.digest();
	lm2Response.insert(lm2Response.end(), nonce.begin(), nonce.end());

	return lm2Response;
}


std::vector<unsigned char> NTLMCredentials::createNTLMv2Response(const std::vector<unsigned char>& ntlm2Hash, const std::vector<unsigned char>& challenge, const std::vector<unsigned char>& nonce, const std::vector<unsigned char>& targetInfo, Poco::UInt64 timestamp)
{
	poco_assert (challenge.size() == 8);
	poco_assert (nonce.size() == 8);

	std::vector<unsigned char> blob;
	blob.resize(28 + targetInfo.size() + 4 + 16);

	Poco::MemoryOutputStream blobStream(reinterpret_cast<char*>(&blob[16]), blob.size() - 16);
	Poco::BinaryWriter writer(blobStream, Poco::BinaryWriter::LITTLE_ENDIAN_BYTE_ORDER);
	writer << Poco::UInt32(0x0101);
	writer << Poco::UInt32(0);
	writer << timestamp;
	writer.writeRaw(reinterpret_cast<const char*>(&nonce[0]), nonce.size());
	writer << Poco::UInt32(0);
	if (targetInfo.size() > 0)
	{
		writer.writeRaw(reinterpret_cast<const char*>(&targetInfo[0]), targetInfo.size());
	}
	writer << Poco::UInt32(0);

	poco_assert_dbg (blobStream.charsWritten() == blob.size() - 16);

	std::string ntlm2HashString(reinterpret_cast<const char*>(&ntlm2Hash[0]), ntlm2Hash.size());
	Poco::HMACEngine<Poco::MD5Engine> hmac2(ntlm2HashString);
	hmac2.update(&challenge[0], challenge.size());
	hmac2.update(&blob[16], blob.size() - 16);
	Poco::DigestEngine::Digest d = hmac2.digest();

	poco_assert_dbg (d.size() == 16);

	std::memcpy(&blob[0], &d[0], 16);

	return blob;
}


std::vector<unsigned char> NTLMCredentials::formatNegotiateMessage(const NegotiateMessage& message)
{
	Poco::UTF8Encoding utf8;
	Poco::UTF16Encoding utf16(Poco::UTF16Encoding::LITTLE_ENDIAN_BYTE_ORDER);
	Poco::TextConverter converter(utf8, utf16);

	std::string utf16Domain;
	converter.convert(message.domain, utf16Domain);

	std::string utf16Workstation;
	converter.convert(message.workstation, utf16Workstation);

	std::size_t size = 8 // signature
		+ 4  // type
		+ 4  // flags
		+ 8 + utf16Domain.size()
		+ 8 + utf16Workstation.size();

	Poco::UInt32 flags = message.flags | NTLM_FLAG_NEGOTIATE_UNICODE | NTLM_FLAG_REQUEST_TARGET | NTLM_FLAG_NEGOTIATE_NTLM | NTLM_FLAG_NEGOTIATE_NTLM2_KEY | NTLM_FLAG_NEGOTIATE_ALWAYS_SIGN;
	if (!utf16Domain.empty()) flags |= NTLM_FLAG_DOMAIN_SUPPLIED;
	if (!utf16Workstation.empty()) flags |= NTLM_FLAG_WORKST_SUPPLIED;

	BufferDesc domainDesc(static_cast<Poco::UInt16>(utf16Domain.size()), 8 + 4 + 4 + 8);
	BufferDesc workstDesc(static_cast<Poco::UInt16>(utf16Workstation.size()), domainDesc.offset + domainDesc.length);

	std::vector<unsigned char> buffer(size);
	Poco::MemoryOutputStream bufferStream(reinterpret_cast<char*>(&buffer[0]), buffer.size());
	Poco::BinaryWriter writer(bufferStream, Poco::BinaryWriter::LITTLE_ENDIAN_BYTE_ORDER);
	writer.writeRaw(NTLMSSP.c_str(), 8);
	writer << Poco::UInt32(NTLM_MESSAGE_TYPE_NEGOTIATE);
	writer << flags;
	writeBufferDesc(writer, domainDesc);
	writeBufferDesc(writer, workstDesc);
	writer.writeRaw(utf16Domain);
	writer.writeRaw(utf16Workstation);

	return buffer;
}


bool NTLMCredentials::parseChallengeMessage(const unsigned char* buffer, std::size_t size, ChallengeMessage& message)
{
	Poco::MemoryInputStream istr(reinterpret_cast<const char*>(buffer), size);
	Poco::BinaryReader reader(istr, Poco::BinaryReader::LITTLE_ENDIAN_BYTE_ORDER);

	std::string signature;
	reader.readRaw(7, signature);
	if (signature != NTLMSSP) return false;

	Poco::UInt8 zero;
	reader >> zero;
	if (zero != 0) return false;

	Poco::UInt32 type = 0;
	reader >> type;
	if (type != NTLM_MESSAGE_TYPE_CHALLENGE) return false;

	BufferDesc targetDesc;
	readBufferDesc(reader, targetDesc);
	if (targetDesc.offset + targetDesc.length > size) return false;

	reader >> message.flags;

	message.challenge.resize(8);
	reader.readRaw(reinterpret_cast<char*>(&message.challenge[0]), 8);

	if (message.flags & NTLM_FLAG_NEGOTIATE_TARGET)
	{
		Poco::UInt64 reserved;
		reader >> reserved;
	}

	BufferDesc targetInfoDesc;
	if (message.flags & NTLM_FLAG_NEGOTIATE_TARGET)
	{
		readBufferDesc(reader, targetInfoDesc);
		if (targetInfoDesc.offset + targetInfoDesc.length > size) return false;
	}

	if (targetDesc.length > 0)
	{
		if (message.flags & NTLM_FLAG_NEGOTIATE_UNICODE)
		{
			Poco::UTF16Encoding utf16(Poco::UTF16Encoding::LITTLE_ENDIAN_BYTE_ORDER);
			Poco::UTF8Encoding utf8;
			Poco::TextConverter converter(utf16, utf8);
			converter.convert(buffer + targetDesc.offset, targetDesc.length, message.target);
			if (targetDesc.reserved == 0) message.target.resize(std::strlen(message.target.c_str()));
		}
		else
		{
			message.target.assign(buffer + targetDesc.offset, buffer + targetDesc.offset + targetDesc.length);
		}
	}

	if (targetInfoDesc.length > 0)
	{
		message.targetInfo.assign(buffer + targetInfoDesc.offset, buffer + targetInfoDesc.offset + targetInfoDesc.length);
	}

	return true;
}


std::vector<unsigned char> NTLMCredentials::formatAuthenticateMessage(const AuthenticateMessage& message)
{
	Poco::UTF8Encoding utf8;
	Poco::UTF16Encoding utf16(Poco::UTF16Encoding::LITTLE_ENDIAN_BYTE_ORDER);
	Poco::TextConverter converter(utf8, utf16);

	std::string utf16Target;
	converter.convert(message.target, utf16Target);

	std::string utf16Username;
	converter.convert(message.username, utf16Username);

	std::string utf16Workstation;
	converter.convert(message.workstation, utf16Workstation);

	std::size_t size = 8 // signature
		+ 4  // type
		+ 8 + message.lmResponse.size()
		+ 8 + message.ntlmResponse.size()
		+ 8 + utf16Target.size()
		+ 8 + utf16Username.size()
		+ 8 + utf16Workstation.size()
		+ 8  // session key
		+ 4; // flags

	Poco::UInt32 flags = message.flags | NTLM_FLAG_NEGOTIATE_UNICODE;

	BufferDesc lmDesc(static_cast<Poco::UInt16>(message.lmResponse.size()), 64);
	BufferDesc ntlmDesc(static_cast<Poco::UInt16>(message.ntlmResponse.size()), lmDesc.offset + lmDesc.length);
	BufferDesc targetDesc(static_cast<Poco::UInt16>(utf16Target.size()), ntlmDesc.offset + ntlmDesc.length);
	BufferDesc usernameDesc(static_cast<Poco::UInt16>(utf16Username.size()), targetDesc.offset + targetDesc.length);
	BufferDesc workstDesc(static_cast<Poco::UInt16>(utf16Workstation.size()), usernameDesc.offset + usernameDesc.length);
	BufferDesc sessionKeyDesc(0, workstDesc.offset + workstDesc.length);

	std::vector<unsigned char> buffer(size);
	Poco::MemoryOutputStream bufferStream(reinterpret_cast<char*>(&buffer[0]), buffer.size());
	Poco::BinaryWriter writer(bufferStream, Poco::BinaryWriter::LITTLE_ENDIAN_BYTE_ORDER);
	writer.writeRaw(NTLMSSP.c_str(), 8);
	writer << Poco::UInt32(NTLM_MESSAGE_TYPE_AUTHENTICATE);
	writeBufferDesc(writer, lmDesc);
	writeBufferDesc(writer, ntlmDesc);
	writeBufferDesc(writer, targetDesc);
	writeBufferDesc(writer, usernameDesc);
	writeBufferDesc(writer, workstDesc);
	writeBufferDesc(writer, sessionKeyDesc);
	writer << flags;
	writer.writeRaw(reinterpret_cast<const char*>(&message.lmResponse[0]), message.lmResponse.size());
	writer.writeRaw(reinterpret_cast<const char*>(&message.ntlmResponse[0]), message.ntlmResponse.size());
	writer.writeRaw(utf16Target);
	writer.writeRaw(utf16Username);
	writer.writeRaw(utf16Workstation);

	return buffer;
}


void NTLMCredentials::readBufferDesc(Poco::BinaryReader& reader, BufferDesc& desc)
{
	reader >> desc.length >> desc.reserved >> desc.offset;
}


void NTLMCredentials::writeBufferDesc(Poco::BinaryWriter& writer, const BufferDesc& desc)
{
	writer << desc.length << desc.reserved << desc.offset;
}


void NTLMCredentials::splitUsername(const std::string& usernameAndDomain, std::string& username, std::string& domain)
{
	std::string::size_type pos = usernameAndDomain.find('\\');
	if (pos != std::string::npos)
	{
		domain.assign(usernameAndDomain, 0, pos);
		username.assign(usernameAndDomain, pos + 1, std::string::npos);
		return;
	}
	else
	{
		pos = usernameAndDomain.find('@');
		if (pos != std::string::npos)
		{
			username.assign(usernameAndDomain, 0, pos);
			domain.assign(usernameAndDomain, pos + 1, std::string::npos);
			return;
		}
	}
	username = usernameAndDomain;
}


std::string NTLMCredentials::toBase64(const std::vector<unsigned char>& buffer)
{
	std::ostringstream ostr;
	Poco::Base64Encoder base64(ostr);
	base64.rdbuf()->setLineLength(0);
	base64.write(reinterpret_cast<const char*>(&buffer[0]), buffer.size());
	base64.close();
	return ostr.str();
}


std::vector<unsigned char> NTLMCredentials::fromBase64(const std::string& base64)
{
	Poco::MemoryInputStream istr(base64.data(), base64.size());
	Poco::Base64Decoder debase64(istr);
	std::vector<unsigned char> buffer(base64.size());
	debase64.read(reinterpret_cast<char*>(&buffer[0]), buffer.size());
	buffer.resize(static_cast<std::size_t>(debase64.gcount()));
	return buffer;
}


} } // namespace Poco::Net
