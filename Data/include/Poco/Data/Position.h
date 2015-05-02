//
// PositionExtraction.h
//
// $Id: //poco/Main/Data/include/Poco/Data/Position.h#9 $
//
// Library: Data
// Package: DataCore
// Module:  Position
//
// Definition of the PositionExtraction class.
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Data_Position_INCLUDED
#define Data_Position_INCLUDED


#include "Poco/Data/Limit.h"


namespace Poco {
namespace Data {


class Data_API Position
	/// Utility class wrapping unsigned integer. Used to
	/// indicate the recordset position in batch SQL statements.
{
public:
	typedef Poco::UInt32 Position_Type;

	Position(Position_Type value);
		/// Creates the Position.

	~Position();
		/// Destroys the Position.

	Position_Type value() const;
		/// Returns the position value.
	
private:
	Position();

	Position_Type _value;
};


///
/// inlines
///
inline Position::Position_Type Position::value() const
{ 
	return _value; 
}


namespace Keywords {


template <typename T>
inline Position from(const T& value)
	/// Convenience function for creation of position.
{
	return Position(value);
}


} // namespace Keywords


} } // namespace Poco::Data


#endif // Data_Position_INCLUDED
