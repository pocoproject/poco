//
// JSONDeserializer.cpp
//
// Library: RemotingNG/REST
// Package: REST
// Module:  JSONDeserializer
//
// Copyright (c) 2010-2016, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/RemotingNG/REST/JSONDeserializer.h"
#include "Poco/RemotingNG/REST/JSONSerializer.h"
#include "Poco/RemotingNG/REST/JSONParser.h"
#include "Poco/RemotingNG/REST/JSObject.h"
#include "Poco/RemotingNG/REST/RESTServerException.h"
#include "Poco/NumberParser.h"
#include "Poco/Base64Decoder.h"
#include <sstream>


namespace Poco {
namespace RemotingNG {
namespace REST {


JSONDeserializer::JSONDeserializer(std::istream& stream):
	_objectDepth(0)
{
	setupImpl(stream);
}


JSONDeserializer::~JSONDeserializer()
{
}


void JSONDeserializer::deserializeError(const std::string& reason, int status)
{
	poco_assert_dbg (_objectStack.size() == 1);

	std::string exception;
	std::string message;
	if (_objectStack.front()->has(JSONSerializer::JSON_ERROR))
	{
		exception = (*_objectStack.front())[JSONSerializer::JSON_ERROR]->stringValue();
	}
	if (_objectStack.front()->has(JSONSerializer::JSON_DETAIL))
	{
		message = (*_objectStack.front())[JSONSerializer::JSON_DETAIL]->stringValue();
	}
	std::string fullMessage;
	if (!exception.empty())
	{
		fullMessage += exception;
	}
	if (!fullMessage.empty() && !message.empty())
	{
		fullMessage += ": ";
	}
	fullMessage += message;

	throw RESTServerException(reason, fullMessage, status, _objectStack.front());
}


Poco::RemotingNG::SerializerBase::MessageType JSONDeserializer::findMessage(std::string&)
{
	poco_bugcheck();
	return Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST;
}


void JSONDeserializer::deserializeMessageBegin(const std::string&, SerializerBase::MessageType)
{
	poco_bugcheck();
}


void JSONDeserializer::deserializeMessageEnd(const std::string& name, SerializerBase::MessageType type)
{
	poco_bugcheck();
}


bool JSONDeserializer::deserializeStructBegin(const std::string& name, bool isMandatory)
{
	if (_objectDepth == 0 && _objectStack.size() == 1 && _objectStack.back()->type() == JSValue::JS_OBJECT)
	{
		_objectDepth++;
		return true;
	}

	bool found = false;
	JSValue::Ptr pObject = findValue(name, isMandatory, found);
	if (found)
	{
		if (pObject->type() != JSValue::JS_OBJECT)
			Poco::RemotingNG::DeserializerException("expected object, found something else", name);

		_objectStack.push_back(pObject);
		_objectDepth++;
		return true;
	}
	else if (isMandatory)
	{
		throw Poco::RemotingNG::MissingParamException(name);
	}
	return false;
}


void JSONDeserializer::deserializeStructEnd(const std::string& name)
{
	_objectDepth--;
	_objectStack.pop_back();
}


bool JSONDeserializer::deserializeSequenceBegin(const std::string& name, bool isMandatory, Poco::UInt32& sizeHint)
{
	if (_objectDepth == 0 && _objectStack.size() == 1 && _objectStack.back()->type() == JSValue::JS_ARRAY)
	{
		_indexStack.push_back(0);
		sizeHint = static_cast<Poco::UInt32>(_objectStack.back()->size());
		_objectDepth++;
		return true;
	}

	bool found = false;
	JSValue::Ptr pArray = findValue(name, isMandatory, found);
	if (found)
	{
		if (pArray->type() != JSValue::JS_ARRAY)
			Poco::RemotingNG::DeserializerException("expected array, found something else", name);

		sizeHint = static_cast<Poco::UInt32>(pArray->size());
		_objectStack.push_back(pArray);
		_indexStack.push_back(0);
		_objectDepth++;
		return true;
	}
	else if (isMandatory)
	{
		throw Poco::RemotingNG::MissingParamException(name);
	}
	return false;
}


void JSONDeserializer::deserializeSequenceEnd(const std::string& name)
{
	_objectDepth--;
	_indexStack.pop_back();
	_objectStack.pop_back();
}


bool JSONDeserializer::deserializeNullableBegin(const std::string& name, bool isMandatory, bool& isNull)
{
	bool found = false;
	JSValue::Ptr pValue = findValue(name, isMandatory, found);
	if (found)
	{
		isNull = pValue.isNull();
		// findValue() will be called again and must return the same value
		// Therefore we must adjust the _indexStack's topmost counter.
		if (!isNull && _objectStack.back()->type() == JSValue::JS_ARRAY)
		{
			_indexStack.back()--;
		}
	}
	return found;
}


void JSONDeserializer::deserializeNullableEnd(const std::string& name)
{
}


bool JSONDeserializer::deserializeOptionalBegin(const std::string& name, bool isMandatory, bool& isSpecified)
{
	JSValue::Ptr pValue = findValue(name, false, isSpecified);
	if (isSpecified)
	{
		// findValue() will be called again and must return the same value
		// Therefore we must adjust the _indexStack's topmost counter.
		if (_objectStack.back()->type() == JSValue::JS_ARRAY)
		{
			_indexStack.back()--;
		}
	}
	return isSpecified;
}


void JSONDeserializer::deserializeOptionalEnd(const std::string& name)
{
}


bool JSONDeserializer::deserialize(const std::string& name, bool isMandatory, Poco::Int8& val)
{
	bool found = false;
	JSValue::Ptr pValue = findValue(name, isMandatory, found);
	if (found)
	{
		if (!pValue) throw Poco::NullValueException(name);
		val = static_cast<Poco::Int8>(pValue->asInt32());
	}
	return found;
}


bool JSONDeserializer::deserialize(const std::string& name, bool isMandatory, Poco::UInt8& val)
{
	bool found = false;
	JSValue::Ptr pValue = findValue(name, isMandatory, found);
	if (found)
	{
		if (!pValue) throw Poco::NullValueException(name);
		val = static_cast<Poco::UInt8>(pValue->asUInt32());
	}
	return found;
}


bool JSONDeserializer::deserialize(const std::string& name, bool isMandatory, Poco::Int16& val)
{
	bool found = false;
	JSValue::Ptr pValue = findValue(name, isMandatory, found);
	if (found)
	{
		if (!pValue) throw Poco::NullValueException(name);
		val = static_cast<Poco::Int16>(pValue->asInt32());
	}
	return found;
}


bool JSONDeserializer::deserialize(const std::string& name, bool isMandatory, Poco::UInt16& val)
{
	bool found = false;
	JSValue::Ptr pValue = findValue(name, isMandatory, found);
	if (found)
	{
		if (!pValue) throw Poco::NullValueException(name);
		val = static_cast<Poco::UInt16>(pValue->asUInt32());
	}
	return found;
}


bool JSONDeserializer::deserialize(const std::string& name, bool isMandatory, Poco::Int32& val)
{
	bool found = false;
	JSValue::Ptr pValue = findValue(name, isMandatory, found);
	if (found)
	{
		if (!pValue) throw Poco::NullValueException(name);
		val = pValue->asInt32();
	}
	return found;
}


bool JSONDeserializer::deserialize(const std::string& name, bool isMandatory, Poco::UInt32& val)
{
	bool found = false;
	JSValue::Ptr pValue = findValue(name, isMandatory, found);
	if (found)
	{
		if (!pValue) throw Poco::NullValueException(name);
		val = pValue->asUInt32();
	}
	return found;
}


bool JSONDeserializer::deserialize(const std::string& name, bool isMandatory, long& val)
{
	bool found = false;
	JSValue::Ptr pValue = findValue(name, isMandatory, found);
	if (found)
	{
		if (!pValue) throw Poco::NullValueException(name);
#ifdef POCO_INT64_IS_LONG
		val = pValue->asInt64();
#else
		val = pValue->asInt32();
#endif
	}
	return found;
}


bool JSONDeserializer::deserialize(const std::string& name, bool isMandatory, unsigned long& val)
{
	bool found = false;
	JSValue::Ptr pValue = findValue(name, isMandatory, found);
	if (found)
	{
		if (!pValue) throw Poco::NullValueException(name);
#ifdef POCO_INT64_IS_LONG
		val = pValue->asUInt64();
#else
		val = pValue->asUInt32();
#endif
	}
	return found;
}


#ifndef POCO_INT64_IS_LONG
bool JSONDeserializer::deserialize(const std::string& name, bool isMandatory, Poco::Int64& val)
{
	bool found = false;
	JSValue::Ptr pValue = findValue(name, isMandatory, found);
	if (found)
	{
		if (!pValue) throw Poco::NullValueException(name);
		val = pValue->asInt64();
	}
	return found;
}


bool JSONDeserializer::deserialize(const std::string& name, bool isMandatory, Poco::UInt64& val)
{
	bool found = false;
	JSValue::Ptr pValue = findValue(name, isMandatory, found);
	if (found)
	{
		if (!pValue) throw Poco::NullValueException(name);
		val = pValue->asUInt64();
	}
	return found;
}
#endif


bool JSONDeserializer::deserialize(const std::string& name, bool isMandatory, float& val)
{
	bool found = false;
	JSValue::Ptr pValue = findValue(name, isMandatory, found);
	if (found)
	{
		if (!pValue) throw Poco::NullValueException(name);
		val = static_cast<float>(pValue->asDouble());
	}
	return found;
}


bool JSONDeserializer::deserialize(const std::string& name, bool isMandatory, double& val)
{
	bool found = false;
	JSValue::Ptr pValue = findValue(name, isMandatory, found);
	if (found)
	{
		if (!pValue) throw Poco::NullValueException(name);
		val = pValue->asDouble();
	}
	return found;
}


bool JSONDeserializer::deserialize(const std::string& name, bool isMandatory, bool& val)
{
	bool found = false;
	JSValue::Ptr pValue = findValue(name, isMandatory, found);
	if (found)
	{
		if (!pValue) throw Poco::NullValueException(name);
		val = pValue->asBool();
	}
	return found;
}


bool JSONDeserializer::deserialize(const std::string& name, bool isMandatory, char& val)
{
	bool found = false;
	JSValue::Ptr pValue = findValue(name, isMandatory, found);
	if (found)
	{
		if (!pValue) throw Poco::NullValueException(name);
		const std::string& str = pValue->stringValue();
		if (str.size() == 1)
			val = str[0];
		else
			throw Poco::RemotingNG::DeserializerException(std::string("Illegal char value ") + str);
	}
	return found;
}


bool JSONDeserializer::deserialize(const std::string& name, bool isMandatory, std::string& val)
{
	bool found = false;
	JSValue::Ptr pValue = findValue(name, isMandatory, found);
	if (found)
	{
		if (!pValue) throw Poco::NullValueException(name);
		val = pValue->stringValue();
	}
	return found;
}


bool JSONDeserializer::deserialize(const std::string& name, bool isMandatory, std::vector<char>& val)
{
	bool found = false;
	JSValue::Ptr pValue = findValue(name, isMandatory, found);
	if (found)
	{
		if (!pValue) throw Poco::NullValueException(name);
		std::istringstream istr(pValue->stringValue());
		Poco::Base64Decoder decoder(istr);
		val.clear();
		int ch = decoder.get();
		while (ch != -1)
		{
			val.push_back(static_cast<char>(ch));
			ch = decoder.get();
		}
	}
	return found;
}


JSValue::Ptr JSONDeserializer::findValue(const std::string& name, bool isMandatory, bool& found)
{
	poco_assert (!_objectStack.empty());

	if (_objectStack.back()->type() == JSValue::JS_ARRAY)
	{
		if (_indexStack.empty())
		{
			throw Poco::RemotingNG::DeserializerException("Expected object, but found array", name);
		}
		else if (_indexStack.back() < _objectStack.back()->size())
		{
			found = true;
			return (*_objectStack.back())[_indexStack.back()++];
		}
		else
		{
			found = false;
		}
	}
	else if (_objectStack.back()->type() == JSValue::JS_OBJECT)
	{
		if (_objectStack.back()->has(name))
		{
			found = true;
			return (*_objectStack.back())[name];
		}
		else
		{
			found = false;
		}
	}
	else if (_objectDepth == 0 && _objectStack.size() == 1)
	{
		found = true;
		return _objectStack.back();
	}
	if (isMandatory)
	{
		throw Poco::RemotingNG::MissingParamException(name);
	}
	return JSValue::Ptr();
}


void JSONDeserializer::resetImpl()
{
	_objectStack.clear();
	_indexStack.clear();
	_objectDepth = 0;
}


void JSONDeserializer::setupImpl(std::istream& stream)
{
	resetImpl();
	JSONParser parser(stream);
	_objectStack.push_back(parser.parse());
	int ch = stream.get();
	while (ch != -1) ch = stream.get();
}


} } } // namespace Poco::RemotingNG::REST
