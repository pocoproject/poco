//
// Transcoder.cpp
//
// Library: Data
// Package: DataCore
// Module:  Transcoder
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Data/Transcoder.h"


namespace Poco {
namespace Data {


Transcoder::Transcoder(Poco::TextEncoding::Ptr pFromEncoding,
	Poco::TextEncoding::Ptr pToEncoding):
	_pFromEncoding(pFromEncoding),
	_pToEncoding(pToEncoding),
	_pConverter(new Poco::TextConverter(*_pFromEncoding, *_pToEncoding)),
	_pReverseConverter(new Poco::TextConverter(*_pToEncoding, *_pFromEncoding))
{
}


Transcoder::~Transcoder()
{
}


Transcoder::Ptr Transcoder::create(Poco::TextEncoding::Ptr pFromEncoding,
	Poco::TextEncoding::Ptr pToEncoding)
{
	Ptr pTranscoder;

	if (!pFromEncoding && !pToEncoding)
		return pTranscoder;

	if (!pFromEncoding) pFromEncoding = Poco::TextEncoding::find("UTF-8");
	if (!pToEncoding) pToEncoding = Poco::TextEncoding::find("UTF-8");

	if (pToEncoding->isA(pFromEncoding->canonicalName()))
		return pTranscoder;
	
	pTranscoder.reset(new Transcoder(pFromEncoding, pToEncoding));
	return pTranscoder;
}


void Transcoder::transcode(const std::string& from, std::string& to)
{
	if (_pConverter)
	{
		to.clear();
		_pConverter->convert(from, to);
	}
}


void Transcoder::reverseTranscode(const std::string& from, std::string& to)
{
	if (_pConverter)
	{
		to.clear();
		_pReverseConverter->convert(from, to);
	}
}


} } // namespace Poco::Data
