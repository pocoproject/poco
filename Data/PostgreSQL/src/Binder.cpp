//
// Binder.cpp
//
// Library: Data/PostgreSQL
// Package: PostgreSQL
// Module:  Binder
//
// Copyright (c) 2015, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Data/PostgreSQL/Binder.h"
#include "Poco/NumberFormatter.h"
#include "Poco/DateTimeFormat.h"


namespace Poco {
namespace Data {
namespace PostgreSQL {


Binder::Binder()
{
}


Binder::~Binder()
{
}


void Binder::bind(std::size_t pos, const Poco::Int8& val, Direction dir)
{
	poco_assert(dir == PD_IN);
	realBind(pos, Poco::Data::MetaColumn::FDT_INT8, &val, sizeof(Poco::Int8));
}


void Binder::bind(std::size_t pos, const Poco::UInt8& val, Direction dir)
{
	poco_assert(dir == PD_IN);
	realBind(pos, Poco::Data::MetaColumn::FDT_UINT8, &val, sizeof(Poco::UInt8));
}


void Binder::bind(std::size_t pos, const Poco::Int16& val, Direction dir)
{
	poco_assert(dir == PD_IN);
	realBind(pos, Poco::Data::MetaColumn::FDT_INT16, &val, sizeof(Poco::Int16));
}


void Binder::bind(std::size_t pos, const Poco::UInt16& val, Direction dir)
{
	poco_assert(dir == PD_IN);
	realBind(pos, Poco::Data::MetaColumn::FDT_UINT16, &val, sizeof(Poco::UInt16));
}


void Binder::bind(std::size_t pos, const Poco::Int32& val, Direction dir)
{
	poco_assert(dir == PD_IN);
	realBind(pos, Poco::Data::MetaColumn::FDT_INT32, &val, sizeof(Poco::Int32));
}


void Binder::bind(std::size_t pos, const Poco::UInt32& val, Direction dir)
{
	poco_assert(dir == PD_IN);
	realBind(pos, Poco::Data::MetaColumn::FDT_UINT32, &val, sizeof(Poco::UInt32));
}


void Binder::bind(std::size_t pos, const Poco::Int64& val, Direction dir)
{
	poco_assert(dir == PD_IN);
	realBind(pos, Poco::Data::MetaColumn::FDT_INT64, &val, sizeof(Poco::Int64));
}


void Binder::bind(std::size_t pos, const Poco::UInt64& val, Direction dir)
{
	poco_assert(dir == PD_IN);
	realBind(pos, Poco::Data::MetaColumn::FDT_UINT64, &val, sizeof(Poco::UInt64));
}


#ifndef POCO_INT64_IS_LONG
void Binder::bind(std::size_t pos, const long& val, Direction dir)
{
	poco_assert(dir == PD_IN);
	realBind(pos, Poco::Data::MetaColumn::FDT_INT64, &val, sizeof(Poco::Int64));
}


void Binder::bind(std::size_t pos, const unsigned long& val, Direction dir)
{
	poco_assert(dir == PD_IN);
	realBind(pos, Poco::Data::MetaColumn::FDT_UINT64, &val, sizeof(Poco::UInt64));
}
#endif


void Binder::bind(std::size_t pos, const bool& val, Direction dir)
{
	poco_assert(dir == PD_IN);
	realBind(pos, Poco::Data::MetaColumn::FDT_BOOL, &val, sizeof(bool));
}


void Binder::bind(std::size_t pos, const float& val, Direction dir)
{
	poco_assert(dir == PD_IN);
	realBind(pos, Poco::Data::MetaColumn::FDT_FLOAT, &val, sizeof(float));
}


void Binder::bind(std::size_t pos, const double& val, Direction dir)
{
	poco_assert(dir == PD_IN);
	realBind(pos, Poco::Data::MetaColumn::FDT_DOUBLE, &val, sizeof(double));
}


void Binder::bind(std::size_t pos, const char& val, Direction dir)
{
	poco_assert(dir == PD_IN);
	// USING UINT8 because Poco::Data::MetaColumn does not have a single character type, just std::string
	realBind(pos, Poco::Data::MetaColumn::FDT_UINT8, &val, sizeof(char));
}

// complex types

void Binder::bind(std::size_t pos, const std::string& val, Direction dir)
{
	poco_assert(dir == PD_IN);
	realBind(pos, Poco::Data::MetaColumn::FDT_STRING, &val, static_cast<int>(val.size()));
}


void Binder::bind(std::size_t pos, const Poco::Data::BLOB& val, Direction dir)
{
	poco_assert(dir == PD_IN);
	realBind(pos, Poco::Data::MetaColumn::FDT_BLOB, &val, static_cast<int>(val.size()));
}


void Binder::bind(std::size_t pos, const Poco::Data::CLOB& val, Direction dir)
{
	poco_assert(dir == PD_IN);
	realBind(pos, Poco::Data::MetaColumn::FDT_CLOB, &val, static_cast<int>(val.size()));
}


void Binder::bind(std::size_t pos, const Poco::DateTime& val, Direction dir)
{
	poco_assert(dir == PD_IN);
	realBind(pos, Poco::Data::MetaColumn::FDT_TIMESTAMP, &val, sizeof(Poco::DateTime));
}


void Binder::bind(std::size_t pos, const Date& val, Direction dir)
{
	poco_assert(dir == PD_IN);
	realBind(pos, Poco::Data::MetaColumn::FDT_DATE, &val, sizeof(Date));
}


void Binder::bind(std::size_t pos, const Time& val, Direction dir)
{
	poco_assert(dir == PD_IN);
	realBind(pos, Poco::Data::MetaColumn::FDT_TIME, &val, sizeof(Time));
}


void Binder::bind(std::size_t pos, const UUID& val, Direction dir)
{
	poco_assert(dir == PD_IN);
	realBind(pos, Poco::Data::MetaColumn::FDT_UUID, &val, sizeof(UUID));
}


void Binder::bind(std::size_t pos, const NullData&, Direction dir)
{
	poco_assert(dir == PD_IN);
	realBind(pos, Poco::Data::MetaColumn::FDT_UNKNOWN, 0, 0);
}


std::size_t Binder::size() const
{
	return static_cast<std::size_t>(_bindVector.size());
}


InputParameterVector
Binder::bindVector() const
{
	return _bindVector;
}


void Binder::updateBindVectorToCurrentValues()
{
	InputParameterVector::iterator itr    = _bindVector.begin();
	InputParameterVector::iterator itrEnd = _bindVector.end();

	for (; itr != itrEnd; ++itr)
	{
		switch (itr->fieldType())
		{
		case Poco::Data::MetaColumn::FDT_INT8:
			itr->setStringVersionRepresentation(Poco::NumberFormatter::format(* static_cast<const Poco::Int8*>(itr->pData())));
			break;

		case Poco::Data::MetaColumn::FDT_UINT8:
			itr->setStringVersionRepresentation(Poco::NumberFormatter::format(* static_cast<const Poco::UInt8*>(itr->pData())));
			break;

		case Poco::Data::MetaColumn::FDT_INT16:
			itr->setStringVersionRepresentation(Poco::NumberFormatter::format(* static_cast<const Poco::Int16*>(itr->pData())));
			break;

		case Poco::Data::MetaColumn::FDT_UINT16:
			itr->setStringVersionRepresentation(Poco::NumberFormatter::format(* static_cast<const Poco::UInt16*>(itr->pData())));
			break;

		case Poco::Data::MetaColumn::FDT_INT32:
			itr->setStringVersionRepresentation(Poco::NumberFormatter::format(* static_cast<const Poco::Int32*>(itr->pData())));
			break;

		case Poco::Data::MetaColumn::FDT_UINT32:
			itr->setStringVersionRepresentation(Poco::NumberFormatter::format(* static_cast<const Poco::UInt32*>(itr->pData())));
			break;

		case Poco::Data::MetaColumn::FDT_INT64:
			itr->setStringVersionRepresentation(Poco::NumberFormatter::format(* static_cast<const Poco::Int64*>(itr->pData())));
			break;

		case Poco::Data::MetaColumn::FDT_UINT64:
			itr->setStringVersionRepresentation(Poco::NumberFormatter::format(* static_cast<const Poco::UInt64*>(itr->pData())));
			break;

		case Poco::Data::MetaColumn::FDT_BOOL:
			{
				const bool currentBoolValue = * static_cast<const bool*>(itr->pData());
				itr->setStringVersionRepresentation(currentBoolValue ? "TRUE" : "FALSE");
			}
			break;

		case Poco::Data::MetaColumn::FDT_FLOAT:
			itr->setStringVersionRepresentation(Poco::NumberFormatter::format(* static_cast<const float*>(itr->pData())));
			break;

		case Poco::Data::MetaColumn::FDT_DOUBLE:
			itr->setStringVersionRepresentation(Poco::NumberFormatter::format(* static_cast<const double*>(itr->pData())));
			break;

//		case Poco::Data::MetaColumn::FDT_CHAR:
//			itr->setStringVersionRepresentation(std::string(static_cast<const char*>(itr->pData()), 1));  // single character string
//			break;

		case Poco::Data::MetaColumn::FDT_STRING:
			itr->setStringVersionRepresentation(* static_cast<const std::string*>(itr->pData()));
			break;

		case Poco::Data::MetaColumn::FDT_TIMESTAMP:
			{
				const Poco::DateTime& dateTime = * static_cast<const Poco::DateTime*>(itr->pData());
				itr->setStringVersionRepresentation(DateTimeFormatter::format(dateTime, Poco::DateTimeFormat::ISO8601_FRAC_FORMAT));
			}
			break;

		case Poco::Data::MetaColumn::FDT_DATE:
			{
				const Poco::Data::Date& date = * static_cast<const Poco::Data::Date*>(itr->pData());
				itr->setStringVersionRepresentation(DateTimeFormatter::format(Poco::DateTime(date.year(), date.month(), date.day()), "%Y-%m-%d"));
			}
			break;

		case Poco::Data::MetaColumn::FDT_TIME:
			{
				const Poco::Data::Time& time = * static_cast<const Poco::Data::Time*>(itr->pData());
				itr->setStringVersionRepresentation(DateTimeFormatter::format(Poco::DateTime(0, 1, 1, time.hour(), time.minute(), time.second()), "%H:%M:%s%z"));
			}
			break;

		case Poco::Data::MetaColumn::FDT_BLOB:
			{
				const Poco::Data::BLOB& blob = * static_cast<const Poco::Data::BLOB*>(itr->pData());
				itr->setNonStringVersionRepresentation(static_cast<const void*> (blob.rawContent()), blob.size());
			}
			break;

		case Poco::Data::MetaColumn::FDT_CLOB:
			{
				const Poco::Data::CLOB& clob = * static_cast<const Poco::Data::CLOB*>(itr->pData());
				itr->setNonStringVersionRepresentation(static_cast<const void*> (clob.rawContent()), clob.size());
			}
			break;

		case Poco::Data::MetaColumn::FDT_UUID:
			{
				const Poco::UUID& uuid = * static_cast<const Poco::UUID*>(itr->pData());
				itr->setStringVersionRepresentation(uuid.toString());
			}
			break;

		case Poco::Data::MetaColumn::FDT_UNKNOWN:
		default:
			break;
		}
	}
}


void Binder::realBind(std::size_t aPosition, Poco::Data::MetaColumn::ColumnDataType aFieldType, const void* aBufferPtr, std::size_t aLength)
{
	try
	{
		if (aPosition >= _bindVector.size())
		{
			_bindVector.resize(aPosition + 1);
		}

		InputParameter inputParameter(aFieldType, aBufferPtr, aLength);

		_bindVector[aPosition] = inputParameter;
	}
	catch (std::bad_alloc&)
	{
		PostgreSQLException("Memory allocation error while binding");
	}
}


void Binder::bind(std::size_t /*pos*/, const std::vector<Poco::Int8>& /*val*/, Direction /*dir*/)
{
	throw NotImplementedException();
}


void Binder::bind(std::size_t /*pos*/, const std::deque<Poco::Int8>& /*val*/, Direction /*dir*/)
{
	throw NotImplementedException();
}


void Binder::bind(std::size_t /*pos*/, const std::list<Poco::Int8>& /*val*/, Direction /*dir*/)
{
	throw NotImplementedException();
}


void Binder::bind(std::size_t /*pos*/, const std::vector<Poco::UInt8>& /*val*/, Direction /*dir*/)
{
	throw NotImplementedException();
}


void Binder::bind(std::size_t /*pos*/, const std::deque<Poco::UInt8>& /*val*/, Direction /*dir*/)
{
	throw NotImplementedException();
}


void Binder::bind(std::size_t /*pos*/, const std::list<Poco::UInt8>& /*val*/, Direction /*dir*/)
{
	throw NotImplementedException();
}


void Binder::bind(std::size_t /*pos*/, const std::vector<Poco::Int16>& /*val*/, Direction /*dir*/)
{
	throw NotImplementedException();
}


void Binder::bind(std::size_t /*pos*/, const std::deque<Poco::Int16>& /*val*/, Direction /*dir*/)
{
	throw NotImplementedException();
}


void Binder::bind(std::size_t /*pos*/, const std::list<Poco::Int16>& /*val*/, Direction /*dir*/)
{
	throw NotImplementedException();
}


void Binder::bind(std::size_t /*pos*/, const std::vector<Poco::UInt16>& /*val*/, Direction /*dir*/)
{
	throw NotImplementedException();
}


void Binder::bind(std::size_t /*pos*/, const std::deque<Poco::UInt16>& /*val*/, Direction /*dir*/)
{
	throw NotImplementedException();
}


void Binder::bind(std::size_t /*pos*/, const std::list<Poco::UInt16>& /*val*/, Direction /*dir*/)
{
	throw NotImplementedException();
}


void Binder::bind(std::size_t /*pos*/, const std::vector<Poco::Int32>& /*val*/, Direction /*dir*/)
{
	throw NotImplementedException();
}


void Binder::bind(std::size_t /*pos*/, const std::deque<Poco::Int32>& /*val*/, Direction /*dir*/)
{
	throw NotImplementedException();
}


void Binder::bind(std::size_t /*pos*/, const std::list<Poco::Int32>& /*val*/, Direction /*dir*/)
{
	throw NotImplementedException();
}


void Binder::bind(std::size_t /*pos*/, const std::vector<Poco::UInt32>& /*val*/, Direction /*dir*/)
{
	throw NotImplementedException();
}


void Binder::bind(std::size_t /*pos*/, const std::deque<Poco::UInt32>& /*val*/, Direction /*dir*/)
{
	throw NotImplementedException();
}


void Binder::bind(std::size_t /*pos*/, const std::list<Poco::UInt32>& /*val*/, Direction /*dir*/)
{
	throw NotImplementedException();
}


void Binder::bind(std::size_t /*pos*/, const std::vector<Poco::Int64>& /*val*/, Direction /*dir*/)
{
	throw NotImplementedException();
}


void Binder::bind(std::size_t /*pos*/, const std::deque<Poco::Int64>& /*val*/, Direction /*dir*/)
{
	throw NotImplementedException();
}


void Binder::bind(std::size_t /*pos*/, const std::list<Poco::Int64>& /*val*/, Direction /*dir*/)
{
	throw NotImplementedException();
}


void Binder::bind(std::size_t /*pos*/, const std::vector<Poco::UInt64>& /*val*/, Direction /*dir*/)
{
	throw NotImplementedException();
}


void Binder::bind(std::size_t /*pos*/, const std::deque<Poco::UInt64>& /*val*/, Direction /*dir*/)
{
	throw NotImplementedException();
}


void Binder::bind(std::size_t /*pos*/, const std::list<Poco::UInt64>& /*val*/, Direction /*dir*/)
{
	throw NotImplementedException();
}


void Binder::bind(std::size_t /*pos*/, const std::vector<bool>& /*val*/, Direction /*dir*/)
{
	throw NotImplementedException();
}


void Binder::bind(std::size_t /*pos*/, const std::deque<bool>& /*val*/, Direction /*dir*/)
{
	throw NotImplementedException();
}


void Binder::bind(std::size_t /*pos*/, const std::list<bool>& /*val*/, Direction /*dir*/)
{
	throw NotImplementedException();
}


void Binder::bind(std::size_t /*pos*/, const std::vector<float>& /*val*/, Direction /*dir*/)
{
	throw NotImplementedException();
}


void Binder::bind(std::size_t /*pos*/, const std::deque<float>& /*val*/, Direction /*dir*/)
{
	throw NotImplementedException();
}


void Binder::bind(std::size_t /*pos*/, const std::list<float>& /*val*/, Direction /*dir*/)
{
	throw NotImplementedException();
}


void Binder::bind(std::size_t /*pos*/, const std::vector<double>& /*val*/, Direction /*dir*/)
{
	throw NotImplementedException();
}


void Binder::bind(std::size_t /*pos*/, const std::deque<double>& /*val*/, Direction /*dir*/)
{
	throw NotImplementedException();
}


void Binder::bind(std::size_t /*pos*/, const std::list<double>& /*val*/, Direction /*dir*/)
{
	throw NotImplementedException();
}


void Binder::bind(std::size_t /*pos*/, const std::vector<char>& /*val*/, Direction /*dir*/)
{
	throw NotImplementedException();
}


void Binder::bind(std::size_t /*pos*/, const std::deque<char>& /*val*/, Direction /*dir*/)
{
	throw NotImplementedException();
}


void Binder::bind(std::size_t /*pos*/, const std::list<char>& /*val*/, Direction /*dir*/)
{
	throw NotImplementedException();
}


void Binder::bind(std::size_t /*pos*/, const std::vector<Poco::Data::BLOB>& /*val*/, Direction /*dir*/)
{
	throw NotImplementedException();
}


void Binder::bind(std::size_t /*pos*/, const std::deque<Poco::Data::BLOB>& /*val*/, Direction /*dir*/)
{
	throw NotImplementedException();
}


void Binder::bind(std::size_t /*pos*/, const std::list<Poco::Data::BLOB>& /*val*/, Direction /*dir*/)
{
	throw NotImplementedException();
}


void Binder::bind(std::size_t /*pos*/, const std::vector<Poco::Data::CLOB>& /*val*/, Direction /*dir*/)
{
	throw NotImplementedException();
}


void Binder::bind(std::size_t /*pos*/, const std::deque<Poco::Data::CLOB>& /*val*/, Direction /*dir*/)
{
	throw NotImplementedException();
}


void Binder::bind(std::size_t /*pos*/, const std::list<Poco::Data::CLOB>& /*val*/, Direction /*dir*/)
{
	throw NotImplementedException();
}


void Binder::bind(std::size_t /*pos*/, const std::vector<Poco::DateTime>& /*val*/, Direction /*dir*/)
{
	throw NotImplementedException();
}


void Binder::bind(std::size_t /*pos*/, const std::deque<Poco::DateTime>& /*val*/, Direction /*dir*/)
{
	throw NotImplementedException();
}


void Binder::bind(std::size_t /*pos*/, const std::list<Poco::DateTime>& /*val*/, Direction /*dir*/)
{
	throw NotImplementedException();
}


void Binder::bind(std::size_t /*pos*/, const std::vector<Poco::Data::Date>& /*val*/, Direction /*dir*/)
{
	throw NotImplementedException();
}


void Binder::bind(std::size_t /*pos*/, const std::deque<Poco::Data::Date>& /*val*/, Direction /*dir*/)
{
	throw NotImplementedException();
}


void Binder::bind(std::size_t /*pos*/, const std::list<Poco::Data::Date>& /*val*/, Direction /*dir*/)
{
	throw NotImplementedException();
}


void Binder::bind(std::size_t /*pos*/, const std::vector<Poco::Data::Time>& /*val*/, Direction /*dir*/)
{
	throw NotImplementedException();
}


void Binder::bind(std::size_t /*pos*/, const std::deque<Poco::Data::Time>& /*val*/, Direction /*dir*/)
{
	throw NotImplementedException();
}


void Binder::bind(std::size_t /*pos*/, const std::list<Poco::Data::Time>& /*val*/, Direction /*dir*/)
{
	throw NotImplementedException();
}


void Binder::bind(std::size_t /*pos*/, const std::vector<Poco::Data::NullData>& /*val*/, Direction /*dir*/)
{
	throw NotImplementedException();
}


void Binder::bind(std::size_t /*pos*/, const std::deque<Poco::Data::NullData>& /*val*/, Direction /*dir*/)
{
	throw NotImplementedException();
}


void Binder::bind(std::size_t /*pos*/, const std::list<Poco::Data::NullData>& /*val*/, Direction /*dir*/)
{
	throw NotImplementedException();
}


void Binder::bind(std::size_t /*pos*/, const std::vector<std::string>& /*val*/, Direction /*dir*/)
{
	throw NotImplementedException();
}


void Binder::bind(std::size_t /*pos*/, const std::deque<std::string>& /*val*/, Direction /*dir*/)
{
	throw NotImplementedException();
}


void Binder::bind(std::size_t /*pos*/, const std::list<std::string>& /*val*/, Direction /*dir*/)
{
	throw NotImplementedException();
}


} } } // namespace Poco::Data::PostgreSQL
