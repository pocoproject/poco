//
// TestDocuments.h
//
// Documents of a given size for the MongoDB tests.
//
// Copyright (c) 2026, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef TestDocuments_INCLUDED
#define TestDocuments_INCLUDED


#include "Poco/MongoDB/Binary.h"
#include "Poco/MongoDB/Document.h"
#include "Poco/Bugcheck.h"


inline Poco::MongoDB::Document::Ptr sizedDocument(Poco::Int32 id, Poco::Int32 size)
	/// Returns {_id: id, p: binary} of exactly size bytes.
{
	poco_assert (size >= 22);
	Poco::MongoDB::Binary::Ptr payload = new Poco::MongoDB::Binary(size - 22, 0);
	// Buffer::clear() zero-fills the buffer; it does not change its size.
	payload->buffer().clear();
	Poco::MongoDB::Document::Ptr doc = new Poco::MongoDB::Document();
	doc->add(std::string("_id"), id);
	doc->add(std::string("p"), payload);
	return doc;
}


#endif // TestDocuments_INCLUDED
