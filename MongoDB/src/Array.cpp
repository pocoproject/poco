//
// Array.cpp
//
// Library: MongoDB
// Package: MongoDB
// Module:  Array
//
// Copyright (c) 2012-2025, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/MongoDB/Array.h"
#include <sstream>


namespace Poco {
namespace MongoDB {


Array::Array():
	Document()
{
}


Array::~Array()
{
}


Element::Ptr Array::get(std::size_t pos) const
{
	std::string name = Poco::NumberFormatter::format(pos);
	return Document::get(name);
}


std::string Array::toString(int indent) const
{
	std::ostringstream oss;

	oss << '[';

	if (indent > 0) oss << std::endl;

	// Use protected accessor to get ordered names
	const auto& names = orderedNames();
	for (auto it = names.begin(), total = names.end(); it != total; ++it)
	{
		if (it != names.begin())
		{
			oss << ',';
			if (indent > 0) oss << std::endl;
		}

		for (int i = 0; i < indent; ++i) oss << ' ';

		Element::Ptr element = Document::get(*it);
		oss << element->toString(indent > 0 ? indent + 2 : 0);
	}

	if (indent > 0)
	{
		oss << std::endl;
		if (indent >= 2) indent -= 2;
		for (int i = 0; i < indent; ++i) oss << ' ';
	}

	oss << ']';

	return oss.str();
}


} } // Namespace Poco::Mongo
