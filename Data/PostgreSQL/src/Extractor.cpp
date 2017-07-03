//
// Extractor.cpp
//
// $Id: //poco/1.4/Data/PostgreSQL/src/Extractor.cpp#1 $
//
// Library: Data/PostgreSQL
// Package: PostgreSQL
// Module:  Extractor
//
// Copyright (c) 2015, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Data/PostgreSQL/Extractor.h"

#include "Poco/Data/Date.h"
#include "Poco/Data/Time.h"

#include "Poco/NumberParser.h"
#include "Poco/DateTimeParser.h"

#include <limits>

namespace Poco {
namespace Data {
namespace PostgreSQL {


Extractor::Extractor(StatementExecutor& st /*, ResultMetadata& md */)
: _statementExecutor (st)
{
}


Extractor::~Extractor()
{
}


bool Extractor::extract(std::size_t pos, Poco::Int8& val)
{

	OutputParameter outputParameter = extractPreamble(pos);

	int tempVal;

	if	(	  isColumnNull(outputParameter)
		 || ! Poco::NumberParser::tryParse(outputParameter.pData(), tempVal)
		)
	{
		return false;
	}

	val = tempVal;

	return true;
}


bool Extractor::extract(std::size_t pos, Poco::UInt8& val)
{
	OutputParameter outputParameter = extractPreamble(pos);

	unsigned int tempVal;

	if	(	  isColumnNull(outputParameter)
		 || ! Poco::NumberParser::tryParseUnsigned(outputParameter.pData(), tempVal)
		)
	{
		return false;
	}

	val = tempVal;

	return true;
}


bool Extractor::extract(std::size_t pos, Poco::Int16& val)
{
	OutputParameter outputParameter = extractPreamble(pos);

	int tempVal;

	if	(	isColumnNull(outputParameter)
		 || ! Poco::NumberParser::tryParse(outputParameter.pData(), tempVal)
		)
	{
		return false;
	}

	val = tempVal;

	return true;
}


bool Extractor::extract(std::size_t pos, Poco::UInt16& val)
{
	OutputParameter outputParameter = extractPreamble(pos);

	unsigned int tempVal;

	if	(	isColumnNull(outputParameter)
		 || ! Poco::NumberParser::tryParseUnsigned(outputParameter.pData(), tempVal)
		)
	{
		return false;
	}

	val = tempVal;

	return true;
}


bool Extractor::extract(std::size_t pos, Poco::Int32& val)
{
	OutputParameter outputParameter = extractPreamble(pos);

	if	(	isColumnNull(outputParameter)
		 || ! Poco::NumberParser::tryParse(outputParameter.pData(), val)
		)
	{
		return false;
	}

	return true;
}


bool Extractor::extract(std::size_t pos, Poco::UInt32& val)
{
	OutputParameter outputParameter = extractPreamble(pos);

	if	(	isColumnNull(outputParameter)
		 || ! Poco::NumberParser::tryParseUnsigned(outputParameter.pData(), val)
		)
	{
		return false;
	}

	return true;
}


bool Extractor::extract(std::size_t pos, Poco::Int64& val)
{
	OutputParameter outputParameter = extractPreamble(pos);

	if	(	isColumnNull(outputParameter)
		 || ! Poco::NumberParser::tryParse64(outputParameter.pData(), val)
		)
	{
		return false;
	}

	return true;
}


bool Extractor::extract(std::size_t pos, Poco::UInt64& val)
{
	OutputParameter outputParameter = extractPreamble(pos);

	if	(	isColumnNull(outputParameter)
		 || ! Poco::NumberParser::tryParseUnsigned64(outputParameter.pData(), val)
		)
	{
		return false;
	}

	return true;
}


#ifndef POCO_LONG_IS_64_BIT
bool Extractor::extract(std::size_t pos, long& val)
{
	OutputParameter outputParameter = extractPreamble(pos);

	Poco::Int64 tempVal;

	if	(	isColumnNull(outputParameter)
		 || ! Poco::NumberParser::tryParse64(outputParameter.pData(), tempVal)
		)
	{
		return false;
	}

	val = tempVal;

	return true;
}


bool Extractor::extract(std::size_t pos, unsigned long& val)
{
	OutputParameter outputParameter = extractPreamble(pos);

	Poco::UInt64 tempVal;

	if	(	isColumnNull(outputParameter)
		 || ! Poco::NumberParser::tryParseUnsigned64(outputParameter.pData(), tempVal)
		)
	{
		return false;
	}

	val = tempVal;

	return true;
}
#endif


bool Extractor::extract(std::size_t pos, bool& val)
{
	OutputParameter outputParameter = extractPreamble(pos);

	if	(	isColumnNull(outputParameter))
	{
		return false;
	}

	if ('t' == *outputParameter.pData())
	{
		val = true;
	}
	else
	{
		val = false;
	}

	return true;
}


bool Extractor::extract(std::size_t pos, float& val)
{
	OutputParameter outputParameter = extractPreamble(pos);

	double tempVal;

	if	(	isColumnNull(outputParameter)
		 || ! Poco::NumberParser::tryParseFloat(outputParameter.pData(), tempVal)
		)
	{
		return false;
	}

	val = tempVal;

	return true;
}


bool Extractor::extract(std::size_t pos, double& val)
{
	OutputParameter outputParameter = extractPreamble(pos);

	if	(	isColumnNull(outputParameter)
		 || ! Poco::NumberParser::tryParseFloat(outputParameter.pData(), val)
		)
	{
		return false;
	}

	return true;
}


bool Extractor::extract(std::size_t pos, char& val)
{
	OutputParameter outputParameter = extractPreamble(pos);

	if	(isColumnNull(outputParameter))
	{
		return false;
	}

	val = *outputParameter.pData();

	return true;
}


bool Extractor::extract(std::size_t pos, std::string& val)
{
	OutputParameter outputParameter = extractPreamble(pos);

	if (isColumnNull(outputParameter))
	{
		return false;
	}

	val.assign(outputParameter.pData(), outputParameter.size());

	return true;
}


bool Extractor::extract(std::size_t pos, Poco::Data::BLOB& val)
{
	OutputParameter outputParameter = extractPreamble(pos);

	if (isColumnNull(outputParameter))
	{
		return false;
	}

	// convert the PostgreSQL text format to binary and append to the BLOB
	// Format: \x10843029479abcf ...  two characters for every byte
	//
	//  The code below can be made more efficient by converting more than one byte at a time
	//  also if BLOB had a resize method it would be useful to allocate memory in one
	//  attempt.
	//

	const char * pBLOB = reinterpret_cast<const char*>(outputParameter.pData());
	std::size_t BLOBSize  = outputParameter.size();

	if	(	'\\' == pBLOB[0]
		 && 'x'  == pBLOB[1]	// preamble to BYTEA data format in text form is \x
		)
	{
		BLOBSize -= 2;  // lose the preamble
		BLOBSize /= 2;   // each byte is encoded as two text characters

		for (int i = 0; i < BLOBSize * 2; i += 2)
		{
			std::string buffer(&pBLOB[i + 2], 2);
			unsigned int binaryBuffer = 0;
			if (Poco::NumberParser::tryParseHex(buffer, binaryBuffer))
			{
				unsigned char finalBinaryBuffer = binaryBuffer; // downsize
				val.appendRaw(&finalBinaryBuffer, 1);
			}
		}
	}
	return true;
}


bool Extractor::extract(std::size_t pos, Poco::Data::CLOB& val)
{
	OutputParameter outputParameter = extractPreamble(pos);

	if (isColumnNull(outputParameter))
	{
		return false;
	}

	val.assignRaw(outputParameter.pData(), outputParameter.size());

	return true;
}

bool Extractor::extract(std::size_t pos, DateTime& val)
{
	OutputParameter outputParameter = extractPreamble(pos);

	if (isColumnNull(outputParameter))
	{
		return false;
	}

	int tzd = -1;
	DateTime dateTime;

	if (! DateTimeParser::tryParse(outputParameter.pData(), dateTime, tzd))
	{
		return false;
	}

	dateTime.makeUTC(tzd);

	val = dateTime;

	return true;
}


bool Extractor::extract(std::size_t pos, Date& val)
{
	OutputParameter outputParameter = extractPreamble(pos);

	if (isColumnNull(outputParameter))
	{
		return false;
	}

	int tzd = -1;
	DateTime dateTime;

	if (! DateTimeParser::tryParse(outputParameter.pData(), dateTime, tzd))
	{
		return false;
	}

	dateTime.makeUTC(tzd);

	val.assign(dateTime.year(), dateTime.month(), dateTime.day());

	return true;
}


bool Extractor::extract(std::size_t pos, Time& val)
{
	OutputParameter outputParameter = extractPreamble(pos);

	if (isColumnNull(outputParameter))
	{
		return false;
	}

	int tzd = -1;
	DateTime dateTime;

	if (! DateTimeParser::tryParse("%H:%M:%s%z", outputParameter.pData(), dateTime, tzd))
	{
		return false;
	}

	// dateTime.makeUTC(tzd); // TODO
	// Note: Poco::Data::Time should be extended to support the fractional components of Poco::DateTime

	val.assign(dateTime.hour(), dateTime.minute(), dateTime.second());

	return true;
}


bool Extractor::extract(std::size_t pos, Any& val)
{
	return extractStringImpl (pos, val);
}


bool Extractor::extract(std::size_t pos, Dynamic::Var& val)
{
	return extractStringImpl (pos, val);
}


bool Extractor::isNull(std::size_t col, std::size_t row)
{
	OutputParameter outputParameter = extractPreamble(col);

	if (isColumnNull(outputParameter))
	{
		return true;
	}

	return false;
}


void Extractor::reset()
{
	AbstractExtractor::reset();
}


const OutputParameter&
Extractor::extractPreamble(std::size_t aPosition) const
{
	if (_statementExecutor.columnsReturned() <= aPosition)
	{
		throw PostgreSQLException("Extractor: attempt to extract more parameters than query result contains");
	}

	return _statementExecutor.resultColumn(aPosition);
}


bool
Extractor::isColumnNull (const OutputParameter& anOutputParameter) const
{
	return anOutputParameter.isNull()
		|| 0 == anOutputParameter.pData();
}


//////////////
// Not implemented
//////////////


bool Extractor::extract(std::size_t , std::vector<Poco::Int8>&)
{
	throw NotImplementedException("std::vector extractor must be implemented.");
}


bool Extractor::extract(std::size_t , std::deque<Poco::Int8>&)
{
	throw NotImplementedException("std::deque extractor must be implemented.");
}


bool Extractor::extract(std::size_t , std::list<Poco::Int8>&)
{
	throw NotImplementedException("std::list extractor must be implemented.");
}
	
	
bool Extractor::extract(std::size_t , std::vector<Poco::UInt8>&)
{
	throw NotImplementedException("std::vector extractor must be implemented.");
}


bool Extractor::extract(std::size_t , std::deque<Poco::UInt8>&)
{
	throw NotImplementedException("std::deque extractor must be implemented.");
}


bool Extractor::extract(std::size_t , std::list<Poco::UInt8>&)
{
	throw NotImplementedException("std::list extractor must be implemented.");
}


bool Extractor::extract(std::size_t , std::vector<Poco::Int16>&)
{
	throw NotImplementedException("std::vector extractor must be implemented.");
}


bool Extractor::extract(std::size_t , std::deque<Poco::Int16>&)
{
	throw NotImplementedException("std::deque extractor must be implemented.");
}


bool Extractor::extract(std::size_t , std::list<Poco::Int16>&)
{
	throw NotImplementedException("std::list extractor must be implemented.");
}


bool Extractor::extract(std::size_t , std::vector<Poco::UInt16>&)
{
	throw NotImplementedException("std::vector extractor must be implemented.");
}


bool Extractor::extract(std::size_t , std::deque<Poco::UInt16>&)
{
	throw NotImplementedException("std::deque extractor must be implemented.");
}


bool Extractor::extract(std::size_t , std::list<Poco::UInt16>&)
{
	throw NotImplementedException("std::list extractor must be implemented.");
}


bool Extractor::extract(std::size_t , std::vector<Poco::Int32>&)
{
	throw NotImplementedException("std::vector extractor must be implemented.");
}


bool Extractor::extract(std::size_t , std::deque<Poco::Int32>&)
{
	throw NotImplementedException("std::deque extractor must be implemented.");
}


bool Extractor::extract(std::size_t , std::list<Poco::Int32>&)
{
	throw NotImplementedException("std::list extractor must be implemented.");
}


bool Extractor::extract(std::size_t , std::vector<Poco::UInt32>&)
{
	throw NotImplementedException("std::vector extractor must be implemented.");
}


bool Extractor::extract(std::size_t , std::deque<Poco::UInt32>&)
{
	throw NotImplementedException("std::deque extractor must be implemented.");
}


bool Extractor::extract(std::size_t , std::list<Poco::UInt32>&)
{
	throw NotImplementedException("std::list extractor must be implemented.");
}


bool Extractor::extract(std::size_t , std::vector<Poco::Int64>&)
{
	throw NotImplementedException("std::vector extractor must be implemented.");
}


bool Extractor::extract(std::size_t , std::deque<Poco::Int64>&)
{
	throw NotImplementedException("std::deque extractor must be implemented.");
}


bool Extractor::extract(std::size_t , std::list<Poco::Int64>&)
{
	throw NotImplementedException("std::list extractor must be implemented.");
}


bool Extractor::extract(std::size_t , std::vector<Poco::UInt64>&)
{
	throw NotImplementedException("std::vector extractor must be implemented.");
}


bool Extractor::extract(std::size_t , std::deque<Poco::UInt64>&)
{
	throw NotImplementedException("std::deque extractor must be implemented.");
}


bool Extractor::extract(std::size_t , std::list<Poco::UInt64>&)
{
	throw NotImplementedException("std::list extractor must be implemented.");
}


#ifndef POCO_LONG_IS_64_BIT
bool Extractor::extract(std::size_t , std::vector<long>&)
{
	throw NotImplementedException("std::vector extractor must be implemented.");
}


bool Extractor::extract(std::size_t , std::deque<long>&)
{
	throw NotImplementedException("std::deque extractor must be implemented.");
}


bool Extractor::extract(std::size_t , std::list<long>&)
{
	throw NotImplementedException("std::list extractor must be implemented.");
}
#endif


bool Extractor::extract(std::size_t , std::vector<bool>&)
{
	throw NotImplementedException("std::vector extractor must be implemented.");
}


bool Extractor::extract(std::size_t , std::deque<bool>&)
{
	throw NotImplementedException("std::deque extractor must be implemented.");
}


bool Extractor::extract(std::size_t , std::list<bool>&)
{
	throw NotImplementedException("std::list extractor must be implemented.");
}


bool Extractor::extract(std::size_t , std::vector<float>&)
{
	throw NotImplementedException("std::vector extractor must be implemented.");
}


bool Extractor::extract(std::size_t , std::deque<float>&)
{
	throw NotImplementedException("std::deque extractor must be implemented.");
}


bool Extractor::extract(std::size_t , std::list<float>&)
{
	throw NotImplementedException("std::list extractor must be implemented.");
}


bool Extractor::extract(std::size_t , std::vector<double>&)
{
	throw NotImplementedException("std::vector extractor must be implemented.");
}


bool Extractor::extract(std::size_t , std::deque<double>&)
{
	throw NotImplementedException("std::deque extractor must be implemented.");
}


bool Extractor::extract(std::size_t , std::list<double>&)
{
	throw NotImplementedException("std::list extractor must be implemented.");
}


bool Extractor::extract(std::size_t , std::vector<char>&)
{
	throw NotImplementedException("std::vector extractor must be implemented.");
}


bool Extractor::extract(std::size_t , std::deque<char>&)
{
	throw NotImplementedException("std::deque extractor must be implemented.");
}


bool Extractor::extract(std::size_t , std::list<char>&)
{
	throw NotImplementedException("std::list extractor must be implemented.");
}


bool Extractor::extract(std::size_t , std::vector<std::string>&)
{
	throw NotImplementedException("std::vector extractor must be implemented.");
}


bool Extractor::extract(std::size_t , std::deque<std::string>&)
{
	throw NotImplementedException("std::deque extractor must be implemented.");
}


bool Extractor::extract(std::size_t , std::list<std::string>&)
{
	throw NotImplementedException("std::list extractor must be implemented.");
}


bool Extractor::extract(std::size_t , std::vector<BLOB>&)
{
	throw NotImplementedException("std::vector extractor must be implemented.");
}


bool Extractor::extract(std::size_t , std::deque<BLOB>&)
{
	throw NotImplementedException("std::deque extractor must be implemented.");
}


bool Extractor::extract(std::size_t , std::list<BLOB>&)
{
	throw NotImplementedException("std::list extractor must be implemented.");
}


bool Extractor::extract(std::size_t , std::vector<CLOB>&)
{
	throw NotImplementedException("std::vector extractor must be implemented.");
}


bool Extractor::extract(std::size_t , std::deque<CLOB>&)
{
	throw NotImplementedException("std::deque extractor must be implemented.");
}


bool Extractor::extract(std::size_t , std::list<CLOB>&)
{
	throw NotImplementedException("std::list extractor must be implemented.");
}


bool Extractor::extract(std::size_t , std::vector<DateTime>&)
{
	throw NotImplementedException("std::vector extractor must be implemented.");
}


bool Extractor::extract(std::size_t , std::deque<DateTime>&)
{
	throw NotImplementedException("std::deque extractor must be implemented.");
}


bool Extractor::extract(std::size_t , std::list<DateTime>&)
{
	throw NotImplementedException("std::list extractor must be implemented.");
}


bool Extractor::extract(std::size_t , std::vector<Date>&)
{
	throw NotImplementedException("std::vector extractor must be implemented.");
}


bool Extractor::extract(std::size_t , std::deque<Date>&)
{
	throw NotImplementedException("std::deque extractor must be implemented.");
}


bool Extractor::extract(std::size_t , std::list<Date>&)
{
	throw NotImplementedException("std::list extractor must be implemented.");
}


bool Extractor::extract(std::size_t , std::vector<Time>&)
{
	throw NotImplementedException("std::vector extractor must be implemented.");
}


bool Extractor::extract(std::size_t , std::deque<Time>&)
{
	throw NotImplementedException("std::deque extractor must be implemented.");
}


bool Extractor::extract(std::size_t , std::list<Time>&)
{
	throw NotImplementedException("std::list extractor must be implemented.");
}


bool Extractor::extract(std::size_t , std::vector<Any>&)
{
	throw NotImplementedException("std::vector extractor must be implemented.");
}


bool Extractor::extract(std::size_t , std::deque<Any>&)
{
	throw NotImplementedException("std::deque extractor must be implemented.");
}


bool Extractor::extract(std::size_t , std::list<Any>&)
{
	throw NotImplementedException("std::list extractor must be implemented.");
}


bool Extractor::extract(std::size_t , std::vector<Dynamic::Var>&)
{
	throw NotImplementedException("std::vector extractor must be implemented.");
}


bool Extractor::extract(std::size_t , std::deque<Dynamic::Var>&)
{
	throw NotImplementedException("std::deque extractor must be implemented.");
}


bool Extractor::extract(std::size_t , std::list<Dynamic::Var>&)
{
	throw NotImplementedException("std::list extractor must be implemented.");
}



} } } // namespace Poco::Data::PostgreSQL
