//
// JSONRowFormatter.cpp
//
// $Id: //poco/Main/Data/src/JSONRowFormatter.cpp#1 $
//
// Library: Data
// Package: DataCore
// Module:  JSONRowFormatter
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Data/JSONRowFormatter.h"
#include "Poco/String.h"
#include "Poco/JSONString.h"
#include "Poco/Format.h"


using Poco::trimInPlace;
using Poco::format;
using Poco::toJSON;


namespace Poco {
namespace Data {


const int JSONRowFormatter::JSON_FMT_MODE_SMALL;
const int JSONRowFormatter::JSON_FMT_MODE_ROW_COUNT;
const int JSONRowFormatter::JSON_FMT_MODE_COLUMN_NAMES;
const int JSONRowFormatter::JSON_FMT_MODE_FULL;


JSONRowFormatter::JSONRowFormatter(int mode) : RowFormatter("{", "]}"),
	_firstTime(true)
{
	if (mode == JSON_FMT_MODE_FULL)
	{
		mode |= JSON_FMT_MODE_ROW_COUNT;
		mode |= JSON_FMT_MODE_COLUMN_NAMES;
		//setPostfix("]}");
	}

	setJSONMode(mode);
}


JSONRowFormatter::~JSONRowFormatter()
{
}


void JSONRowFormatter::adjustPrefix()
{
	if (printRowCount())
	{
		std::ostringstream ostr;
		ostr << "{\"count\":" << getTotalRowCount() << ",";
		if (_mode & JSON_FMT_MODE_FULL)
			ostr << '[';
		setPrefix(ostr.str());
	}
}


void JSONRowFormatter::setJSONMode(int mode)
{
	if (mode < JSON_FMT_MODE_SMALL ||
		mode > (JSON_FMT_MODE_SMALL | JSON_FMT_MODE_ROW_COUNT | JSON_FMT_MODE_COLUMN_NAMES | JSON_FMT_MODE_FULL))
	{
		throw Poco::InvalidArgumentException(
			Poco::format("JSONRowFormatter mode must be between "
			"%d (JSON_FMT_MODE_SMALL) and %d (JSON_FMT_MODE_FULL)",
			JSON_FMT_MODE_SMALL,
			JSON_FMT_MODE_FULL));
	}

	_mode = mode;
	if (!(_mode & JSON_FMT_MODE_SMALL) && !(_mode & JSON_FMT_MODE_FULL))
		_mode |= JSON_FMT_MODE_SMALL;
	else if (_mode & JSON_FMT_MODE_FULL)
	{
		_mode |= JSON_FMT_MODE_ROW_COUNT;
	}

	adjustPrefix();
}


std::string& JSONRowFormatter::formatValues(const ValueVec& vals, std::string& formattedValues)
{
	std::ostringstream str;
	if (!_firstTime) str << ',';
	if (isSmall())
	{
		if (_firstTime)
		{
			if (printColumnNames())
				str << ",\"values\":";

			str << '[';
		}

		str << '[';
		ValueVec::const_iterator it = vals.begin();
		ValueVec::const_iterator end = vals.end();
		for (; it != end;)
		{
			if (!it->isEmpty())
			{
				if (it->isString() || it->isDate() || it->isTime())
				{
					std::string val = it->convert<std::string>();
					trimInPlace(val);
					str << toJSON(val);
				}
				else
					str << it->convert<std::string>();
			}
			else
				str << "null";

			if (++it == end) break;
			
			str << ',';
		}
		str << ']';
	}
	else if (isFull())
	{
		str << '{';
		ValueVec::const_iterator it = vals.begin();
		ValueVec::const_iterator end = vals.end();
		NameVec::iterator nIt = _pNames->begin();
		NameVec::iterator nEnd = _pNames->end();
		for (; it != end && nIt != nEnd; ++nIt)
		{
			if (!it->isEmpty())
			{
				if (it->isString() || it->isDate() || it->isTime())
				{
					std::string val = it->convert<std::string>();
					trimInPlace(val);
					str << '"' << *nIt << "\":" << toJSON(val);
				}
				else
					str << '"' << *nIt << "\":" << it->convert<std::string>();
			}
			else
				str << '"' << *nIt << "\":null";

			if (++it != end) str << ',';
		}
		str << '}';
	}

	_firstTime = false;
	return formattedValues = str.str();
}


std::string& JSONRowFormatter::formatNames(const NameVecPtr pNames, std::string& formattedNames)
{
	if (isFull())
	{
		// names are used in formatValues
		if (pNames && !_pNames) _pNames = pNames;
		return formattedNames = "";
	}
	else if (printColumnNames())
	{
		std::ostringstream ostr;
		ostr << "\"names\":[";
		for (NameVec::const_iterator it = pNames->begin(),
			end = pNames->end();;)
		{
			ostr << '"' << *it << '"';
			if (++it == end) break;
			ostr << ',';
		}
		ostr << "]";
		return formattedNames = ostr.str();
	}

	return formattedNames = "";
}


} }// namespace Poco::Data
