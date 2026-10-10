//
// Deserializer.cpp
//
// Library: RemotingNG/JSONRPC
// Package: JSONRPC
// Module:  Deserializer
//
// Copyright (c) 2010-2013, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/RemotingNG/JSONRPC/Deserializer.h"
#include "Poco/RemotingNG/JSONRPC/Serializer.h"
#include "Poco/RemotingNG/JSONRPC/JSONParser.h"
#include "Poco/RemotingNG/JSONRPC/JSObject.h"
#include "Poco/RemotingNG/RemotingException.h"
#include "Poco/NumberParser.h"
#include "Poco/Base64Decoder.h"
#include <sstream>


namespace Poco {
namespace RemotingNG {
namespace JSONRPC {


Deserializer::Deserializer():
	_pAssociatedSerializer(0)
{
}


Deserializer::~Deserializer()
{
}


Poco::RemotingNG::SerializerBase::MessageType Deserializer::findMessage(std::string& name)
{
	poco_assert (!_objectStack.empty());

	name = (*_objectStack.front())[Serializer::JSON_METHOD]->stringValue();
	return Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST;
}


void Deserializer::deserializeMessageBegin(const std::string& name, SerializerBase::MessageType type)
{
	poco_assert_dbg (_objectStack.size() == 1);

	switch (type)
	{
	case Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST:
		{
			_id = (*_objectStack.front())[Serializer::JSON_ID]->asInt32();
			JSValue::Ptr pParams;
			if (_objectStack.front()->has(Serializer::JSON_PARAMS))
				pParams = (*_objectStack.front())[Serializer::JSON_PARAMS];
			else
				pParams = new JSObject;
			_objectStack.push_back(pParams);
			if (pParams->type() == JSValue::JS_ARRAY)
			{
				_indexStack.push_back(0);
			}
			if (_pAssociatedSerializer) _pAssociatedSerializer->resetId(_id);
		}
		break;

	case Poco::RemotingNG::SerializerBase::MESSAGE_REPLY:
		if (_objectStack.front()->has(Serializer::JSON_ERROR))
		{
			JSValue::Ptr pError = (*_objectStack.front())[Serializer::JSON_ERROR];
			throw RemoteException((*pError)[Serializer::JSON_MESSAGE]->stringValue(), (*pError)[Serializer::JSON_CODE]->asInt32());
		}
		else
		{
			_id = (*_objectStack.front())[Serializer::JSON_ID]->asInt32();
			JSValue::Ptr pResult = (*_objectStack.front())[Serializer::JSON_RESULT];
			_objectStack.push_back(pResult);
		}
		break;

	default:
		poco_bugcheck();
	}
}


void Deserializer::deserializeMessageEnd(const std::string& name, SerializerBase::MessageType type)
{
	poco_assert_dbg (_objectStack.size() == 2);

	switch (type)
	{
	case Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST:
		if (_objectStack.back()->type() == JSValue::JS_ARRAY)
		{
			_indexStack.pop_back();
		}
		// fallthrough

	case Poco::RemotingNG::SerializerBase::MESSAGE_REPLY:
		_objectStack.pop_back();
		poco_assert_dbg(_objectStack.size() == 1);
		break;

	default:
		poco_bugcheck();
	}
}


bool Deserializer::deserializeStructBegin(const std::string& name, bool isMandatory)
{
	if (name.empty() && _objectStack.size() == 1 && _objectStack.back()->type() == JSValue::JS_OBJECT)
		return true;

	bool found = false;
	JSValue::Ptr pObject = findValue(name, isMandatory, found);
	if (found)
	{
		if (pObject->type() != JSValue::JS_OBJECT)
			Poco::RemotingNG::DeserializerException("expected object, found something else", name);

		_objectStack.push_back(pObject);
		return true;
	}
	else if (isMandatory)
	{
		throw Poco::RemotingNG::MissingParamException(name);
	}
	return false;
}


void Deserializer::deserializeStructEnd(const std::string& name)
{
	_objectStack.pop_back();
}


bool Deserializer::deserializeSequenceBegin(const std::string& name, bool isMandatory, Poco::UInt32& sizeHint)
{
	if (name.empty() && _objectStack.size() == 1 && _objectStack.back()->type() == JSValue::JS_ARRAY)
	{
		_indexStack.push_back(0);
		sizeHint = static_cast<Poco::UInt32>(_objectStack.back()->size());
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
		return true;
	}
	else if (isMandatory)
	{
		throw Poco::RemotingNG::MissingParamException(name);
	}
	return false;
}


void Deserializer::deserializeSequenceEnd(const std::string& name)
{
	_indexStack.pop_back();
	_objectStack.pop_back();
}


bool Deserializer::deserializeNullableBegin(const std::string& name, bool isMandatory, bool& isNull)
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


void Deserializer::deserializeNullableEnd(const std::string& name)
{
}


bool Deserializer::deserialize(const std::string& name, bool isMandatory, Poco::Int8& val)
{
	bool found = false;
	JSValue::Ptr pValue = findValue(name, isMandatory, found);
	if (found)
	{
		val = static_cast<Poco::Int8>(pValue->asInt32());
	}
	return found;
}


bool Deserializer::deserialize(const std::string& name, bool isMandatory, Poco::UInt8& val)
{
	bool found = false;
	JSValue::Ptr pValue = findValue(name, isMandatory, found);
	if (found)
	{
		val = static_cast<Poco::UInt8>(pValue->asUInt32());
	}
	return found;
}


bool Deserializer::deserialize(const std::string& name, bool isMandatory, Poco::Int16& val)
{
	bool found = false;
	JSValue::Ptr pValue = findValue(name, isMandatory, found);
	if (found)
	{
		val = static_cast<Poco::Int16>(pValue->asInt32());
	}
	return found;
}


bool Deserializer::deserialize(const std::string& name, bool isMandatory, Poco::UInt16& val)
{
	bool found = false;
	JSValue::Ptr pValue = findValue(name, isMandatory, found);
	if (found)
	{
		val = static_cast<Poco::UInt16>(pValue->asUInt32());
	}
	return found;
}


bool Deserializer::deserialize(const std::string& name, bool isMandatory, Poco::Int32& val)
{
	bool found = false;
	JSValue::Ptr pValue = findValue(name, isMandatory, found);
	if (found)
	{
		val = pValue->asInt32();
	}
	return found;
}


bool Deserializer::deserialize(const std::string& name, bool isMandatory, Poco::UInt32& val)
{
	bool found = false;
	JSValue::Ptr pValue = findValue(name, isMandatory, found);
	if (found)
	{
		val = pValue->asUInt32();
	}
	return found;
}


bool Deserializer::deserialize(const std::string& name, bool isMandatory, long& val)
{
	bool found = false;
	JSValue::Ptr pValue = findValue(name, isMandatory, found);
	if (found)
	{
#ifdef POCO_INT64_IS_LONG
		val = pValue->asInt64();
#else
		val = pValue->asInt32();
#endif
	}
	return found;
}


bool Deserializer::deserialize(const std::string& name, bool isMandatory, unsigned long& val)
{
	bool found = false;
	JSValue::Ptr pValue = findValue(name, isMandatory, found);
	if (found)
	{
#ifdef POCO_INT64_IS_LONG
		val = pValue->asUInt64();
#else
		val = pValue->asUInt32();
#endif
	}
	return found;
}


#ifndef POCO_INT64_IS_LONG
bool Deserializer::deserialize(const std::string& name, bool isMandatory, Poco::Int64& val)
{
	bool found = false;
	JSValue::Ptr pValue = findValue(name, isMandatory, found);
	if (found)
	{
		val = pValue->asInt64();
	}
	return found;
}


bool Deserializer::deserialize(const std::string& name, bool isMandatory, Poco::UInt64& val)
{
	bool found = false;
	JSValue::Ptr pValue = findValue(name, isMandatory, found);
	if (found)
	{
		val = pValue->asUInt64();
	}
	return found;
}
#endif


bool Deserializer::deserialize(const std::string& name, bool isMandatory, float& val)
{
	bool found = false;
	JSValue::Ptr pValue = findValue(name, isMandatory, found);
	if (found)
	{
		val = static_cast<float>(pValue->asDouble());
	}
	return found;
}


bool Deserializer::deserialize(const std::string& name, bool isMandatory, double& val)
{
	bool found = false;
	JSValue::Ptr pValue = findValue(name, isMandatory, found);
	if (found)
	{
		val = pValue->asDouble();
	}
	return found;
}


bool Deserializer::deserialize(const std::string& name, bool isMandatory, bool& val)
{
	bool found = false;
	JSValue::Ptr pValue = findValue(name, isMandatory, found);
	if (found)
	{
		val = pValue->asBool();
	}
	return found;
}


bool Deserializer::deserialize(const std::string& name, bool isMandatory, char& val)
{
	bool found = false;
	JSValue::Ptr pValue = findValue(name, isMandatory, found);
	if (found)
	{
		const std::string& str = pValue->stringValue();
		if (str.size() == 1)
			val = str[0];
		else
			throw Poco::RemotingNG::DeserializerException(std::string("Illegal char value ") + str);
	}
	return found;
}


bool Deserializer::deserialize(const std::string& name, bool isMandatory, std::string& val)
{
	bool found = false;
	JSValue::Ptr pValue = findValue(name, isMandatory, found);
	if (found)
	{
		val = pValue->stringValue();
	}
	return found;
}


bool Deserializer::deserialize(const std::string& name, bool isMandatory, std::vector<char>& val)
{
	bool found = false;
	JSValue::Ptr pValue = findValue(name, isMandatory, found);
	if (found)
	{
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


void Deserializer::setupImpl(std::istream& in)
{
	resetImpl();
	JSONParser parser(in);
	_objectStack.push_back(parser.parse());
	int ch = in.get();
	while (ch != -1) ch = in.get();
}


JSValue::Ptr Deserializer::findValue(const std::string& name, bool isMandatory, bool& found)
{
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
	else if (_objectStack.back()->has(name))
	{
		found = true;
		return (*_objectStack.back())[name];
	}
	else if (isMandatory)
	{
		throw Poco::RemotingNG::MissingParamException(name);
	}
	return JSValue::Ptr();
}


void Deserializer::resetImpl()
{
	_objectStack.clear();
	_indexStack.clear();
}


void Deserializer::associateSerializer(Serializer& ser)
{
	_pAssociatedSerializer = &ser;
}


} } } // namespace Poco::RemotingNG::JSONRPC
