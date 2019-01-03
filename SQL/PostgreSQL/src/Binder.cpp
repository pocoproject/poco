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


#include "Poco/SQL/PostgreSQL/Binder.h"
#include "Poco/NumberFormatter.h"
#include "Poco/DateTimeFormat.h"


namespace Poco {
namespace SQL {
namespace PostgreSQL {


Binder::Binder()
{
}


Binder::~Binder()
{
}


void Binder::bind(std::size_t pos, const Poco::Int8& val, Direction dir, const WhenNullCb& /*nullCb*/)
{
	poco_assert(dir == PD_IN);
	realBind(pos, Poco::SQL::MetaColumn::FDT_INT8, &val, sizeof(Poco::Int8));
}


void Binder::bind(std::size_t pos, const Poco::UInt8& val, Direction dir, const WhenNullCb& /*nullCb*/)
{
	poco_assert(dir == PD_IN);
	realBind(pos, Poco::SQL::MetaColumn::FDT_UINT8, &val, sizeof(Poco::UInt8));
}


void Binder::bind(std::size_t pos, const Poco::Int16& val, Direction dir, const WhenNullCb& /*nullCb*/)
{
	poco_assert(dir == PD_IN);
	realBind(pos, Poco::SQL::MetaColumn::FDT_INT16, &val, sizeof(Poco::Int16));
}


void Binder::bind(std::size_t pos, const Poco::UInt16& val, Direction dir, const WhenNullCb& /*nullCb*/)
{
	poco_assert(dir == PD_IN);
	realBind(pos, Poco::SQL::MetaColumn::FDT_UINT16, &val, sizeof(Poco::UInt16));
}


void Binder::bind(std::size_t pos, const Poco::Int32& val, Direction dir, const WhenNullCb& /*nullCb*/)
{
	poco_assert(dir == PD_IN);
	realBind(pos, Poco::SQL::MetaColumn::FDT_INT32, &val, sizeof(Poco::Int32));
}


void Binder::bind(std::size_t pos, const Poco::UInt32& val, Direction dir, const WhenNullCb& /*nullCb*/)
{
	poco_assert(dir == PD_IN);
	realBind(pos, Poco::SQL::MetaColumn::FDT_UINT32, &val, sizeof(Poco::UInt32));
}


void Binder::bind(std::size_t pos, const Poco::Int64& val, Direction dir, const WhenNullCb& /*nullCb*/)
{
	poco_assert(dir == PD_IN);
	realBind(pos, Poco::SQL::MetaColumn::FDT_INT64, &val, sizeof(Poco::Int64));
}


void Binder::bind(std::size_t pos, const Poco::UInt64& val, Direction dir, const WhenNullCb& /*nullCb*/)
{
	poco_assert(dir == PD_IN);
	realBind(pos, Poco::SQL::MetaColumn::FDT_UINT64, &val, sizeof(Poco::UInt64));
}


#ifndef POCO_LONG_IS_64_BIT

void Binder::bind(std::size_t pos, const long& val, Direction dir, const WhenNullCb& /*nullCb*/)
{
	poco_assert(dir == PD_IN);
	realBind(pos, Poco::SQL::MetaColumn::FDT_INT64, &val, sizeof(Poco::Int64));
}


void Binder::bind(std::size_t pos, const unsigned long& val, Direction dir, const WhenNullCb& /*nullCb*/)
{
	poco_assert(dir == PD_IN);
	realBind(pos, Poco::SQL::MetaColumn::FDT_UINT64, &val, sizeof(Poco::UInt64));
}

#endif // POCO_LONG_IS_64_BIT


void Binder::bind(std::size_t pos, const bool& val, Direction dir, const WhenNullCb& /*nullCb*/)
{
	poco_assert(dir == PD_IN);
	realBind(pos, Poco::SQL::MetaColumn::FDT_BOOL, &val, sizeof(bool));
}

	
void Binder::bind(std::size_t pos, const float& val, Direction dir, const WhenNullCb& /*nullCb*/)
{
	poco_assert(dir == PD_IN);
	realBind(pos, Poco::SQL::MetaColumn::FDT_FLOAT, &val, sizeof(float));
}


void Binder::bind(std::size_t pos, const double& val, Direction dir, const WhenNullCb& /*nullCb*/)
{
	poco_assert(dir == PD_IN);
	realBind(pos, Poco::SQL::MetaColumn::FDT_DOUBLE, &val, sizeof(double));
}


void Binder::bind(std::size_t pos, const char& val, Direction dir, const WhenNullCb& /*nullCb*/)
{
	poco_assert(dir == PD_IN);
	// USING UINT8 because Poco::SQL::MetaColumn does not have a single character type, just std::string
	realBind(pos, Poco::SQL::MetaColumn::FDT_UINT8, &val, sizeof(char));
}

// complex types

void Binder::bind(std::size_t pos, const std::string& val, Direction dir, const WhenNullCb& /*nullCb*/)
{
	poco_assert(dir == PD_IN);
	realBind(pos, Poco::SQL::MetaColumn::FDT_STRING, &val, static_cast<int>(val.size()));
}


void Binder::bind(std::size_t pos, const Poco::SQL::BLOB& val, Direction dir, const WhenNullCb& /*nullCb*/)
{
	poco_assert(dir == PD_IN);
	realBind(pos, Poco::SQL::MetaColumn::FDT_BLOB, &val, static_cast<int>(val.size()));
}


void Binder::bind(std::size_t pos, const Poco::SQL::CLOB& val, Direction dir, const WhenNullCb& /*nullCb*/)
{
	poco_assert(dir == PD_IN);
	realBind(pos, Poco::SQL::MetaColumn::FDT_CLOB, &val, static_cast<int>(val.size()));
}


void Binder::bind(std::size_t pos, const Poco::DateTime& val, Direction dir, const WhenNullCb& /*nullCb*/)
{
	poco_assert(dir == PD_IN);
	realBind(pos, Poco::SQL::MetaColumn::FDT_TIMESTAMP, &val, sizeof(Poco::DateTime));
}


void Binder::bind(std::size_t pos, const Date& val, Direction dir, const WhenNullCb& /*nullCb*/)
{
	poco_assert(dir == PD_IN);
	realBind(pos, Poco::SQL::MetaColumn::FDT_DATE, &val, sizeof(Date));
}


void Binder::bind(std::size_t pos, const Time& val, Direction dir, const WhenNullCb& /*nullCb*/)
{
	poco_assert(dir == PD_IN);
	realBind(pos, Poco::SQL::MetaColumn::FDT_TIME, &val, sizeof(Time));
}


void Binder::bind(std::size_t pos, const NullData&, Direction dir, const std::type_info&)
{
	poco_assert(dir == PD_IN);
	realBind(pos, Poco::SQL::MetaColumn::FDT_UNKNOWN, 0, 0);
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
	InputParameterVector::iterator itr		= _bindVector.begin();
	InputParameterVector::iterator itrEnd	= _bindVector.end();

	for (; itr != itrEnd; ++itr)
	{
		switch (itr->fieldType())
		{
		case Poco::SQL::MetaColumn::FDT_INT8:
			itr->setStringVersionRepresentation(Poco::NumberFormatter::format(* static_cast<const Poco::Int8*>(itr->pData())));
			break;

		case Poco::SQL::MetaColumn::FDT_UINT8:
			itr->setStringVersionRepresentation(Poco::NumberFormatter::format(* static_cast<const Poco::UInt8*>(itr->pData())));
			break;

		case Poco::SQL::MetaColumn::FDT_INT16:
			itr->setStringVersionRepresentation(Poco::NumberFormatter::format(* static_cast<const Poco::Int16*>(itr->pData())));
			break;

		case Poco::SQL::MetaColumn::FDT_UINT16:
			itr->setStringVersionRepresentation(Poco::NumberFormatter::format(* static_cast<const Poco::UInt16*>(itr->pData())));
			break;

		case Poco::SQL::MetaColumn::FDT_INT32:
			itr->setStringVersionRepresentation(Poco::NumberFormatter::format(* static_cast<const Poco::Int32*>(itr->pData())));
			break;

		case Poco::SQL::MetaColumn::FDT_UINT32:
			itr->setStringVersionRepresentation(Poco::NumberFormatter::format(* static_cast<const Poco::UInt32*>(itr->pData())));
			break;

		case Poco::SQL::MetaColumn::FDT_INT64:
			itr->setStringVersionRepresentation(Poco::NumberFormatter::format(* static_cast<const Poco::Int64*>(itr->pData())));
			break;

		case Poco::SQL::MetaColumn::FDT_UINT64:
			itr->setStringVersionRepresentation(Poco::NumberFormatter::format(* static_cast<const Poco::UInt64*>(itr->pData())));
			break;

		case Poco::SQL::MetaColumn::FDT_BOOL:
			{
				const bool currentBoolValue = * static_cast<const bool*>(itr->pData());
				itr->setStringVersionRepresentation(currentBoolValue ? "TRUE" : "FALSE");
			}
			break;

		case Poco::SQL::MetaColumn::FDT_FLOAT:
			itr->setStringVersionRepresentation(Poco::NumberFormatter::format(* static_cast<const float*>(itr->pData())));
			break;

		case Poco::SQL::MetaColumn::FDT_DOUBLE:
			itr->setStringVersionRepresentation(Poco::NumberFormatter::format(* static_cast<const double*>(itr->pData())));
			break;

//		case Poco::SQL::MetaColumn::FDT_CHAR:
//			itr->setStringVersionRepresentation(std::string(static_cast<const char*>(itr->pData()), 1));  // single character string
//			break;

		case Poco::SQL::MetaColumn::FDT_STRING:
			itr->setStringVersionRepresentation(* static_cast<const std::string*>(itr->pData()));
			break;

		case Poco::SQL::MetaColumn::FDT_TIMESTAMP:
			{
				const Poco::DateTime& dateTime = * static_cast<const Poco::DateTime*>(itr->pData());
				itr->setStringVersionRepresentation(DateTimeFormatter::format(dateTime, Poco::DateTimeFormat::ISO8601_FRAC_FORMAT));
			}
			break;

		case Poco::SQL::MetaColumn::FDT_DATE:
			{
				const Poco::SQL::Date& date = * static_cast<const Poco::SQL::Date*>(itr->pData());
				itr->setStringVersionRepresentation(DateTimeFormatter::format(Poco::DateTime(date.year(), date.month(), date.day()), "%Y-%m-%d"));
			}
			break;

		case Poco::SQL::MetaColumn::FDT_TIME:
			{
				const Poco::SQL::Time& time = * static_cast<const Poco::SQL::Time*>(itr->pData());
				itr->setStringVersionRepresentation(DateTimeFormatter::format(Poco::DateTime(0, 1, 1, time.hour(), time.minute(), time.second()), "%H:%M:%s%z"));
			}
			break;

		case Poco::SQL::MetaColumn::FDT_BLOB:
			{
				const Poco::SQL::BLOB& blob = * static_cast<const Poco::SQL::BLOB*>(itr->pData());
				itr->setNonStringVersionRepresentation(static_cast<const void*> (blob.rawContent()), blob.size());
			}
			break;

		case Poco::SQL::MetaColumn::FDT_CLOB:
			{
				const Poco::SQL::CLOB& clob = * static_cast<const Poco::SQL::CLOB*>(itr->pData());
				itr->setNonStringVersionRepresentation(static_cast<const void*> (clob.rawContent()), clob.size());
			}

		case Poco::SQL::MetaColumn::FDT_UNKNOWN:
		default:
			break;
		}
	}
}


//
// Private
//

void Binder::realBind(std::size_t aPosition, Poco::SQL::MetaColumn::ColumnDataType aFieldType, const void* aBufferPtr, std::size_t aLength)
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


void Binder::bind(std::size_t pos, const std::vector<Poco::Int8>& val, Direction dir)
{
    poco_assert(dir == PD_IN);

    std::vector<Poco::Int8>::iterator first = const_cast<std::vector<Poco::Int8> &>(val).begin();
	std::vector<Poco::Int8>::iterator last = const_cast<std::vector<Poco::Int8> &>(val).end();
    realContainerBind<std::vector<Poco::Int8>::iterator, Poco::Int8>(pos, first, last);
}


void Binder::bind(std::size_t pos, const std::deque<Poco::Int8>& val, Direction dir)
{
    poco_assert(dir == PD_IN);

    std::deque<Poco::Int8>::iterator first = const_cast<std::deque<Poco::Int8> &>(val).begin();
    std::deque<Poco::Int8>::iterator last = const_cast<std::deque<Poco::Int8> &>(val).end();
    realContainerBind<std::deque<Poco::Int8>::iterator, Poco::Int8>(pos, first, last);
}


void Binder::bind(std::size_t pos, const std::list<Poco::Int8>& val, Direction dir)
{
    poco_assert(dir == PD_IN);

    std::list<Poco::Int8>::iterator first = const_cast<std::list<Poco::Int8> &>(val).begin();
    std::list<Poco::Int8>::iterator last = const_cast<std::list<Poco::Int8> &>(val).end();
    realContainerBind<std::list<Poco::Int8>::iterator, Poco::Int8>(pos, first, last);
}


void Binder::bind(std::size_t pos, const std::vector<Poco::UInt8>& val, Direction dir)
{
    poco_assert(dir == PD_IN);

	std::vector<Poco::UInt8>::iterator first = const_cast<std::vector<Poco::UInt8> &>(val).begin();
	std::vector<Poco::UInt8>::iterator last = const_cast<std::vector<Poco::UInt8> &>(val).end();
	realContainerBind<std::vector<Poco::UInt8>::iterator, Poco::UInt8>(pos, first, last);
}


void Binder::bind(std::size_t pos, const std::deque<Poco::UInt8>& val, Direction dir)
{
    poco_assert(dir == PD_IN);

    std::deque<Poco::UInt8>::iterator first = const_cast<std::deque<Poco::UInt8> &>(val).begin();
    std::deque<Poco::UInt8>::iterator last = const_cast<std::deque<Poco::UInt8> &>(val).end();
    realContainerBind<std::deque<Poco::UInt8>::iterator, Poco::UInt8>(pos, first, last);
}


void Binder::bind(std::size_t pos, const std::list<Poco::UInt8>& val, Direction dir)
{
    poco_assert(dir == PD_IN);

    std::list<Poco::UInt8>::iterator first = const_cast<std::list<Poco::UInt8> &>(val).begin();
    std::list<Poco::UInt8>::iterator last = const_cast<std::list<Poco::UInt8> &>(val).end();
    realContainerBind<std::list<Poco::UInt8>::iterator, Poco::UInt8>(pos, first, last);
}


void Binder::bind(std::size_t pos, const std::vector<Poco::Int16>& val, Direction dir)
{
    poco_assert(dir == PD_IN);

    std::vector<Poco::Int16>::iterator first = const_cast<std::vector<Poco::Int16> &>(val).begin();
    std::vector<Poco::Int16>::iterator last = const_cast<std::vector<Poco::Int16> &>(val).end();
    realContainerBind<std::vector<Poco::Int16>::iterator, Poco::Int16>(pos, first, last);
}


void Binder::bind(std::size_t pos, const std::deque<Poco::Int16>& val, Direction dir)
{
    poco_assert(dir == PD_IN);

    std::deque<Poco::Int16>::iterator first = const_cast<std::deque<Poco::Int16> &>(val).begin();
    std::deque<Poco::Int16>::iterator last = const_cast<std::deque<Poco::Int16> &>(val).end();
    realContainerBind<std::deque<Poco::Int16>::iterator, Poco::Int16>(pos, first, last);
}


void Binder::bind(std::size_t pos, const std::list<Poco::Int16>& val, Direction dir)
{
    poco_assert(dir == PD_IN);

    std::list<Poco::Int16>::iterator first = const_cast<std::list<Poco::Int16> &>(val).begin();
    std::list<Poco::Int16>::iterator last = const_cast<std::list<Poco::Int16> &>(val).end();
    realContainerBind<std::list<Poco::Int16>::iterator, Poco::Int16>(pos, first, last);
}


void Binder::bind(std::size_t pos, const std::vector<Poco::UInt16>& val, Direction dir)
{
    poco_assert(dir == PD_IN);

    std::vector<Poco::UInt16>::iterator first = const_cast<std::vector<Poco::UInt16> &>(val).begin();
    std::vector<Poco::UInt16>::iterator last = const_cast<std::vector<Poco::UInt16> &>(val).end();
    realContainerBind<std::vector<Poco::UInt16>::iterator, Poco::UInt16>(pos, first, last);
}


void Binder::bind(std::size_t pos, const std::deque<Poco::UInt16>& val, Direction dir)
{
    poco_assert(dir == PD_IN);

    std::deque<Poco::UInt16>::iterator first = const_cast<std::deque<Poco::UInt16> &>(val).begin();
    std::deque<Poco::UInt16>::iterator last = const_cast<std::deque<Poco::UInt16> &>(val).end();
    realContainerBind<std::deque<Poco::UInt16>::iterator, Poco::UInt16>(pos, first, last);
}


void Binder::bind(std::size_t pos, const std::list<Poco::UInt16>& val, Direction dir)
{
    poco_assert(dir == PD_IN);

    std::list<Poco::UInt16>::iterator first = const_cast<std::list<Poco::UInt16> &>(val).begin();
    std::list<Poco::UInt16>::iterator last = const_cast<std::list<Poco::UInt16> &>(val).end();
    realContainerBind<std::list<Poco::UInt16>::iterator, Poco::UInt16>(pos, first, last);
}


void Binder::bind(std::size_t pos, const std::vector<Poco::Int32>& val, Direction dir)
{
    poco_assert(dir == PD_IN);

    std::vector<Poco::Int32>::iterator first = const_cast<std::vector<Poco::Int32> &>(val).begin();
    std::vector<Poco::Int32>::iterator last = const_cast<std::vector<Poco::Int32> &>(val).end();
    realContainerBind<std::vector<Poco::Int32>::iterator, Poco::Int32>(pos, first, last);
}


void Binder::bind(std::size_t pos, const std::deque<Poco::Int32>& val, Direction dir)
{
    poco_assert(dir == PD_IN);

    std::deque<Poco::Int32>::iterator first = const_cast<std::deque<Poco::Int32> &>(val).begin();
    std::deque<Poco::Int32>::iterator last = const_cast<std::deque<Poco::Int32> &>(val).end();
    realContainerBind<std::deque<Poco::Int32>::iterator, Poco::Int32>(pos, first, last);
}


void Binder::bind(std::size_t pos, const std::list<Poco::Int32>& val, Direction dir)
{
    poco_assert(dir == PD_IN);

    std::list<Poco::Int32>::iterator first = const_cast<std::list<Poco::Int32> &>(val).begin();
    std::list<Poco::Int32>::iterator last = const_cast<std::list<Poco::Int32> &>(val).end();
    realContainerBind<std::list<Poco::Int32>::iterator, Poco::Int32>(pos, first, last);
}


void Binder::bind(std::size_t pos, const std::vector<Poco::UInt32>& val, Direction dir)
{
    poco_assert(dir == PD_IN);

    std::vector<Poco::UInt32>::iterator first = const_cast<std::vector<Poco::UInt32> &>(val).begin();
    std::vector<Poco::UInt32>::iterator last = const_cast<std::vector<Poco::UInt32> &>(val).end();
    realContainerBind<std::vector<Poco::UInt32>::iterator, Poco::UInt32>(pos, first, last);
}


void Binder::bind(std::size_t pos, const std::deque<Poco::UInt32>& val, Direction dir)
{
    poco_assert(dir == PD_IN);

    std::deque<Poco::UInt32>::iterator first = const_cast<std::deque<Poco::UInt32> &>(val).begin();
    std::deque<Poco::UInt32>::iterator last = const_cast<std::deque<Poco::UInt32> &>(val).end();
    realContainerBind<std::deque<Poco::UInt32>::iterator, Poco::UInt32>(pos, first, last);
}


void Binder::bind(std::size_t pos, const std::list<Poco::UInt32>& val, Direction dir)
{
    poco_assert(dir == PD_IN);

    std::list<Poco::UInt32>::iterator first = const_cast<std::list<Poco::UInt32> &>(val).begin();
    std::list<Poco::UInt32>::iterator last = const_cast<std::list<Poco::UInt32> &>(val).end();
    realContainerBind<std::list<Poco::UInt32>::iterator, Poco::UInt32>(pos, first, last);
}


void Binder::bind(std::size_t pos, const std::vector<Poco::Int64>& val, Direction dir)
{
    poco_assert(dir == PD_IN);

    std::vector<Poco::Int64>::iterator first = const_cast<std::vector<Poco::Int64> &>(val).begin();
    std::vector<Poco::Int64>::iterator last = const_cast<std::vector<Poco::Int64> &>(val).end();
    realContainerBind<std::vector<Poco::Int64>::iterator, Poco::Int64>(pos, first, last);
}


void Binder::bind(std::size_t pos, const std::deque<Poco::Int64>& val, Direction dir)
{
    poco_assert(dir == PD_IN);

    std::deque<Poco::Int64>::iterator first = const_cast<std::deque<Poco::Int64> &>(val).begin();
    std::deque<Poco::Int64>::iterator last = const_cast<std::deque<Poco::Int64> &>(val).end();
    realContainerBind<std::deque<Poco::Int64>::iterator, Poco::Int64>(pos, first, last);
}


void Binder::bind(std::size_t pos, const std::list<Poco::Int64>& val, Direction dir)
{
    poco_assert(dir == PD_IN);

    std::list<Poco::Int64>::iterator first = const_cast<std::list<Poco::Int64> &>(val).begin();
    std::list<Poco::Int64>::iterator last = const_cast<std::list<Poco::Int64> &>(val).end();
    realContainerBind<std::list<Poco::Int64>::iterator, Poco::Int64>(pos, first, last);
}


void Binder::bind(std::size_t pos, const std::vector<Poco::UInt64>& val, Direction dir)
{
    poco_assert(dir == PD_IN);

    std::vector<Poco::UInt64>::iterator first = const_cast<std::vector<Poco::UInt64> &>(val).begin();
    std::vector<Poco::UInt64>::iterator last = const_cast<std::vector<Poco::UInt64> &>(val).end();
    realContainerBind<std::vector<Poco::UInt64>::iterator, Poco::UInt64>(pos, first, last);
}


void Binder::bind(std::size_t pos, const std::deque<Poco::UInt64>& val, Direction dir)
{
    poco_assert(dir == PD_IN);

    std::deque<Poco::UInt64>::iterator first = const_cast<std::deque<Poco::UInt64> &>(val).begin();
    std::deque<Poco::UInt64>::iterator last = const_cast<std::deque<Poco::UInt64> &>(val).end();
    realContainerBind<std::deque<Poco::UInt64>::iterator, Poco::UInt64>(pos, first, last);
}


void Binder::bind(std::size_t pos, const std::list<Poco::UInt64>& val, Direction dir)
{
    poco_assert(dir == PD_IN);

    std::list<Poco::UInt64>::iterator first = const_cast<std::list<Poco::UInt64> &>(val).begin();
    std::list<Poco::UInt64>::iterator last = const_cast<std::list<Poco::UInt64> &>(val).end();
    realContainerBind<std::list<Poco::UInt64>::iterator, Poco::UInt64>(pos, first, last);
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


void Binder::bind(std::size_t pos, const std::vector<float>& val, Direction dir)
{
    poco_assert(dir == PD_IN);

    std::vector<float>::iterator first = const_cast<std::vector<float> &>(val).begin();
    std::vector<float>::iterator last = const_cast<std::vector<float> &>(val).end();
    realContainerBind<std::vector<float>::iterator, float>(pos, first, last);
}


void Binder::bind(std::size_t pos, const std::deque<float>& val, Direction dir)
{
    poco_assert(dir == PD_IN);

    std::deque<float>::iterator first = const_cast<std::deque<float> &>(val).begin();
    std::deque<float>::iterator last = const_cast<std::deque<float> &>(val).end();
    realContainerBind<std::deque<float>::iterator, float>(pos, first, last);
}


void Binder::bind(std::size_t pos, const std::list<float>& val, Direction dir)
{
    poco_assert(dir == PD_IN);

    std::list<float>::iterator first = const_cast<std::list<float> &>(val).begin();
    std::list<float>::iterator last = const_cast<std::list<float> &>(val).end();
    realContainerBind<std::list<float>::iterator, float>(pos, first, last);
}


void Binder::bind(std::size_t pos, const std::vector<double>& val, Direction dir)
{
    poco_assert(dir == PD_IN);

    std::vector<double>::iterator first = const_cast<std::vector<double> &>(val).begin();
    std::vector<double>::iterator last = const_cast<std::vector<double> &>(val).end();
    realContainerBind<std::vector<double>::iterator, double>(pos, first, last);
}


void Binder::bind(std::size_t pos, const std::deque<double>& val, Direction dir)
{
    poco_assert(dir == PD_IN);

    std::deque<double>::iterator first = const_cast<std::deque<double> &>(val).begin();
    std::deque<double>::iterator last = const_cast<std::deque<double> &>(val).end();
    realContainerBind<std::deque<double>::iterator, double>(pos, first, last);
}


void Binder::bind(std::size_t pos, const std::list<double>& val, Direction dir)
{
    poco_assert(dir == PD_IN);

    std::list<double>::iterator first = const_cast<std::list<double> &>(val).begin();
    std::list<double>::iterator last = const_cast<std::list<double> &>(val).end();
    realContainerBind<std::list<double>::iterator, double>(pos, first, last);
}


void Binder::bind(std::size_t pos, const std::vector<char>& val, Direction dir)
{
    poco_assert(dir == PD_IN);

    std::vector<char>::iterator first = const_cast<std::vector<char> &>(val).begin();
    std::vector<char>::iterator last = const_cast<std::vector<char> &>(val).end();
    realContainerBind<std::vector<char>::iterator, char>(pos, first, last);
}


void Binder::bind(std::size_t pos, const std::deque<char>& val, Direction dir)
{
    poco_assert(dir == PD_IN);

    std::deque<char>::iterator first = const_cast<std::deque<char> &>(val).begin();
    std::deque<char>::iterator last = const_cast<std::deque<char> &>(val).end();
    realContainerBind<std::deque<char>::iterator, char>(pos, first, last);
}


void Binder::bind(std::size_t pos, const std::list<char>& val, Direction dir)
{
    poco_assert(dir == PD_IN);

    std::list<char>::iterator first = const_cast<std::list<char> &>(val).begin();
    std::list<char>::iterator last = const_cast<std::list<char> &>(val).end();
    realContainerBind<std::list<char>::iterator, char>(pos, first, last);
}


void Binder::bind(std::size_t pos, const std::vector<Poco::SQL::BLOB>& val, Direction dir)
{
    poco_assert(dir == PD_IN);

    std::vector<Poco::SQL::BLOB>::iterator first = const_cast<std::vector<Poco::SQL::BLOB> &>(val).begin();
    std::vector<Poco::SQL::BLOB>::iterator last = const_cast<std::vector<Poco::SQL::BLOB> &>(val).end();
    realContainerBind<std::vector<Poco::SQL::BLOB>::iterator, Poco::SQL::BLOB>(pos, first, last);
}


void Binder::bind(std::size_t pos, const std::deque<Poco::SQL::BLOB>& val, Direction dir)
{
    poco_assert(dir == PD_IN);

    std::deque<Poco::SQL::BLOB>::iterator first = const_cast<std::deque<Poco::SQL::BLOB> &>(val).begin();
    std::deque<Poco::SQL::BLOB>::iterator last = const_cast<std::deque<Poco::SQL::BLOB> &>(val).end();
    realContainerBind<std::deque<Poco::SQL::BLOB>::iterator, Poco::SQL::BLOB>(pos, first, last);
}


void Binder::bind(std::size_t pos, const std::list<Poco::SQL::BLOB>& val, Direction dir)
{
    poco_assert(dir == PD_IN);

    std::list<Poco::SQL::BLOB>::iterator first = const_cast<std::list<Poco::SQL::BLOB> &>(val).begin();
    std::list<Poco::SQL::BLOB>::iterator last = const_cast<std::list<Poco::SQL::BLOB> &>(val).end();
    realContainerBind<std::list<Poco::SQL::BLOB>::iterator, Poco::SQL::BLOB>(pos, first, last);
}


void Binder::bind(std::size_t pos, const std::vector<Poco::SQL::CLOB>& val, Direction dir)
{
    poco_assert(dir == PD_IN);

    std::vector<Poco::SQL::CLOB>::iterator first = const_cast<std::vector<Poco::SQL::CLOB> &>(val).begin();
    std::vector<Poco::SQL::CLOB>::iterator last = const_cast<std::vector<Poco::SQL::CLOB> &>(val).end();
    realContainerBind<std::vector<Poco::SQL::CLOB>::iterator, Poco::SQL::CLOB>(pos, first, last);
}


void Binder::bind(std::size_t pos, const std::deque<Poco::SQL::CLOB>& val, Direction dir)
{
    poco_assert(dir == PD_IN);

    std::deque<Poco::SQL::CLOB>::iterator first = const_cast<std::deque<Poco::SQL::CLOB> &>(val).begin();
    std::deque<Poco::SQL::CLOB>::iterator last = const_cast<std::deque<Poco::SQL::CLOB> &>(val).end();
    realContainerBind<std::deque<Poco::SQL::CLOB>::iterator, Poco::SQL::CLOB>(pos, first, last);
}


void Binder::bind(std::size_t pos, const std::list<Poco::SQL::CLOB>& val, Direction dir)
{
    poco_assert(dir == PD_IN);

    std::list<Poco::SQL::CLOB>::iterator first = const_cast<std::list<Poco::SQL::CLOB> &>(val).begin();
    std::list<Poco::SQL::CLOB>::iterator last = const_cast<std::list<Poco::SQL::CLOB> &>(val).end();
    realContainerBind<std::list<Poco::SQL::CLOB>::iterator, Poco::SQL::CLOB>(pos, first, last);
}


void Binder::bind(std::size_t pos, const std::vector<Poco::DateTime>& val, Direction dir)
{
    poco_assert(dir == PD_IN);

    std::vector<Poco::DateTime>::iterator first = const_cast<std::vector<Poco::DateTime> &>(val).begin();
    std::vector<Poco::DateTime>::iterator last = const_cast<std::vector<Poco::DateTime> &>(val).end();
    realContainerBind<std::vector<Poco::DateTime>::iterator, Poco::DateTime>(pos, first, last);
}


void Binder::bind(std::size_t pos, const std::deque<Poco::DateTime>& val, Direction dir)
{
    poco_assert(dir == PD_IN);

    std::deque<Poco::DateTime>::iterator first = const_cast<std::deque<Poco::DateTime> &>(val).begin();
    std::deque<Poco::DateTime>::iterator last = const_cast<std::deque<Poco::DateTime> &>(val).end();
    realContainerBind<std::deque<Poco::DateTime>::iterator, Poco::DateTime>(pos, first, last);
}


void Binder::bind(std::size_t pos, const std::list<Poco::DateTime>& val, Direction dir)
{
    poco_assert(dir == PD_IN);

    std::list<Poco::DateTime>::iterator first = const_cast<std::list<Poco::DateTime> &>(val).begin();
    std::list<Poco::DateTime>::iterator last = const_cast<std::list<Poco::DateTime> &>(val).end();
    realContainerBind<std::list<Poco::DateTime>::iterator, Poco::DateTime>(pos, first, last);
}


void Binder::bind(std::size_t pos, const std::vector<Poco::SQL::Date>& val, Direction dir)
{
    poco_assert(dir == PD_IN);

    std::vector<Poco::SQL::Date>::iterator first = const_cast<std::vector<Poco::SQL::Date> &>(val).begin();
    std::vector<Poco::SQL::Date>::iterator last = const_cast<std::vector<Poco::SQL::Date> &>(val).end();
    realContainerBind<std::vector<Poco::SQL::Date>::iterator, Poco::SQL::Date>(pos, first, last);
}


void Binder::bind(std::size_t pos, const std::deque<Poco::SQL::Date>& val, Direction dir)
{
    poco_assert(dir == PD_IN);

    std::deque<Poco::SQL::Date>::iterator first = const_cast<std::deque<Poco::SQL::Date> &>(val).begin();
    std::deque<Poco::SQL::Date>::iterator last = const_cast<std::deque<Poco::SQL::Date> &>(val).end();
    realContainerBind<std::deque<Poco::SQL::Date>::iterator, Poco::SQL::Date>(pos, first, last);
}


void Binder::bind(std::size_t pos, const std::list<Poco::SQL::Date>& val, Direction dir)
{
    poco_assert(dir == PD_IN);

    std::list<Poco::SQL::Date>::iterator first = const_cast<std::list<Poco::SQL::Date> &>(val).begin();
    std::list<Poco::SQL::Date>::iterator last = const_cast<std::list<Poco::SQL::Date> &>(val).end();
    realContainerBind<std::list<Poco::SQL::Date>::iterator, Poco::SQL::Date>(pos, first, last);
}


void Binder::bind(std::size_t pos, const std::vector<Poco::SQL::Time>& val, Direction dir)
{
    poco_assert(dir == PD_IN);

    std::vector<Poco::SQL::Time>::iterator first = const_cast<std::vector<Poco::SQL::Time> &>(val).begin();
    std::vector<Poco::SQL::Time>::iterator last = const_cast<std::vector<Poco::SQL::Time> &>(val).end();
    realContainerBind<std::vector<Poco::SQL::Time>::iterator, Poco::SQL::Time>(pos, first, last);
}


void Binder::bind(std::size_t pos, const std::deque<Poco::SQL::Time>& val, Direction dir)
{
    poco_assert(dir == PD_IN);

    std::deque<Poco::SQL::Time>::iterator first = const_cast<std::deque<Poco::SQL::Time> &>(val).begin();
    std::deque<Poco::SQL::Time>::iterator last = const_cast<std::deque<Poco::SQL::Time> &>(val).end();
    realContainerBind<std::deque<Poco::SQL::Time>::iterator, Poco::SQL::Time>(pos, first, last);
}


void Binder::bind(std::size_t pos, const std::list<Poco::SQL::Time>& val, Direction dir)
{
    poco_assert(dir == PD_IN);

    std::list<Poco::SQL::Time>::iterator first = const_cast<std::list<Poco::SQL::Time> &>(val).begin();
    std::list<Poco::SQL::Time>::iterator last = const_cast<std::list<Poco::SQL::Time> &>(val).end();
    realContainerBind<std::list<Poco::SQL::Time>::iterator, Poco::SQL::Time>(pos, first, last);
}


void Binder::bind(std::size_t /*pos*/, const std::vector<Poco::SQL::NullData>& /*val*/, Direction /*dir*/, const std::type_info&)
{
	throw NotImplementedException();
}


void Binder::bind(std::size_t /*pos*/, const std::deque<Poco::SQL::NullData>& /*val*/, Direction /*dir*/, const std::type_info&)
{
    throw NotImplementedException();
}


void Binder::bind(std::size_t /*pos*/, const std::list<Poco::SQL::NullData>& /*val*/, Direction /*dir*/, const std::type_info&)
{
	throw NotImplementedException();
}


void Binder::bind(std::size_t pos, const std::vector<std::string>& val, Direction dir)
{
    poco_assert(dir == PD_IN);

    std::vector<std::string>::iterator first = const_cast<std::vector<std::string> &>(val).begin();
    std::vector<std::string>::iterator last = const_cast<std::vector<std::string> &>(val).end();
    realContainerBind<std::vector<std::string>::iterator, std::string>(pos, first, last);
}


void Binder::bind(std::size_t pos, const std::deque<std::string>& val, Direction dir)
{
    poco_assert(dir == PD_IN);

    std::deque<std::string>::iterator first = const_cast<std::deque<std::string> &>(val).begin();
    std::deque<std::string>::iterator last = const_cast<std::deque<std::string> &>(val).end();
    realContainerBind<std::deque<std::string>::iterator, std::string>(pos, first, last);
}


void Binder::bind(std::size_t pos, const std::list<std::string>& val, Direction dir)
{
    poco_assert(dir == PD_IN);

    std::list<std::string>::iterator first = const_cast<std::list<std::string> &>(val).begin();
    std::list<std::string>::iterator last = const_cast<std::list<std::string> &>(val).end();
    realContainerBind<std::list<std::string>::iterator, std::string>(pos, first, last);
}


} } } // namespace Poco::SQL::PostgreSQL
