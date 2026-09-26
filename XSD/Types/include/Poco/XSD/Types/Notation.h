//
// Notation.h
//
// Library: XSD/Types
// Package: XSDTypes
// Module:  Notation
//
// Definition of the Notation class.
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties, 
// copied or duplicated in any form, in whole or in part.
//


#ifndef XSDTypes_Notation_INCLUDED
#define XSDTypes_Notation_INCLUDED


#include "Poco/XSD/Types/XSDTypes.h"
#include "Poco/XSD/Types/AnnotatedObject.h"


namespace Poco {
namespace XSD {
namespace Types {


class XSDTypes_API Notation: public AnnotatedObject
	/// This class represents a notation in an XML Schema.
{
public:
	using Ptr = AutoPtr<Notation>;

	Notation(const std::string& id, const std::string& name, const std::string& pubToken, const std::string& system);
		/// Creates the Notation.

	virtual ~Notation();
		/// Destroys the Notation.

	const std::string& name() const;
		/// Gets the name of the notation.

	const std::string& getPublic() const;
		// Gets the public id

	const std::string& getSystem() const;
		/// Gets the system id.

	void accept(Visitor& v) const;

private:
	std::string _name;
	std::string _public;
	std::string _system;
};


//
// inlines
//
inline const std::string& Notation::name() const
{
	return _name;
}


inline const std::string& Notation::getPublic() const
{
	return _public;
}


inline const std::string& Notation::getSystem() const
{
	return _system;
}


} } } // namespace Poco::XSD::Types


#endif // XSDTypes_Notation_INCLUDED
