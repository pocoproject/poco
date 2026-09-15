//
// FormDeserializer.cpp
//
// Library: RemotingNG/REST
// Package: REST
// Module:  FormDeserializer
//
// Copyright (c) 2010-2016, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties,
// copied or duplicated in any form, in whole or in part.
//


#include "Poco/RemotingNG/REST/FormDeserializer.h"
#include "Poco/RemotingNG/REST/FormSerializer.h"
#include "Poco/RemotingNG/RemotingException.h"
#include "Poco/Net/PartHandler.h"
#include "Poco/NumberParser.h"
#include "Poco/Base64Decoder.h"
#include "Poco/String.h"
#include <algorithm>
#include <sstream>


namespace Poco {
namespace RemotingNG {
namespace REST {


class PartHandler: public Poco::Net::PartHandler
{
public:
	PartHandler(FormDeserializer& deserializer):
		_deserializer(deserializer)
	{
	}

	void handlePart(const Poco::Net::MessageHeader& header, std::istream& stream)
	{
		std::string name;
		if (header.has(CONTENT_DISPOSITION))
		{
			std::string disp;
			Poco::Net::NameValueCollection params;
			Poco::Net::MessageHeader::splitParameters(header[CONTENT_DISPOSITION], disp, params);
			name = params.get(NAME, EMPTY);
		}

		if (!name.empty())
		{
			_deserializer.handlePart(name, stream);
		}
	}

	static const std::string CONTENT_DISPOSITION;
	static const std::string NAME;
	static const std::string EMPTY;

private:
	FormDeserializer& _deserializer;
};


const std::string PartHandler::CONTENT_DISPOSITION("Content-Disposition");
const std::string PartHandler::NAME("name");
const std::string PartHandler::EMPTY;


FormDeserializer::FormDeserializer(Poco::Net::HTTPRequest& request):
	_form(request)
{
}


FormDeserializer::FormDeserializer(Poco::Net::HTTPRequest& request, std::istream& stream)
{
	PartHandler partHandler(*this);
	_form.load(request, stream, partHandler);
}


FormDeserializer::FormDeserializer(Poco::Net::HTTPResponse& response, std::istream& stream)
{
	_form.setEncoding(response.getContentType());
	PartHandler partHandler(*this);
	_form.read(stream, partHandler);
}


FormDeserializer::~FormDeserializer()
{
}


bool FormDeserializer::deserializeNullableBegin(const std::string& name, bool isMandatory, bool& isNull)
{
	isNull = !_form.has(name);
	return !isNull;
}


bool FormDeserializer::deserializeBuffer(const std::string& name, bool isMandatory, std::vector<char>& val)
{
	PartMap::iterator it = _parts.find(name);
	if (it != _parts.end())
	{
		std::swap(val, it->second);
		return true;
	}
	else
	{
		return ScalarDeserializer::deserializeBuffer(name, isMandatory, val);
	}
}


const std::string& FormDeserializer::deserializeData(const std::string& name, bool isMandatory, bool& found)
{
	if (_form.has(name))
	{
		found = true;
		return _form.get(name);
	}
	else if (isMandatory)
	{
		throw Poco::RemotingNG::MissingParamException(name);
	}
	else
	{
		found = false;
		return EMPTY;
	}
}


void FormDeserializer::handlePart(const std::string& name, std::istream& stream)
{
	_parts[name].assign(std::istreambuf_iterator<char>(stream), std::istreambuf_iterator<char>());
}


const std::string FormDeserializer::EMPTY;


} } } // namespace Poco::RemotingNG::REST
