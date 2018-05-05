//
// PostgreSQLTypes.h
//
// Library: Data/PostgreSQL
// Package: PostgreSQL
// Module: PostgreSQLTypes
//
// Definition of the SessionHandle class.
//
// Copyright (c) 2015, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//

#ifndef SQL_PostgreSQL_Types_INCLUDED
#define SQL_PostgreSQL_Types_INCLUDED

#include "Poco/SQL/MetaColumn.h"

#include <vector>

#include <libpq-fe.h>

namespace Poco {
namespace SQL {
namespace PostgreSQL {

/// Oid constants duplicated from PostgreSQL "include/postgresql/server/catalog/pg_type.h"
/// because PostgreSQL compile time definitions are too onerous to reproduce for this module

const Oid BOOLOID		= 16;

const Oid INT2OID		= 21;
const Oid INT4OID		= 23;
const Oid INT8OID		= 20;

const Oid FLOAT8OID		= 701; // double
const Oid FLOAT4OID		= 700;
const Oid NUMERICOID	= 1700;

const Oid CHAROID		= 18;
const Oid BPCHAROID		= 1042; // fixed length char
const Oid VARCHAROID	= 1043;

const Oid BYTEAOID		= 17; // BLOB
const Oid TEXTOID		= 25; // CLOB

const Oid DATEOID		= 1082;
const Oid TIMEOID		= 1083;
const Oid TIMETZOID		= 1266;
const Oid TIMESTAMPOID	= 1114;
const Oid TIMESTAMPZOID	= 1184;

// future use
const Oid BITOID		= 1560;
const Oid VARYBITOID	= 1562;
const Oid CASHOID		= 790;
const Oid MACADDROID	= 829;
const Oid UUIDOID		= 2950;

Poco::SQL::MetaColumn::ColumnDataType oidToColumnDataType(const Oid anOID);

class InputParameter
	/// PostgreSQL class to record values for input parameters to SQL statements
{
public:
	typedef Poco::SQL::MetaColumn::ColumnDataType CDT;

	explicit InputParameter(CDT fieldType, const void* dataPtr, std::size_t dataLength);
	explicit InputParameter();

	~InputParameter();

	CDT         fieldType() const;
	const void* pData() const;
	std::size_t size() const;
	bool        isBinary() const;

	void setStringVersionRepresentation(const std::string& aString);
	void setNonStringVersionRepresentation(const void* aPtr, std::size_t theSize);

	const void* pInternalRepresentation() const;

private:
	CDT         _fieldType;
	const void* _pData;
	std::size_t _size;
	bool        _isBinary;
	std::string _stringVersionRepresentation;
	void*       _pNonStringVersionRepresentation;
};

typedef std::vector <InputParameter> InputParameterVector;


class OutputParameter
	/// PostgreSQL class to record values for output parameters to capture the results
{
public:
	typedef Poco::SQL::MetaColumn::ColumnDataType CDT;

	OutputParameter(CDT aFieldType, Oid internalFieldType, std::size_t rowNumber,
		const char* dataPtr, std::size_t size, bool isNull);

	OutputParameter();
	~OutputParameter();

	void setValues(CDT fieldType, Oid internalFieldType, std::size_t rowNumber,
		const char* dataPtr, std::size_t size, bool isNull);

	CDT         fieldType() const;
	Oid         internalFieldType() const;
	std::size_t rowNumber() const;
	const char* pData() const;
	std::size_t size() const;
	bool        isNull() const;

private:


	CDT         _fieldType;
	Oid         _internalFieldType;
	std::size_t _rowNumber;
	const char* _pData;
	std::size_t _size;
	bool        _isNull;
};

typedef std::vector <OutputParameter> OutputParameterVector;


class PQConnectionInfoOptionsFree
	/// PostgreSQL connection Info Options free (RAII)
{
public:
	explicit PQConnectionInfoOptionsFree(PQconninfoOption* aConnectionInfoOptionPtr);
	~PQConnectionInfoOptionsFree();

private:
	PQConnectionInfoOptionsFree            (const PQConnectionInfoOptionsFree&);
	PQConnectionInfoOptionsFree& operator= (const PQConnectionInfoOptionsFree&);

private:
    PQconninfoOption* _pConnectionInfoOption;
};

class PQResultClear
	/// PostgreSQL statement result free (RAII)
{
public:
	explicit PQResultClear(PGresult * aPQResultPtr);
	~PQResultClear();

private:
	PQResultClear            (const PQResultClear&);
	PQResultClear& operator= (const PQResultClear&);

private:
	PGresult* _pPQResult;
};


class PGCancelFree
	/// PostgreSQL Cancel Info Options free (RAII)
{
public:
	explicit PGCancelFree(PGcancel * aStatementCancelPtr);
	~PGCancelFree();

private:
	PGCancelFree            (const PGCancelFree&);
	PGCancelFree& operator= (const PGCancelFree&);

private:
	PGcancel* _pPGCancel;
};


//
// inlines
//

// InputParameter

inline InputParameter::InputParameter(Poco::SQL::MetaColumn::ColumnDataType	aFieldType,
	const void* aDataPtr, std::size_t theSize): _fieldType(aFieldType),
	_pData(aDataPtr),
	_size(theSize),
	_isBinary(false),
	_pNonStringVersionRepresentation(0)
{
	if (Poco::SQL::MetaColumn::FDT_BLOB == _fieldType
	 || Poco::SQL::MetaColumn::FDT_CLOB == _fieldType)
	{
		_isBinary = true;
	}
}


inline InputParameter::InputParameter(): _fieldType(Poco::SQL::MetaColumn::FDT_UNKNOWN),
	_pData(0),
	_size(0),
	_isBinary(false),
	_pNonStringVersionRepresentation(0)
{
}


inline InputParameter::~InputParameter()
{
}


inline const void* InputParameter::pData() const
{
    return _pData;
}


inline Poco::SQL::MetaColumn::ColumnDataType InputParameter::fieldType() const
{
	return _fieldType;
}


inline std::size_t InputParameter::size() const
{
	return _size;
}


inline bool InputParameter::isBinary() const
{
	return _isBinary;
}


inline void InputParameter::setStringVersionRepresentation(const std::string& aString)
{
	_pNonStringVersionRepresentation = 0;
	_stringVersionRepresentation = aString;
	_size = _stringVersionRepresentation.size();
}


inline void InputParameter::setNonStringVersionRepresentation(const void* aPtr, std::size_t theDataLength)
{
	_stringVersionRepresentation = std::string();
	_pNonStringVersionRepresentation = const_cast<void *> (aPtr);
	_size = theDataLength;
}


inline const void* InputParameter::pInternalRepresentation() const
{
	switch (_fieldType)
	{
	case Poco::SQL::MetaColumn::FDT_BOOL:
	case Poco::SQL::MetaColumn::FDT_INT8:
	case Poco::SQL::MetaColumn::FDT_UINT8:
	case Poco::SQL::MetaColumn::FDT_INT16:
	case Poco::SQL::MetaColumn::FDT_UINT16:
	case Poco::SQL::MetaColumn::FDT_INT32:
	case Poco::SQL::MetaColumn::FDT_UINT32:
	case Poco::SQL::MetaColumn::FDT_INT64:
	case Poco::SQL::MetaColumn::FDT_UINT64:
	case Poco::SQL::MetaColumn::FDT_FLOAT:
	case Poco::SQL::MetaColumn::FDT_DOUBLE:
	case Poco::SQL::MetaColumn::FDT_STRING:
	case Poco::SQL::MetaColumn::FDT_DATE:
	case Poco::SQL::MetaColumn::FDT_TIME:
	case Poco::SQL::MetaColumn::FDT_TIMESTAMP:
		return _stringVersionRepresentation.c_str();

	case Poco::SQL::MetaColumn::FDT_BLOB:
	case Poco::SQL::MetaColumn::FDT_CLOB:
		return _pNonStringVersionRepresentation;

	case Poco::SQL::MetaColumn::FDT_UNKNOWN:
	default: return 0;
    }
}


// OutputParameter

inline OutputParameter::OutputParameter(Poco::SQL::MetaColumn::ColumnDataType aFieldType,
	Oid anInternalFieldType,
	std::size_t aRowNumber,
	const char* aDataPtr,
	std::size_t theSize,
	bool anIsNull): _fieldType(aFieldType),
		_internalFieldType(anInternalFieldType),
		_rowNumber(aRowNumber),
		_pData(aDataPtr),
		_size(theSize),
		_isNull(anIsNull)
{
}


inline OutputParameter::OutputParameter()
    : _fieldType         (Poco::SQL::MetaColumn::FDT_UNKNOWN),
      _internalFieldType (static_cast<Oid>(-1)),
      _rowNumber         (0),
      _pData             (0),
      _size              (0),
      _isNull            (true)
{
}


inline OutputParameter::~OutputParameter()
{
}


inline void OutputParameter::setValues(Poco::SQL::MetaColumn::ColumnDataType aFieldType,
	Oid anInternalFieldType,
	std::size_t aRowNumber,
	const char* aDataPtr,
	std::size_t theSize,
	bool anIsNull)
{
    _fieldType         = aFieldType;
    _internalFieldType = anInternalFieldType;
    _rowNumber         = aRowNumber;
    _pData             = aDataPtr;
    _size              = theSize;
    _isNull            = anIsNull;
}


inline Poco::SQL::MetaColumn::ColumnDataType OutputParameter::fieldType() const
{
    return _fieldType;
}


inline Oid OutputParameter::internalFieldType() const
{
    return _internalFieldType;
}


inline std::size_t OutputParameter::rowNumber() const
{
    return _rowNumber;
}


inline const char* OutputParameter::pData() const
{
    return _pData;
}


inline std::size_t OutputParameter::size() const
{
    return _size;
}


inline bool OutputParameter::isNull() const
{
    return _isNull;
}


// PQConnectionInfoOptionsFree

inline PQConnectionInfoOptionsFree::PQConnectionInfoOptionsFree(PQconninfoOption* aConnectionInfoOptionPtr)
    : _pConnectionInfoOption(aConnectionInfoOptionPtr)
{
}


inline PQConnectionInfoOptionsFree::~PQConnectionInfoOptionsFree()
{
    if (_pConnectionInfoOption)
    {
        PQconninfoFree(_pConnectionInfoOption);
        _pConnectionInfoOption = 0;
    }
}


// PQResultClear

inline PQResultClear::PQResultClear(PGresult* aPQResultPtr)
    : _pPQResult(aPQResultPtr)
{
}


inline PQResultClear::~PQResultClear()
{
    if (_pPQResult)
    {
        PQclear(_pPQResult);
        _pPQResult = 0;
    }
}


// PGCancelFree

inline PGCancelFree::PGCancelFree(PGcancel* aStatementCancelPtr)
    : _pPGCancel(aStatementCancelPtr)
{
}


inline PGCancelFree::~PGCancelFree()
{
    if (_pPGCancel)
    {
        PQfreeCancel(_pPGCancel);
        _pPGCancel = 0;
    }
}


}}}

#endif // SQL_PostgreSQL_Types_INCLUDED
