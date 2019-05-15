//
// NTLMCredentials.h
//
// Library: Net
// Package: NTLM
// Module:	NTLMCredentials
//
// Definition of the NTLMCredentials class.
//
// Copyright (c) 2019, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Net_NTLMCredentials_INCLUDED
#define Net_NTLMCredentials_INCLUDED


#include "Poco/Net/Net.h"
#include "Poco/BinaryReader.h"
#include "Poco/BinaryWriter.h"
#include <vector>


namespace Poco {
namespace Net {


class Net_API NTLMCredentials
	/// This is a utility class for working with
	/// NTLMv2 Authentication.
	///
	/// Note: This implementation is based on the
	/// "The NTLM Authentication Protocol and Security Support Provider"
	/// document written by Eric Glass and avilable from
	/// http://davenport.sourceforge.net/ntlm.html
	/// and the NT LAN Manager (NTLM) Authentication Protocol
	/// [MS-NLMP] document by Microsoft.
{
public:
	enum
	{
		NTLM_MESSAGE_TYPE_NEGOTIATE    = 0x01,
		NTLM_MESSAGE_TYPE_CHALLENGE    = 0x02,
		NTLM_MESSAGE_TYPE_AUTHENTICATE = 0x03
	};

	enum
	{
		NTLM_FLAG_NEGOTIATE_UNICODE     = 0x00000001,
		NTLM_FLAG_NEGOTIATE_OEM         = 0x00000002,
		NTLM_FLAG_REQUEST_TARGET        = 0x00000004,
		NTLM_FLAG_NEGOTIATE_NTLM        = 0x00000200,
		NTLM_FLAG_DOMAIN_SUPPLIED       = 0x00001000,
		NTLM_FLAG_WORKST_SUPPLIED       = 0x00002000,
		NTLM_FLAG_NEGOTIATE_LOCAL       = 0x00004000,
		NTLM_FLAG_NEGOTIATE_ALWAYS_SIGN = 0x00008000,
		NTLM_FLAG_NEGOTIATE_NTLM2_KEY   = 0x00080000,
		NTLM_FLAG_TARGET_DOMAIN         = 0x00010000,
		NTLM_FLAG_TARGET_SERVER         = 0x00020000,
		NTLM_FLAG_TARGET_SHARE          = 0x00040000,
		NTLM_FLAG_NEGOTIATE_TARGET      = 0x00800000,
		NTLM_FLAG_NEGOTIATE_128         = 0x20000000,
		NTLM_FLAG_NEGOTIATE_56          = 0x80000000
	};

	struct NegotiateMessage
		/// This message is sent from the client to initiate NTLM authentication.
	{
		NegotiateMessage(): flags(0) {}

		Poco::UInt32 flags;
		std::string domain;
		std::string workstation;
	};

	struct ChallengeMessage
		/// This message is sent back by the server and contains the NTLM challenge.
	{
		ChallengeMessage(): flags(0) {}

		Poco::UInt32 flags;
		std::vector<unsigned char> challenge;
		std::string target;
		std::vector<unsigned char> targetInfo;
	};

	struct AuthenticateMessage
		/// This message is sent from the client to authenticate itself by providing
		/// a response to the server challenge.
	{
		AuthenticateMessage(): flags(0) {}

		Poco::UInt32 flags;
		std::vector<unsigned char> lmResponse;
		std::vector<unsigned char> ntlmResponse;
		std::string target;
		std::string username;
		std::string workstation;
	};

	struct BufferDesc
	{
		BufferDesc():
			length(0),
			reserved(0),
			offset(0)
		{
		}

		BufferDesc(Poco::UInt16 len, Poco::UInt32 off):
			length(len),
			reserved(len),
			offset(off)
		{
		}

		Poco::UInt16 length;
		Poco::UInt16 reserved;
		Poco::UInt32 offset;
	};

	static std::vector<unsigned char> createNonce();
		/// Creates an 8-byte client nonce for NTLM authentication.

	static Poco::UInt64 createTimestamp();
		/// Creates the NTLM timestamp in tenths of a microsecond since January 1, 1601,
		/// using the current system time.

	static std::vector<unsigned char> createPasswordHash(const std::string& password);
		/// Creates the NTLM password hash (MD4 of UTF-16-converted password).

	static std::vector<unsigned char> createNTLMv2Hash(const std::string& username, const std::string& target, const std::string& password);
		/// Creates the NTLMv2 hash, which is the HMAC-MD5 of the concatenated UTF-16 uppercase username and target,
		/// using the password hash as HMAC passphrase.

	static std::vector<unsigned char> createLMv2Response(const std::vector<unsigned char>& ntlm2Hash, const std::vector<unsigned char>& challenge, const std::vector<unsigned char>& nonce);
		/// Creates the LMv2 response by computing the HMAC-MD5 of the challenge and nonce, using the
		/// ntlm2Hash (see createNTLMv2Hash()) as HMAC passphrase.

	static std::vector<unsigned char> createNTLMv2Response(const std::vector<unsigned char>& ntlm2Hash, const std::vector<unsigned char>& challenge, const std::vector<unsigned char>& nonce, const std::vector<unsigned char>& targetInfo, Poco::UInt64 timestamp);
		/// Creates the NTLMv2 response by creating the "blob" and prepending its HMAC-MD5, using the ntlm2Hash as HMAC passphrase.

	static std::vector<unsigned char> formatNegotiateMessage(const NegotiateMessage& message);
		/// Creates the NTLM Type 1 Negotiate message used for initiating NTLM authentication from the client.

	static bool parseChallengeMessage(const unsigned char* buffer, std::size_t size, ChallengeMessage& message);
		/// Parses a NTLM Type 2 Challenge message.
		///
		/// Returns true if the message was parsed successfully, otherwise false.

	static std::vector<unsigned char> formatAuthenticateMessage(const AuthenticateMessage& message);
		/// Creates the NTLM Type 3 Authenticate message used for sending the response to the challenge.

	static void readBufferDesc(Poco::BinaryReader& reader, BufferDesc& desc);
		/// Reads a buffer descriptor.

	static void writeBufferDesc(Poco::BinaryWriter& writer, const BufferDesc& desc);
		/// Writes a buffer descriptor.

	static void splitUsername(const std::string& usernameAndDomain, std::string& username, std::string& domain);
		/// Splits a username containing a domain into plain username and domain.
		/// Supported formats are <DOMAIN>\<username> and <username>@<DOMAIN>.

	static std::string toBase64(const std::vector<unsigned char>& buffer);
		/// Converts the buffer to a base64-encoded string.

	static std::vector<unsigned char> fromBase64(const std::string& base64);
		/// Decodes the given base64-encoded string.

	static const std::string NTLMSSP;
		/// Message signature string.
};


} } // namespace Poco::Net


#endif // Net_NTLMCredentials_INCLUDED
