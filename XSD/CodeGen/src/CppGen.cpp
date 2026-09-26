//
// CppGen.cpp
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "CppGen.h"
#include "Utility.h"
#include "BuiltinTypes.h"
#include "Poco/Exception.h"
#include "Poco/NumberFormatter.h"
#include "Poco/XSD/Types/All.h"
#include "Poco/XSD/Types/Annotation.h"
#include "Poco/XSD/Types/Any.h"
#include "Poco/XSD/Types/AnyAttribute.h"
#include "Poco/XSD/Types/AppInfo.h"
#include "Poco/XSD/Types/Attribute.h"
#include "Poco/XSD/Types/AttributeGroup.h"
#include "Poco/XSD/Types/AttributeGroupRef.h"
#include "Poco/XSD/Types/AttributeRef.h"
#include "Poco/XSD/Types/AttributeTypeRef.h"
#include "Poco/XSD/Types/Choice.h"
#include "Poco/XSD/Types/ComplexType.h"
#include "Poco/XSD/Types/Documentation.h"
#include "Poco/XSD/Types/ElementImpl.h"
#include "Poco/XSD/Types/ElementRef.h"
#include "Poco/XSD/Types/ElementTypeRef.h"
#include "Poco/XSD/Types/Group.h"
#include "Poco/XSD/Types/GroupRef.h"
#include "Poco/XSD/Types/InheritanceInfo.h"
#include "Poco/XSD/Types/List.h"
#include "Poco/XSD/Types/ListTypeRef.h"
#include "Poco/XSD/Types/Notation.h"
#include "Poco/XSD/Types/Schema.h"
#include "Poco/XSD/Types/Sequence.h"
#include "Poco/XSD/Types/SimpleType.h"
#include "Poco/XSD/Types/SimpleRestriction.h"
#include "Poco/XSD/Types/SimpleRestrictionInlineType.h"
#include "Poco/XSD/Types/Union.h"
#include "Poco/XSD/Types/TypesManager.h"
#include "Poco/XSD/Parser/Constants.h"


using namespace Poco::XSD::Types;


CppGen::CppGen(const std::map<std::string, SchemaInfo>& config, const std::string& dllExportMacro, int options):
	_config(config),
	_dllExportMacro(dllExportMacro),
	_options(options),
	_classes(),
	_schemas(),
	_pLastSchema(0),
	_logger(Poco::Logger::get("CppGen")),
	_inChoice(false)
{
}


CppGen::~CppGen()
{
}


void CppGen::visit(const Poco::XSD::Types::Annotation& ann)
{
}


void CppGen::visit(const Poco::XSD::Types::ElementImpl& val)
{
	_logger.debug("visiting element " + val.name());

	// visit the inner type first
	_elements.push(val.name());
	val.type().accept(*this);
	_elements.pop();

	poco_assert (!_classes.empty());
	std::string cppVarName = Utility::xsdNameToVarName(val.name());
	std::string tns;
	if (val.type().getSchema())
		tns = val.type().getSchema()->targetNamespace();
	else
		tns = _lastSchemaNamespace;
	if (tns == Poco::XSD::Parser::Constants::XSD_NAMESPACE_URI)
	{
		_elements.push(val.name());
		TypeInfo ti = createTypeInfo(&val.type());
		_elements.pop();
		ti.setVector((val.getMaxOccurs() > 1));
		ti.setNullable(_inChoice || (val.getMinOccurs() == 0 && val.getMaxOccurs() == 1));
		const std::string& defValue = (val.hasFixed()? val.getFixed(): val.getDefault());
		addVarToClass(_classes.top(), cppVarName, val.name(), tns, ti, (_inChoice || val.getMinOccurs() == 0), val.getNillable(), "elem", val.hasFixed(), val.hasDefault(), defValue);
	}
	else
	{
		_elements.push(val.name());
		std::string typeClassName = createClassName(val.type().name());
		_elements.pop();
		const ClassInfo& cls = classInfo(tns, typeClassName);
		TypeInfo ti = cls.getTypeInfo();
		ti.setVector((val.getMaxOccurs() > 1));
		ti.setNullable(_inChoice || (val.getMinOccurs() == 0 && val.getMaxOccurs() == 1));
		const std::string& defValue = (val.hasFixed()? val.getFixed(): val.getDefault());
		addVarToClass(_classes.top(), cppVarName, val.name(), tns, ti, (_inChoice || val.getMinOccurs() == 0), val.getNillable(), "elem", val.hasFixed(), val.hasDefault(), defValue);
	}
}


void CppGen::visit(const Poco::XSD::Types::ElementRef& val)
{
	_logger.debug("visiting element ref " + val.name());

	poco_assert (!_classes.empty());

	if (!val.type().getSchema())
	{
		_elements.push(val.name());
		val.type().accept(*this);
		_elements.pop();
	}

	std::string cppVarName = Utility::xsdNameToVarName(val.name());
	_elements.push(val.name());
 	TypeInfo ti = createTypeInfo(&val.type());
	_elements.pop();
	ti.setVector((val.getMaxOccurs() > 1));
	ti.setNullable(_inChoice || (val.getMinOccurs() == 0 && val.getMaxOccurs() == 1));
	const std::string& defValue = (val.hasFixed()? val.getFixed(): val.getDefault());
	addVarToClass(_classes.top(), cppVarName, val.name(), val.nameSpace(), ti, (_inChoice || val.getMinOccurs() == 0), val.getNillable(), "elem", val.hasFixed(), val.hasDefault(), defValue);
}


void CppGen::visit(const Poco::XSD::Types::ElementTypeRef& val)
{
	_logger.debug("visiting element type ref " + val.name());

	poco_assert (!_classes.empty());

	std::string cppVarName = Utility::xsdNameToVarName(val.name());
	_elements.push(val.name());
	TypeInfo ti = createTypeInfo(&val.type());
	_elements.pop();
	ti.setVector((val.getMaxOccurs() > 1));
	ti.setNullable(_inChoice || (val.getMinOccurs() == 0 && val.getMaxOccurs() == 1));
	const std::string& defValue = (val.hasFixed()? val.getFixed(): val.getDefault());
	addVarToClass(_classes.top(), cppVarName, val.name(), val.nameSpace(), ti, (_inChoice || val.getMinOccurs() == 0), val.getNillable(), "elem", val.hasFixed(), val.hasDefault(), defValue);
}


void CppGen::prepare(const Poco::XSD::Types::Schema& s)
{
	if (_schemas.find(s.targetNamespace()) != _schemas.end())
		return;

	_logger.debug("preparing schema " + s.targetNamespace());

	_schemas.insert(std::make_pair(s.targetNamespace(), Classes()));
	const Schema::Types& allTypes = s.types();
	Schema::Types::const_iterator it = allTypes.begin();
	for (; it != allTypes.end(); ++it)
	{
		const ComplexType* pType = dynamic_cast<const ComplexType*>(it->second.get());
		if (pType)
		{
			prepare(*pType);
		}
		else
		{
			const SimpleType* pSimple = dynamic_cast<const SimpleType*>(it->second.get());
			poco_check_ptr (pSimple);
			prepare(*pSimple);
		}
	}
}


void CppGen::visit(const Poco::XSD::Types::Schema& s)
{
	_logger.debug("visiting schema " + s.targetNamespace());

	_schemas.insert(std::make_pair(s.targetNamespace(), Classes()));
	_pLastSchema = &config(s.targetNamespace());
	_lastSchemaNamespace = s.targetNamespace();
	const Schema::Types& allTypes = s.types();
	Schema::Types::const_iterator it = allTypes.begin();
	for (; it != allTypes.end(); ++it)
	{
		it->second->accept(*this);
	}
}


void CppGen::visit(const Poco::XSD::Types::Sequence& val)
{
	const Sequence::Content& seq = val.getContent();
	Sequence::Content::const_iterator it = seq.begin();
	for (; it != seq.end(); ++it)
	{
		_inChoice = false;
		(*it)->accept(*this);
	}
}


void CppGen::visit(const Poco::XSD::Types::Documentation& doc)
{
}


void CppGen::visit(const Poco::XSD::Types::AppInfo& doc)
{
}


void CppGen::visit(const Poco::XSD::Types::SimpleType& val)
{
	_logger.debug("visiting simple type " + val.name());

	std::string tns;
	if (val.getSchema())
		tns = val.getSchema()->targetNamespace();
	else
		tns = _lastSchemaNamespace;

	if (tns == Poco::XSD::Parser::Constants::XSD_NAMESPACE_URI)
		return;

	std::string className = createClassName(val.name());

	_typeNameMap[&val] = className;
	_typeNamespaceMap[&val] = tns;

	// a simple type must have a parent -> always go to the root! until builtin namespace
	poco_assert (!val.parents().empty());
	const Type* pParent = val.parents()[0];
	poco_check_ptr (pParent);

	while (!pParent->getSchema() || (pParent->getSchema() && !Utility::isBuiltinNamespace(pParent->getSchema()->targetNamespace())))
	{
		poco_assert (!pParent->parents().empty());
		pParent = pParent->parents()[0];
		poco_check_ptr (pParent);
	}
	TypeInfo ti = createTypeInfo(pParent);
	ClassInfo ci(ti.name(), ti.getNameSpace(), ti.getSchemaNameSpace(), ti.getIncludeFile(), "");
	schema(tns).insert(std::make_pair(className, ci));
}


void CppGen::prepare(const Poco::XSD::Types::SimpleType& val)
{
	poco_assert_dbg (!val.name().empty());
	bool parentIsListType = false;
	bool noParent = true;
	Poco::SharedPtr<TypeInfo> pParent;
	extendsFromSimpleType(val, parentIsListType, noParent, pParent);
}


bool CppGen::extendsFromSimpleType(const Poco::XSD::Types::Type& val, bool& parentIsListType, bool& noParent, Poco::SharedPtr<TypeInfo>& pParentInfo)
{
	if (val.parents().empty())
	{
		noParent = true;
		parentIsListType = false;
		return false;
	}

	const SimpleType* pSimple = dynamic_cast<const SimpleType*>(&val);

	bool isList = false;
	poco_assert_dbg (!val.parents().empty());
	const SimpleType* pParent = dynamic_cast<const SimpleType*>(val.parents()[0]);
	if (!pParent)
	{
		assertClassInfoExists(static_cast<const ComplexType*>(val.parents()[0]));
		// a complex parent class
		parentIsListType = false;
		noParent = false;
		pParentInfo = new TypeInfo(createTypeInfo(val.parents()[0]));
		return false;
	}

	if (pParent->getContent())
		isList |= pParent->getContent()->isList();
	while (!pParent->getSchema() || (pParent->getSchema() && !Utility::isBuiltinNamespace(pParent->getSchema()->targetNamespace())))
	{
		if (pParent->getSchema())
			assertClassInfoExists(pParent);
		poco_assert (!pParent->parents().empty());
		pParent = static_cast<const SimpleType*>(pParent->parents()[0]);
		poco_check_ptr (pParent);
		if (pParent->getContent())
			isList |= pParent->getContent()->isList();
	}

	assertClassInfoExists(pParent);
	noParent = false;
	parentIsListType = isList;
	TypeInfo ti = createTypeInfo(pParent);
	std::string name(ti.name());
	std::string nameSpace;
	if (val.getSchema())
		nameSpace = val.getSchema()->targetNamespace();
	else
		nameSpace = ti.getSchemaNameSpace();
	if (isList)
	{
		name = "vector<";
		if (!ti.getNameSpace().empty())
			name+= ti.getNameSpace()+"::";
		name += ti.name()+">";
		std::string incFiles(ti.getIncludeFile());
		pParentInfo = new TypeInfo(name, "std", nameSpace, "", incFiles, false, true, false);
		ClassInfo ci(name, "std", nameSpace, incFiles, "");
		ci.insert("inline", "");
		if (!val.parents()[0]->name().empty())
			schema(nameSpace).insert(std::make_pair(Utility::xsdNameToClassName(val.parents()[0]->name()), ci));
		if (pSimple)
		{
			// a simple type parent
			if (!Utility::isBuiltinNamespace(nameSpace))
				schema(nameSpace).insert(std::make_pair(createClassName(val.name()), ClassInfo(name, pParentInfo->getNameSpace(), nameSpace, pParentInfo->getIncludeFile(), _dllExportMacro)));
		}
	}
	else
	{
		if (pSimple)
		{
			// a simple type parent: check if the subclass itself is a list type
			isList |= pSimple->getContent()->isList();
			if (isList)
			{
				name = "vector<";
				if (!ti.getNameSpace().empty())
					name+= ti.getNameSpace()+"::";
				name += ti.name()+">";
				std::string incFiles(ti.getIncludeFile());
				pParentInfo = new TypeInfo(name, "std", nameSpace, "", incFiles, false, true, false);
			}
			else
			{
				pParentInfo = new TypeInfo(ti);
			}
			if (!Utility::isBuiltinNamespace(nameSpace))
			{
				ClassInfo ci(name, pParentInfo->getNameSpace(), nameSpace, pParentInfo->getIncludeFile(), "");
				ci.insert("inline", "");
				schema(nameSpace).insert(std::make_pair(createClassName(val.name()), ci));
			}
		}
		else
			pParentInfo = new TypeInfo(ti);
	}

	poco_check_ptr (pParent);
	return true;
}


void CppGen::prepare(const Poco::XSD::Types::ComplexType& val)
{
	poco_assert_dbg (!val.name().empty());
	const SchemaInfo& si = config(val.getSchema()->targetNamespace());
	std::string className = createClassName(val.name());

	_typeNameMap[&val] = className;
	_typeNamespaceMap[&val] = val.getSchema()->targetNamespace();

	ClassInfo ci(className, si.nameSpace(),  val.getSchema()->targetNamespace(), si.createInclude(className), si.dllPrefix());
	ci.addConstructor(Constructor(ci, Utility::AC_PUBLIC));
	ci.insert("serialize", "");
	ci.insert("namespace", val.getSchema()->targetNamespace());
	ci.insert("name", val.name());
	bool parentIsListType = false;
	bool noParent = true;
	Poco::SharedPtr<TypeInfo> pParent;
	bool isSimple= extendsFromSimpleType(val, parentIsListType, noParent, pParent);
	if (!noParent && (parentIsListType || !isSimple))
		ci.setParent(*pParent);

	schema(val.getSchema()->targetNamespace()).insert(std::make_pair(className, ci));
}


void CppGen::visit(const Poco::XSD::Types::ComplexType& val)
{
	_logger.debug("visiting complex type " + val.name());

	std::string className = createClassName(val.name());

	{
		std::string nameSpace;
		if (val.getSchema())
		{
			nameSpace = val.getSchema()->targetNamespace();
			ClassInfo ci = classInfo(nameSpace, className);
			poco_assert (ci.name() == className);
			_classes.push(ci);
		}
		else
		{
			poco_check_ptr (_pLastSchema);
			nameSpace = _lastSchemaNamespace;
			ClassInfo ci(className, _pLastSchema->nameSpace(), nameSpace, _pLastSchema->createInclude(className), _pLastSchema->dllPrefix());
			ci.addConstructor(Constructor(ci, Utility::AC_PUBLIC));
 			ci.insert("serialize", "");
			_classes.push(ci);
		}

		AttributeTypeRef::Ptr pAttr;
		bool parentIsListType = false;
		bool noParent = true;
		Poco::SharedPtr<TypeInfo> pParent;
		bool isSimple = extendsFromSimpleType(val, parentIsListType, noParent, pParent);
		if (!parentIsListType && !noParent && isSimple)
		{
			// special case: a complex type that extends from a simple type
			// e.g. add an attr to a string: handle this as an element with
			// the inline attr set for the element plus the attribute
			if (_classes.top().getVariables().empty()) // we may be visited more than once - only add variables at first visit
			{
				addVarToClass(_classes.top(), "_value", "value", nameSpace, *pParent, true, false, "elem", false, false, "", true);
			}
		}
	}

	if (!val.getParent() || !val.getParent()->getRestriction())
	{
		if (_classes.top().getVariables().empty()) // we may be visited more than once - only add variables at first visit
		{
			// now visit content: elements+attributes etc
			const std::vector<AttributeContent::Ptr>& attrs = val.attributeContent();
			std::vector<AttributeContent::Ptr>::const_iterator it = attrs.begin();
			for (; it != attrs.end(); ++it)
			{
				(*it)->accept(*this);
			}

			if (val.getContent())
				val.getContent()->accept(*this);
		}
	}

	poco_assert_dbg (_classes.top().name() == className);
	if (val.getSchema())
	{
		Classes& theSchema = schema(val.getSchema()->targetNamespace());
		Classes::iterator itClass = theSchema.find(className);
		poco_assert_dbg (itClass != theSchema.end());
		itClass->second = _classes.top();
	}
	else
	{
		//inner type
		Classes& theSchema = schema(_lastSchemaNamespace);
		std::pair<Classes::iterator, bool> aPair = theSchema.insert(std::make_pair(className, _classes.top()));
		if (!aPair.second)
			aPair.first->second = _classes.top();
	}
	_classes.pop();
}


void CppGen::visit(const Poco::XSD::Types::Attribute& val)
{
	_logger.debug("visiting attribute " + val.name());

	// an attribute defines an inner simple type that extends from a primitive type
	// simplified to always use the parent type
	// FIXME: add a typedef?
	poco_assert (!_classes.empty());
	// an attribute is handled as a member var of the complexType
	std::string cppVarName = Utility::xsdNameToVarName(val.name());
	bool parentIsListType = false;
	bool noParent = true;
	Poco::SharedPtr<TypeInfo> pParent;
	const std::string& defValue = (val.hasFixed() ? val.fixedValue() : val.defaultValue());
	if (val.type())
	{
		_elements.push(val.name());
		extendsFromSimpleType(*val.type(), parentIsListType, noParent, pParent);
		_elements.pop();
		poco_check_ptr(pParent);
		addVarToClass(_classes.top(), cppVarName, val.name(), val.nameSpace(), *pParent, (val.usage() == AbstractAttribute::USE_OPTIONAL), false, "attr", val.hasFixed(), val.hasDefault(), defValue);
	}
	else
	{
		_logger.warning("Attribute \"%s\" in namespace \"%s\" has no type, which is non-standard behavior. Assuming type string.", val.name(), val.nameSpace());
		TypeInfo ti = BuiltinTypes::instance().get("string");
		addVarToClass(_classes.top(), cppVarName, val.name(), val.nameSpace(), ti, (val.usage() == AbstractAttribute::USE_OPTIONAL), false, "attr", val.hasFixed(), val.hasDefault(), defValue);
	}
}


void CppGen::generateAbstractAttribute(const Poco::XSD::Types::AbstractAttribute& val)
{
	poco_assert (!_classes.empty());
	// an attribute is handled as a member var of the complexType
	std::string cppVarName = Utility::xsdNameToVarName(val.name());
	bool parentIsListType = false;
	bool noParent = true;
	Poco::SharedPtr<TypeInfo> pParent;
	bool isSimple = extendsFromSimpleType(*val.type(), parentIsListType, noParent, pParent);
	if (!noParent)
	{
		poco_check_ptr (pParent);
	}
	if (noParent || !isSimple)
	{
		pParent = new TypeInfo(createTypeInfo(val.type()));
	}
	const std::string& defValue = (val.hasFixed()? val.fixedValue(): val.defaultValue());
	addVarToClass(_classes.top(), cppVarName, val.name(), val.nameSpace(), *pParent, (val.usage() == AbstractAttribute::USE_OPTIONAL), false, "attr", val.hasFixed(), val.hasDefault(), defValue);
}


void CppGen::addVarToClass(ClassInfo& ci, const std::string& cppVarName, const std::string& xsdName, const std::string& xsdNameSpace, const TypeInfo& cppType, bool isOptional, bool isNillable, const std::string& typeAttr, bool isFixedValue, bool hasDefault, const std::string& defaultValue, bool setInlineAttr)
{
	Variable::Modifiers mod = Variable::V_ISVALUE;
	bool recursiveDataStructure = (ci.name() == cppType.name() && ci.getNameSpace() == cppType.getNameSpace());
	if (cppType.isVector())
		mod = Variable::V_ISVECTOR;
	if (recursiveDataStructure)
	{
		mod = (Variable::Modifiers)((int)mod|Variable::V_ISPOINTER);
	}
	Variable var(cppVarName, cppType,
		Utility::AC_PRIVATE, (int)ci.getVariables().size(),
		mod, isOptional, isNillable);
	var.insert("order", Poco::NumberFormatter::format((int)ci.getVariables().size()));
	var.insert("name", xsdName);
	if (typeAttr != "elem") var.insert("type", typeAttr);
	if (isOptional) var.insert("mandatory", "false");
	if (ci.getSchemaNameSpace() != xsdNameSpace) var.insert("namespace", xsdNameSpace);
	if (setInlineAttr)
		var.insert("inline", "true");

	ci.addVariable(var);

	Variable::Modifiers modMethod = cppType.isScalar() ? Variable::V_ISVALUE : Variable::V_ISCONSTREF;
	if (cppType.isVector())
		modMethod = Variable::V_ISCONSTREFVECTOR;
	if (recursiveDataStructure)
	{
		modMethod = (Variable::Modifiers)((int)modMethod | Variable::V_ISPOINTER);
	}

	// also add getter/setter for the member var
	MethodInfo miGet(Utility::getterMethodName(cppVarName), Utility::AC_PUBLIC, true, false, false, false);
	miGet.setReturnParameter(new Parameter("", var.getType(), 0, modMethod, var.isOptional(), false, var.isNillable()));
	miGet.addCode("return " + var.getName() + ";");

	MethodInfo miSet(Utility::setterMethodName(cppVarName), Utility::AC_PUBLIC, false, false, false, false);
	miSet.addParameter(Parameter("val", var.getType(), 0, modMethod, var.isOptional(), false, var.isNillable()));
	//FIXME: due to how remoting handles ser/deser, we have to provide a setter method
	//even for a fixed value, simply ignore the value
	if (!isFixedValue)
		miSet.addCode(var.getName() + " = val;");

	ci.addMethod(miGet);
	ci.addMethod(miSet);

	if (!isFixedValue && !var.getType().isScalar())
	{
		MethodInfo miSetMove(Utility::setterMethodName(cppVarName), Utility::AC_PUBLIC, false, false, false, false);
		miSetMove.addParameter(Parameter("val", var.getType(), 0, (Variable::Modifiers)(modMethod | Variable::V_ISRVREF), var.isOptional(), false, var.isNillable()));
		miSetMove.addCode(var.getName() + " = std::move(val);");
		ci.addMethod(miSetMove);
	}

	if (cppType.isVector())
	{
		modMethod = Variable::V_ISREFVECTOR;
		if (recursiveDataStructure)
			modMethod = (Variable::Modifiers)((int)modMethod | Variable::V_ISPOINTER);
		MethodInfo miGet(Utility::getterMethodName(cppVarName), Utility::AC_PUBLIC, false, false, false, false);
		miGet.setReturnParameter(new Parameter("", var.getType(), 0, modMethod, var.isOptional(), false, var.isNillable()));
		miGet.addCode("return " + var.getName() + ";");
		ci.addMethod(miGet);
	}

	if (hasDefault || isFixedValue)
	{
		// add initialization code to constructor
		std::vector<Constructor>& constr = ci.getConstructors();
		std::vector<Constructor>::iterator it = constr.begin();
		for (; it != constr.end(); ++it)
		{
			std::string val = BuiltinTypes::instance().generateInitializeValue(ci, *it, var, defaultValue);
			if (recursiveDataStructure)
			{
				it->addInitializationCode(var.getName()+ "(new " + var.getType().getFullName() + "("+val+"))");
			}
			else
				it->addInitializationCode(var.getName()+"("+val+")");
		}
	}
}


void CppGen::visit(const Poco::XSD::Types::AttributeRef& val)
{
	_logger.debug("visiting attribute ref " + val.name());

	generateAbstractAttribute(val);
}


void CppGen::visit(const Poco::XSD::Types::AttributeTypeRef& val)
{
	_logger.debug("visiting attribute type ref " + val.name());

	generateAbstractAttribute(val);
}


void CppGen::generateAttributeGroup(const Poco::XSD::Types::AbstractAttributeGroup& val)
{
	const AbstractAttributeGroup::Attributes& attrs =  val.getAttributes();
	AbstractAttributeGroup::Attributes::const_iterator it = attrs.begin();
	for (; it != attrs.end(); ++it)
	{
		it->second->accept(*this);
	}
}


void CppGen::visit(const Poco::XSD::Types::AttributeGroup& val)
{
	_logger.debug("visiting attribute group " + val.name());

	generateAttributeGroup(val);
}


void CppGen::visit(const Poco::XSD::Types::AttributeGroupRef& val)
{
	_logger.debug("visiting attribute group ref " + val.name());

	generateAttributeGroup(val);
}


void CppGen::visit(const Poco::XSD::Types::Group& val)
{
	if (val.getChild())
		val.getChild()->accept(*this);
}


void CppGen::visit(const Poco::XSD::Types::GroupRef& val)
{
	if (val.getChild())
		val.getChild()->accept(*this);
}


void CppGen::visit(const Poco::XSD::Types::All& val)
{
	const All::Content& seq = val.getContent();
	All::Content::const_iterator it = seq.begin();
	for (; it != seq.end(); ++it)
	{
		_inChoice = false;
		it->second->accept(*this);
	}
}


void CppGen::visit(const Poco::XSD::Types::Any& val)
{
/**
	// the any element, map to Poco::Any
	poco_assert (!_classes.empty());
	std::string cppVarName = "Any";
	TypeInfo ti (cppVarName, "Poco", TypesManager::XSD_NAMESPACE, "Poco/Any.h", false, (val.getMaxOccurs() > 1));

	addVarToClass(_classes.top(), "_any", "any", ti, (val.getMinOccurs() == 0), false, "elem", false, false, "");
**/
}


void CppGen::visit(const Poco::XSD::Types::AnyAttribute& val)
{
/**
	// the any element, map to Poco::Any
	poco_assert (!_classes.empty());
	ClassInfo& theClass = _classes.top();
	std::string cppVarName = "Any";
	TypeInfo ti (cppVarName, "Poco", TypesManager::XSD_NAMESPACE, "Poco/Any.h", false, false);

	addVarToClass(_classes.top(), "_anyAttr", "anyAttr", ti, true, false, "attr", false, false, "");
**/
}


void CppGen::visit(const Poco::XSD::Types::Choice& val)
{
	//FIXME: we treat a choice as a sequence
	const Choice::Content& seq = val.getContent();
	Choice::Content::const_iterator it = seq.begin();
	for (; it != seq.end(); ++it)
	{
		_inChoice = true;
		(*it)->accept(*this);
		_inChoice = false;
	}
}


void CppGen::visit(const Poco::XSD::Types::Notation& val)
{
}


void CppGen::visit(const Poco::XSD::Types::Union& val)
{
	throw Poco::NotImplementedException("union not supported");
}


void CppGen::visit(const Poco::XSD::Types::InheritanceInfo& val)
{
	if (val.type())
		val.type()->accept(*this);
}


void CppGen::visit(const Poco::XSD::Types::List& val)
{
	throw Poco::NotImplementedException("list not reachable");
}


void CppGen::visit(const Poco::XSD::Types::ListTypeRef& val)
{
	throw Poco::NotImplementedException("listtyperef not reachable");
}


void CppGen::visit(const Poco::XSD::Types::SimpleRestriction& val)
{
	throw Poco::NotImplementedException("SimpleRestriction not reachable");
}


void CppGen::visit(const Poco::XSD::Types::SimpleRestrictionInlineType& val)
{
	throw Poco::NotImplementedException("SimpleRestrictionInlineType not reachable");
}


void CppGen::visit(const Poco::XSD::Types::Definitions& val)
{
	_lastSchemaNamespace = val.targetNamespace();

	_schemas.insert(std::make_pair(val.targetNamespace(), Classes()));

	for (Poco::XSD::Types::Definitions::Messages::const_iterator itM = val.messages().begin(); itM != val.messages().end(); ++itM)
	{
		itM->second->accept(*this);
	}

	for (Poco::XSD::Types::Definitions::PortTypes::const_iterator itP = val.portTypes().begin(); itP != val.portTypes().end(); ++itP)
	{
		itP->second->accept(*this);
	}

	for (Poco::XSD::Types::Definitions::Bindings::const_iterator itB = val.bindings().begin(); itB != val.bindings().end(); ++itB)
	{
		itB->second->accept(*this);
	}

	for (Poco::XSD::Types::Definitions::Services::const_iterator itS = val.services().begin(); itS != val.services().end(); ++itS)
	{
		itS->second->accept(*this);
	}
}


void CppGen::visit(const Poco::XSD::Types::Message& val)
{
}


void CppGen::visit(const Poco::XSD::Types::Operation& val)
{
}


void CppGen::visit(const Poco::XSD::Types::PortType& val)
{
	for (Poco::XSD::Types::PortType::Operations::const_iterator it = val.operations().begin(); it != val.operations().end(); ++it)
	{
		it->second->accept(*this);
	}
}


void CppGen::visit(const Poco::XSD::Types::Binding& val)
{
	if (_options & OPT_GENERATE_ALL_BINDINGS)
	{
		generateBinding(val, val.name());
	}
}


void CppGen::generateBinding(const Poco::XSD::Types::Binding& binding, const std::string& name)
{
	std::string transport = binding.bindingProperties().get("soap.transport", "");
	std::string style = binding.bindingProperties().get("soap.style", "");
	if (transport == "http://schemas.xmlsoap.org/soap/http")
	{
		if (style == "document")
		{
			generateDocumentBinding(binding, name);
		}
		else if (style == "rpc")
		{
			generateRpcBinding(binding, name);
		}
	}
}

void CppGen::generateDocumentBinding(const Poco::XSD::Types::Binding& binding, const std::string& name)
{
	const SchemaInfo& si = config(_lastSchemaNamespace);
	std::string className = createClassName(name);
	ClassInfo ci(className, si.nameSpace(),  _lastSchemaNamespace, si.createInclude(className), si.dllPrefix());
	ci.setDestructor(Utility::AC_PUBLIC, true);
	ci.insert("remote", "");
	ci.insert("namespace", _lastSchemaNamespace);
	ci.insert("name", name);

	Poco::XSD::Types::PortType::Ptr pPortType = binding.getPortType();
	for (Poco::XSD::Types::PortType::Operations::const_iterator ito = pPortType->operations().begin(); ito != pPortType->operations().end(); ++ito)
	{
		Operation::Ptr pOperation = ito->second;
		std::string methodName = Utility::xsdNameToMethodName(pOperation->name());
		MethodInfo mi(methodName, Utility::AC_PUBLIC, false, false, true, true);

		std::string soapAction = pOperation->bindingProperties().get("soap.soapAction", "");
		if (soapAction.empty()) soapAction = "\"\"";
		mi.insert("action", soapAction);

		std::string wsaInputAction = pOperation->inputBindingProperties().get("wsa.action", "");
		if (!wsaInputAction.empty())
		{
			mi.insert("request", wsaInputAction);
		}
		std::string wsaOutputAction = pOperation->outputBindingProperties().get("wsa.action", "");
		if (!wsaOutputAction.empty())
		{
			mi.insert("reply", wsaOutputAction);
		}
		std::string wsaFaultAction = pOperation->faultBindingProperties().get("wsa.action", "");
		if (!wsaFaultAction.empty())
		{
			mi.insert("fault", wsaFaultAction);
		}

		std::string inputBodyUse = pOperation->inputBindingProperties().get("soap.body.use", "literal");
		if (inputBodyUse != "literal") throw Poco::NotImplementedException("SOAP input body use " + inputBodyUse + " is not supported.");

		std::string outputBodyUse = pOperation->outputBindingProperties().get("soap.body.use", "literal");
		if (outputBodyUse != "literal") throw Poco::NotImplementedException("SOAP output body use " + outputBodyUse + " is not supported.");

		std::string soapVersion = binding.bindingProperties().get("soap.version", "");
		createHeaderParameters(mi, pOperation->inputBindingProperties(), soapVersion, Parameter::DIR_IN);
		createHeaderParameters(mi, pOperation->outputBindingProperties(), soapVersion, Parameter::DIR_OUT);

		const Poco::XML::Name& inputMsgName = pOperation->getInputMessage();
		if (inputMsgName.localName().empty()) throw Poco::NotImplementedException("SOAP events are not supported.");

		if (isWrapped(pOperation->name(), inputMsgName))
		{
			std::string requestName = createWrappedParameters(mi, inputMsgName, Parameter::DIR_IN);
			mi.insert("name", requestName);

			const Poco::XML::Name& outputMsgName = pOperation->getOutputMessage();
			if (!outputMsgName.localName().empty())
			{
				std::string replyName = createWrappedParameters(mi, outputMsgName, Parameter::DIR_OUT);
				mi.insert("replyName", replyName);
			}
			else
			{
				mi.insert("oneway", "true");
			}
		}
		else
		{
			std::vector<std::string> parameterOrder;
			if (!(_options & OPT_IGNORE_PARAMETER_ORDER))
			{
				parameterOrder = pOperation->parameterOrder();
			}
			createParameters(mi, inputMsgName, Parameter::DIR_IN, parameterOrder);
			mi.insert("name", "#");

			const Poco::XML::Name& outputMsgName = pOperation->getOutputMessage();
			if (!outputMsgName.localName().empty())
			{
				parameterOrder.clear();
				createParameters(mi, outputMsgName, Parameter::DIR_OUT, parameterOrder);
				mi.insert("replyName", "#");
			}
			else
			{
				mi.insert("oneway", "true");
			}
		}

		ci.addMethod(mi);
	}

	schema(_lastSchemaNamespace).insert(std::make_pair(className, ci));
}


void CppGen::generateRpcBinding(const Poco::XSD::Types::Binding& binding, const std::string& name)
{
	const SchemaInfo& si = config(_lastSchemaNamespace);
	std::string className = createClassName(name);
	ClassInfo ci(className, si.nameSpace(),  _lastSchemaNamespace, si.createInclude(className), si.dllPrefix());
	ci.setDestructor(Utility::AC_PUBLIC, true);
	ci.insert("remote", "");
	ci.insert("namespace", _lastSchemaNamespace);
	ci.insert("name", name);

	Poco::XSD::Types::PortType::Ptr pPortType = binding.getPortType();
	for (Poco::XSD::Types::PortType::Operations::const_iterator ito = pPortType->operations().begin(); ito != pPortType->operations().end(); ++ito)
	{
		Operation::Ptr pOperation = ito->second;
		std::string methodName = Utility::xsdNameToMethodName(pOperation->name());
		MethodInfo mi(methodName, Utility::AC_PUBLIC, false, false, true, true);

		std::string soapAction = pOperation->bindingProperties().get("soap.soapAction", "");
		if (soapAction.empty()) soapAction = "\"\"";
		mi.insert("action", soapAction);

		std::string wsaInputAction = pOperation->inputBindingProperties().get("wsa.action", "");
		if (!wsaInputAction.empty())
		{
			mi.insert("request", wsaInputAction);
		}
		std::string wsaOutputAction = pOperation->outputBindingProperties().get("wsa.action", "");
		if (!wsaOutputAction.empty())
		{
			mi.insert("reply", wsaOutputAction);
		}
		std::string wsaFaultAction = pOperation->faultBindingProperties().get("wsa.action", "");
		if (!wsaFaultAction.empty())
		{
			mi.insert("fault", wsaFaultAction);
		}

		std::string inputBodyUse = pOperation->inputBindingProperties().get("soap.body.use", "literal");
		if (inputBodyUse != "literal") throw Poco::NotImplementedException("SOAP input body use " + inputBodyUse + " is not supported.");

		std::string outputBodyUse = pOperation->outputBindingProperties().get("soap.body.use", "literal");
		if (outputBodyUse != "literal") throw Poco::NotImplementedException("SOAP output body use " + outputBodyUse + " is not supported.");

		std::string soapVersion = binding.bindingProperties().get("soap.version", "");
		createHeaderParameters(mi, pOperation->inputBindingProperties(), soapVersion, Parameter::DIR_IN);
		createHeaderParameters(mi, pOperation->outputBindingProperties(), soapVersion, Parameter::DIR_OUT);

		const Poco::XML::Name& inputMsgName = pOperation->getInputMessage();
		if (inputMsgName.localName().empty()) throw Poco::NotImplementedException("SOAP events are not supported.");

		std::vector<std::string> parameterOrder;
		if (!(_options & OPT_IGNORE_PARAMETER_ORDER))
		{
			parameterOrder = pOperation->parameterOrder();
		}
		createParameters(mi, inputMsgName, Parameter::DIR_IN, parameterOrder);
		mi.insert("name", pOperation->name());

		const Poco::XML::Name& outputMsgName = pOperation->getOutputMessage();
		if (!outputMsgName.localName().empty())
		{
			parameterOrder.clear();
			createParameters(mi, outputMsgName, Parameter::DIR_OUT, parameterOrder, true);
			mi.insert("replyName", pOperation->name() + "Response");
		}
		else
		{
			mi.insert("oneway", "true");
		}

		ci.addMethod(mi);
	}

	schema(_lastSchemaNamespace).insert(std::make_pair(className, ci));
}


void CppGen::visit(const Poco::XSD::Types::Service& val)
{
	const Poco::XSD::Types::Definitions& defs = Poco::XSD::Types::TypesManager::instance().getDefinitions(_lastSchemaNamespace);
	// find SOAP/HTTP port/binding
	for (Poco::XSD::Types::Service::Ports::const_iterator it = val.ports().begin(); it != val.ports().end(); ++it)
	{
		Poco::XSD::Types::Definitions::Bindings::const_iterator itb = defs.bindings().find(it->binding.localName());
		if (itb != defs.bindings().end())
		{
			Poco::XSD::Types::Binding::Ptr pBinding = itb->second;
			generateBinding(*pBinding, val.name());
		}
	}
}


void CppGen::createHeaderParameters(MethodInfo& mi, const Poco::XSD::Types::BindingProperties& bindingProps, const std::string& soapVersion, Parameter::Direction direction)
{
	int i = 0;
	while (bindingProps.has(Poco::format("soap.header[%d].message", i)))
	{
		std::string messageQ = bindingProps.get(Poco::format("soap.header[%d].message", i), "");
		std::string messageURI = bindingProps.get(Poco::format("soap.header[%d].message.namespaceURI", i), "");
		std::string messageLocal = bindingProps.get(Poco::format("soap.header[%d].message.localName", i), "");
		Poco::XML::Name message(messageQ, messageURI, messageLocal);
		std::string part = bindingProps.get(Poco::format("soap.header[%d].part", i), "");
		std::string use = bindingProps.get(Poco::format("soap.header[%d].use", i), "literal");
		std::string ver = bindingProps.get(Poco::format("soap.header[%d].soapVersion", i), "");
		if (use == "literal" && ver == soapVersion)
		{
			createHeaderParameters(mi, message, part, direction);
		}
		i++;
	}
}


void CppGen::createHeaderParameters(MethodInfo& mi, const Poco::XML::Name& messageName, const std::string& partName, Parameter::Direction direction)
{
	const Poco::XSD::Types::Definitions& defs = Poco::XSD::Types::TypesManager::instance().getDefinitions(_lastSchemaNamespace);

	Definitions::Messages::const_iterator itm = defs.messages().find(messageName.localName());
	if (itm == defs.messages().end()) throw Poco::NotFoundException("message", messageName.localName());
	Poco::XSD::Types::Message::Ptr pMessage = itm->second;
	for (Poco::XSD::Types::Message::Parts::const_iterator itp = pMessage->parts().begin(); itp != pMessage->parts().end(); itp++)
	{
		if (itp->name == partName)
		{
			const Poco::XML::Name& elemName = itp->elementName;

			Poco::XSD::Types::TypesManager& tm = Poco::XSD::Types::TypesManager::instance();
			Poco::XSD::Types::QName qname(elemName.localName(), elemName.namespaceURI());
			const Poco::XSD::Types::Element* pElem = tm.getElement(qname);
			const Poco::XSD::Types::Type& type = pElem->type();

			TypeNameMap::const_iterator ittm = _typeNameMap.find(&type);
			if (ittm != _typeNameMap.end())
			{
				std::string typeClassName = ittm->second;
				std::string typeClassNamespace = _typeNamespaceMap[&type];

				CppGen::Classes& classes = schema(typeClassNamespace);
				Classes::const_iterator iti = classes.find(typeClassName);
				if (iti != classes.end())
				{
					const ClassInfo& ci = iti->second;

					Variable::Modifiers mod;
					if (direction == Parameter::DIR_IN)
						mod = Variable::V_ISCONSTREF;
					else
						mod = Variable::V_ISREF;
					std::string parName = Utility::xsdNameToParamName(pElem->name());

					makeUniqueParameterName(mi, parName);
					int order = static_cast<int>(mi.getParameters().size());
					Parameter par(parName, ci.getTypeInfo(), order, mod, false, true, ci.getTypeInfo().isNullable());
					par.setDirection(direction);
					par.insert("direction", direction == Parameter::DIR_IN ? "in" : "out");
					par.insert("name", pElem->name());
					par.insert("header", "true");
					par.insert("order", Poco::NumberFormatter::format(order));
					mi.addParameter(par);
				}
				else throw Poco::NotFoundException("class: ", typeClassName);
			}
			else throw Poco::NotFoundException(Poco::format("No C++ class for type '%s' of parameter element '%s'", type.name(), elemName.localName()));
		}
	}
}


bool CppGen::isWrapped(const std::string& operationName, const Poco::XML::Name& messageName)
{
	const Poco::XSD::Types::Definitions& defs = Poco::XSD::Types::TypesManager::instance().getDefinitions(_lastSchemaNamespace);

	// TODO: check namespace
	Definitions::Messages::const_iterator itm = defs.messages().find(messageName.localName());
	if (itm == defs.messages().end()) throw Poco::NotFoundException("message", messageName.localName());
	Poco::XSD::Types::Message::Ptr pMessage = itm->second;

	bool isWrapped = false;
	if (pMessage->parts().size() == 1)
	{
		const Poco::XML::Name& elemName = pMessage->parts()[0].elementName;
		isWrapped = elemName.localName() == operationName;
	}
	return isWrapped;
}


std::string CppGen::createWrappedParameters(MethodInfo& mi, const Poco::XML::Name& messageName, Parameter::Direction direction)
{
	std::string result;
	const Poco::XSD::Types::Definitions& defs = Poco::XSD::Types::TypesManager::instance().getDefinitions(_lastSchemaNamespace);

	// TODO: check namespace
	Definitions::Messages::const_iterator itm = defs.messages().find(messageName.localName());
	if (itm == defs.messages().end()) throw Poco::NotFoundException("message", messageName.localName());
	Poco::XSD::Types::Message::Ptr pMessage = itm->second;

	if (pMessage->parts().size() == 1)
	{
		const Poco::XML::Name& elemName = pMessage->parts()[0].elementName;
		Poco::XSD::Types::TypesManager& tm = Poco::XSD::Types::TypesManager::instance();
		Poco::XSD::Types::QName qname(elemName.localName(), elemName.namespaceURI());
		const Poco::XSD::Types::Element* pElem = tm.getElement(qname);
		if (!pElem) throw Poco::NotFoundException("element", qname.name());
		const Poco::XSD::Types::Type& type = pElem->type();
		result = elemName.localName();

		TypeNameMap::const_iterator ittm = _typeNameMap.find(&type);
		if (ittm != _typeNameMap.end())
		{
			std::string typeClassName = ittm->second;
			std::string typeClassNamespace = _typeNamespaceMap[&type];

			CppGen::Classes& classes = schema(typeClassNamespace);
			Classes::const_iterator iti = classes.find(typeClassName);
			if (iti != classes.end())
			{
				const ClassInfo& ci = iti->second;
				if (ci.getSchemaNameSpace() != _lastSchemaNamespace)
				{
					mi.insert("namespace", ci.getSchemaNameSpace());
				}
				const std::map<int, Variable>& vars = ci.getVariables();
				int orderOffset = static_cast<int>(mi.getParameters().size());
				for (std::map<int, Variable>::const_iterator itv = vars.begin(); itv != vars.end(); ++itv)
				{
					std::string parName = Utility::varNameToParamName(itv->second.getName());
					makeUniqueParameterName(mi, parName);
					Parameter par(parName, itv->second.getType(), itv->first + orderOffset, itv->second.getModifiers(), itv->second.isOptional(), false, itv->second.isNillable());
					if (!itv->second.getType().isScalar() || direction == Parameter::DIR_OUT)
					{
						if (direction == Parameter::DIR_IN)
						{
							par.setModifiers(static_cast<Variable::Modifiers>(par.getModifiers() | Variable::V_ISCONSTREF));
						}
						else
						{
							par.setModifiers(static_cast<Variable::Modifiers>(par.getModifiers() | Variable::V_ISREF));
						}
					}
					par.setDirection(direction);
					par.insert("direction", direction == Parameter::DIR_IN ? "in" : "out");
					par.insert("name", itv->second.get("name"));
					mi.addParameter(par);
				}
			}
			else throw Poco::NotFoundException("class: ", typeClassName);
		}
		else throw Poco::NotFoundException(Poco::format("No C++ class for type '%s' of wrapped parameter element '%s'", type.name(), elemName.localName()));
	}
	else throw Poco::NotImplementedException("Cannot generate method for message with multiple parts: " + pMessage->name());

	return result;
}


void CppGen::createParameters(MethodInfo& mi, const Poco::XML::Name& messageName, Parameter::Direction direction, std::vector<std::string>& parameterOrder, bool detectReturn)
{
	const Poco::XSD::Types::Definitions& defs = Poco::XSD::Types::TypesManager::instance().getDefinitions(_lastSchemaNamespace);

	// TODO: check namespace
	Definitions::Messages::const_iterator itm = defs.messages().find(messageName.localName());
	if (itm == defs.messages().end()) throw Poco::NotFoundException("message", messageName.localName());
	Poco::XSD::Types::Message::Ptr pMessage = itm->second;

	if (parameterOrder.empty())
	{
		for (int part = 0; part < pMessage->parts().size(); part++)
		{
			parameterOrder.push_back(pMessage->parts()[part].name);
		}
	}

	std::map<std::string, int> parameterOrderMap;
	int parameterOffset = static_cast<int>(mi.getParameters().size());
	for (std::vector<std::string>::const_iterator it = parameterOrder.begin(); it != parameterOrder.end(); ++it)
	{
		int offset = parameterOffset++;
		parameterOrderMap[*it] = offset;
	}

	for (int part = 0; part < pMessage->parts().size(); part++)
	{
		const Poco::XSD::Types::Type* pType = 0;
		std::string paramName;
		std::string cppParamName;
		TypeInfo typeInfo;
		bool optional = false;

		const std::string& partName = pMessage->parts()[part].name;
		const Poco::XML::Name& elemName = pMessage->parts()[part].elementName;
		const Poco::XML::Name& typeName = pMessage->parts()[part].typeName;
		Poco::XSD::Types::TypesManager& tm = Poco::XSD::Types::TypesManager::instance();
		if (!elemName.localName().empty())
		{
			Poco::XSD::Types::QName qname(elemName.localName(), elemName.namespaceURI());
			const Poco::XSD::Types::Element* pElem = tm.getElement(qname);
			if (!pElem) throw Poco::NotFoundException("element", qname.name());
			pType = &pElem->type();
			paramName = elemName.localName();
			cppParamName = Utility::xsdNameToParamName(paramName);

			_elements.push(elemName.localName());
			typeInfo = createTypeInfo(pType);
			_elements.pop();
			typeInfo.setVector((pElem->getMaxOccurs() > 1));
			typeInfo.setNullable(pElem->getMinOccurs() == 0 && pElem->getMaxOccurs() == 1);
			// TODO: const std::string& defValue = (pElem->hasFixed() ? pElem->getFixed() : pElem->getDefault());
			optional = pElem->getMinOccurs() == 0;
		}
		else if (!typeName.localName().empty())
		{
			Poco::XSD::Types::QName qname(typeName.localName(), typeName.namespaceURI());
			pType = tm.getType(qname);
			if (!pType) throw Poco::NotFoundException("type", qname.name());
			paramName = partName;
			cppParamName = Utility::xsdNameToParamName(paramName);
			typeInfo = createTypeInfo(pType);
		}

		makeUniqueParameterName(mi, cppParamName);

		Variable::Modifiers mod = Variable::V_ISVALUE;
		if (typeInfo.isVector())
			mod = Variable::V_ISVECTOR;

		Parameter par(cppParamName, typeInfo, parameterOrderMap[partName], mod, optional, false, typeInfo.isNullable());
		if (!typeInfo.isScalar() || direction == Parameter::DIR_OUT)
		{
			if (direction == Parameter::DIR_IN)
			{
				par.setModifiers(static_cast<Variable::Modifiers>(par.getModifiers() | Variable::V_ISCONSTREF));
			}
			else if (!detectReturn || paramName != "return")
			{
				par.setModifiers(static_cast<Variable::Modifiers>(par.getModifiers() | Variable::V_ISREF));
			}
		}

		par.setDirection(direction);
		par.insert("direction", direction == Parameter::DIR_IN ? "in" : "out");
		par.insert("name", paramName);
		par.insert("order", Poco::NumberFormatter::format(par.getOrder()));
		if (detectReturn && paramName == "return")
			mi.setReturnParameter(new Parameter(par));
		else
			mi.addParameter(par);
	}
}


void CppGen::makeUniqueParameterName(const MethodInfo& mi, std::string& name)
{
	std::string originalName(name);
	int count = 2;
	std::map<int, Parameter>::const_iterator it = mi.getParameters().begin();
	std::map<int, Parameter>::const_iterator end = mi.getParameters().end();
	while (it != end)
	{
		if (it->second.getName() == name)
		{
			name = originalName + Poco::NumberFormatter::format(count++);
			it = mi.getParameters().begin();
			continue;
		}
		it++;
	}
}


const SchemaInfo& CppGen::config(const std::string& ns) const
{
	std::map<std::string, SchemaInfo>::const_iterator it = _config.find(ns);
	if (it == _config.end())
		throw Poco::NotFoundException(
			"No C++ mapping for schema target namespace \"" + ns + "\" defined. "
			"Add a <schema targetNamespace=\"" + ns + "\"> element to the code generator configuration "
			"containing at least a <namespace> element to fix this error.");
	return it->second;
}


CppGen::Classes& CppGen::schema(const std::string& ns)
{
	Schemas::iterator it = _schemas.find(ns);
	if (it == _schemas.end())
	{
		TypesManager& tm = TypesManager::instance();
		const TypesManager::Schemas& allSchemas = tm.getSchemas();
		TypesManager::Schemas::const_iterator itS = allSchemas.find(ns);
		if (itS != allSchemas.end())
		{
			prepare(*itS->second);
			it = _schemas.find(ns);
			poco_assert (it != _schemas.end());
			return it->second;
		}
		else throw Poco::NotFoundException("Schema", ns);
	}
	return it->second;
}


const CppGen::Classes& CppGen::schema(const std::string& ns) const
{
	Schemas::const_iterator it = _schemas.find(ns);
	if (it == _schemas.end())
		throw Poco::NotFoundException("Schema", ns);
	return it->second;
}


const ClassInfo& CppGen::classInfo(const std::string& ns, const std::string& name) const
{
	const CppGen::Classes& classes = schema(ns);
	CppGen::Classes::const_iterator it = classes.find(name);
	if (it == classes.end())
		throw Poco::NotFoundException("Class", ns + "::" + name);
	return it->second;
}


void CppGen::assertClassInfoExists(const Poco::XSD::Types::ComplexType* pVal)
{
	if (!pVal)
		return;

	poco_assert_dbg (pVal->getSchema());
	const CppGen::Classes& classes = schema(pVal->getSchema()->targetNamespace());
	CppGen::Classes::const_iterator it = classes.find(Utility::xsdNameToClassName(pVal->name()));
	if (it == classes.end())
		prepare(*pVal);
}


void CppGen::assertClassInfoExists(const Poco::XSD::Types::SimpleType* pVal)
{
	if (!pVal)
		return;

	if (Utility::isBuiltinNamespace(pVal->getSchema()->targetNamespace()))
		return;

	poco_assert_dbg (pVal->getSchema());
	const CppGen::Classes& classes = schema(pVal->getSchema()->targetNamespace());
	CppGen::Classes::const_iterator it = classes.find(Utility::xsdNameToClassName(pVal->name()));
	if (it == classes.end())
		prepare(*pVal);
}


std::string CppGen::createClassName(const std::string& xsdClassName)
{
	if (xsdClassName.empty())
		return genInnerClassName();
	else
		return Utility::xsdNameToClassName(xsdClassName);
}


std::string CppGen::genInnerClassName()
{
	poco_assert (!_classes.empty());
	poco_assert (!_elements.empty());

	return _classes.top().name() + "_" + Utility::xsdNameToClassName(_elements.top());
}


TypeInfo CppGen::createTypeInfo(const Poco::XSD::Types::Type* pType)
{
	static BuiltinTypes& xsdTypes = BuiltinTypes::instance();
	poco_assert_dbg (pType);

	if (pType->getSchema())
	{
		if (Utility::isBuiltinNamespace(pType->getSchema()->targetNamespace()))
		{
			// a builtin type, map it to a cpp type
			return xsdTypes.get(pType->name());
		}

		const std::string& xmlNS = pType->getSchema()->targetNamespace();
		const ClassInfo& cls = classInfo(xmlNS, Utility::xsdNameToClassName(pType->name()));
		return cls.getTypeInfo();
	}
	else
	{
		poco_check_ptr (_pLastSchema);

		const std::string& xmlNS = _pLastSchema->id();
		std::string className = createClassName(pType->name());
		const ClassInfo& cls = classInfo(xmlNS, className);
		return cls.getTypeInfo();
	}
}


void CppGen::postProcess()
{
	// iterate over all schemas, addFullConstructor

	Schemas::iterator it = _schemas.begin();
	for (; it != _schemas.end(); ++it)
	{
		Classes::iterator itC = it->second.begin();
		for (; itC != it->second.end(); ++itC)
		{
			addFullConstructor(itC->second);
		}
	}
}


void CppGen::addFullConstructor(ClassInfo& ci)
{
	// only add a full constructor if we have members,
	// or one of the superclasses has members!
	// (otherwise duplicate default constructor)
	// accumulate data starting with the top root class
	std::vector<ClassInfo*> hierarchy;
	buildHierarchy(hierarchy, ci);
	std::vector<const Variable*> vars;
	extractVariables(vars, hierarchy);

	if (vars.empty()) return;

	int varCnt = static_cast<int>(ci.getVariables().size());
	int totCnt = static_cast<int>(vars.size());
	int parentVars = totCnt - varCnt;
	Constructor c(ci, Utility::AC_PUBLIC);
	std::vector<const Variable*>::const_iterator it = vars.begin();
	int cnt(0);

	std::string parentCall;
	const TypeInfo& par = ci.getParent();
	if (!par.getNameSpace().empty() && par.getNameSpace() != ci.getNameSpace())
		parentCall = par.getNameSpace() + "::";
	parentCall += par.name() + "(";
	bool writeColon = false;
	for (; it != vars.end(); ++it, ++cnt)
	{
		int modMethod = Variable::V_ISVALUE;
		if (!(*it)->getType().isScalar())
			modMethod = Variable::V_ISCONSTREF;
		if ((*it)->getType().isVector())
			modMethod |= Variable::V_ISCONSTREFVECTOR;
		if ((*it)->isPointer())
			modMethod |= Variable::V_ISPOINTER;
		std::string paramName = Utility::varNameToParamName((*it)->getName());
		Parameter param(paramName, (*it)->getType(), cnt, static_cast<Variable::Modifiers>(modMethod), (*it)->isOptional(), false, (*it)->isNillable());
		c.addParameter(param);
		if (cnt < parentVars)
		{
			if (writeColon)
				parentCall += ", ";
			writeColon = true;
			parentCall += paramName;
			if (cnt == parentVars - 1)
			{
				parentCall += ")";
				if (!par.name().empty())
					c.addInitializationCode(parentCall);
			}
		}
		else
		{
			// add constructor initializer to c
			c.addInitializationCode((*it)->getName()+"("+paramName+")");
		}
	}
	ci.addConstructor(c);
}


void CppGen::buildHierarchy(std::vector<ClassInfo*>& hierarchy, ClassInfo& ci)
{
	static BuiltinTypes& types = BuiltinTypes::instance();
	if (types.isKnownTypeInfo(ci.getParent()))
	{
		hierarchy.push_back(&ci);
		return;
	}

	std::string xsdNS = ci.getParent().getSchemaNameSpace();
	if (!ci.getParent().name().empty())
	{
		ClassInfo& parent = classInfo(xsdNS, ci.getParent().name());
		buildHierarchy(hierarchy, parent);
	}
	hierarchy.push_back(&ci);
}


ClassInfo& CppGen::classInfo(const std::string& ns, const std::string& name)
{
	CppGen::Classes& classes = schema(ns);
	CppGen::Classes::iterator it = classes.find(name);
	if (it == classes.end())
		throw Poco::NotFoundException("Class", ns + "::" + name);
	return it->second;
}


void CppGen::extractVariables(std::vector<const Variable*>& vars, const std::vector<ClassInfo*>& classes)
{
	std::vector<ClassInfo*>::const_iterator it = classes.begin();
	for (; it != classes.end(); ++it)
	{
		const std::map<int, Variable>& tmp = (*it)->getVariables();
		std::map<int, Variable>::const_iterator itV = tmp.begin();
		for (; itV != tmp.end(); ++itV)
		{
			vars.push_back(&(itV->second));
		}
	}
}
