//
// MySQLException.cpp
//
// Library: Data/MySQL
// Package: MySQL
// Module:  ResultMetadata
//
// Copyright (c) 2008, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Data/MySQL/ResultMetadata.h"
#include "Poco/Data/MySQL/MySQLException.h"
#include <cstring>


namespace
{
	class ResultMetadataHandle
		/// Simple exception-safe wrapper
	{
	public:
		explicit ResultMetadataHandle(MYSQL_STMT* stmt)
		{
			h = mysql_stmt_result_metadata(stmt);
		}

		~ResultMetadataHandle()
		{
			if (h)
			{
				mysql_free_result(h);
			}
		}

		operator MYSQL_RES* ()
		{
			return h;
		}

	private:
		MYSQL_RES* h;
	};

	std::size_t fieldSize(const MYSQL_FIELD& field)
		/// Convert field MySQL-type and field MySQL-length to actual field length
	{
		switch (field.type)
		{
		case MYSQL_TYPE_TINY:     return sizeof(char);
		case MYSQL_TYPE_SHORT:    return sizeof(short);
		case MYSQL_TYPE_INT24:
		case MYSQL_TYPE_LONG:     return sizeof(Poco::Int32);
		case MYSQL_TYPE_FLOAT:    return sizeof(float);
		case MYSQL_TYPE_DOUBLE:   return sizeof(double);
		case MYSQL_TYPE_LONGLONG: return sizeof(Poco::Int64);

		case MYSQL_TYPE_DATE:
		case MYSQL_TYPE_TIME:
		case MYSQL_TYPE_DATETIME:
		case MYSQL_TYPE_TIMESTAMP:
			return sizeof(MYSQL_TIME);

		case MYSQL_TYPE_DECIMAL:
		case MYSQL_TYPE_NEWDECIMAL:
		case MYSQL_TYPE_STRING:
		case MYSQL_TYPE_VAR_STRING:
		case MYSQL_TYPE_TINY_BLOB:
		case MYSQL_TYPE_MEDIUM_BLOB:
		case MYSQL_TYPE_LONG_BLOB:
		case MYSQL_TYPE_BLOB:
#ifdef POCO_MYSQL_JSON
		case MYSQL_TYPE_JSON:
#endif		
			return field.length;

		default:
			throw Poco::Data::MySQL::StatementException("unknown field type");
		}
	}

	Poco::Data::MetaColumn::ColumnDataType fieldType(const MYSQL_FIELD& field)
		/// Convert field MySQL-type to Poco-type
	{
		bool unsig = ((field.flags & UNSIGNED_FLAG) == UNSIGNED_FLAG);

		switch (field.type)
		{
		case MYSQL_TYPE_TINY:
			if (unsig) return Poco::Data::MetaColumn::FDT_UINT8;
			return Poco::Data::MetaColumn::FDT_INT8;

		case MYSQL_TYPE_SHORT:
			if (unsig) return Poco::Data::MetaColumn::FDT_UINT16;
			return Poco::Data::MetaColumn::FDT_INT16;

		case MYSQL_TYPE_INT24:
		case MYSQL_TYPE_LONG:
			if (unsig) return Poco::Data::MetaColumn::FDT_UINT32;
			return Poco::Data::MetaColumn::FDT_INT32;

		case MYSQL_TYPE_FLOAT:
			return Poco::Data::MetaColumn::FDT_FLOAT;

		case MYSQL_TYPE_DECIMAL:
		case MYSQL_TYPE_NEWDECIMAL:
		case MYSQL_TYPE_DOUBLE:
			return Poco::Data::MetaColumn::FDT_DOUBLE;

		case MYSQL_TYPE_LONGLONG:
			if (unsig) return Poco::Data::MetaColumn::FDT_UINT64;
			return Poco::Data::MetaColumn::FDT_INT64;

		case MYSQL_TYPE_DATE:
			return Poco::Data::MetaColumn::FDT_DATE;

		case MYSQL_TYPE_TIME:
			return Poco::Data::MetaColumn::FDT_TIME;

		case MYSQL_TYPE_DATETIME:
		case MYSQL_TYPE_TIMESTAMP:
			return Poco::Data::MetaColumn::FDT_TIMESTAMP;

		case MYSQL_TYPE_STRING:
		case MYSQL_TYPE_VAR_STRING:
			return Poco::Data::MetaColumn::FDT_STRING;

		case MYSQL_TYPE_TINY_BLOB:
		case MYSQL_TYPE_MEDIUM_BLOB:
		case MYSQL_TYPE_LONG_BLOB:
		case MYSQL_TYPE_BLOB:
			return Poco::Data::MetaColumn::FDT_BLOB;
#ifdef POCO_MYSQL_JSON
		case MYSQL_TYPE_JSON:
			return Poco::Data::MetaColumn::FDT_JSON;
#endif
		default:
			return Poco::Data::MetaColumn::FDT_UNKNOWN;
		}
	}
} // namespace


namespace Poco {
namespace Data {
namespace MySQL {


ResultMetadata::~ResultMetadata()
{
	freeMemory();
}


void ResultMetadata::reset()
{
	freeMemory();
	_columns.resize(0);
	_row.resize(0);
	_buffer.resize(0);
	_lengths.resize(0);
	_isNull.resize(0);
}


void ResultMetadata::freeMemory()
{
	for (std::vector<char*>::iterator it = _buffer.begin(); it != _buffer.end(); ++it)
		std::free(*it);
}


void ResultMetadata::init(MYSQL_STMT* stmt)
{
	ResultMetadataHandle h(stmt);

	if (!h)
	{
		// some queries (eg. INSERT) don't have result
		reset();
		return;
	}

	std::size_t count = mysql_num_fields(h);
	MYSQL_FIELD* fields = mysql_fetch_fields(h);

	_columns.reserve(count);

	for (std::size_t i = 0; i < count; i++)
	{
		std::size_t size = fieldSize(fields[i]);
		if (size == 0xFFFFFFFF) size = 0;

		_columns.push_back(MetaColumn(
			i,                               // position
			fields[i].name,                  // name
			fieldType(fields[i]),            // type
			size,                            // length
			0,                               // TODO: precision
			!IS_NOT_NULL(fields[i].flags)    // nullable
			));
	}

	_buffer.resize(count);
	_row.resize(count);
	_lengths.resize(count);
	_isNull.resize(count);

	for (std::size_t i = 0; i < count; i++)
	{
		std::memset(&_row[i], 0, sizeof(MYSQL_BIND));
		unsigned int len = static_cast<unsigned int>(_columns[i].length());
		_buffer[i] = (char*) std::calloc(len, sizeof(char));
		_row[i].buffer_type   = fields[i].type;
		_row[i].buffer_length = len;
		_row[i].buffer        = _buffer[i];
		_row[i].length        = &_lengths[i];
		_row[i].is_null       = reinterpret_cast<my_bool*>(&_isNull[i]); // workaround to make it work with both MySQL 8 and earlier
		_row[i].is_unsigned   = (fields[i].flags & UNSIGNED_FLAG) > 0;
	}
}


std::size_t ResultMetadata::columnsReturned() const
{
	return static_cast<std::size_t>(_columns.size());
}


const MetaColumn& ResultMetadata::metaColumn(std::size_t pos) const
{
	return _columns[pos];
}


MYSQL_BIND* ResultMetadata::row()
{
	return &_row[0];
}


std::size_t ResultMetadata::length(std::size_t pos) const
{
	return _lengths[pos];
}


const unsigned char* ResultMetadata::rawData(std::size_t pos) const
{
	if ((_lengths[pos] == 0) && (_row[pos].buffer == nullptr))
		return reinterpret_cast<const unsigned char*>("");
	else
		poco_check_ptr (_row[pos].buffer);
	return reinterpret_cast<const unsigned char*>(_row[pos].buffer);
}


bool ResultMetadata::isNull(std::size_t pos) const
{
	return (_isNull[pos] != 0);
}


void ResultMetadata::adjustColumnSizeToFit(std::size_t pos)
{
	std::free(_buffer[pos]);
	_buffer[pos] = (char*) std::calloc(_lengths[pos], sizeof(char));
	_row[pos].buffer = _buffer[pos];
	_row[pos].buffer_length = _lengths[pos];
}


} } } // namespace Poco::Data::MySQL
