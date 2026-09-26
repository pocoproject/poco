//
// BindingProperties.h
//
// Library: XSD/Types
// Package: WSDL
// Module:  BindingProperties
//
// Definition of the BindingProperties class.
//
// Copyright (c) 2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef XSDTypes_BindingProperties_INCLUDED
#define XSDTypes_BindingProperties_INCLUDED


#include "Poco/XSD/Types/XSDTypes.h"
#include "Poco/Exception.h"
#include <map>


namespace Poco {
namespace XSD {
namespace Types {


class XSDTypes_API BindingProperties
	/// This class stores binding information for Operation and PortType objects.
	///
	/// Note: Property names should be multi-level, e.g., "soap.body.use" for the
	/// "use" attribute in the <soap:body> element.
{
public:
	using Map = std::map<std::string, std::string>;

	BindingProperties();
		/// Creates the BindingProperties.

	~BindingProperties();
		/// Destroys the BindingProperties.

	void set(const std::string& name, const std::string& value);
		/// Sets a binding property.
		
	bool has(const std::string& name) const;
		/// Returns true if a property with the given name has been defined.
		
	const std::string& get(const std::string& name) const;
		/// Returns the value of the property with the given name.
		/// Throws a Poco::NotFoundException if the property has not been set.
		
	const std::string& get(const std::string& name, const std::string& deflt) const;
		/// Returns the valuie of the property with the given name, or
		/// the given default value if the property has not been set.

private:
	Map _map;
};


//
// inlines
//
inline void BindingProperties::set(const std::string& name, const std::string& value)
{
	_map[name] = value;
}


inline bool BindingProperties::has(const std::string& name) const
{
	return _map.find(name) != _map.end();
}


inline const std::string& BindingProperties::get(const std::string& name) const
{
	Map::const_iterator it = _map.find(name);
	if (it != _map.end())
		return it->second;
	else
		throw Poco::NotFoundException(name);
}


inline const std::string& BindingProperties::get(const std::string& name, const std::string& deflt) const
{
	Map::const_iterator it = _map.find(name);
	if (it != _map.end())
		return it->second;
	else
		return deflt;
}


} } } // namespace Poco::XSD::Types


#endif // XSDTypes_BindingProperties_INCLUDED
