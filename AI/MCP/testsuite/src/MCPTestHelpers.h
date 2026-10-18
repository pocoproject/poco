//
// MCPTestHelpers.h
//
// Helpers shared by the AI/MCP tests: requests that carry the _meta object
// of protocol revision 2026-07-28, and accessors of replies.
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef MCPTestHelpers_INCLUDED
#define MCPTestHelpers_INCLUDED


#include "Poco/AI/MCP/MCP.h"
#include "Poco/AI/MCP/Message.h"
#include "Poco/JSON/Object.h"
#include <string>


namespace MCPTest {


inline Poco::JSON::Object::Ptr meta()
	/// The _meta object of a request: the protocol revision and no capabilities.
{
	Poco::JSON::Object::Ptr pMeta = new Poco::JSON::Object;
	pMeta->set(Poco::AI::MCP::META_PROTOCOL_VERSION, Poco::AI::MCP::PROTOCOL_VERSION);
	pMeta->set(Poco::AI::MCP::META_CLIENT_CAPABILITIES, Poco::JSON::Object::Ptr(new Poco::JSON::Object));
	return pMeta;
}


inline Poco::JSON::Object::Ptr params(const Poco::JSON::Object::Ptr& pParams = nullptr)
	/// A copy of pParams (or empty params) with the _meta object added.
{
	Poco::JSON::Object::Ptr pCopy = pParams != nullptr ? Poco::JSON::Object::Ptr(new Poco::JSON::Object(*pParams)) : Poco::JSON::Object::Ptr(new Poco::JSON::Object);
	pCopy->set("_meta", meta());
	return pCopy;
}


inline Poco::JSON::Object::Ptr request(const Poco::AI::MCP::Id& id, const std::string& method, const Poco::JSON::Object::Ptr& pParams = nullptr)
	/// A request of the current revision.
{
	return Poco::AI::MCP::Request::make(id, method, params(pParams));
}


inline int errorCode(const Poco::JSON::Object::Ptr& pReply)
	/// The code of an error reply; 0 for a reply that is not an error.
{
	if (pReply.isNull() || !pReply->isObject("error")) return 0;
	return pReply->getObject("error")->getValue<int>("code");
}


inline Poco::JSON::Object::Ptr result(const Poco::JSON::Object::Ptr& pReply)
	/// The result object of a reply, or null.
{
	if (pReply.isNull() || !pReply->isObject("result")) return Poco::JSON::Object::Ptr();
	return pReply->getObject("result");
}


inline int code(Poco::AI::MCP::ErrorCode errorCode)
{
	return static_cast<int>(errorCode);
}


} // namespace MCPTest


#endif // MCPTestHelpers_INCLUDED
