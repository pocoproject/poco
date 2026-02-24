//
// Database.cpp
//
// Library: MongoDB
// Package: MongoDB
// Module:  Database
//
// Copyright (c) 2012-2025, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/MongoDB/Database.h"
#include "Poco/MongoDB/Array.h"
#include "Poco/MongoDB/Binary.h"
#include "Poco/Base64Decoder.h"
#include "Poco/Base64Encoder.h"
#include "Poco/DigestEngine.h"
#include "Poco/Exception.h"
#include "Poco/Format.h"
#include "Poco/HMACEngine.h"
#include "Poco/MD5Engine.h"
#include "Poco/MemoryStream.h"
#include "Poco/NumberParser.h"
#include "Poco/PBKDF2Engine.h"
#include "Poco/Random.h"
#include "Poco/RandomStream.h"
#include "Poco/SHA1Engine.h"
#include "Poco/StreamCopier.h"
#include <map>
#include <sstream>
#include <utility>

using namespace std::string_literals;


namespace Poco {
namespace MongoDB {


const std::string Database::AUTH_SCRAM_SHA1("SCRAM-SHA-1");


namespace
{
	std::map<std::string, std::string> parseKeyValueList(const std::string& str)
	{
		std::map<std::string, std::string> kvm;
		std::string::size_type pos = 0;
		while (pos < str.size())
		{
			// Find key-value separators using find() instead of character-by-character iteration
			std::string::size_type eqPos = str.find('=', pos);
			if (eqPos == std::string::npos)
				break;

			std::string::size_type commaPos = str.find(',', eqPos);
			if (commaPos == std::string::npos)
				commaPos = str.length();

			// Extract key and value using substr (single allocation each)
			std::string key = str.substr(pos, eqPos - pos);
			std::string value = str.substr(eqPos + 1, commaPos - eqPos - 1);

			kvm[std::move(key)] = std::move(value);
			pos = commaPos + 1;
		}
		return kvm;
	}

	std::string decodeBase64(const std::string& base64)
	{
		Poco::MemoryInputStream istr(base64.data(), base64.size());
		Poco::Base64Decoder decoder(istr);
		std::string result;
		Poco::StreamCopier::copyToString(decoder, result);
		return result;
	}

	std::string encodeBase64(const std::string& data)
	{
		std::ostringstream ostr;
		Poco::Base64Encoder encoder(ostr);
		encoder.rdbuf()->setLineLength(0);
		encoder << data;
		encoder.close();
		return ostr.str();
	}

	std::string digestToBinaryString(Poco::DigestEngine& engine)
	{
		Poco::DigestEngine::Digest d = engine.digest();
		return { reinterpret_cast<const char*>(&d[0]), d.size() };
	}

	std::string digestToHexString(Poco::DigestEngine& engine)
	{
		Poco::DigestEngine::Digest d = engine.digest();
		return Poco::DigestEngine::digestToHex(d);
	}

	std::string digestToBase64(Poco::DigestEngine& engine)
	{
		return encodeBase64(digestToBinaryString(engine));
	}

	std::string hashCredentials(const std::string& username, const std::string& password)
	{
		Poco::MD5Engine md5;
		md5.update(username);
		md5.update(std::string(":mongo:"));
		md5.update(password);
		return digestToHexString(md5);
	}

	std::string createNonce()
	{
		Poco::MD5Engine md5;
		Poco::RandomInputStream randomStream;
		Poco::Random random;
		for (int i = 0; i < 4; i++)
		{
			md5.update(randomStream.get());
			md5.update(random.nextChar());
		}
		return digestToHexString(md5);
	}
} // namespace


Database::Database(const std::string& db):
	_dbname(db)
{
}


Database::~Database()
{
}


bool Database::authenticate(Connection& connection, const std::string& username, const std::string& password, const std::string& method)
{
	if (username.empty()) throw Poco::InvalidArgumentException("empty username");
	if (password.empty()) throw Poco::InvalidArgumentException("empty password");

	if (method == AUTH_SCRAM_SHA1)
		return authSCRAM(connection, username, password);
	else
		throw Poco::InvalidArgumentException("authentication method", method);
}


bool Database::authSCRAM(Connection& connection, const std::string& username, const std::string& password)
{
	std::string clientNonce(createNonce());
	std::string clientFirstMsg = Poco::format("n=%s,r=%s", username, clientNonce);

	Poco::SharedPtr<OpMsgMessage> pCommand = createOpMsgMessage("$cmd");
	pCommand->setCommandName("saslStart");
	pCommand->body()
		.add<std::string>("mechanism", AUTH_SCRAM_SHA1)
		.add<Binary::Ptr>("payload", new Binary(Poco::format("n,,%s", clientFirstMsg)))
		.add<bool>("autoAuthorize", true);

	OpMsgMessage response;
	connection.sendRequest(*pCommand, response);

	Int32 conversationId = 0;
	std::string serverFirstMsg;

	if (!response.responseOk())
	{
		return false;
	}
	{
		Document::Ptr pDoc = new Document(response.body());
		Binary::Ptr pPayload = pDoc->get<Binary::Ptr>("payload");
		serverFirstMsg = pPayload->toRawString();
		conversationId = pDoc->get<Int32>("conversationId");
	}

	std::map<std::string, std::string> kvm = parseKeyValueList(serverFirstMsg);
	const std::string serverNonce = kvm["r"];
	const std::string salt = decodeBase64(kvm["s"]);
	const unsigned iterations = Poco::NumberParser::parseUnsigned(kvm["i"]);
	const Poco::UInt32 dkLen = 20;

	std::string hashedPassword = hashCredentials(username, password);

	Poco::PBKDF2Engine<Poco::HMACEngine<Poco::SHA1Engine> > pbkdf2(salt, iterations, dkLen);
	pbkdf2.update(hashedPassword);
	std::string saltedPassword = digestToBinaryString(pbkdf2);

	std::string clientFinalNoProof = Poco::format("c=biws,r=%s", serverNonce);
	std::string authMessage = Poco::format("%s,%s,%s", clientFirstMsg, serverFirstMsg, clientFinalNoProof);

	Poco::HMACEngine<Poco::SHA1Engine> hmacKey(saltedPassword);
	hmacKey.update(std::string("Client Key"));
	std::string clientKey = digestToBinaryString(hmacKey);

	Poco::SHA1Engine sha1;
	sha1.update(clientKey);
	std::string storedKey = digestToBinaryString(sha1);

	Poco::HMACEngine<Poco::SHA1Engine> hmacSig(storedKey);
	hmacSig.update(authMessage);
	std::string clientSignature = digestToBinaryString(hmacSig);

	std::string clientProof(clientKey);
	for (std::size_t i = 0; i < clientProof.size(); i++)
	{
		clientProof[i] ^= clientSignature[i];
	}

	std::string clientFinal = Poco::format("%s,p=%s", clientFinalNoProof, encodeBase64(clientProof));

	pCommand = createOpMsgMessage("$cmd");
	pCommand->setCommandName("saslContinue");
	pCommand->body()
		.add<Poco::Int32>("conversationId", conversationId)
		.add<Binary::Ptr>("payload", new Binary(clientFinal));

	std::string serverSecondMsg;
	connection.sendRequest(*pCommand, response);

	if (!response.responseOk())
	{
		return false;
	}
	{
		Document::Ptr pDoc = new Document(response.body());
		Binary::Ptr pPayload = pDoc->get<Binary::Ptr>("payload");
		serverSecondMsg = pPayload->toRawString();
	}

	Poco::HMACEngine<Poco::SHA1Engine> hmacSKey(saltedPassword);
	hmacSKey.update(std::string("Server Key"));
	std::string serverKey = digestToBinaryString(hmacSKey);

	Poco::HMACEngine<Poco::SHA1Engine> hmacSSig(serverKey);
	hmacSSig.update(authMessage);
	std::string serverSignature = digestToBase64(hmacSSig);

	kvm = parseKeyValueList(serverSecondMsg);
	std::string serverSignatureReceived = kvm["v"];

	if (serverSignature != serverSignatureReceived)
		throw Poco::ProtocolException("server signature verification failed");

	pCommand = createOpMsgMessage("$cmd");
	pCommand->setCommandName("saslContinue");
	pCommand->body()
		.add<Poco::Int32>("conversationId", conversationId)
		.add<Binary::Ptr>("payload", new Binary);

	connection.sendRequest(*pCommand, response);
	return response.responseOk();
}


Document::Ptr Database::queryBuildInfo(Connection& connection) const
{
	Poco::SharedPtr<Poco::MongoDB::OpMsgMessage> request = createOpMsgMessage();
	request->setCommandName(OpMsgMessage::CMD_BUILD_INFO);

	Poco::MongoDB::OpMsgMessage response;
	connection.sendRequest(*request, response);

	Document::Ptr buildInfo;
	if (response.responseOk())
	{
		buildInfo = new Document(response.body());
	}
	else
	{
		throw Poco::ProtocolException("Didn't get a response from the buildInfo command");
	}
	return buildInfo;
}


Document::Ptr Database::queryServerHello(Connection& connection) const
{
	Poco::SharedPtr<Poco::MongoDB::OpMsgMessage> request = createOpMsgMessage();
	request->setCommandName(OpMsgMessage::CMD_HELLO);

	Poco::MongoDB::OpMsgMessage response;
	connection.sendRequest(*request, response);

	Document::Ptr hello;
	if (response.responseOk())
	{
		hello = new Document(response.body());
	}
	else
	{
		throw Poco::ProtocolException("Didn't get a response from the hello command");
	}
	return hello;
}


Int64 Database::count(Connection& connection, const std::string& collectionName) const
{
	Poco::SharedPtr<Poco::MongoDB::OpMsgMessage> request = createOpMsgMessage(collectionName);
	request->setCommandName(OpMsgMessage::CMD_COUNT);

	Poco::MongoDB::OpMsgMessage response;
	connection.sendRequest(*request, response);

	if (response.responseOk())
	{
		const auto& body = response.body();
		return body.getInteger("n");
	}

	return -1;
}


Poco::MongoDB::Document::Ptr Database::createIndex(
	Connection& connection,
	const std::string& collection,
	const IndexedFields& indexedFields,
	const std::string &indexName,
	unsigned long options,
	int expirationSeconds,
	int version)
{
// https://www.mongodb.com/docs/manual/reference/command/createIndexes/

	MongoDB::Document::Ptr keys = new MongoDB::Document();

	for (const auto& [name, ascending]: indexedFields) {
		keys->add(name, ascending ? 1 : -1);
	}

	MongoDB::Document::Ptr index = new MongoDB::Document();
	if (!indexName.empty())
	{
		index->add("name"s, indexName);
	}
	index->add("key"s, keys);
	index->add("ns"s, _dbname + '.' + collection);
	index->add("name"s, indexName);

	if (options & INDEX_UNIQUE) {
		index->add("unique"s, true);
	}
	if (options & INDEX_BACKGROUND) {
		index->add("background"s, true);
	}
	if (options & INDEX_SPARSE) {
		index->add("sparse"s, true);
	}
	if (expirationSeconds > 0) {
		index->add("expireAfterSeconds"s, static_cast<Poco::Int32>(expirationSeconds));
	}
	if (version > 0) {
		index->add("version"s, static_cast<Poco::Int32>(version));
	}

	MongoDB::Array::Ptr indexes = new MongoDB::Array();
	indexes->add(index);

	auto request = createOpMsgMessage(collection);
	request->setCommandName(OpMsgMessage::CMD_CREATE_INDEXES);
	request->body().add("indexes", indexes);

	OpMsgMessage response;
	connection.sendRequest(*request, response);

	MongoDB::Document::Ptr result = new MongoDB::Document(response.body());

	return result;
}


} } // namespace Poco::MongoDB
