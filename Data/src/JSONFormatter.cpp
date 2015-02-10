//
// JSONFormatter.cpp
//
// $Id: //poco/Main/Data/src/JSONFormatter.cpp#1 $
//
// Library: Data
// Package: DataCore
// Module:  JSONFormatter
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Data/JSONFormatter.h"
#include "Poco/String.h"
#include "Poco/JSONString.h"
#include "Poco/Format.h"


using Poco::trimInPlace;
using Poco::format;
using Poco::toJSON;


namespace Poco {
namespace Data {


	JSONFormatter::JSONFormatter() : _rowCounter(0)
	{
	}


	JSONFormatter::~JSONFormatter()
	{
	}


	std::string& JSONFormatter::formatValues(const ValueVec& vals, std::string& formattedValues)
	{
		std::ostringstream str;

		str << "{\"count\":" << getTotalRowCount() << ",\"rows\":[{";

		std::string pref = str.str();
		if (prefix() != pref) setPrefix(pref);
		else
		{
			str.str("");
			str << ",{";
		}

		if (postfix().empty()) setPostfix("]}");

		ValueVec::const_iterator it = vals.begin();
		ValueVec::const_iterator end = vals.end();
		NameVec::iterator nIt = _pNames->begin();
		NameVec::iterator nEnd = _pNames->end();
		for (; it != end && nIt != nEnd; ++nIt)
		{
			if (!it->isEmpty())
			{
				if (it->isString())
				{
					std::string val = it->convert<std::string>();
					trimInPlace(val);
					str << '"' << *nIt << "\":" <<  toJSON(val);
				}
				else
					str << '"' << *nIt << "\":" << it->convert<std::string>();
			}
			else
				str << '"' << *nIt << "\":null";

			if (++it != end) str << ',';
		}

		str << '}';
		++_rowCounter;
		return formattedValues = str.str();
	}


} }// namespace Poco::Data
