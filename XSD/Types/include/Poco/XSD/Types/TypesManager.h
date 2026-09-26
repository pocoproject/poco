//
// TypesManager.h
//
// Library: XSD/Types
// Package: XSDTypes
// Module:  TypesManager
//
// Definition of the TypesManager class.
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties, 
// copied or duplicated in any form, in whole or in part.
//


#ifndef XSDTypes_TypesManager_INCLUDED
#define XSDTypes_TypesManager_INCLUDED


#include "Poco/XSD/Types/XSDTypes.h"
#include "Poco/XSD/Types/Schema.h"
#include "Poco/XSD/Types/Definitions.h"
#include "Poco/URI.h"
#include "Poco/Mutex.h"
#include "Poco/SingletonHolder.h"
#include <map>


namespace Poco {
namespace XSD {
namespace Types {


class XSDTypes_API TypesManager
	/// TypesManager manages all known schemas with types and exported elements.
{
public:
	static const std::string XSD_NAMESPACE;
	static const std::string XSD_NAMESPACE1998;
	static const std::string XSD_TYPE_ANYTYPE;
	static const std::string XSD_TYPE_ANYSIMPLETYPE;
	static const std::string XSD_TYPE_STRING;
	static const std::string XSD_TYPE_BOOLEAN;
	static const std::string XSD_TYPE_DECIMAL;
	static const std::string XSD_TYPE_FLOAT;
	static const std::string XSD_TYPE_DOUBLE;
	static const std::string XSD_TYPE_DURATION;
	static const std::string XSD_TYPE_DATETIME;
	static const std::string XSD_TYPE_TIME;
	static const std::string XSD_TYPE_DATE;
	static const std::string XSD_TYPE_GYEARMONTH;
	static const std::string XSD_TYPE_GYEAR;
	static const std::string XSD_TYPE_GMONTHDAY;
	static const std::string XSD_TYPE_GDAY;
	static const std::string XSD_TYPE_GMONTH;
	static const std::string XSD_TYPE_HEX_BINARY;
	static const std::string XSD_TYPE_BASE64BINARY;
	static const std::string XSD_TYPE_ANYURI;
	static const std::string XSD_TYPE_QNAME;
	static const std::string XSD_TYPE_NOTATION;
	static const std::string XSD_TYPE_NORMALIZEDSTRING;
	static const std::string XSD_TYPE_INTEGER;
	static const std::string XSD_TYPE_TOKEN;
	static const std::string XSD_TYPE_NONPOSITIVEINTEGER;
	static const std::string XSD_TYPE_LONG;
	static const std::string XSD_TYPE_NONNEGATIVEINTEGER;
	static const std::string XSD_TYPE_LANGUAGE;
	static const std::string XSD_TYPE_NAME;
	static const std::string XSD_TYPE_NMTOKEN;
	static const std::string XSD_TYPE_NEGATIVEINTEGER;
	static const std::string XSD_TYPE_INT;
	static const std::string XSD_TYPE_UNSIGNEDLONG;
	static const std::string XSD_TYPE_POSITIVEINTEGER;
	static const std::string XSD_TYPE_NCNAME;
	static const std::string XSD_TYPE_NMTOKENS;
	static const std::string XSD_TYPE_SHORT;
	static const std::string XSD_TYPE_UNSIGNEDINT;
	static const std::string XSD_TYPE_ID;
	static const std::string XSD_TYPE_IDREF;
	static const std::string XSD_TYPE_ENTITY;
	static const std::string XSD_TYPE_BYTE;
	static const std::string XSD_TYPE_UNSIGNEDSHORT;
	static const std::string XSD_TYPE_IDREFS;
	static const std::string XSD_TYPE_ENTITIES;
	static const std::string XSD_TYPE_UNSIGNEDBYTE;

	using Schemas = std::map<std::string, Schema::Ptr>;
		/// Maps a namespace to a XML Schema file

	using Definitionss = std::map<std::string, Definitions::Ptr>;
		/// Maps a namespace to a WSDL file

	static TypesManager& instance();
		/// Returns the TypesManager singleton.
		
	void fixupSchemas();
		/// Fixes up all loaded schemas.
		/// Must be called after all schemas have been loaded to
		/// resolve dependencies.

	bool hasSchema(const std::string& ns) const;
		/// Checks if a schema with the given namespace exists.

	bool hasDefinitions(const std::string& ns) const;
		/// Checks if a WSDL with the given namespace exists.

	bool hasSchemaLocation(const Poco::URI& uri) const;
		/// Returns true if from the given URI a schema was loaded.

	void addSchema(Schema::Ptr pSchema, const Poco::URI& schemaLocation);
		/// Will fail if a schema with the same namespace exists and conflicts with the new one.
		/// Ignores schemas that reference the builtin schemas.
		
	void addDefinitions(Definitions::Ptr pDefinitions);
		/// Adds a WSDL document.

	Schema::Ptr findSchema(const std::string& ns);
		/// Returns the Schema for the given target namespace
		/// or a null pointer if no schema exists for the given target namespace.

	Schema::Ptr findSchema(const Poco::URI& schemaLocation);
		/// Returns the Schema for the given target namespace
		/// or a null pointer if no schema exists for the given target namespace.

	Schema& getSchema(const std::string& ns);
		/// Returns the Schema for the given target namespace.
		/// Throws a Poco::NotFoundException if no schema has been defined for the
		/// given target namespace.
	
	Definitions::Ptr findDefinitions(const std::string& ns);
		/// Returns the WSDL Definitions for the given target namespace
		/// or a null pointer if no definitions exist for the given target namespace.

	Definitions& getDefinitions(const std::string& ns);
		/// Returns the WSDL Definitions for the given target namespace.
		/// Throws a Poco::NotFoundException if no WSDL has been defined for the
		/// given target namespace.
		
	const Definitionss& getDefinitions() const;
		/// Returns all known WSDL definitions.

	const Schemas& getSchemas() const;
		/// Returns all known schemas.

	bool eraseSchema(const std::string& ns);
		/// Deletes the schema, Returns true if the schema was found and erased.

	const Type* getType(const QName& ref) const;
		/// Returns a type for the given QName or null if not found.

	const Element* getElement(const QName& ref) const;
		/// Returns an element for the given QName or null if not found.

	const AbstractAttribute* getAttribute(const QName& ref) const;
		/// Returns an attribute for the given QName or null if not found.

	const AbstractAttributeGroup* getAttributeGroup(const QName& ref) const;
		/// Returns an attribute group for the given QName or null if not found.

	const Group* getGroup(const QName& ref) const;
		/// Returns an attribute group for the given QName or null if not found.

private:
	void setSchemaInternal(Schema::Ptr pSchema, const Poco::URI& schemaLocation);
		/// Will fail if a schema with the same namespace exists and conflicts with the new one.
		/// Set fixupSchema to false if are inside an include from another schema file.

	TypesManager();
		/// Creates the TypesManager.

	TypesManager(const TypesManager&);
	TypesManager& operator=(const TypesManager&);

	~TypesManager();
		/// Destroys the TypesManager.

private:
	Schemas               _schemas;
	Schemas               _schemaLocations;
	Definitionss          _definitions;
	mutable Poco::Mutex   _mutex;

	friend class Poco::SingletonHolder<TypesManager>;
};


//
// inlines
//
inline bool TypesManager::hasSchemaLocation(const Poco::URI& uri) const
{
	return (_schemaLocations.find(uri.toString()) != _schemaLocations.end());
}


inline const TypesManager::Schemas& TypesManager::getSchemas() const
{
	return _schemas;
}


inline const TypesManager::Definitionss& TypesManager::getDefinitions() const
{
	return _definitions;
}


} } } // namespace Poco::XSD::Types


#endif // XSDTypes_TypesManager_INCLUDED
