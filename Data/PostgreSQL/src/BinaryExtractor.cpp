//
// BinaryExtractor.cpp
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


#include "Poco/Data/PostgreSQL/BinaryExtractor.h"
#include "Poco/Data/Date.h"
#include "Poco/Data/Time.h"
#include "Poco/NumberParser.h"
#include "Poco/DateTimeParser.h"
#include "Poco/ByteOrder.h"
#include <limits>


namespace Poco {
namespace Data {
namespace PostgreSQL {


namespace
{
	template<typename T>
	static T fromNetwork(T value)
	{
#if defined(POCO_ARCH_BIG_ENDIAN)
		return value;
#else
		T flip = value;
		std::size_t halfSize = sizeof(T) / 2;
		char* flipP = reinterpret_cast<char*>(&flip);

		for (std::size_t i = 0; i < halfSize; i++)
		{
			std::swap(flipP[i], flipP[sizeof(T) - i - 1]);
		}
		return flip;
#endif
	}

	template <typename T>
	bool readBinaryValue(const OutputParameter& param, T& value)
	{
		throw Poco::NotImplementedException("readBinaryValue", typeid(value).name());
	}

	bool readBinaryValue(const OutputParameter& param, bool& value)
	{
		if (param.size() < 1) return false;

		value = *param.pData() != 0;
		return true;
	}

	bool readBinaryValue(const OutputParameter& param, Poco::Int16& value)
	{
		if (param.size() < sizeof(value)) return false;

		Poco::Int16 tempValue;
		std::memcpy(&tempValue, param.pData(), sizeof(tempValue));
		value = Poco::ByteOrder::fromNetwork(tempValue);
		return true;
	}

	bool readBinaryValue(const OutputParameter& param, Poco::Int32& value)
	{
		if (param.size() < sizeof(value)) return false;

		Poco::Int32 tempValue;
		std::memcpy(&tempValue, param.pData(), sizeof(tempValue));
		value = Poco::ByteOrder::fromNetwork(tempValue);
		return true;
	}

	bool readBinaryValue(const OutputParameter& param, Poco::Int64& value)
	{
		if (param.size() < sizeof(value)) return false;

		Poco::Int64 tempValue;
		std::memcpy(&tempValue, param.pData(), sizeof(tempValue));
		value = Poco::ByteOrder::fromNetwork(tempValue);
		return true;
	}

#ifndef POCO_INT64_IS_LONG
	bool readBinaryValue(const OutputParameter& param, long& value)
	{
		if (param.size() < sizeof(value)) return false;

		Poco::Int64 tempValue;
		std::memcpy(&tempValue, param.pData(), sizeof(tempValue));
		value = Poco::ByteOrder::fromNetwork(tempValue);
		return true;
	}
#endif

	bool readBinaryValue(const OutputParameter& param, float& value)
	{
		if (param.size() < sizeof(value)) return false;

		float tempValue;
		std::memcpy(&tempValue, param.pData(), sizeof(tempValue));
		value = fromNetwork(tempValue);
		return true;
	}

	bool readBinaryValue(const OutputParameter& param, double& value)
	{
		if (param.size() < sizeof(value)) return false;

		double tempValue;
		std::memcpy(&tempValue, param.pData(), sizeof(tempValue));
		value = fromNetwork(tempValue);
		return true;
	}

	bool readDate(const OutputParameter& param, Poco::Data::Date& date)
	{
		const Poco::Int64 PG_EPOCH = 946684800000000; // Microseconds between Postgres Epoch (2000-01-01) and Unix Epoch (1970-01-01)
		Poco::Int32 pgDate; // days since 2000-01-01
		if (readBinaryValue(param, pgDate))
		{
			Poco::Timestamp ts(PG_EPOCH + Poco::Timestamp::resolution()*86400*pgDate);
			Poco::DateTime dt(ts);
			date.assign(dt.year(), dt.month(), dt.day());
			return true;
		}
		return false;
	}

	bool readTime(const OutputParameter& param, Poco::Data::Time& time)
	{
		Poco::Int64 pgTime; // microseconds since midnight
		if (readBinaryValue(param, pgTime))
		{
			Poco::Timestamp ts(pgTime);
			Poco::DateTime dt(ts);
			time.assign(dt.hour(), dt.minute(), dt.second());
			return true;
		}
		return false;
	}

	bool readDateTime(const OutputParameter& param, Poco::DateTime& dateTime)
	{
		const Poco::Int64 PG_EPOCH = 946684800000000; // Microseconds between Postgres Epoch (2000-01-01) and Unix Epoch (1970-01-01)
		Poco::Int64 pgDateTime;
		if (readBinaryValue(param, pgDateTime))
		{
			pgDateTime += PG_EPOCH;
			Poco::Timestamp ts(pgDateTime);
			dateTime = ts;
			return true;
		}
		return false;
	}

	bool readUUID(const OutputParameter& param, Poco::UUID& uuid)
	{
		if (param.size() == 16)
		{
			uuid.copyFrom(param.pData());
			return true;
		}
		return false;
	}

	bool readVar(const OutputParameter& param, Poco::Dynamic::Var& var)
	{
		const Oid oid = param.internalFieldType();
		bool success = false;

		switch (oid)
		{
		case BOOLOID:
			{
				bool value;
				success = readBinaryValue(param, value);
				if (success) var = value;
				break;
			}
		case INT2OID:
			{
				Poco::Int16 value;
				success = readBinaryValue(param, value);
				if (success) var = value;
				break;
			}
		case INT4OID:
			{
				Poco::Int32 value;
				success = readBinaryValue(param, value);
				if (success) var = value;
				break;
			}
		case INT8OID:
			{
				Poco::Int64 value;
				success = readBinaryValue(param, value);
				if (success) var = value;
				break;
			}

		// floating point
		case FLOAT8OID:
			{
				double value;
				success = readBinaryValue(param, value);
				if (success) var = value;
				break;
			}
		case FLOAT4OID:
			{
				float value;
				success = readBinaryValue(param, value);
				if (success) var = value;
				break;
			}
		case NUMERICOID:
			throw PostgreSQLException("Cannot extract numeric value in binary extraction mode");

		// character strings
		case CHAROID:
		case BPCHAROID:
		case VARCHAROID:
		default:
			{
				success = true;
				std::string value(param.pData(), param.size());
				var = value;
				break;
			}
		// BLOB, CLOB
		case BYTEAOID:
			{
				success = true;
				Poco::Data::BLOB value(reinterpret_cast<const unsigned char*>(param.pData()), param.size());
				var = value;
				break;
			}
		case TEXTOID:
			{
				success = true;
				Poco::Data::CLOB value(param.pData(), param.size());
				var = value;
				break;
			}
		// date
		case DATEOID:
			{
				Date value;
				success = readDate(param, value);
				if (success) var = value;
				break;
			}
		// time
		case TIMEOID:
		case TIMETZOID:
			{
				Time value;
				success = readTime(param, value);
				if (success) var = value;
				break;
			}
		//timestamp
		case TIMESTAMPOID:
		case TIMESTAMPTZOID:
			{
				DateTime value;
				success = readDateTime(param, value);
				if (success) var = value;
				break;
			}
		case UUIDOID:
			{
				UUID uuid;
				success = readUUID(param, uuid);
				if (success) var = uuid;
				break;
			}
		}

		return success;
	}

	template <typename T>
	bool readAndConvertBinaryValue(const OutputParameter& param, Oid expectedType, T& value)
	{
		if (param.internalFieldType() == expectedType)
		{
			return readBinaryValue(param, value);
		}
		else
		{
			Poco::Dynamic::Var var;
			if (readVar(param, var))
			{
				value = var.convert<T>();
				return true;
			}
			else return false;
		}
	}

	bool readAndConvertStringValue(const OutputParameter& param, std::string& value)
	{
		const Oid oid = param.internalFieldType();
		switch (oid)
		{
		case CHAROID:
		case BPCHAROID:
		case VARCHAROID:
		case TEXTOID:
		case BYTEAOID:
			value.assign(param.pData(), param.size());
			return true;
		default:
			{
				Poco::Dynamic::Var var;
				if (readVar(param, var))
				{
					value = var.convert<std::string>();
					return true;
				}
			}
			return false;
		}
	}
}


BinaryExtractor::BinaryExtractor(StatementExecutor& st /*, ResultMetadata& md */):
	_statementExecutor(st)
{
}


BinaryExtractor::~BinaryExtractor()
{
}


bool BinaryExtractor::extract(std::size_t pos, Poco::Int8& val)
{
	const OutputParameter& outputParameter = extractPreamble(pos);

	if (isColumnNull(outputParameter) || !readAndConvertBinaryValue(outputParameter, INVALIDOID, val))
	{
		return false;
	}
	return true;
}


bool BinaryExtractor::extract(std::size_t pos, Poco::UInt8& val)
{
	const OutputParameter& outputParameter = extractPreamble(pos);

	if (isColumnNull(outputParameter) || !readAndConvertBinaryValue(outputParameter, INVALIDOID, val))
	{
		return false;
	}
	return true;
}


bool BinaryExtractor::extract(std::size_t pos, Poco::Int16& val)
{
	const OutputParameter& outputParameter = extractPreamble(pos);

	if (isColumnNull(outputParameter) || !readAndConvertBinaryValue(outputParameter, INT2OID, val))
	{
		return false;
	}
	return true;
}


bool BinaryExtractor::extract(std::size_t pos, Poco::UInt16& val)
{
	const OutputParameter& outputParameter = extractPreamble(pos);

	if (isColumnNull(outputParameter) || !readAndConvertBinaryValue(outputParameter, INVALIDOID, val))
	{
		return false;
	}
	return true;
}


bool BinaryExtractor::extract(std::size_t pos, Poco::Int32& val)
{
	const OutputParameter& outputParameter = extractPreamble(pos);

	if (isColumnNull(outputParameter) || !readAndConvertBinaryValue(outputParameter, INT4OID, val))
	{
		return false;
	}
	return true;
}


bool BinaryExtractor::extract(std::size_t pos, Poco::UInt32& val)
{
	const OutputParameter& outputParameter = extractPreamble(pos);

	if (isColumnNull(outputParameter) || !readAndConvertBinaryValue(outputParameter, INVALIDOID, val))
	{
		return false;
	}
	return true;
}


bool BinaryExtractor::extract(std::size_t pos, Poco::Int64& val)
{
	const OutputParameter& outputParameter = extractPreamble(pos);

	if (isColumnNull(outputParameter) || !readAndConvertBinaryValue(outputParameter, INT8OID, val))
	{
		return false;
	}
	return true;
}


bool BinaryExtractor::extract(std::size_t pos, Poco::UInt64& val)
{
	const OutputParameter& outputParameter = extractPreamble(pos);

	if (isColumnNull(outputParameter) || !readAndConvertBinaryValue(outputParameter, INVALIDOID, val))
	{
		return false;
	}
	return true;
}


#ifndef POCO_INT64_IS_LONG
bool BinaryExtractor::extract(std::size_t pos, long& val)
{
	const OutputParameter& outputParameter = extractPreamble(pos);

	if (isColumnNull(outputParameter) || !readAndConvertBinaryValue(outputParameter, INT8OID, val))
	{
		return false;
	}
	return true;
}


bool BinaryExtractor::extract(std::size_t pos, unsigned long& val)
{
	const OutputParameter& outputParameter = extractPreamble(pos);

	if (isColumnNull(outputParameter) || !readAndConvertBinaryValue(outputParameter, INVALIDOID, val))
	{
		return false;
	}
	return true;
}
#endif


bool BinaryExtractor::extract(std::size_t pos, bool& val)
{
	const OutputParameter& outputParameter = extractPreamble(pos);

	if (isColumnNull(outputParameter) || !readAndConvertBinaryValue(outputParameter, BOOLOID, val))
	{
		return false;
	}
	return true;
}


bool BinaryExtractor::extract(std::size_t pos, float& val)
{
	const OutputParameter& outputParameter = extractPreamble(pos);

	if (isColumnNull(outputParameter) || !readAndConvertBinaryValue(outputParameter, FLOAT4OID, val))
	{
		return false;
	}
	return true;
}


bool BinaryExtractor::extract(std::size_t pos, double& val)
{
	const OutputParameter& outputParameter = extractPreamble(pos);
	if (isColumnNull(outputParameter) || !readAndConvertBinaryValue(outputParameter, FLOAT8OID, val))
	{
		return false;
	}
	return true;
}


bool BinaryExtractor::extract(std::size_t pos, char& val)
{
	const OutputParameter& outputParameter = extractPreamble(pos);

	std::string str;
	if (isColumnNull(outputParameter) || !readAndConvertStringValue(outputParameter, str))
	{
		return false;
	}
	val = str.empty() ? 0 : str[0];
	return true;
}


bool BinaryExtractor::extract(std::size_t pos, std::string& val)
{
	const OutputParameter& outputParameter = extractPreamble(pos);

	if (isColumnNull(outputParameter) || !readAndConvertStringValue(outputParameter, val))
	{
		return false;
	}
	return true;
}


bool BinaryExtractor::extract(std::size_t pos, Poco::Data::BLOB& val)
{
	const OutputParameter& outputParameter = extractPreamble(pos);

	if (isColumnNull(outputParameter))
	{
		return false;
	}

	if (outputParameter.internalFieldType() == BYTEAOID)
	{
		val = Poco::Data::BLOB(reinterpret_cast<const unsigned char*>(outputParameter.pData()), outputParameter.size()); // don't share contents with _default
		return true;
	}
	else return false;
}


bool BinaryExtractor::extract(std::size_t pos, Poco::Data::CLOB& val)
{
	const OutputParameter& outputParameter = extractPreamble(pos);

	if (isColumnNull(outputParameter))
	{
		return false;
	}

	if (outputParameter.internalFieldType() == TEXTOID)
	{
		val = Poco::Data::CLOB(outputParameter.pData(), outputParameter.size()); // don't share contents with _default
		return true;
	}
	else return false;
}


bool BinaryExtractor::extract(std::size_t pos, DateTime& val)
{
	const OutputParameter& outputParameter = extractPreamble(pos);

	if (isColumnNull(outputParameter))
	{
		return false;
	}

	if (outputParameter.internalFieldType() == TIMESTAMPOID || outputParameter.internalFieldType() == TIMESTAMPTZOID)
	{
		return readDateTime(outputParameter, val);
	}
	else return false;
}


bool BinaryExtractor::extract(std::size_t pos, Date& val)
{
	const OutputParameter& outputParameter = extractPreamble(pos);

	if (isColumnNull(outputParameter))
	{
		return false;
	}

	if (outputParameter.internalFieldType() == DATEOID)
	{
		return readDate(outputParameter, val);
	}
	else return false;
}


bool BinaryExtractor::extract(std::size_t pos, Time& val)
{
	const OutputParameter& outputParameter = extractPreamble(pos);

	if (isColumnNull(outputParameter))
	{
		return false;
	}

	if (outputParameter.internalFieldType() == TIMEOID || outputParameter.internalFieldType() == TIMETZOID)
	{
		return readTime(outputParameter, val);
	}
	else return false;
}


bool BinaryExtractor::extract(std::size_t pos, UUID& val)
{
	const OutputParameter& outputParameter = extractPreamble(pos);

	if (isColumnNull(outputParameter))
	{
		return false;
	}

	if (outputParameter.internalFieldType() == UUIDOID)
	{
		return readUUID(outputParameter, val);
	}
	else return false;
}


bool BinaryExtractor::extract(std::size_t pos, Any& val)
{
	throw PostgreSQLException("BinaryExtractor: extraction to Poco::Any is not supported");
}


bool BinaryExtractor::extract(std::size_t pos, Dynamic::Var& val)
{
	const OutputParameter& outputParameter = extractPreamble(pos);

	if (isColumnNull(outputParameter))
	{
		return false;
	}
	return readVar(outputParameter, val);
}

//////////////
// Nullable
//////////////

bool BinaryExtractor::extract(std::size_t pos, Poco::Nullable<Poco::Int8>& val)
{
	return extractNullable(pos, val);
}


bool BinaryExtractor::extract(std::size_t pos, Poco::Nullable<Poco::UInt8>& val)
{
	return extractNullable(pos, val);
}


bool BinaryExtractor::extract(std::size_t pos, Poco::Nullable<Poco::Int16>& val)
{
	return extractNullable(pos, val);
}


bool BinaryExtractor::extract(std::size_t pos, Poco::Nullable<Poco::UInt16>& val)
{
	return extractNullable(pos, val);
}


bool BinaryExtractor::extract(std::size_t pos, Poco::Nullable<Poco::Int32>& val)
{
	return extractNullable(pos, val);
}


bool BinaryExtractor::extract(std::size_t pos, Poco::Nullable<Poco::UInt32>& val)
{
	return extractNullable(pos, val);
}


bool BinaryExtractor::extract(std::size_t pos, Poco::Nullable<Poco::Int64>& val)
{
	return extractNullable(pos, val);
}


bool BinaryExtractor::extract(std::size_t pos, Poco::Nullable<Poco::UInt64>& val)
{
	return extractNullable(pos, val);
}


#ifndef POCO_INT64_IS_LONG
bool BinaryExtractor::extract(std::size_t pos, Poco::Nullable<long>& val)
{
	return extractNullable(pos, val);
}


bool BinaryExtractor::extract(std::size_t pos, Poco::Nullable<unsigned long>& val)
{
	return extractNullable(pos, val);
}
#endif

bool BinaryExtractor::extract(std::size_t pos, Poco::Nullable<bool>& val)
{
	return extractNullable(pos, val);
}


bool BinaryExtractor::extract(std::size_t pos, Poco::Nullable<float>& val)
{
	return extractNullable(pos, val);
}


bool BinaryExtractor::extract(std::size_t pos, Poco::Nullable<double>& val)
{
	return extractNullable(pos, val);
}


bool BinaryExtractor::extract(std::size_t pos, Poco::Nullable<char>& val)
{
	return extractNullable(pos, val);
}


bool BinaryExtractor::extract(std::size_t pos, Poco::Nullable<std::string>& val)
{
	return extractNullable(pos, val);
}


bool BinaryExtractor::extract(std::size_t pos, Poco::Nullable<UTF16String>& val)
{
	throw NotImplementedException(poco_src_loc);
}


bool BinaryExtractor::extract(std::size_t pos, Poco::Nullable<BLOB>& val)
{
	return extractNullable(pos, val);
}


bool BinaryExtractor::extract(std::size_t pos, Poco::Nullable<CLOB>& val)
{
	return extractNullable(pos, val);
}


bool BinaryExtractor::extract(std::size_t pos, Poco::Nullable<DateTime>& val)
{
	return extractNullable(pos, val);
}


bool BinaryExtractor::extract(std::size_t pos, Poco::Nullable<Date>& val)
{
	return extractNullable(pos, val);
}


bool BinaryExtractor::extract(std::size_t pos, Poco::Nullable<Time>& val)
{
	return extractNullable(pos, val);
}


bool BinaryExtractor::extract(std::size_t pos, Poco::Nullable<UUID>& val)
{
	return extractNullable(pos, val);
}


bool BinaryExtractor::extract(std::size_t pos, Poco::Nullable<Any>& val)
{
	return extractNullable(pos, val);
}


bool BinaryExtractor::extract(std::size_t pos, Poco::Nullable<Poco::Dynamic::Var>& val)
{
	return extractNullable(pos, val);
}


bool BinaryExtractor::isNull(std::size_t col, std::size_t /*row*/)
{
	const OutputParameter& outputParameter = extractPreamble(col);

	if (isColumnNull(outputParameter))
	{
		return true;
	}

	return false;
}


void BinaryExtractor::reset()
{
	AbstractExtractor::reset();
}


const OutputParameter& BinaryExtractor::extractPreamble(std::size_t aPosition) const
{
	if (_statementExecutor.columnsReturned() <= aPosition)
	{
		throw PostgreSQLException("BinaryExtractor: attempt to extract more parameters than query result contains");
	}

	return _statementExecutor.resultColumn(aPosition);
}


//////////////
// Not implemented
//////////////


bool BinaryExtractor::extract(std::size_t, std::vector<Poco::Int8>&)
{
	throw NotImplementedException("std::vector extractor must be implemented.");
}


bool BinaryExtractor::extract(std::size_t, std::deque<Poco::Int8>&)
{
	throw NotImplementedException("std::deque extractor must be implemented.");
}


bool BinaryExtractor::extract(std::size_t, std::list<Poco::Int8>&)
{
	throw NotImplementedException("std::list extractor must be implemented.");
}


bool BinaryExtractor::extract(std::size_t, std::vector<Poco::UInt8>&)
{
	throw NotImplementedException("std::vector extractor must be implemented.");
}


bool BinaryExtractor::extract(std::size_t, std::deque<Poco::UInt8>&)
{
	throw NotImplementedException("std::deque extractor must be implemented.");
}


bool BinaryExtractor::extract(std::size_t, std::list<Poco::UInt8>&)
{
	throw NotImplementedException("std::list extractor must be implemented.");
}


bool BinaryExtractor::extract(std::size_t, std::vector<Poco::Int16>&)
{
	throw NotImplementedException("std::vector extractor must be implemented.");
}


bool BinaryExtractor::extract(std::size_t, std::deque<Poco::Int16>&)
{
	throw NotImplementedException("std::deque extractor must be implemented.");
}


bool BinaryExtractor::extract(std::size_t, std::list<Poco::Int16>&)
{
	throw NotImplementedException("std::list extractor must be implemented.");
}


bool BinaryExtractor::extract(std::size_t, std::vector<Poco::UInt16>&)
{
	throw NotImplementedException("std::vector extractor must be implemented.");
}


bool BinaryExtractor::extract(std::size_t, std::deque<Poco::UInt16>&)
{
	throw NotImplementedException("std::deque extractor must be implemented.");
}


bool BinaryExtractor::extract(std::size_t, std::list<Poco::UInt16>&)
{
	throw NotImplementedException("std::list extractor must be implemented.");
}


bool BinaryExtractor::extract(std::size_t, std::vector<Poco::Int32>&)
{
	throw NotImplementedException("std::vector extractor must be implemented.");
}


bool BinaryExtractor::extract(std::size_t, std::deque<Poco::Int32>&)
{
	throw NotImplementedException("std::deque extractor must be implemented.");
}


bool BinaryExtractor::extract(std::size_t, std::list<Poco::Int32>&)
{
	throw NotImplementedException("std::list extractor must be implemented.");
}


bool BinaryExtractor::extract(std::size_t, std::vector<Poco::UInt32>&)
{
	throw NotImplementedException("std::vector extractor must be implemented.");
}


bool BinaryExtractor::extract(std::size_t, std::deque<Poco::UInt32>&)
{
	throw NotImplementedException("std::deque extractor must be implemented.");
}


bool BinaryExtractor::extract(std::size_t, std::list<Poco::UInt32>&)
{
	throw NotImplementedException("std::list extractor must be implemented.");
}


bool BinaryExtractor::extract(std::size_t, std::vector<Poco::Int64>&)
{
	throw NotImplementedException("std::vector extractor must be implemented.");
}


bool BinaryExtractor::extract(std::size_t, std::deque<Poco::Int64>&)
{
	throw NotImplementedException("std::deque extractor must be implemented.");
}


bool BinaryExtractor::extract(std::size_t, std::list<Poco::Int64>&)
{
	throw NotImplementedException("std::list extractor must be implemented.");
}


bool BinaryExtractor::extract(std::size_t, std::vector<Poco::UInt64>&)
{
	throw NotImplementedException("std::vector extractor must be implemented.");
}


bool BinaryExtractor::extract(std::size_t, std::deque<Poco::UInt64>&)
{
	throw NotImplementedException("std::deque extractor must be implemented.");
}


bool BinaryExtractor::extract(std::size_t, std::list<Poco::UInt64>&)
{
	throw NotImplementedException("std::list extractor must be implemented.");
}


#ifndef POCO_INT64_IS_LONG
bool BinaryExtractor::extract(std::size_t, std::vector<long>&)
{
	throw NotImplementedException("std::vector extractor must be implemented.");
}


bool BinaryExtractor::extract(std::size_t, std::deque<long>&)
{
	throw NotImplementedException("std::deque extractor must be implemented.");
}


bool BinaryExtractor::extract(std::size_t, std::list<long>&)
{
	throw NotImplementedException("std::list extractor must be implemented.");
}

bool BinaryExtractor::extract(std::size_t, std::vector<unsigned long>&)
{
	throw NotImplementedException("std::vector extractor must be implemented.");
}


bool BinaryExtractor::extract(std::size_t, std::deque<unsigned long>&)
{
	throw NotImplementedException("std::deque extractor must be implemented.");
}


bool BinaryExtractor::extract(std::size_t, std::list<unsigned long>&)
{
	throw NotImplementedException("std::list extractor must be implemented.");
}
#endif


bool BinaryExtractor::extract(std::size_t, std::vector<bool>&)
{
	throw NotImplementedException("std::vector extractor must be implemented.");
}


bool BinaryExtractor::extract(std::size_t, std::deque<bool>&)
{
	throw NotImplementedException("std::deque extractor must be implemented.");
}


bool BinaryExtractor::extract(std::size_t, std::list<bool>&)
{
	throw NotImplementedException("std::list extractor must be implemented.");
}


bool BinaryExtractor::extract(std::size_t, std::vector<float>&)
{
	throw NotImplementedException("std::vector extractor must be implemented.");
}


bool BinaryExtractor::extract(std::size_t, std::deque<float>&)
{
	throw NotImplementedException("std::deque extractor must be implemented.");
}


bool BinaryExtractor::extract(std::size_t, std::list<float>&)
{
	throw NotImplementedException("std::list extractor must be implemented.");
}


bool BinaryExtractor::extract(std::size_t, std::vector<double>&)
{
	throw NotImplementedException("std::vector extractor must be implemented.");
}


bool BinaryExtractor::extract(std::size_t, std::deque<double>&)
{
	throw NotImplementedException("std::deque extractor must be implemented.");
}


bool BinaryExtractor::extract(std::size_t, std::list<double>&)
{
	throw NotImplementedException("std::list extractor must be implemented.");
}


bool BinaryExtractor::extract(std::size_t, std::vector<char>&)
{
	throw NotImplementedException("std::vector extractor must be implemented.");
}


bool BinaryExtractor::extract(std::size_t, std::deque<char>&)
{
	throw NotImplementedException("std::deque extractor must be implemented.");
}


bool BinaryExtractor::extract(std::size_t, std::list<char>&)
{
	throw NotImplementedException("std::list extractor must be implemented.");
}


bool BinaryExtractor::extract(std::size_t, std::vector<std::string>&)
{
	throw NotImplementedException("std::vector extractor must be implemented.");
}


bool BinaryExtractor::extract(std::size_t, std::deque<std::string>&)
{
	throw NotImplementedException("std::deque extractor must be implemented.");
}


bool BinaryExtractor::extract(std::size_t, std::list<std::string>&)
{
	throw NotImplementedException("std::list extractor must be implemented.");
}


bool BinaryExtractor::extract(std::size_t, std::vector<BLOB>&)
{
	throw NotImplementedException("std::vector extractor must be implemented.");
}


bool BinaryExtractor::extract(std::size_t, std::deque<BLOB>&)
{
	throw NotImplementedException("std::deque extractor must be implemented.");
}


bool BinaryExtractor::extract(std::size_t, std::list<BLOB>&)
{
	throw NotImplementedException("std::list extractor must be implemented.");
}


bool BinaryExtractor::extract(std::size_t, std::vector<CLOB>&)
{
	throw NotImplementedException("std::vector extractor must be implemented.");
}


bool BinaryExtractor::extract(std::size_t, std::deque<CLOB>&)
{
	throw NotImplementedException("std::deque extractor must be implemented.");
}


bool BinaryExtractor::extract(std::size_t, std::list<CLOB>&)
{
	throw NotImplementedException("std::list extractor must be implemented.");
}


bool BinaryExtractor::extract(std::size_t, std::vector<DateTime>&)
{
	throw NotImplementedException("std::vector extractor must be implemented.");
}


bool BinaryExtractor::extract(std::size_t, std::deque<DateTime>&)
{
	throw NotImplementedException("std::deque extractor must be implemented.");
}


bool BinaryExtractor::extract(std::size_t, std::list<DateTime>&)
{
	throw NotImplementedException("std::list extractor must be implemented.");
}


bool BinaryExtractor::extract(std::size_t, std::vector<Date>&)
{
	throw NotImplementedException("std::vector extractor must be implemented.");
}


bool BinaryExtractor::extract(std::size_t, std::deque<Date>&)
{
	throw NotImplementedException("std::deque extractor must be implemented.");
}


bool BinaryExtractor::extract(std::size_t, std::list<Date>&)
{
	throw NotImplementedException("std::list extractor must be implemented.");
}


bool BinaryExtractor::extract(std::size_t, std::vector<Time>&)
{
	throw NotImplementedException("std::vector extractor must be implemented.");
}


bool BinaryExtractor::extract(std::size_t, std::deque<Time>&)
{
	throw NotImplementedException("std::deque extractor must be implemented.");
}


bool BinaryExtractor::extract(std::size_t, std::list<Time>&)
{
	throw NotImplementedException("std::list extractor must be implemented.");
}


bool BinaryExtractor::extract(std::size_t, std::vector<Any>&)
{
	throw NotImplementedException("std::vector extractor must be implemented.");
}


bool BinaryExtractor::extract(std::size_t, std::deque<Any>&)
{
	throw NotImplementedException("std::deque extractor must be implemented.");
}


bool BinaryExtractor::extract(std::size_t, std::list<Any>&)
{
	throw NotImplementedException("std::list extractor must be implemented.");
}


bool BinaryExtractor::extract(std::size_t, std::vector<Dynamic::Var>&)
{
	throw NotImplementedException("std::vector extractor must be implemented.");
}


bool BinaryExtractor::extract(std::size_t, std::deque<Dynamic::Var>&)
{
	throw NotImplementedException("std::deque extractor must be implemented.");
}


bool BinaryExtractor::extract(std::size_t, std::list<Dynamic::Var>&)
{
	throw NotImplementedException("std::list extractor must be implemented.");
}


} } } // namespace Poco::Data::PostgreSQL
