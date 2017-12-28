//
// AbstractExtraction.cpp
//
// Library: Data
// Package: DataCore
// Module:  AbstractExtraction
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/SQL/AbstractExtraction.h"


namespace Poco {
namespace SQL {


AbstractExtraction::AbstractExtraction(const std::string& type, Poco::UInt32 limit,
	Poco::UInt32 extractionPosition,
	bool bulk):
	_type(type),
	_pExtractor(0),
	_limit(limit),
	_position(extractionPosition),
	_bulk(bulk),
	_emptyStringIsNull(false),
	_forceEmptyString(false)
{
}


AbstractExtraction::~AbstractExtraction()
{
}


} } // namespace Poco::SQL
