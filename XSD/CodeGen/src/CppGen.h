//
// CppGen.h
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef CodeGen_CppGen_H_INCLUDED
#define CodeGen_CppGen_H_INCLUDED


#include "Poco/XSD/Types/Visitor.h"
#include "Poco/XSD/Types/Type.h"
#include "Poco/XSD/Types/AbstractAttribute.h"
#include "Poco/XSD/Types/AbstractAttributeGroup.h"
#include "Poco/XSD/Types/BindingProperties.h"
#include "Poco/XSD/Types/Binding.h"
#include "Poco/XML/Name.h"
#include "Poco/SharedPtr.h"
#include "Poco/Logger.h"
#include "SchemaInfo.h"
#include "ClassInfo.h"
#include <map>
#include <stack>


class CppGen: public Poco::XSD::Types::Visitor
	/// Cpp code generator.
	/// You must call first prepare(schema) then visit(schema)
{
public:
	enum Options
	{
		OPT_GENERATE_ALL_BINDINGS = 1,
		OPT_IGNORE_PARAMETER_ORDER = 2
	};
	
	using Classes = std::map<std::string, ClassInfo>;
	using Schemas = std::map<std::string, Classes>;
	using TypeNameMap = std::map<const Poco::XSD::Types::Type*, std::string>;

	CppGen(const std::map<std::string, SchemaInfo>& config, const std::string& dllExportMacro, int options = 0);

	~CppGen();

	void visit(const Poco::XSD::Types::Annotation& ann);
		/// Visits an object of type Annotation

	void visit(const Poco::XSD::Types::ElementImpl& ann);
		/// Visits an object of type ElementImpl

	void visit(const Poco::XSD::Types::ElementRef& ann);
		/// Visits an object of type ElementRef

	void visit(const Poco::XSD::Types::ElementTypeRef& ann);
		/// Visits an object of type ElementTypeRef

	void prepare(const Poco::XSD::Types::Schema& ann);
		/// Prepares to visit an object of type Schema

	void visit(const Poco::XSD::Types::Schema& ann);
		/// Visits an object of type Schema

	void visit(const Poco::XSD::Types::Sequence& ann);
		/// Visits an object of type Sequence

	void visit(const Poco::XSD::Types::Documentation& doc);
		/// Visits an object of type Documentation

	void visit(const Poco::XSD::Types::AppInfo& doc);
		/// Visits an object of type AppInfo

	void visit(const Poco::XSD::Types::SimpleType& val);
		/// Visits an object of type SimpleType

	void visit(const Poco::XSD::Types::ComplexType& val);
		/// Visits an object of type ComplexType

	void visit(const Poco::XSD::Types::Attribute& val);
		/// Visits an object of type Attribute

	void visit(const Poco::XSD::Types::AttributeRef& val);
		/// Visits an object of type AttributeRef

	void visit(const Poco::XSD::Types::AttributeTypeRef& val);
		/// Visits an object of type AttributeTypeRef

	void visit(const Poco::XSD::Types::AttributeGroup& val);
		/// Visits an object of type AttributeGroup

	void visit(const Poco::XSD::Types::AttributeGroupRef& val);
		/// Visits an object of type AttributeGroupRef

	void visit(const Poco::XSD::Types::Group& val);
		/// Visits an object of type Group

	void visit(const Poco::XSD::Types::GroupRef& val);
		/// Visits an object of type GroupRef

	void visit(const Poco::XSD::Types::All& val);
		/// Visits an object of type All

	void visit(const Poco::XSD::Types::Any& val);
		/// Visits an object of type Any

	void visit(const Poco::XSD::Types::AnyAttribute& val);
		/// Visits an object of type AnyAttribute

	void visit(const Poco::XSD::Types::Choice& val);
		/// Visits an object of type Choice

	void visit(const Poco::XSD::Types::Notation& val);
		/// Visits an object of type Notation

	void visit(const Poco::XSD::Types::Union& val);
		/// Visits an object of type Union

	void visit(const Poco::XSD::Types::InheritanceInfo& val);
		/// Visits an object of type InheritanceInfo

	void visit(const Poco::XSD::Types::List& val);
		/// Visits an object of type List

	void visit(const Poco::XSD::Types::ListTypeRef& val);
		/// Visits an object of type ListTypeRef

	void visit(const Poco::XSD::Types::SimpleRestriction& val);
		/// Visits an object of type SimpleRestriction

	void visit(const Poco::XSD::Types::SimpleRestrictionInlineType& val);
		/// Visits an object of type SimpleRestrictionInlineType

	void visit(const Poco::XSD::Types::Definitions& val);
		/// Visits an object of type Message.
	
	void visit(const Poco::XSD::Types::Message& val);
		/// Visits an object of type Message.
		
	void visit(const Poco::XSD::Types::Operation& val);
		/// Visits an object of type Operation.
		
	void visit(const Poco::XSD::Types::PortType& val);
		/// Visits an object of type PortType.

	void visit(const Poco::XSD::Types::Binding& val);
		/// Visits an object of type Binding.
		
	void visit(const Poco::XSD::Types::Service& val);
		/// Visits an object of type Service.

	const CppGen::Schemas& getSchemas() const;

	const SchemaInfo& config(const std::string& ns) const;

	const Classes& schema(const std::string& ns) const;

	const ClassInfo& classInfo(const std::string& ns, const std::string& name) const;

	TypeInfo createTypeInfo(const Poco::XSD::Types::Type* pType);

	void postProcess();
		/// Postprocesses all the internal classes

private:
	std::string createClassName(const std::string& xsdClassName);
		/// Creates a class name. If xsdClassName is empty, an innerclassName
		/// will be generated, set autoIncrement to false if you are not in visit(complexType)

	std::string genInnerClassName();
		/// generates an inner class name for an otherwise unnamed inner type

	Classes& schema(const std::string& ns);

	ClassInfo& classInfo(const std::string& ns, const std::string& name);

	void generateAbstractAttribute(const Poco::XSD::Types::AbstractAttribute& val);

	void generateAttributeGroup(const Poco::XSD::Types::AbstractAttributeGroup& val);

	void prepare(const Poco::XSD::Types::ComplexType& val);

	void prepare(const Poco::XSD::Types::SimpleType& val);

	bool extendsFromSimpleType(const Poco::XSD::Types::Type& val, 
		bool& parentIsListType, 
		bool& noParent, 
		Poco::SharedPtr<TypeInfo>& pParent);

	void assertClassInfoExists(const Poco::XSD::Types::ComplexType* val);

	void assertClassInfoExists(const Poco::XSD::Types::SimpleType* val);

	std::string mapCppToXSD(const std::string& cppNS) const;

	void addVarToClass(ClassInfo& ci, 
		const std::string& cppVarName, 
		const std::string& xsdName, 
		const std::string& xsdNameSpace,
		const TypeInfo& cppType, 
		bool isOptional,
		bool isNillable,
		const std::string& typeAttr,
		bool isFixedValue, 
		bool hasDefault, 
		const std::string& defaultValue,
		bool setInlineAttr = false);

	void addFullConstructor(ClassInfo& ci);
	/// Adds a constructor that takes all member variables (except fixed ones)

	void buildHierarchy(std::vector<ClassInfo*>& hierarchy, ClassInfo& ci);
		/// builds up a hierarchy, root class is at the beginning

	void extractVariables(std::vector<const Variable*>& vars, const std::vector<ClassInfo*>& classes);

	bool isWrapped(const std::string& operationName, const Poco::XML::Name& messageName);
	std::string createWrappedParameters(MethodInfo& mi, const Poco::XML::Name& messageName, Parameter::Direction direction);
	void createParameters(MethodInfo& mi, const Poco::XML::Name& messageName, Parameter::Direction direction, std::vector<std::string>& parameterOrder, bool detectReturn = false);
	void createHeaderParameters(MethodInfo& mi, const Poco::XSD::Types::BindingProperties& bindingProps, const std::string& soapVersion, Parameter::Direction direction);
	void createHeaderParameters(MethodInfo& mi, const Poco::XML::Name& messageName, const std::string& partName, Parameter::Direction direction);
	void makeUniqueParameterName(const MethodInfo& mi, std::string& name);
	void generateBinding(const Poco::XSD::Types::Binding& binding, const std::string& name);
	void generateDocumentBinding(const Poco::XSD::Types::Binding& binding, const std::string& name);
	void generateRpcBinding(const Poco::XSD::Types::Binding& binding, const std::string& name);

private:
	enum COLLTYPE
	{
		CT_SEQ,
		CT_CHOICE,
		CT_ALL
	};

	std::map<std::string, SchemaInfo> _config;
	std::string _dllExportMacro;
	int _options;
	std::stack<ClassInfo> _classes;
	std::stack<std::string> _elements;
	Schemas _schemas;
	const SchemaInfo* _pLastSchema;
	std::string _lastSchemaNamespace;
	std::stack<COLLTYPE> _inCollection;
	std::stack<std::string> _choiceName; /// Contains the name of the choice -> generate choicemembername from that
	std::stack<int>   _choiceMaxOccurs;
	Poco::Logger& _logger;
	bool _inChoice;
	TypeNameMap _typeNameMap;
	TypeNameMap _typeNamespaceMap;
};


//
// inlines
//
inline const CppGen::Schemas& CppGen::getSchemas() const
{
	return _schemas;
}


#endif // CodeGen_CppGen_H_INCLUDED
