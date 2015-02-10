//
// JSONFormatter.h
//
// $Id: //poco/Main/Data/include/Poco/Data/JSONFormatter.h#9 $
//
// Library: Data
// Package: DataCore
// Module:  JSONFormatter
//
// Definition of the JSONFormatter class.
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Data_JSONFormatter_INCLUDED
#define Data_JSONFormatter_INCLUDED


#include "Poco/Data/RowFormatter.h"


namespace Poco {
namespace Data {


class Data_API JSONFormatter: public Poco::Data::RowFormatter
	/// Class for JSON formatting of data rows.
{
public:	
	JSONFormatter();
		/// Creates a new JSONFormatter.

	~JSONFormatter();
		/// Destroy the JSONFormatter.

	std::string& formatNames(const NameVecPtr pNames, std::string& formattedNames);
	std::string& formatValues(const ValueVec& vals, std::string& formattedValues);
	
private:
	NameVecPtr _pNames;
	int        _rowCounter;
};


//
// inlines
//
inline std::string& JSONFormatter::formatNames(const NameVecPtr pNames, std::string& formattedNames)
{
	// names are used in formatValues
	if (pNames && !_pNames) _pNames = pNames;
	return formattedNames = "";
}

} } // namespace Poco::Data


#endif // Data_JSONFormatter_INCLUDED
