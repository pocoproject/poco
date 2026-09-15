//
// BinaryDeserializer.cpp
//
// Library: RemotingNG
// Package: Serialization
// Module:  BinaryDeserializer
//
// Copyright (c) 2006-2014, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// License: Applied Informatics Software and Source Code License Agreement
//


#include "Poco/RemotingNG/BinaryDeserializer.h"
#include "Poco/RemotingNG/BinarySerializer.h"


namespace Poco {
namespace RemotingNG {


BinaryDeserializer::BinaryDeserializer():
	_messageType(SerializerBase::MESSAGE_REQUEST),
	_curLevel(0)
{
}


BinaryDeserializer::~BinaryDeserializer()
{
}


void BinaryDeserializer::deserializeEndPoint(std::string& oid, std::string& tid)
{
	*_pReader >> tid >> oid;
}


SerializerBase::MessageType BinaryDeserializer::findMessage(std::string& name)
{
	if (_messageName.empty())
	{
		findMessageImpl();
	}
	name = _messageName;
	return _messageType;
}


void BinaryDeserializer::deserializeMessageBegin(const std::string& name, SerializerBase::MessageType type)
{
	if (_messageName.empty())
	{
		findMessageImpl();
	}
	poco_assert (name == _messageName && type == _messageType);
	_curLevel++;
}


void BinaryDeserializer::deserializeMessageEnd(const std::string& /*name*/, SerializerBase::MessageType /*type*/)
{
	_curLevel--;
	poco_assert (_curLevel == 0);

	std::string endTag;
	_pReader->readRaw(static_cast<std::streamsize>(BinarySerializer::MESSAGE_END_TAG.size()), endTag);
	if (endTag != BinarySerializer::MESSAGE_END_TAG)
		throw DeserializerException("no valid message end tag found");
}


bool BinaryDeserializer::deserializeStructBegin(const std::string& /*name*/, bool /*isMandatory*/)
{
	bool ok = handleVector();
	if (ok)
		_curLevel++;
	return ok && checkStream();
}


void BinaryDeserializer::deserializeStructEnd(const std::string& /*name*/)
{
	_curLevel--;
}


bool BinaryDeserializer::deserializeSequenceBegin(const std::string& /*name*/, bool /*isMandatory*/, Poco::UInt32& lengthHint)
{
	bool ok = handleVector();
	if (ok)
	{
		_curLevel++;
		lengthHint = 0;
		_pReader->read7BitEncoded(lengthHint);
		_sequenceLengths.push(std::make_pair(static_cast<int>(lengthHint), _curLevel));
	}
	return ok && checkStream();
}


void BinaryDeserializer::deserializeSequenceEnd(const std::string& /*name*/)
{
	poco_assert_dbg (!_sequenceLengths.empty());
	poco_assert_dbg (_sequenceLengths.top().first == 0 || _sequenceLengths.top().first == -1);
	poco_assert_dbg (_sequenceLengths.top().second == _curLevel);
	_sequenceLengths.pop();
	_curLevel--;
}


bool BinaryDeserializer::deserializeNullableBegin(const std::string& /*name*/, bool /*isMandatory*/, bool& isNull)
{
	if (!_sequenceLengths.empty())
	{
		if (_curLevel == _sequenceLengths.top().second)
		{
			if (_sequenceLengths.top().first > 0)
			{
				*_pReader >> isNull;
				if (isNull) _sequenceLengths.top().first--;
				return checkStream();
			}
			else _sequenceLengths.top().first--; // to satisfy deserializeSequenceEnd() assertions
			return false;
		}
	}
	*_pReader >> isNull;
	return checkStream();
}


void BinaryDeserializer::deserializeNullableEnd(const std::string& /*name*/)
{
}


bool BinaryDeserializer::deserialize(const std::string& /*name*/, bool /*isMandatory*/, Poco::Int8& value)
{
	bool ok = handleVector();
	if (ok) *_pReader >> value;
	return ok && checkStream();
}


bool BinaryDeserializer::deserialize(const std::string& /*name*/, bool /*isMandatory*/, Poco::UInt8& value)
{
	bool ok = handleVector();
	if (ok) *_pReader >> value;
	return ok && checkStream();
}


bool BinaryDeserializer::deserialize(const std::string& /*name*/, bool /*isMandatory*/, Poco::Int16& value)
{
	bool ok = handleVector();
	if (ok) *_pReader >> value;
	return ok && checkStream();
}


bool BinaryDeserializer::deserialize(const std::string& /*name*/, bool /*isMandatory*/, Poco::UInt16& value)
{
	bool ok = handleVector();
	if (ok) *_pReader >> value;
	return ok && checkStream();
}


bool BinaryDeserializer::deserialize(const std::string& /*name*/, bool /*isMandatory*/, Poco::Int32& value)
{
	bool ok = handleVector();
	if (ok) *_pReader >> value;
	return ok && checkStream();
}


bool BinaryDeserializer::deserialize(const std::string& /*name*/, bool /*isMandatory*/, Poco::UInt32& value)
{
	bool ok = handleVector();
	if (ok) *_pReader >> value;
	return ok && checkStream();
}


bool BinaryDeserializer::deserialize(const std::string& /*name*/, bool /*isMandatory*/, long& value)
{
	bool ok = handleVector();
	if (ok)
	{
		Poco::Int64 tmp;
		*_pReader >> tmp;
		value = static_cast<long>(tmp);
	}
	return ok && checkStream();
}


bool BinaryDeserializer::deserialize(const std::string& /*name*/, bool /*isMandatory*/, unsigned long& value)
{
	bool ok = handleVector();
	if (ok)
	{
		Poco::UInt64 tmp;
		*_pReader >> tmp;
		value = static_cast<unsigned long>(tmp);
	}
	return ok && checkStream();
}


#ifndef POCO_INT64_IS_LONG
bool BinaryDeserializer::deserialize(const std::string& /*name*/, bool /*isMandatory*/, Poco::Int64& value)
{
	bool ok = handleVector();
	if (ok) *_pReader >> value;
	return ok && checkStream();
}


bool BinaryDeserializer::deserialize(const std::string& /*name*/, bool /*isMandatory*/, Poco::UInt64& value)
{
	bool ok = handleVector();
	if (ok) *_pReader >> value;
	return ok && checkStream();
}
#endif


bool BinaryDeserializer::deserialize(const std::string& /*name*/, bool /*isMandatory*/, float& value)
{
	bool ok = handleVector();
	if (ok) *_pReader >> value;
	return ok && checkStream();
}


bool BinaryDeserializer::deserialize(const std::string& /*name*/, bool /*isMandatory*/, double& value)
{
	bool ok = handleVector();
	if (ok) *_pReader >> value;
	return ok && checkStream();
}


bool BinaryDeserializer::deserialize(const std::string& /*name*/, bool /*isMandatory*/, bool& value)
{
	bool ok = handleVector();
	if (ok) *_pReader >> value;
	return ok && checkStream();
}


bool BinaryDeserializer::deserialize(const std::string& /*name*/, bool /*isMandatory*/, char& value)
{
	bool ok = handleVector();
	if (ok) *_pReader >> value;
	return ok && checkStream();
}


bool BinaryDeserializer::deserialize(const std::string& /*name*/, bool /*isMandatory*/, std::string& value)
{
	bool ok = handleVector();
	if (ok) *_pReader >> value;
	return ok && checkStream();
}


bool BinaryDeserializer::deserialize(const std::string& /*name*/, bool /*isMandatory*/, std::vector<char>& value)
{
	bool ok = handleVector();
	if (ok)
	{
		Poco::UInt32 size;
		_pReader->read7BitEncoded(size);
		value.resize(size);
		if (size > 0)
		{
			_pReader->readRaw(&value[0], size);
		}
	}
	return ok && checkStream();
}


void BinaryDeserializer::resetImpl()
{
	_messageName.clear();
	_curLevel = 0;
	while (!_sequenceLengths.empty())
		_sequenceLengths.pop();
}


void BinaryDeserializer::setupImpl(std::istream& istr)
{
	_pReader.reset(new Poco::BinaryReader(istr));
	_pReader->readBOM();
}


bool BinaryDeserializer::handleVector()
{
	if (!_sequenceLengths.empty())
	{
		if (_curLevel == _sequenceLengths.top().second)
		{
			_sequenceLengths.top().first--;
			return _sequenceLengths.top().first >= 0;
		}
	}
	return true;
}


void BinaryDeserializer::findMessageImpl()
{
	std::string startTag;
	_pReader->readRaw(static_cast<std::streamsize>(BinarySerializer::MESSAGE_START_TAG.size()), startTag);
	if (!_pReader->good())
		throw ProtocolException("no message received");
	if (startTag != BinarySerializer::MESSAGE_START_TAG)
		throw DeserializerException("no valid message start tag found");
	Poco::UInt8 code(0);
	*_pReader >> code;
	switch (code)
	{
	case BinarySerializer::MESSAGE_CODE_REQUEST:
		_messageType = SerializerBase::MESSAGE_REQUEST;
		break;
	case BinarySerializer::MESSAGE_CODE_REPLY:
		_messageType = SerializerBase::MESSAGE_REPLY;
		break;
	case BinarySerializer::MESSAGE_CODE_EVENT:
		_messageType = SerializerBase::MESSAGE_EVENT;
		break;
	case BinarySerializer::MESSAGE_CODE_EVENT_REPLY:
		_messageType = SerializerBase::MESSAGE_EVENT_REPLY;
		break;
	case BinarySerializer::MESSAGE_CODE_FAULT:
		_messageType = SerializerBase::MESSAGE_FAULT;
		break;
	default:
		throw DeserializerException("invalid message type code");
	}
	*_pReader >> _messageName;
	if (_messageType == SerializerBase::MESSAGE_FAULT)
	{
		_curLevel++; // simulate deserializeMessageBegin()
		std::string excName;
		std::string excMessage;
		std::string excType;
		*_pReader >> excName >> excMessage >> excType;
		deserializeMessageEnd(_messageName, SerializerBase::MESSAGE_FAULT);
		throw RemoteException(excName, excMessage);
	}
}


} } // namespace Poco::Remoting
