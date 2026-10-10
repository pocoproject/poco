//
// FormSerializer.cpp
//
// Library: RemotingNG/REST
// Package: REST
// Module:  FormSerializer
//
// Copyright (c) 2010-2016, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/RemotingNG/REST/FormSerializer.h"
#include "Poco/Net/PartSource.h"
#include "Poco/MemoryStream.h"
#include <sstream>


namespace Poco {
namespace RemotingNG {
namespace REST {


class CharVectorPartSource: public Poco::Net::PartSource
{
public:
	CharVectorPartSource(const std::vector<char>& data, const std::string& filename, const std::string& contentType):
		Poco::Net::PartSource(contentType),
		_data(data),
		_filename(filename),
		_contentType(contentType),
		_stream(&data[0], data.size())
	{
	}

	~CharVectorPartSource()
	{
	}

	std::istream& stream()
	{
		return _stream;
	}

	const std::string& filename() const
	{
		return _filename;
	}

	std::streamsize getContentLength() const
	{
		return _data.size();
	}

private:
	const std::vector<char>& _data;
	std::string _filename;
	std::string _contentType;
	Poco::MemoryInputStream _stream;
};


FormSerializer::FormSerializer(Poco::Net::HTTPRequest& request):
	_pRequest(&request),
	_pResponse(0),
	_pStream(0)
{
}


FormSerializer::FormSerializer(Poco::Net::HTTPRequest& request, std::ostream& stream):
	_pRequest(&request),
	_pResponse(0),
	_pStream(&stream)
{
	if (_pRequest->getContentType() == Poco::Net::HTMLForm::ENCODING_MULTIPART)
	{
		_form.setEncoding(Poco::Net::HTMLForm::ENCODING_MULTIPART);
	}
}


FormSerializer::FormSerializer(Poco::Net::HTTPResponse& response, std::ostream& stream):
	_pRequest(0),
	_pResponse(&response),
	_pStream(&stream)
{
	if (_pResponse->getContentType() == Poco::Net::HTMLForm::ENCODING_MULTIPART)
	{
		_form.setEncoding(Poco::Net::HTMLForm::ENCODING_MULTIPART);
	}
}


FormSerializer::~FormSerializer()
{
}


void FormSerializer::close()
{
	if (_pRequest)
	{
		if (_pStream)
		{
			_form.prepareSubmit(*_pRequest);
			_form.write(*_pStream);
		}
		else
		{
			std::ostringstream str;
			str << _pRequest->getURI() << '?';
			_form.write(str);
			_pRequest->setURI(str.str());
		}
	}
	else if (_pResponse)
	{
		if (_pStream)
		{
			_pResponse->setContentType(_form.getEncoding());
			_form.write(*_pStream);
		}
	}
}


void FormSerializer::serializeData(const std::string& name, const std::string& val)
{
	_form.set(name, val);
}


void FormSerializer::serializeBuffer(const std::string& name, const std::vector<char>& val)
{
	if (_form.getEncoding() == Poco::Net::HTMLForm::ENCODING_MULTIPART)
	{
		std::string contentType = getProperty(PROP_CONTENT_TYPE, APPLICATION_OCTET_STREAM);
		_form.addPart(name, new CharVectorPartSource(val, name, contentType));
	}
	else
	{
		ScalarSerializer::serializeBuffer(name, val);
	}
}


const std::string FormSerializer::APPLICATION_OCTET_STREAM("application/octet-stream");


} } } // namespace Poco::RemotingNG::REST
