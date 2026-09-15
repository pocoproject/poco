//
// Deserializer.cpp
//
// Library: RemotingNG/SOAP
// Package: SOAP
// Module:  Deserializer
//
// Copyright (c) 2006-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties,
// copied or duplicated in any form, in whole or in part.
//


#include "Poco/RemotingNG/SOAP/Deserializer.h"
#include "Poco/RemotingNG/SOAP/Serializer.h"
#include "Poco/RemotingNG/SOAP/Transport.h"
#include "Poco/RemotingNG/SOAP/SOAPFaultException.h"
#include "Poco/Net/MultipartReader.h"
#include "Poco/Net/MessageHeader.h"
#include "Poco/SAX/InputSource.h"
#include "Poco/NumberParser.h"
#include "Poco/Base64Decoder.h"
#include "Poco/Buffer.h"
#include "Poco/DateTimeParser.h"
#include "Poco/DateTimeFormat.h"
#include <sstream>
#include <algorithm>


using namespace std::string_literals;


namespace Poco {
namespace RemotingNG {
namespace SOAP {


Deserializer::Deserializer():
	_soapVersion(Serializer::SOAP_1_1),
	_mtom(false),
	_pRoot(0)
{
	_reader.setParent(&_internalReader);
	_reader.setContentHandler(&_contentHandler);
}


Deserializer::~Deserializer()
{
}


void Deserializer::addHeaderInterceptor(HeaderInterceptor::Ptr pInterceptor)
{
	_headerInterceptors.push_back(pInterceptor);
}


void Deserializer::removeHeaderInterceptor(HeaderInterceptor::Ptr pInterceptor)
{
	_headerInterceptors.erase(
		std::remove(_headerInterceptors.begin(), _headerInterceptors.end(), pInterceptor),
		_headerInterceptors.end()
	);
}


void Deserializer::enableMTOM(bool enable)
{
	_mtom = enable;
}


bool Deserializer::isMTOMEnabled() const
{
	return _mtom;
}


void Deserializer::parseContentType(const std::string& contentType)
{
	std::string primaryContentType;
	Poco::Net::NameValueCollection params;
	Poco::Net::MessageHeader::splitParameters(contentType, primaryContentType, params);
	_boundary = params.get("boundary"s, ""s);
	std::string start = params.get("start"s, ""s);
	if (!start.empty())
	{
		_startContentID = unbracketContentID(start);
	}
}


Poco::RemotingNG::SerializerBase::MessageType Deserializer::findMessage(std::string& name)
{
	poco_assert_dbg (_entries.size() == 1);

	// the first child in the root element is the method name or the SOAP envelope or the SOAP fault inside the soap envelope
	const SOAPContentHandler::Node* pCur = 0;
	pCur = _entries.top().first;
	pCur = &pCur->_children.at(0);
	if (pCur->_name != Serializer::ENVELOPE) // we deliberately don't check namespaces!
		throw Poco::RemotingNG::DeserializerException("No SOAP Envelope found");
	if (pCur->_children.size() == 0)
		throw Poco::RemotingNG::DeserializerException("SOAP Envelope is empty");
	pCur = &(pCur->_children.back()); // skip body/header, body is always after header
	if (!pCur->_children.empty())
	{
		const SOAPContentHandler::Node* pFirstChild = &pCur->_children[0];
		if (pFirstChild->_name == Serializer::FAULT && pFirstChild->_uri == Serializer::SOAP11NAMESPACE)
		{
			if (pFirstChild->_children.size() < 2)
				throw Poco::RemotingNG::DeserializerException("SOAP Fault is malformed");

			if (pFirstChild->_children[0]._name != Serializer::FAULTCODE)
				throw Poco::RemotingNG::DeserializerException("SOAP Fault is missing faultcode");

			if (pFirstChild->_children[1]._name != Serializer::FAULTSTRING)
				throw Poco::RemotingNG::DeserializerException("SOAP Fault is missing faultstring");
			name = pFirstChild->_name;
			return Poco::RemotingNG::SerializerBase::MESSAGE_FAULT;
		}
		else if (pFirstChild->_name == Serializer::FAULT && pFirstChild->_uri == Serializer::SOAP12NAMESPACE)
		{
			if (pFirstChild->_children.size() < 2)
				throw Poco::RemotingNG::DeserializerException("SOAP Fault is malformed");

			if (pFirstChild->_children[0]._name != Serializer::CODE)
				throw Poco::RemotingNG::DeserializerException("SOAP Fault is missing Code");

			if (pFirstChild->_children[1]._name != Serializer::REASON)
				throw Poco::RemotingNG::DeserializerException("SOAP Fault is missing Reason");
			name = pFirstChild->_name;
			return Poco::RemotingNG::SerializerBase::MESSAGE_FAULT;
		}
		else
		{
			name = pFirstChild->_name;
			return Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST;
		}
	}

	pCur = _entries.top().first;
	poco_assert_dbg (!pCur->_children.empty());
	name = pCur->_children.at(0)._name;
	return Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST;
}


void Deserializer::deserializeMessageBegin(const std::string& name, Poco::RemotingNG::SerializerBase::MessageType /*type*/)
{
	std::string foundName;
	Poco::RemotingNG::SerializerBase::MessageType foundType = findMessage(foundName);
	const SOAPContentHandler::Node* pCur = _entries.top().first;
	_entries.push(std::make_pair(&pCur->_children.at(0), 0));
	while (hasProperty(Poco::RemotingNG::SerializerBase::PROP_HEADER))
	{
		_headerElements.insert(getProperty(Poco::RemotingNG::SerializerBase::PROP_HEADER));
		popProperty(Poco::RemotingNG::SerializerBase::PROP_HEADER);
	}

	const auto& env = *_entries.top().first;
	if (env._children.size() > 1)
	{
		for (auto pInterceptor: _headerInterceptors)
		{
			pInterceptor->interceptHeader(env._children[0]);
		}
	}

	if (foundType == Poco::RemotingNG::SerializerBase::MESSAGE_FAULT)
	{
		pCur = _entries.top().first; // envelope
		if (pCur->_children.size() < 1) throw Poco::RemotingNG::DeserializerException("SOAP Envelope is empty");
		_entries.push(std::make_pair(&pCur->_children.back(), 0));
		pCur = _entries.top().first; // body
		if (pCur->_children.size() < 1) throw Poco::RemotingNG::DeserializerException("SOAP Body is empty");
		_entries.push(std::make_pair(&pCur->_children.back(), 0));
		pCur = _entries.top().first; // fault
		const SOAPContentHandler::Node* pFault = pCur;
		if (pCur->_children.size() < 2) throw Poco::RemotingNG::DeserializerException("SOAP Fault does not contain code and/or reason");
		if (pCur->_uri == Serializer::SOAP12NAMESPACE)
		{
			if (pCur->_children[1]._name != Serializer::REASON) throw Poco::RemotingNG::DeserializerException("SOAP 1.2 Fault does not contain Reason");
			if (pCur->_children[0]._name != Serializer::CODE) throw Poco::RemotingNG::DeserializerException("SOAP 1.2 Fault does not contain Code");
			pCur = &pCur->_children[1];
			if (pCur->_children.size() < 1) throw Poco::RemotingNG::DeserializerException("SOAP Reason is empty");
			if (pCur->_children[0]._name != Serializer::REASON_TEXT) throw Poco::RemotingNG::DeserializerException("SOAP 1.2 Fault Reason does not contain Text");
			throw SOAPFaultException(pCur->_children[0]._value, *pFault);
		}
		else if (pCur->_uri == Serializer::SOAP11NAMESPACE)
		{
			if (pCur->_children[1]._name != Serializer::FAULTSTRING) throw Poco::RemotingNG::DeserializerException("SOAP 1.1 Fault does not contain faultstring");
			if (pCur->_children[0]._name != Serializer::FAULTCODE) throw Poco::RemotingNG::DeserializerException("SOAP 1.1 Fault does not contain faultcode");
			throw SOAPFaultException(pCur->_children[1]._value, *pFault);
		}
		else throw Poco::RemotingNG::DeserializerException("SOAP Fault element has unprocessable content");
	}
	else if (foundName == name || name == "#")
	{
		// we are either expecting soap:header or soap:Body
		// in any case we need the method entry first!
		// the rule for deserializing is: first the attrs, then the header elements, then the rest!
		pCur = _entries.top().first; // pCur contains envelope
		if (pCur->_children.size() < 1) throw Poco::RemotingNG::DeserializerException("SOAP Envelope is empty");
		_entries.push(std::make_pair(&pCur->_children.back(), 0)); // get the body
		pCur = _entries.top().first; // body
		if (name != "#")
		{
			if (pCur->_children.size() < 1) throw Poco::RemotingNG::DeserializerException("SOAP Body is empty");
			_entries.push(std::make_pair(&pCur->_children.at(0), 0)); // push the methodname
		}
		checkAttributes();
	}
	else throw Poco::RemotingNG::DeserializerException("Unexpected element in SOAP envelope", foundName);
}


void Deserializer::deserializeMessageEnd(const std::string& name, Poco::RemotingNG::SerializerBase::MessageType /*type*/)
{
	poco_assert_dbg (name == "#" || _entries.top().first->_name == name);
	if (name != "#")
	{
		_entries.pop(); // the method info
	}
	_entries.pop(); // the soap envelope
	_entries.pop(); // the soap body

	// pop the document entry too
	_entries.pop();
	poco_assert_dbg (_entries.empty());
	poco_assert_dbg (_attr.empty());
}


bool Deserializer::deserializeStructBegin(const std::string& name, bool isMandatory)
{
	// with optional types it can happen that they are not present
	// types are either in header or in body
	checkHeaderStart(name);
	Deserializer::EntryPair& entryPair = _entries.top();
	const SOAPContentHandler::Node* pCur = entryPair.first;
	std::size_t numChildren = pCur->_children.size();
	if (numChildren > entryPair.second)
	{
		const SOAPContentHandler::Node* pCurChild = &pCur->_children[entryPair.second];
		if (pCurChild->_name == name)
		{
			entryPair.second++; // move to the next child
			_entries.push(std::make_pair(pCurChild, 0));
			checkAttributes();
			return true;
		}
		else
		{
			_attr.clear();
		}
	}
	else
	{
		// else not found: do not change the position, if the data was optional, it is okay
		// only clear the attributes from the not found complex types!
		_attr.clear();
	}
	handleConditionalError(name, isMandatory);
	return false;
}


void Deserializer::deserializeStructEnd(const std::string& name)
{
	Deserializer::EntryPair& entryPair = _entries.top();
	const SOAPContentHandler::Node* pCur = entryPair.first;
	if (pCur->_name == name)
	{
		_entries.pop();
	}
	else
	{
		// a optional element
	}
	checkHeaderEnd(name);
	poco_assert_dbg(_attr.empty());
}


bool Deserializer::deserializeSequenceBegin(const std::string& name, bool /*isMandatory*/, Poco::UInt32& sizeHint)
{
	sizeHint = 0; // no size information available
	// check if we have at least one single vector element, do not change the position!
	Deserializer::EntryPair& entryPair = _entries.top();
	const SOAPContentHandler::Node* pCur = entryPair.first;
	std::size_t numChildren = pCur->_children.size();
	if (numChildren > entryPair.second)
	{
		const SOAPContentHandler::Node* pCurChild = &pCur->_children[entryPair.second];

		if (pCurChild->_name == name)
		{
			return true;
		}
	}
	return false;
}


void Deserializer::deserializeSequenceEnd(const std::string& /*name*/)
{
}


bool Deserializer::deserializeNullableBegin(const std::string& name, bool /*isMandatory*/, bool& isNull)
{
	Deserializer::EntryPair& entryPair = _entries.top();
	const SOAPContentHandler::Node* pCur = entryPair.first;
	std::size_t numChildren = pCur->_children.size();
	if (numChildren > entryPair.second)
	{
		const SOAPContentHandler::Node* pCurChild = &pCur->_children[entryPair.second];

		if (pCurChild->_name == name)
		{
			const std::string& nil = pCurChild->_attributes.getValue(Serializer::XSINAMESPACE, Serializer::XSINIL);
			isNull = (nil == Serializer::VAL_TRUE || nil == "1");
			if (isNull) entryPair.second++;
			return true;
		}
	}
	return false;
}


void Deserializer::deserializeNullableEnd(const std::string& /*name*/)
{
}


bool Deserializer::deserializeOptionalBegin(const std::string& name, bool /*isMandatory*/, bool& isSpecified)
{
	Deserializer::EntryPair& entryPair = _entries.top();
	const SOAPContentHandler::Node* pCur = entryPair.first;
	std::size_t numChildren = pCur->_children.size();
	if (numChildren > entryPair.second)
	{
		const SOAPContentHandler::Node* pCurChild = &pCur->_children[entryPair.second];

		if (pCurChild->_name == name)
		{
			isSpecified = true;
			return true;
		}
	}
	isSpecified = false;
	return false;
}


void Deserializer::deserializeOptionalEnd(const std::string& /*name*/)
{
}


void Deserializer::checkHeaderStart(const std::string& /*name*/)
{
	// note: the initial layout is root:Envelope:Body:method
	// push header once
	if (!_headerElements.empty() && _entries.size() == 4)
	{
		_entries.push(std::make_pair(&_pRoot->_children.at(0)._children.at(0), 0));
	}
}


void Deserializer::checkHeaderEnd(const std::string& name)
{
	// note: the initial layout is root:Envelope:Body:method
	// push header once
	bool wasEmpty = _headerElements.empty();
	if (!wasEmpty && _entries.size() == 5)
	{
		_headerElements.erase(name);
		if (_headerElements.empty())
		{
			//layout is root:Envelope:Body:method:header
			_entries.pop();
		}
	}
}


bool Deserializer::deserialize(const std::string& name, bool isMandatory, Poco::Int8& val)
{
	bool ret = false;
	const std::string& strVal = getData(name, isMandatory, ret);
	if (ret)
	{
		val = (Poco::Int8)Poco::NumberParser::parse(strVal); //todo: overflow/underflow checks
	}
	return ret;
}


bool Deserializer::deserialize(const std::string& name, bool isMandatory, Poco::UInt8& val)
{
	bool ret = false;
	const std::string& strVal = getData(name, isMandatory, ret);
	if (ret)
	{
		val = (Poco::UInt8) Poco::NumberParser::parseUnsigned(strVal); //todo: overflow/underflow checks
	}
	return ret;
}


bool Deserializer::deserialize(const std::string& name, bool isMandatory, Poco::Int16& val)
{
	bool ret = false;
	const std::string& strVal = getData(name, isMandatory, ret);
	if (ret)
	{
		val = (Poco::Int16)Poco::NumberParser::parse(strVal); //todo: overflow/underflow checks
	}
	return ret;
}


bool Deserializer::deserialize(const std::string& name, bool isMandatory, Poco::UInt16& val)
{
	bool ret = false;
	const std::string& strVal = getData(name, isMandatory, ret);
	if (ret)
	{
		val = (Poco::UInt16) Poco::NumberParser::parseUnsigned(strVal); //todo: overflow/underflow checks
	}
	return ret;
}


bool Deserializer::deserialize(const std::string& name, bool isMandatory, Poco::Int32& val)
{
	bool ret = false;
	const std::string& strVal = getData(name, isMandatory, ret);
	if (ret)
	{
		val = Poco::NumberParser::parse(strVal);
	}
	return ret;
}


bool Deserializer::deserialize(const std::string& name, bool isMandatory, Poco::UInt32& val)
{
	bool ret = false;
	const std::string& strVal = getData(name, isMandatory, ret);
	if (ret)
	{
		val = Poco::NumberParser::parseUnsigned(strVal);
	}
	return ret;
}


bool Deserializer::deserialize(const std::string& name, bool isMandatory, long& val)
{
	bool ret = false;
	const std::string& strVal = getData(name, isMandatory, ret);
	if (ret)
	{
#ifndef POCO_INT64_IS_LONG
		val = Poco::NumberParser::parse(strVal);
#else
		val = Poco::NumberParser::parse64(strVal);
#endif
	}
	return ret;
}


bool Deserializer::deserialize(const std::string& name, bool isMandatory, unsigned long& val)
{
	bool ret = false;
	const std::string& strVal = getData(name, isMandatory, ret);
	if (ret)
	{
#ifndef POCO_INT64_IS_LONG
		val = Poco::NumberParser::parseUnsigned(strVal);
#else
		val = Poco::NumberParser::parseUnsigned64(strVal);
#endif
	}
	return ret;
}


#ifndef POCO_INT64_IS_LONG
bool Deserializer::deserialize(const std::string& name, bool isMandatory, Poco::Int64& val)
{
	bool ret = false;
	const std::string& strVal = getData(name, isMandatory, ret);
	if (ret)
	{
		val = Poco::NumberParser::parse64(strVal);
	}
	return ret;
}


bool Deserializer::deserialize(const std::string& name, bool isMandatory, Poco::UInt64& val)
{
	bool ret = false;
	const std::string& strVal = getData(name, isMandatory, ret);
	if (ret)
	{
		val = Poco::NumberParser::parseUnsigned64(strVal);
	}
	return ret;
}
#endif


bool Deserializer::deserialize(const std::string& name, bool isMandatory, float& val)
{
	bool ret = false;
	const std::string& strVal = getData(name, isMandatory, ret);
	if (ret)
	{
		val = (float)Poco::NumberParser::parseFloat(strVal);
	}
	return ret;
}


bool Deserializer::deserialize(const std::string& name, bool isMandatory, double& val)
{
	bool ret = false;
	const std::string& strVal = getData(name, isMandatory, ret);
	if (ret)
	{
		val = Poco::NumberParser::parseFloat(strVal);
	}
	return ret;
}


bool Deserializer::deserialize(const std::string& name, bool isMandatory, bool& val)
{
	// parse for true/1 or false/0
	bool ret = false;
	const std::string& strVal = getData(name, isMandatory, ret);
	if (ret)
	{
		if (strVal == Serializer::VAL_FALSE || strVal == "0")
			val = false;
		else if (strVal == Serializer::VAL_TRUE || strVal == "1")
			val = true;
		else
		{
			reset();
			throw Poco::RemotingNG::DeserializerException("Illegal bool value ", strVal);
		}
	}
	return ret;
}


bool Deserializer::deserialize(const std::string& name, bool isMandatory, char& val)
{
	bool ret = false;
	const std::string& strVal = getData(name, isMandatory, ret);
	if (ret)
	{
		if (strVal.size() != 1)
		{
			reset();
			throw Poco::RemotingNG::DeserializerException("Illegal char value ", strVal);
		}
		val = strVal[0];
	}
	return ret;
}


bool Deserializer::deserialize(const std::string& name, bool isMandatory, std::string& val)
{
	bool ret = false;
	const std::string& strVal = getData(name, isMandatory, ret);
	if (ret)
	{
		val = strVal;
	}
	return ret;
}


bool Deserializer::deserialize(const std::string& name, bool isMandatory, Poco::DateTime& value)
{
	bool ret = false;
	const std::string& strVal = getData(name, isMandatory, ret);
	if (ret)
	{
		int tzd = 0;
		const std::string xsdType = getProperty(SerializerBase::PROP_XSDTYPE, ""s);
		if (xsdType == "date"s)
			Poco::DateTimeParser::parse("%Y-%m-%d%z"s, strVal, value, tzd);
		else if (xsdType == "time"s)
			Poco::DateTimeParser::parse("%H:%M:%s%z"s, strVal, value, tzd);
		else
			Poco::DateTimeParser::parse(Poco::DateTimeFormat::ISO8601_FRAC_FORMAT, strVal, value, tzd);
		value.makeUTC(tzd);
	}
	return ret;
}


bool Deserializer::deserialize(const std::string& name, bool isMandatory, Poco::LocalDateTime& value)
{
	bool ret = false;
	const std::string& strVal = getData(name, isMandatory, ret);
	if (ret)
	{
		int tzd = 0;
		Poco::DateTime dt;
		const std::string xsdType = getProperty(SerializerBase::PROP_XSDTYPE, ""s);
		if (xsdType == "date"s)
			Poco::DateTimeParser::parse("%Y-%m-%d%z"s, strVal, dt, tzd);
		else if (xsdType == "time"s)
			Poco::DateTimeParser::parse("%H:%M:%s%z"s, strVal, dt, tzd);
		else
			Poco::DateTimeParser::parse(Poco::DateTimeFormat::ISO8601_FRAC_FORMAT, strVal, dt, tzd);
		value = Poco::LocalDateTime(tzd, dt, false);
	}
	return ret;
}


bool Deserializer::deserialize(const std::string& name, bool isMandatory, std::vector<char>& val)
{
	Deserializer::EntryPair& entryPair = _entries.top();
	const SOAPContentHandler::Node* pCur = entryPair.first;
	std::size_t numChildren = pCur->_children.size();
	if (numChildren > entryPair.second)
	{
		const SOAPContentHandler::Node* pCurChild = &pCur->_children[entryPair.second];

		if (pCurChild->_name == name)
		{
			if (_mtom && !pCurChild->_children.empty())
			{
				if (pCurChild->_children[0]._name == Serializer::XOPINCLUDE)
				{
					std::string href = pCurChild->_children[0]._attributes.getValue(Serializer::EMPTYSTRING, Serializer::XOPHREF);
					if (href.size() > 4 && href.compare(0, 4, "cid:") == 0)
					{
						std::string cid(href, 4);
						std::map<std::string, std::vector<char>>::iterator it = _parts.find(cid);
						if (it != _parts.end())
						{
							val.clear();
							std::swap(val, it->second);
						}
					}
				}
			}
			else
			{
				std::istringstream istr(pCurChild->_value);
				Poco::Base64Decoder decoder(istr);
				val.clear();
				int ch = decoder.get();
				while (ch != -1)
				{
					val.push_back(static_cast<char>(ch));
					ch = decoder.get();
				}
			}
			entryPair.second++;
			return true;
		}
	}
	// else not found: do not change the position, if the data was optional, it is okay
	handleConditionalError(name, isMandatory);
	return false;
}


void Deserializer::handleConditionalError(const std::string& name, bool isMandatory)
{
	if (isMandatory)
	{
		_attr.clear();
		throw Poco::RemotingNG::MissingParamException(name);
	}
}


const std::string& Deserializer::getData(const std::string& name, bool isMandatory, bool& found)
{
	found = false;
	Deserializer::EntryPair& entryPair = _entries.top();
	const SOAPContentHandler::Node* pParent = entryPair.first;
	if (name.empty())
	{
		found = true;
		return pParent->_value;
	}
	std::size_t numChildren = pParent->_children.size();
	// check for attribute!
	AttrMap::iterator it = _attr.find(name);
	if (it != _attr.end())
	{
		int idx = pParent->_attributes.getIndex(it->first);
		if (idx < 0)
			idx = pParent->_attributes.getIndex(it->second.first, it->first);
		if (idx < 0)
			idx = pParent->_attributes.getIndex(Serializer::EMPTYSTRING, it->first);
		_attr.erase(name);
		if (idx < 0)
		{
			handleConditionalError(name, isMandatory);
			return Serializer::EMPTYSTRING;
		}
		found = true;
		_tmp = pParent->_attributes.getValue(idx);
		return _tmp;
	}
	else _attr.clear();

	if (numChildren > entryPair.second)
	{
		const SOAPContentHandler::Node& pCurChild = pParent->_children[entryPair.second];
		if (pCurChild._name == name)
		{
			entryPair.second++; // move to the next child
			found = true;
			return pCurChild._value;
		}
	}
	// else not found: do not change the position, if the data was optional, it is okay
	handleConditionalError(name, isMandatory);
	return Serializer::EMPTYSTRING;
}


void Deserializer::checkAttributes()
{
	const SOAPContentHandler::Node* pCur = _entries.top().first;
	AttrMap::const_iterator it = _attr.begin();
	AttrMap::const_iterator itEnd = _attr.end();
	for (; it != itEnd; ++it)
	{
		if (pCur->_attributes.getIndex(Serializer::EMPTYSTRING, it->first) < 0 && pCur->_attributes.getIndex(it->second.first, it->first) < 0 && pCur->_attributes.getIndex(it->first) < 0 && it->second.second)
		{
			throw Poco::RemotingNG::MissingParamException("Missing mandatory attribute", it->first);
		}
	}
}


void Deserializer::resetImpl()
{
	_attr.clear();
	while (!_entries.empty())
	{
		_entries.pop();
	}
}


void Deserializer::setupImpl(std::istream& in)
{
	poco_assert_dbg (_entries.empty());

	if (_mtom)
	{
		Poco::Net::MultipartReader multipartReader(in, _boundary);
		bool firstPart = true;
		while (multipartReader.hasNextPart())
		{
			Poco::Net::MessageHeader header;
			multipartReader.nextPart(header);
			std::string contentID = unbracketContentID(header.get("Content-ID", ""));
			if (contentID == _startContentID || (contentID.empty() && firstPart))
			{
				XML::InputSource source(multipartReader.stream());
				_reader.parse(&source);
			}
			else
			{
				std::vector<char> content;
				_parts[contentID] = content;
				copyToVector(multipartReader.stream(), _parts[contentID]);
			}
			firstPart = false;
		}
	}
	else
	{
		XML::InputSource source(in);
		_reader.parse(&source);
	}
	_entries.push(std::make_pair(&_contentHandler.content(), 0));
	_pRoot = _entries.top().first;
	if (_pRoot->_children.empty()) throw DeserializerException("No SOAP Envelope found");
	_soapVersion = _pRoot->_children[0]._uri == Serializer::SOAP12NAMESPACE ? Serializer::SOAP_1_2 : Serializer::SOAP_1_1;
}


void Deserializer::pushAttribute(const std::string& ns, const std::string& attrName, bool isMandatory)
{
	_attr.insert(std::make_pair(attrName, std::make_pair(ns, isMandatory)));
}


std::string Deserializer::unbracketContentID(const std::string& cid)
{
	if (cid[0] == '<' && cid.size() > 2)
		return cid.substr(1, cid.size() - 2);
	else
		return cid;
}


void Deserializer::copyToVector(std::istream& istr, std::vector<char>& vec)
{
	Poco::Buffer<char> buffer(4096);
	istr.read(buffer.begin(), buffer.size());
	std::streamsize n = istr.gcount();
	while (n > 0)
	{
		vec.insert(vec.end(), buffer.begin(), buffer.begin() + n);
		if (istr)
		{
			istr.read(buffer.begin(), buffer.size());
			n = istr.gcount();
		}
		else n = 0;
	}
}


} } } // namespace Poco::RemotingNG::SOAP
