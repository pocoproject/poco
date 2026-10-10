//
// StateMachine.cpp
//
// Library: XSD/Parser
// Package: XSDParser
// Module:  StateMachine
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/XSD/Parser/StateMachine.h"
#include "Poco/XSD/Parser/XSDContentHandler.h"
#include "Poco/XSD/Parser/Constants.h"


namespace Poco::XSD::Parser {


StateMachine::StateMachine():_stateInfos(128)
{
	initialize();
}


StateMachine::~StateMachine() = default;


void StateMachine::initialize()
{
	defineStateAll();
	defineStateAnnotation();
	defineStateAny();
	defineStateAnyAttribute();
	defineStateAppInfo();
	defineStateAttribute();
	defineStateAttributeGroup();
	defineStateChoice();
	defineStateComplexContent();
	defineStateComplexType();
	defineStateDocument();
	defineStateDocumentation();
	defineStateElement();
	defineStateSimpleExtension();
	defineStateComplexExtension();
	defineStateField();
	defineStateGroup();
	defineStateXSDImport();
	defineStateInclude();
	defineStateKey();
	defineStateKeyref();
	defineStateList();
	defineStateNotation();
	defineStateRedefine();
	defineStateSimpleTypeRestriction();
	defineStateSimpleContentRestriction();
	defineStateComplexRestriction();
	defineStateSchema();
	defineStateSelector();
	defineStateSequence();
	defineStateSimpleContent();
	defineStateSimpleType();
	defineStateUnion();
	defineStateUnique();
	defineStateEnumeration();
	defineStateFractionDigits();
	defineStateLength();
	defineStateMaxExclusive();
	defineStateMaxInclusive();
	defineStateMaxLength();
	defineStateMinExclusive();
	defineStateMinInclusive();
	defineStateMinLength();
	defineStatePattern();
	defineStateTotalDigits();
	defineStateUninitialized();
	defineStateWhiteSpace();
	defineStateMetaAny();
	defineStateDefinitions();
	defineStateWSDLImport();
	defineStateTypes();
	defineStateMessage();
	defineStatePart();
	defineStatePortType();
	defineStateOperation();
	defineStateInput();
	defineStateOutput();
	defineStateFault();
	defineStateBinding();
	defineStateSoapBinding();
	defineStateSoapOperation();
	defineStateSoapHeader();
	defineStateSoapHeaderFault();
	defineStateSoapBody();
	defineStateSoapFault();
	defineStateSoap12Binding();
	defineStateSoap12Operation();
	defineStateSoap12Header();
	defineStateSoap12HeaderFault();
	defineStateSoap12Body();
	defineStateSoap12Fault();
	defineStateService();
	defineStatePort();
	defineStateSoapAddress();
	defineStateWSDLDocumentation();
}


StateMachine::State StateMachine::state(const std::string& uri, const std::string& elementName, StateMachine::State parent) const
{
	if (parent >= StateMachine::ST_INCOMPLEXCONTENT)
	{
		if (uri == Constants::XSD_NAMESPACE_URI && elementName == Constants::XSD_RESTRICTION)
		{
			// eval parent
			if (parent == StateMachine::ST_INCOMPLEXCONTENT)
				return _states.find(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_COMPLEXRESTRICTION))->second;
			else if (parent == StateMachine::ST_INSIMPLECONTENT)
				return _states.find(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_SIMPLECONTENTRESTRICTION))->second;
			else
				return _states.find(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_SIMPLETYPERESTRICTION))->second;
		}
		else if (uri == Constants::XSD_NAMESPACE_URI && elementName == Constants::XSD_EXTENSION)
		{
			// eval parent
			if (parent == StateMachine::ST_INCOMPLEXCONTENT)
				return _states.find(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_COMPLEXEXTENSION))->second;
			else
				return _states.find(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_SIMPLEEXTENSION))->second;
		}
	}
	poco_assert_dbg (elementName != Constants::XSD_RESTRICTION);
	poco_assert_dbg (elementName != Constants::XSD_EXTENSION);
	if (const auto it = _states.find(cat(uri, elementName)); it == _states.end())
		return ST_INUNINITIALIZED;
	else 
		return it->second;
}


void StateMachine::defineStateAll()
{
	poco_assert_dbg(_states.find(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_ALL)) == _states.end());

	//Content: (annotation?, element*)
	
	_states.try_emplace(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_ALL), ST_INALL);

	StateSet aSet;
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_ANNOTATION));
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_ELEMENT));

	StateInfo info(ST_INALL, &XSDContentHandler::stateAllStart, &XSDContentHandler::stateAllEnd, aSet);
	_stateInfos [ST_INALL] = info;
}


void StateMachine::defineStateAnnotation()
{
	poco_assert_dbg(_states.find(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_ANNOTATION)) == _states.end());

	//Content: (appinfo | documentation)*

	_states.try_emplace(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_ANNOTATION), ST_INANNOTATION);

	StateSet aSet;
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_APPINFO));
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_DOCUMENTATION));

	StateInfo info(ST_INANNOTATION, &XSDContentHandler::stateAnnotationStart, &XSDContentHandler::stateAnnotationEnd, aSet);
	_stateInfos [ST_INANNOTATION] = info;
}


void StateMachine::defineStateAny()
{
	poco_assert_dbg(_states.find(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_ANY)) == _states.end());

	// Content: (annotation?)
	
	_states.try_emplace(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_ANY), ST_INANY);

	StateSet aSet;
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_ANNOTATION));
	
	StateInfo info(ST_INANY, &XSDContentHandler::stateAnyStart, &XSDContentHandler::stateAnyEnd, aSet);
	_stateInfos [ST_INANY] = info;
}


void StateMachine::defineStateAnyAttribute()
{
	poco_assert_dbg(_states.find(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_ANYATTRIBUTE)) == _states.end());

	// Content: (annotation?)
	
	_states.try_emplace(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_ANYATTRIBUTE), ST_INANYATTRIBUTE);

	StateSet aSet;
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_ANNOTATION));
	
	StateInfo info(ST_INANYATTRIBUTE, &XSDContentHandler::stateAnyAttributeStart, &XSDContentHandler::stateAnyAttributeEnd, aSet);
	_stateInfos [ST_INANYATTRIBUTE] = info;
}


void StateMachine::defineStateAppInfo()
{
	poco_assert_dbg(_states.find(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_APPINFO)) == _states.end());

	// Content: ({any})*
	
	_states.try_emplace(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_APPINFO), ST_INAPPINFO);

	StateSet aSet;
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_METAANY));

	StateInfo info(ST_INAPPINFO, &XSDContentHandler::stateAppInfoStart, &XSDContentHandler::stateAppInfoEnd, aSet);
	_stateInfos[ST_INAPPINFO] = info;
}


void StateMachine::defineStateAttribute()
{
	poco_assert_dbg(_states.find(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_ATTRIBUTE)) == _states.end());

	//Content: (annotation?, simpleType?)
	
	_states.try_emplace(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_ATTRIBUTE), ST_INATTRIBUTE);

	StateSet aSet;
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_ANNOTATION));
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_SIMPLETYPE));

	StateInfo info(ST_INATTRIBUTE, &XSDContentHandler::stateAttributeStart, &XSDContentHandler::stateAttributeEnd, aSet);
	_stateInfos[ST_INATTRIBUTE] = info;
}


void StateMachine::defineStateAttributeGroup()
{
	poco_assert_dbg(_states.find(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_ATTRIBUTEGROUP)) == _states.end());

	//Content: (annotation?, ((attribute | attributeGroup)*, anyAttribute?))
	
	_states.try_emplace(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_ATTRIBUTEGROUP), ST_INATTRIBUTEGROUP);

	StateSet aSet;
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_ANNOTATION));
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_ATTRIBUTE));
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_ATTRIBUTEGROUP));
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_ANYATTRIBUTE));

	StateInfo info(ST_INATTRIBUTEGROUP, &XSDContentHandler::stateAttributeGroupStart, &XSDContentHandler::stateAttributeGroupEnd, aSet);
	_stateInfos[ST_INATTRIBUTEGROUP] = info;
}


void StateMachine::defineStateChoice()
{
	poco_assert_dbg(_states.find(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_CHOICE)) == _states.end());

	//  Content: (annotation?, (element | group | choice | sequence | any)*)
	
	_states.try_emplace(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_CHOICE), ST_INCHOICE);

	StateSet aSet;
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_ANNOTATION));
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_ELEMENT));
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_GROUP));
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_CHOICE));
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_SEQUENCE));
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_ANY));

	StateInfo info(ST_INCHOICE, &XSDContentHandler::stateChoiceStart, &XSDContentHandler::stateChoiceEnd, aSet);
	_stateInfos[ST_INCHOICE] = info;
}


void StateMachine::defineStateComplexContent()
{
	poco_assert_dbg(_states.find(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_COMPLEXCONTENT)) == _states.end());

	//  Content: (annotation?, (restriction | extension))
	
	_states.try_emplace(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_COMPLEXCONTENT), ST_INCOMPLEXCONTENT);

	StateSet aSet;
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_ANNOTATION));
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_RESTRICTION));
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_EXTENSION));

	StateInfo info(ST_INCOMPLEXCONTENT, &XSDContentHandler::stateComplexContentStart, &XSDContentHandler::stateComplexContentEnd, aSet);
	_stateInfos[ST_INCOMPLEXCONTENT] = info;
}


void StateMachine::defineStateComplexType()
{
	poco_assert_dbg(_states.find(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_COMPLEXTYPE)) == _states.end());

	//  Content: (annotation?, (simpleContent | complexContent | ((group | all | choice | sequence)?, ((attribute | attributeGroup)*, anyAttribute?))))
	
	_states.try_emplace(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_COMPLEXTYPE), ST_INCOMPLEXTYPE);

	StateSet aSet;
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_ANNOTATION));
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_SIMPLECONTENT));
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_COMPLEXCONTENT));
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_GROUP));
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_ALL));
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_CHOICE));
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_SEQUENCE));
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_ATTRIBUTE));
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_ATTRIBUTEGROUP));
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_ANYATTRIBUTE));

	StateInfo info(ST_INCOMPLEXTYPE, &XSDContentHandler::stateComplexTypeStart, &XSDContentHandler::stateComplexTypeEnd, aSet);
	_stateInfos[ST_INCOMPLEXTYPE] = info;
}


void StateMachine::defineStateDocument()
{
	//  Content: schema or definitions (WSDL)
	
	StateSet aSet;
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_SCHEMA));
	aSet.insert(cat(Constants::WSDL_NAMESPACE_URI, Constants::WSDL_DEFINITIONS));

	StateInfo info(ST_INDOCUMENT, &XSDContentHandler::stateUninitializedStart, &XSDContentHandler::stateUninitializedEnd, aSet);
	_stateInfos[ST_INDOCUMENT] = info;
}


void StateMachine::defineStateDocumentation()
{
	poco_assert_dbg(_states.find(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_DOCUMENTATION)) == _states.end());

	//  Content: ({any})*
	
	_states.try_emplace(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_DOCUMENTATION), ST_INDOCUMENTATION);

	StateSet aSet;
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_METAANY));

	StateInfo info(ST_INDOCUMENTATION, &XSDContentHandler::stateDocumentationStart, &XSDContentHandler::stateDocumentationEnd, aSet);
	_stateInfos[ST_INDOCUMENTATION] = info;
}


void StateMachine::defineStateElement()
{
	poco_assert_dbg(_states.find(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_ELEMENT)) == _states.end());

	//  Content: (annotation?, ((simpleType | complexType)?, (unique | key | keyref)*))
	
	_states.try_emplace(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_ELEMENT), ST_INELEMENT);

	StateSet aSet;
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_ANNOTATION));
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_SIMPLETYPE));
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_COMPLEXTYPE));
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_UNIQUE));
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_KEY));
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_KEYREF));

	StateInfo info(ST_INELEMENT, &XSDContentHandler::stateElementStart, &XSDContentHandler::stateElementEnd, aSet);
	_stateInfos[ST_INELEMENT] = info;
}


void StateMachine::defineStateSimpleExtension()
{
	poco_assert_dbg(_states.find(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_SIMPLEEXTENSION)) == _states.end());

	//parent is simpleContent
	//  Content: (annotation?, ((attribute | attributeGroup)*, anyAttribute?))
	
	_states.try_emplace(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_SIMPLEEXTENSION), ST_INSIMPLEEXTENSION);

	StateSet aSet;
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_ANNOTATION));
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_ATTRIBUTE));
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_ATTRIBUTEGROUP));
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_ANYATTRIBUTE));

	StateInfo info(ST_INSIMPLEEXTENSION, &XSDContentHandler::stateSimpleExtensionStart, &XSDContentHandler::stateSimpleExtensionEnd, aSet);
	_stateInfos[ST_INSIMPLEEXTENSION] = info;
}


void StateMachine::defineStateComplexExtension()
{
	poco_assert_dbg(_states.find(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_COMPLEXEXTENSION)) == _states.end());

	//parent is complexContent:
	//  Content: (annotation?, ((group | all | choice | sequence)?, ((attribute | attributeGroup)*, anyAttribute?)))
	
	_states.try_emplace(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_COMPLEXEXTENSION), ST_INCOMPLEXEXTENSION);

	StateSet aSet;
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_ANNOTATION));
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_GROUP));
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_ALL));
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_CHOICE));
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_SEQUENCE));
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_ATTRIBUTE));
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_ATTRIBUTEGROUP));
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_ANYATTRIBUTE));

	StateInfo info(ST_INCOMPLEXEXTENSION, &XSDContentHandler::stateComplexExtensionStart, &XSDContentHandler::stateComplexExtensionEnd, aSet);
	_stateInfos[ST_INCOMPLEXEXTENSION] = info;
}


void StateMachine::defineStateField()
{
	poco_assert_dbg(_states.find(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_FIELD)) == _states.end());

	//  Content: (annotation?)
	
	_states.try_emplace(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_FIELD), ST_INFIELD);

	StateSet aSet;
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_ANNOTATION));

	StateInfo info(ST_INFIELD, &XSDContentHandler::stateFieldStart, &XSDContentHandler::stateFieldEnd, aSet);
	_stateInfos[ST_INFIELD] = info;
}


void StateMachine::defineStateGroup()
{
	poco_assert_dbg(_states.find(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_GROUP)) == _states.end());

	//  Content: (annotation?, (all | choice | sequence)?)
	
	_states.try_emplace(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_GROUP), ST_INGROUP);

	StateSet aSet;
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_ANNOTATION));
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_ALL));
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_CHOICE));
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_SEQUENCE));
	
	StateInfo info(ST_INGROUP, &XSDContentHandler::stateGroupStart, &XSDContentHandler::stateGroupEnd, aSet);
	_stateInfos[ST_INGROUP] = info;
}


void StateMachine::defineStateXSDImport()
{
	poco_assert_dbg(_states.find(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_IMPORT)) == _states.end());

	//  Content: (annotation?)
	
	_states.try_emplace(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_IMPORT), ST_INXSDIMPORT);

	StateSet aSet;
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_ANNOTATION));
	
	StateInfo info(ST_INXSDIMPORT, &XSDContentHandler::stateXSDImportStart, &XSDContentHandler::stateXSDImportEnd, aSet);
	_stateInfos[ST_INXSDIMPORT] = info;
}


void StateMachine::defineStateInclude()
{
	poco_assert_dbg(_states.find(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_INCLUDE)) == _states.end());

	//  Content: (annotation?)
	
	_states.try_emplace(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_INCLUDE), ST_ININCLUDE);

	StateSet aSet;
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_ANNOTATION));

	StateInfo info(ST_ININCLUDE, &XSDContentHandler::stateIncludeStart, &XSDContentHandler::stateIncludeEnd, aSet);
	_stateInfos[ST_ININCLUDE] = info;
}


void StateMachine::defineStateKey()
{
	poco_assert_dbg(_states.find(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_KEY)) == _states.end());

	//  Content: (annotation?, (selector, field+))
	
	_states.try_emplace(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_KEY), ST_INKEY);

	StateSet aSet;
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_ANNOTATION));
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_SELECTOR));
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_FIELD));

	StateInfo info(ST_INKEY, &XSDContentHandler::stateKeyStart, &XSDContentHandler::stateKeyEnd, aSet);
	_stateInfos[ST_INKEY] = info;
}


void StateMachine::defineStateKeyref()
{
	poco_assert_dbg(_states.find(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_KEYREF)) == _states.end());

	//  Content: (annotation?, (selector, field+))
	
	_states.try_emplace(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_KEYREF), ST_INKEYREF);

	StateSet aSet;
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_ANNOTATION));
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_SELECTOR));
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_FIELD));

	StateInfo info(ST_INKEYREF, &XSDContentHandler::stateKeyrefStart, &XSDContentHandler::stateKeyrefEnd, aSet);
	_stateInfos[ST_INKEYREF] = info;
}


void StateMachine::defineStateList()
{
	poco_assert_dbg(_states.find(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_LIST)) == _states.end());

	//  Content: (annotation?, simpleType?)
	
	_states.try_emplace(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_LIST), ST_INLIST);

	StateSet aSet;
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_ANNOTATION));
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_SIMPLETYPE));

	StateInfo info(ST_INLIST, &XSDContentHandler::stateListStart, &XSDContentHandler::stateListEnd, aSet);
	_stateInfos[ST_INLIST] = info;
}


void StateMachine::defineStateNotation()
{
	poco_assert_dbg(_states.find(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_NOTATION)) == _states.end());

	//  Content: (annotation?)
	
	_states.try_emplace(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_NOTATION), ST_INNOTATION);

	StateSet aSet;
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_ANNOTATION));

	StateInfo info(ST_INNOTATION, &XSDContentHandler::stateNotationStart, &XSDContentHandler::stateNotationEnd, aSet);
	_stateInfos[ST_INNOTATION] = info;
}


void StateMachine::defineStateRedefine()
{
	poco_assert_dbg(_states.find(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_REDEFINE)) == _states.end());

	//  Content: (annotation | (simpleType | complexType | group | attributeGroup))*
	
	_states.try_emplace(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_REDEFINE), ST_INREDEFINE);

	StateSet aSet;
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_ANNOTATION));
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_SIMPLETYPE));
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_COMPLEXTYPE));
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_GROUP));
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_ATTRIBUTEGROUP));

	StateInfo info(ST_INREDEFINE, &XSDContentHandler::stateRedefineStart, &XSDContentHandler::stateRedefineEnd, aSet);
	_stateInfos[ST_INREDEFINE] = info;
}


void StateMachine::defineStateSimpleTypeRestriction()
{
	poco_assert_dbg(_states.find(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_SIMPLETYPERESTRICTION)) == _states.end());

	// parent is SimpleType
	//  Content: (annotation?, (simpleType?, (minExclusive | minInclusive | maxExclusive | maxInclusive | totalDigits | fractionDigits | length | minLength | maxLength | enumeration | whiteSpace | pattern)*))
	_states.try_emplace(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_SIMPLETYPERESTRICTION), ST_INSIMPLETYPERESTRICTION);

	StateSet aSet;
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_ANNOTATION));
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_SIMPLETYPE));
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_MINEXCLUSIVE));
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_MININCLUSIVE));
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_MAXEXCLUSIVE));
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_MAXINCLUSIVE));
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_TOTALDIGITS));
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_FRACTIONDIGITS));
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_LENGTH));
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_MINLENGTH));
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_MAXLENGTH));
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_ENUMERATION));
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_WHITESPACE));
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_PATTERN));

	StateInfo info(ST_INSIMPLETYPERESTRICTION, &XSDContentHandler::stateSimpleTypeRestrictionStart, &XSDContentHandler::stateSimpleTypeRestrictionEnd, aSet);
	_stateInfos[ST_INSIMPLETYPERESTRICTION] = info;
}


void StateMachine::defineStateSimpleContentRestriction()
{
	poco_assert_dbg(_states.find(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_SIMPLECONTENTRESTRICTION)) == _states.end());

	// parent is simpleContent 
	//Content: (annotation?, (simpleType?, (minExclusive | minInclusive | maxExclusive | maxInclusive | 
	//          totalDigits | fractionDigits | length | minLength | maxLength | enumeration | whiteSpace | pattern)*)?, 
	//          ((attribute | attributeGroup)*, anyAttribute?))
	_states.try_emplace(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_SIMPLECONTENTRESTRICTION), ST_INSIMPLECONTENTRESTRICTION);

	StateSet aSet;
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_ANNOTATION));
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_SIMPLETYPE));
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_MINEXCLUSIVE));
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_MININCLUSIVE));
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_MAXEXCLUSIVE));
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_MAXINCLUSIVE));
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_TOTALDIGITS));
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_FRACTIONDIGITS));
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_LENGTH));
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_MINLENGTH));
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_MAXLENGTH));
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_ENUMERATION));
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_WHITESPACE));
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_PATTERN));
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_ATTRIBUTE));
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_ATTRIBUTEGROUP));
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_ANYATTRIBUTE));

	StateInfo info(ST_INSIMPLECONTENTRESTRICTION, &XSDContentHandler::stateSimpleContentRestrictionStart, &XSDContentHandler::stateSimpleContentRestrictionEnd, aSet);
	_stateInfos[ST_INSIMPLECONTENTRESTRICTION] = info;
}


void StateMachine::defineStateComplexRestriction()
{
	poco_assert_dbg(_states.find(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_COMPLEXRESTRICTION)) == _states.end());

	//  Content: (annotation?, ((group | all | choice | sequence)?, ((attribute | attributeGroup)*, anyAttribute?)))
	
	_states.try_emplace(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_COMPLEXRESTRICTION), ST_INCOMPLEXRESTRICTION);

	StateSet aSet;
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_ANNOTATION));
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_GROUP));
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_ALL));
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_CHOICE));
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_SEQUENCE));
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_ATTRIBUTE));
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_ATTRIBUTEGROUP));
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_ANYATTRIBUTE));

	StateInfo info(ST_INCOMPLEXRESTRICTION, &XSDContentHandler::stateComplexRestrictionStart, &XSDContentHandler::stateComplexRestrictionEnd, aSet);
	_stateInfos[ST_INCOMPLEXRESTRICTION] = info;
}


void StateMachine::defineStateSchema()
{
	poco_assert_dbg(_states.find(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_SCHEMA)) == _states.end());

	//  Content: ((include | import | redefine | annotation)*, (((simpleType | complexType | group | attributeGroup) | element | attribute | notation), annotation*)*)
	
	_states.try_emplace(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_SCHEMA), ST_INSCHEMA);

	StateSet aSet;
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_INCLUDE));
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_IMPORT));
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_REDEFINE));
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_ANNOTATION));
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_SIMPLETYPE));
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_COMPLEXTYPE));
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_GROUP));
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_ATTRIBUTEGROUP));
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_ELEMENT));
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_ATTRIBUTE));
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_NOTATION));

	StateInfo info(ST_INSCHEMA, &XSDContentHandler::stateSchemaStart, &XSDContentHandler::stateSchemaEnd, aSet);
	_stateInfos[ST_INSCHEMA] = info;
}


void StateMachine::defineStateSelector()
{
	poco_assert_dbg(_states.find(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_SELECTOR)) == _states.end());

	//  Content: (annotation?)
	
	_states.try_emplace(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_SELECTOR), ST_INSELECTOR);

	StateSet aSet;
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_ANNOTATION));

	StateInfo info(ST_INSELECTOR, &XSDContentHandler::stateSelectorStart, &XSDContentHandler::stateSelectorEnd, aSet);
	_stateInfos[ST_INSELECTOR] = info;
}


void StateMachine::defineStateSequence()
{
	poco_assert_dbg(_states.find(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_SEQUENCE)) == _states.end());

	//  Content: (annotation?, (element | group | choice | sequence | any)*)
	
	_states.try_emplace(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_SEQUENCE), ST_INSEQUENCE);

	StateSet aSet;
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_ANNOTATION));
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_ELEMENT));
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_GROUP));
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_CHOICE));
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_SEQUENCE));
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_ANY));

	StateInfo info(ST_INSEQUENCE, &XSDContentHandler::stateSequenceStart, &XSDContentHandler::stateSequenceEnd, aSet);
	_stateInfos[ST_INSEQUENCE] = info;
}


void StateMachine::defineStateSimpleContent()
{
	poco_assert_dbg(_states.find(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_SIMPLECONTENT)) == _states.end());

	//  Content: (annotation?, (restriction | extension))
	
	_states.try_emplace(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_SIMPLECONTENT), ST_INSIMPLECONTENT);

	StateSet aSet;
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_ANNOTATION));
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_RESTRICTION));
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_EXTENSION));

	StateInfo info(ST_INSIMPLECONTENT, &XSDContentHandler::stateSimpleContentStart, &XSDContentHandler::stateSimpleContentEnd, aSet);
	_stateInfos[ST_INSIMPLECONTENT] = info;
}


void StateMachine::defineStateSimpleType()
{
	poco_assert_dbg(_states.find(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_SIMPLETYPE)) == _states.end());

	//  Content: (annotation?, (restriction | list | union))
	
	_states.try_emplace(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_SIMPLETYPE), ST_INSIMPLETYPE);

	StateSet aSet;
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_ANNOTATION));
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_RESTRICTION));
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_LIST));
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_UNION));

	StateInfo info(ST_INSIMPLETYPE, &XSDContentHandler::stateSimpleTypeStart, &XSDContentHandler::stateSimpleTypeEnd, aSet);
	_stateInfos[ST_INSIMPLETYPE] = info;
}


void StateMachine::defineStateUnion()
{
	poco_assert_dbg(_states.find(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_UNION)) == _states.end());

	//  Content: (annotation?, simpleType*)
	
	_states.try_emplace(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_UNION), ST_INUNION);

	StateSet aSet;
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_ANNOTATION));
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_SIMPLETYPE));

	StateInfo info(ST_INUNION, &XSDContentHandler::stateUnionStart, &XSDContentHandler::stateUnionEnd, aSet);
	_stateInfos[ST_INUNION] = info;
}


void StateMachine::defineStateUnique()
{
	poco_assert_dbg(_states.find(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_UNIQUE)) == _states.end());

	//  Content: (annotation?, (selector, field+))
	
	_states.try_emplace(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_UNIQUE), ST_INUNIQUE);

	StateSet aSet;
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_ANNOTATION));
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_SELECTOR));
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_FIELD));

	StateInfo info(ST_INUNIQUE, &XSDContentHandler::stateUniqueStart, &XSDContentHandler::stateUniqueEnd, aSet);
	_stateInfos[ST_INUNIQUE] = info;
}


void StateMachine::defineStateEnumeration()
{
	poco_assert_dbg(_states.find(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_ENUMERATION)) == _states.end());

	//  Content: (annotation?)
	
	_states.try_emplace(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_ENUMERATION), ST_INENUMERATION);

	StateSet aSet;
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_ANNOTATION));

	StateInfo info(ST_INENUMERATION, &XSDContentHandler::stateEnumerationStart, &XSDContentHandler::stateEnumerationEnd, aSet);
	_stateInfos[ST_INENUMERATION] = info;
}


void StateMachine::defineStateFractionDigits()
{
	poco_assert_dbg(_states.find(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_FRACTIONDIGITS)) == _states.end());

	//  Content: (annotation?)
	
	_states.try_emplace(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_FRACTIONDIGITS), ST_INFRACTIONDIGITS);

	StateSet aSet;
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_ANNOTATION));

	StateInfo info(ST_INFRACTIONDIGITS, &XSDContentHandler::stateFractionDigitsStart, &XSDContentHandler::stateFractionDigitsEnd, aSet);
	_stateInfos[ST_INFRACTIONDIGITS] = info;
}


void StateMachine::defineStateLength()
{
	poco_assert_dbg(_states.find(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_LENGTH)) == _states.end());

	//  Content: (annotation?)
	
	_states.try_emplace(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_LENGTH), ST_INLENGTH);

	StateSet aSet;
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_ANNOTATION));

	StateInfo info(ST_INLENGTH, &XSDContentHandler::stateLengthStart, &XSDContentHandler::stateLengthEnd, aSet);
	_stateInfos[ST_INLENGTH] = info;
}


void StateMachine::defineStateMaxExclusive()
{
	poco_assert_dbg(_states.find(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_MAXEXCLUSIVE)) == _states.end());

	//  Content: (annotation?)
	
	_states.try_emplace(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_MAXEXCLUSIVE), ST_INMAXEXCLUSIVE);

	StateSet aSet;
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_ANNOTATION));

	StateInfo info(ST_INMAXEXCLUSIVE, &XSDContentHandler::stateMaxExclusiveStart, &XSDContentHandler::stateMaxExclusiveEnd, aSet);
	_stateInfos[ST_INMAXEXCLUSIVE] = info;
}


void StateMachine::defineStateMaxInclusive()
{
	poco_assert_dbg(_states.find(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_MAXINCLUSIVE)) == _states.end());

	//  Content: (annotation?)
	
	_states.try_emplace(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_MAXINCLUSIVE), ST_INMAXINCLUSIVE);

	StateSet aSet;
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_ANNOTATION));

	StateInfo info(ST_INMAXINCLUSIVE, &XSDContentHandler::stateMaxInclusiveStart, &XSDContentHandler::stateMaxInclusiveEnd, aSet);
	_stateInfos[ST_INMAXINCLUSIVE] = info;
}


void StateMachine::defineStateMaxLength()
{
	poco_assert_dbg(_states.find(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_MAXLENGTH)) == _states.end());

	//  Content: (annotation?)
	
	_states.try_emplace(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_MAXLENGTH), ST_INMAXLENGTH);

	StateSet aSet;
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_ANNOTATION));

	StateInfo info(ST_INMAXLENGTH, &XSDContentHandler::stateMaxLengthStart, &XSDContentHandler::stateMaxLengthEnd, aSet);
	_stateInfos[ST_INMAXLENGTH] = info;
}


void StateMachine::defineStateMinExclusive()
{
	poco_assert_dbg(_states.find(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_MINEXCLUSIVE)) == _states.end());

	//  Content: (annotation?)
	
	_states.try_emplace(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_MINEXCLUSIVE), ST_INMINEXCLUSIVE);

	StateSet aSet;
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_ANNOTATION));

	StateInfo info(ST_INMINEXCLUSIVE, &XSDContentHandler::stateMinExclusiveStart, &XSDContentHandler::stateMinExclusiveEnd, aSet);
	_stateInfos[ST_INMINEXCLUSIVE] = info;
}


void StateMachine::defineStateMinInclusive()
{
	poco_assert_dbg(_states.find(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_MININCLUSIVE)) == _states.end());

	//  Content: (annotation?)
	
	_states.try_emplace(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_MININCLUSIVE), ST_INMININCLUSIVE);

	StateSet aSet;
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_ANNOTATION));

	StateInfo info(ST_INMININCLUSIVE, &XSDContentHandler::stateMinInclusiveStart, &XSDContentHandler::stateMinInclusiveEnd, aSet);
	_stateInfos[ST_INMININCLUSIVE] = info;
}


void StateMachine::defineStateMinLength()
{
	poco_assert_dbg(_states.find(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_MINLENGTH)) == _states.end());

	//  Content: (annotation?)
	
	_states.try_emplace(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_MINLENGTH), ST_INMINLENGTH);

	StateSet aSet;
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_ANNOTATION));

	StateInfo info(ST_INMINLENGTH, &XSDContentHandler::stateMinLengthStart, &XSDContentHandler::stateMinLengthEnd, aSet);
	_stateInfos[ST_INMINLENGTH] = info;
}


void StateMachine::defineStatePattern()
{
	poco_assert_dbg(_states.find(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_PATTERN)) == _states.end());

	//  Content: (annotation?)
	
	_states.try_emplace(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_PATTERN), ST_INPATTERN);

	StateSet aSet;
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_ANNOTATION));

	StateInfo info(ST_INPATTERN, &XSDContentHandler::statePatternStart, &XSDContentHandler::statePatternEnd, aSet);
	_stateInfos[ST_INPATTERN] = info;
}


void StateMachine::defineStateTotalDigits()
{
	poco_assert_dbg(_states.find(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_TOTALDIGITS)) == _states.end());

	//  Content: (annotation?)
	
	_states.try_emplace(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_TOTALDIGITS), ST_INTOTALDIGITS);

	StateSet aSet;
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_ANNOTATION));

	StateInfo info(ST_INTOTALDIGITS, &XSDContentHandler::stateTotalDigitsStart, &XSDContentHandler::stateTotalDigitsEnd, aSet);
	_stateInfos[ST_INTOTALDIGITS] = info;
}


void StateMachine::defineStateUninitialized()
{
	//no content: illegal state
	StateSet aSet;
	StateInfo info(ST_INUNINITIALIZED, &XSDContentHandler::stateUninitializedStart, &XSDContentHandler::stateUninitializedEnd, aSet);

	_stateInfos[ST_INUNINITIALIZED] = info;
}


void StateMachine::defineStateWhiteSpace()
{
	poco_assert_dbg(_states.find(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_WHITESPACE)) == _states.end());

	//  Content: (annotation?)
	
	_states.try_emplace(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_WHITESPACE), ST_INWHITESPACE);

	StateSet aSet;
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_ANNOTATION));

	StateInfo info(ST_INWHITESPACE, &XSDContentHandler::stateWhiteSpaceStart, &XSDContentHandler::stateWhiteSpaceEnd, aSet);
	_stateInfos[ST_INWHITESPACE] = info;
}



void StateMachine::defineStateMetaAny()
{
	poco_assert_dbg(_states.find(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_METAANY)) == _states.end());

	// Content: (annotation?)
	
	_states.try_emplace(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_METAANY), ST_INMETAANY);

	StateSet aSet;
	
	StateInfo info(ST_INMETAANY, &XSDContentHandler::stateMetaAnyStart, &XSDContentHandler::stateMetaAnyEnd, aSet);
	_stateInfos [ST_INMETAANY] = info;
}


void StateMachine::defineStateDefinitions()
{
	poco_assert_dbg(_states.find(cat(Constants::WSDL_NAMESPACE_URI, Constants::WSDL_DEFINITIONS)) == _states.end());

	_states.try_emplace(cat(Constants::WSDL_NAMESPACE_URI, Constants::WSDL_DEFINITIONS), ST_INDEFINITIONS);

	StateSet aSet;
	aSet.insert(cat(Constants::WSDL_NAMESPACE_URI, Constants::WSDL_IMPORT));
	aSet.insert(cat(Constants::WSDL_NAMESPACE_URI, Constants::WSDL_TYPES));
	aSet.insert(cat(Constants::WSDL_NAMESPACE_URI, Constants::WSDL_MESSAGE));
	aSet.insert(cat(Constants::WSDL_NAMESPACE_URI, Constants::WSDL_PORTTYPE));
	aSet.insert(cat(Constants::WSDL_NAMESPACE_URI, Constants::WSDL_BINDING));
	aSet.insert(cat(Constants::WSDL_NAMESPACE_URI, Constants::WSDL_SERVICE));
	aSet.insert(cat(Constants::WSDL_NAMESPACE_URI, Constants::WSDL_DOCUMENTATION));
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_DOCUMENTATION)); // some WSDLs use xsd:documentation instead of wsdl:documentation
	
	StateInfo info(ST_INDEFINITIONS, &XSDContentHandler::stateDefinitionsStart, &XSDContentHandler::stateDefinitionsEnd, aSet);
	_stateInfos[ST_INDEFINITIONS] = info;
}


void StateMachine::defineStateWSDLImport()
{
	poco_assert_dbg(_states.find(cat(Constants::WSDL_NAMESPACE_URI, Constants::WSDL_IMPORT)) == _states.end());

	//  Content: (annotation?)

	_states.try_emplace(cat(Constants::WSDL_NAMESPACE_URI, Constants::WSDL_IMPORT), ST_INWSDLIMPORT);

	StateSet aSet;
	StateInfo info(ST_INWSDLIMPORT, &XSDContentHandler::stateWSDLImportStart, &XSDContentHandler::stateWSDLImportEnd, aSet);
	_stateInfos[ST_INWSDLIMPORT] = info;
}


void StateMachine::defineStateTypes()
{
	poco_assert_dbg(_states.find(cat(Constants::WSDL_NAMESPACE_URI, Constants::WSDL_TYPES)) == _states.end());

	_states.try_emplace(cat(Constants::WSDL_NAMESPACE_URI, Constants::WSDL_TYPES), ST_INTYPES);

	StateSet aSet;
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_SCHEMA));
	aSet.insert(cat(Constants::WSDL_NAMESPACE_URI, Constants::WSDL_DOCUMENTATION));
	
	StateInfo info(ST_INTYPES, &XSDContentHandler::stateTypesStart, &XSDContentHandler::stateTypesEnd, aSet);
	_stateInfos[ST_INTYPES] = info;
}


void StateMachine::defineStateMessage()
{
	poco_assert_dbg(_states.find(cat(Constants::WSDL_NAMESPACE_URI, Constants::WSDL_MESSAGE)) == _states.end());

	_states.try_emplace(cat(Constants::WSDL_NAMESPACE_URI, Constants::WSDL_MESSAGE), ST_INMESSAGE);

	StateSet aSet;
	aSet.insert(cat(Constants::WSDL_NAMESPACE_URI, Constants::WSDL_PART));
	aSet.insert(cat(Constants::WSDL_NAMESPACE_URI, Constants::WSDL_DOCUMENTATION));
	
	StateInfo info(ST_INMESSAGE, &XSDContentHandler::stateMessageStart, &XSDContentHandler::stateMessageEnd, aSet);
	_stateInfos[ST_INMESSAGE] = info;
}


void StateMachine::defineStatePart()
{
	poco_assert_dbg(_states.find(cat(Constants::WSDL_NAMESPACE_URI, Constants::WSDL_PART)) == _states.end());

	_states.try_emplace(cat(Constants::WSDL_NAMESPACE_URI, Constants::WSDL_PART), ST_INPART);

	StateSet aSet;
	aSet.insert(cat(Constants::WSDL_NAMESPACE_URI, Constants::WSDL_DOCUMENTATION));
	
	StateInfo info(ST_INPART, &XSDContentHandler::statePartStart, &XSDContentHandler::statePartEnd, aSet);
	_stateInfos[ST_INPART] = info;
}


void StateMachine::defineStatePortType()
{
	poco_assert_dbg(_states.find(cat(Constants::WSDL_NAMESPACE_URI, Constants::WSDL_PORTTYPE)) == _states.end());

	_states.try_emplace(cat(Constants::WSDL_NAMESPACE_URI, Constants::WSDL_PORTTYPE), ST_INPORTTYPE);

	StateSet aSet;
	aSet.insert(cat(Constants::WSDL_NAMESPACE_URI, Constants::WSDL_OPERATION));
	aSet.insert(cat(Constants::WSDL_NAMESPACE_URI, Constants::WSDL_DOCUMENTATION));
	
	StateInfo info(ST_INPORTTYPE, &XSDContentHandler::statePortTypeStart, &XSDContentHandler::statePortTypeEnd, aSet);
	_stateInfos[ST_INPORTTYPE] = info;
}


void StateMachine::defineStateOperation()
{
	poco_assert_dbg(_states.find(cat(Constants::WSDL_NAMESPACE_URI, Constants::WSDL_OPERATION)) == _states.end());

	_states.try_emplace(cat(Constants::WSDL_NAMESPACE_URI, Constants::WSDL_OPERATION), ST_INOPERATION);

	StateSet aSet;
	aSet.insert(cat(Constants::WSDL_NAMESPACE_URI, Constants::WSDL_DOCUMENTATION));
	aSet.insert(cat(Constants::WSDL_NAMESPACE_URI, Constants::WSDL_INPUT));
	aSet.insert(cat(Constants::WSDL_NAMESPACE_URI, Constants::WSDL_OUTPUT));
	aSet.insert(cat(Constants::WSDL_NAMESPACE_URI, Constants::WSDL_FAULT));
	aSet.insert(cat(Constants::SOAP_NAMESPACE_URI, Constants::SOAP_OPERATION));
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_METAANY));
	
	StateInfo info(ST_INOPERATION, &XSDContentHandler::stateOperationStart, &XSDContentHandler::stateOperationEnd, aSet);
	_stateInfos[ST_INOPERATION] = info;
}


void StateMachine::defineStateWSDLDocumentation()
{
	poco_assert_dbg(_states.find(cat(Constants::WSDL_NAMESPACE_URI, Constants::WSDL_DOCUMENTATION)) == _states.end());

	//  Content: ({any})*
	
	_states.try_emplace(cat(Constants::WSDL_NAMESPACE_URI, Constants::WSDL_DOCUMENTATION), ST_INWSDLDOCUMENTATION);

	StateSet aSet;
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_METAANY));

	StateInfo info(ST_INWSDLDOCUMENTATION, &XSDContentHandler::stateWSDLDocumentationStart, &XSDContentHandler::stateWSDLDocumentationEnd, aSet);
	_stateInfos[ST_INWSDLDOCUMENTATION] = info;
}


void StateMachine::defineStateInput()
{
	poco_assert_dbg(_states.find(cat(Constants::WSDL_NAMESPACE_URI, Constants::WSDL_INPUT)) == _states.end());

	_states.try_emplace(cat(Constants::WSDL_NAMESPACE_URI, Constants::WSDL_INPUT), ST_ININPUT);

	StateSet aSet;
	aSet.insert(cat(Constants::SOAP_NAMESPACE_URI, Constants::SOAP_HEADER));
	aSet.insert(cat(Constants::SOAP_NAMESPACE_URI, Constants::SOAP_BODY));
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_METAANY));
	aSet.insert(cat(Constants::WSDL_NAMESPACE_URI, Constants::WSDL_DOCUMENTATION));
	
	StateInfo info(ST_ININPUT, &XSDContentHandler::stateInputStart, &XSDContentHandler::stateInputEnd, aSet);
	_stateInfos[ST_ININPUT] = info;
}


void StateMachine::defineStateOutput()
{
	poco_assert_dbg(_states.find(cat(Constants::WSDL_NAMESPACE_URI, Constants::WSDL_OUTPUT)) == _states.end());

	_states.try_emplace(cat(Constants::WSDL_NAMESPACE_URI, Constants::WSDL_OUTPUT), ST_INOUTPUT);

	StateSet aSet;
	aSet.insert(cat(Constants::SOAP_NAMESPACE_URI, Constants::SOAP_HEADER));
	aSet.insert(cat(Constants::SOAP_NAMESPACE_URI, Constants::SOAP_BODY));
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_METAANY));
	aSet.insert(cat(Constants::WSDL_NAMESPACE_URI, Constants::WSDL_DOCUMENTATION));
	
	StateInfo info(ST_INOUTPUT, &XSDContentHandler::stateOutputStart, &XSDContentHandler::stateOutputEnd, aSet);
	_stateInfos[ST_INOUTPUT] = info;
}


void StateMachine::defineStateFault()
{
	poco_assert_dbg(_states.find(cat(Constants::WSDL_NAMESPACE_URI, Constants::WSDL_FAULT)) == _states.end());

	_states.try_emplace(cat(Constants::WSDL_NAMESPACE_URI, Constants::WSDL_FAULT), ST_INFAULT);

	StateSet aSet;
	aSet.insert(cat(Constants::SOAP_NAMESPACE_URI, Constants::SOAP_FAULT));
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_METAANY));
	aSet.insert(cat(Constants::WSDL_NAMESPACE_URI, Constants::WSDL_DOCUMENTATION));
	
	StateInfo info(ST_INFAULT, &XSDContentHandler::stateFaultStart, &XSDContentHandler::stateFaultEnd, aSet);
	_stateInfos[ST_INFAULT] = info;
}


void StateMachine::defineStateBinding()
{
	poco_assert_dbg(_states.find(cat(Constants::WSDL_NAMESPACE_URI, Constants::WSDL_BINDING)) == _states.end());

	_states.try_emplace(cat(Constants::WSDL_NAMESPACE_URI, Constants::WSDL_BINDING), ST_INBINDING);

	StateSet aSet;
	aSet.insert(cat(Constants::WSDL_NAMESPACE_URI, Constants::WSDL_OPERATION));
	aSet.insert(cat(Constants::WSDL_NAMESPACE_URI, Constants::WSDL_DOCUMENTATION));
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_METAANY));
	
	StateInfo info(ST_INBINDING, &XSDContentHandler::stateBindingStart, &XSDContentHandler::stateBindingEnd, aSet);
	_stateInfos[ST_INBINDING] = info;
}


void StateMachine::defineStateSoapBinding()
{
	poco_assert_dbg(_states.find(cat(Constants::SOAP_NAMESPACE_URI, Constants::SOAP_BINDING)) == _states.end());

	_states.try_emplace(cat(Constants::SOAP_NAMESPACE_URI, Constants::SOAP_BINDING), ST_INSOAPBINDING);

	StateSet aSet;
	aSet.insert(cat(Constants::WSDL_NAMESPACE_URI, Constants::WSDL_DOCUMENTATION));
	
	StateInfo info(ST_INSOAPBINDING, &XSDContentHandler::stateSoapBindingStart, &XSDContentHandler::stateSoapBindingEnd, aSet);
	_stateInfos[ST_INSOAPBINDING] = info;
}


void StateMachine::defineStateSoapOperation()
{
	poco_assert_dbg(_states.find(cat(Constants::SOAP_NAMESPACE_URI, Constants::SOAP_OPERATION)) == _states.end());

	_states.try_emplace(cat(Constants::SOAP_NAMESPACE_URI, Constants::SOAP_OPERATION), ST_INSOAPOPERATION);

	StateSet aSet;
	aSet.insert(cat(Constants::WSDL_NAMESPACE_URI, Constants::WSDL_DOCUMENTATION));
	
	StateInfo info(ST_INSOAPOPERATION, &XSDContentHandler::stateSoapOperationStart, &XSDContentHandler::stateSoapOperationEnd, aSet);
	_stateInfos[ST_INSOAPOPERATION] = info;
}


void StateMachine::defineStateSoapHeader()
{
	poco_assert_dbg(_states.find(cat(Constants::SOAP_NAMESPACE_URI, Constants::SOAP_HEADER)) == _states.end());

	_states.try_emplace(cat(Constants::SOAP_NAMESPACE_URI, Constants::SOAP_HEADER), ST_INSOAPHEADER);

	StateSet aSet;
	aSet.insert(cat(Constants::SOAP_NAMESPACE_URI, Constants::SOAP_HEADERFAULT));
	aSet.insert(cat(Constants::WSDL_NAMESPACE_URI, Constants::WSDL_DOCUMENTATION));
	
	StateInfo info(ST_INSOAPHEADER, &XSDContentHandler::stateSoapHeaderStart, &XSDContentHandler::stateSoapHeaderEnd, aSet);
	_stateInfos[ST_INSOAPHEADER] = info;
}


void StateMachine::defineStateSoapHeaderFault()
{
	poco_assert_dbg(_states.find(cat(Constants::SOAP_NAMESPACE_URI, Constants::SOAP_HEADERFAULT)) == _states.end());

	_states.try_emplace(cat(Constants::SOAP_NAMESPACE_URI, Constants::SOAP_HEADERFAULT), ST_INSOAPHEADERFAULT);

	StateSet aSet;
	aSet.insert(cat(Constants::WSDL_NAMESPACE_URI, Constants::WSDL_DOCUMENTATION));
	
	StateInfo info(ST_INSOAPHEADERFAULT, &XSDContentHandler::stateSoapHeaderFaultStart, &XSDContentHandler::stateSoapHeaderFaultEnd, aSet);
	_stateInfos[ST_INSOAPHEADERFAULT] = info;
}


void StateMachine::defineStateSoapBody()
{
	poco_assert_dbg(_states.find(cat(Constants::SOAP_NAMESPACE_URI, Constants::SOAP_BODY)) == _states.end());

	_states.try_emplace(cat(Constants::SOAP_NAMESPACE_URI, Constants::SOAP_BODY), ST_INSOAPBODY);

	StateSet aSet;
	aSet.insert(cat(Constants::WSDL_NAMESPACE_URI, Constants::WSDL_DOCUMENTATION));
	
	StateInfo info(ST_INSOAPBODY, &XSDContentHandler::stateSoapBodyStart, &XSDContentHandler::stateSoapBodyEnd, aSet);
	_stateInfos[ST_INSOAPBODY] = info;
}


void StateMachine::defineStateSoapFault()
{
	poco_assert_dbg(_states.find(cat(Constants::SOAP_NAMESPACE_URI, Constants::SOAP_FAULT)) == _states.end());

	_states.try_emplace(cat(Constants::SOAP_NAMESPACE_URI, Constants::SOAP_FAULT), ST_INSOAPFAULT);

	StateSet aSet;
	aSet.insert(cat(Constants::WSDL_NAMESPACE_URI, Constants::WSDL_DOCUMENTATION));
	
	StateInfo info(ST_INSOAPFAULT, &XSDContentHandler::stateSoapFaultStart, &XSDContentHandler::stateSoapFaultEnd, aSet);
	_stateInfos[ST_INSOAPFAULT] = info;
}


void StateMachine::defineStateSoap12Binding()
{
	poco_assert_dbg(_states.find(cat(Constants::SOAP12_NAMESPACE_URI, Constants::SOAP_BINDING)) == _states.end());

	_states.try_emplace(cat(Constants::SOAP12_NAMESPACE_URI, Constants::SOAP_BINDING), ST_INSOAP12BINDING);

	StateSet aSet;
	aSet.insert(cat(Constants::WSDL_NAMESPACE_URI, Constants::WSDL_DOCUMENTATION));
	
	StateInfo info(ST_INSOAP12BINDING, &XSDContentHandler::stateSoapBindingStart, &XSDContentHandler::stateSoapBindingEnd, aSet);
	_stateInfos[ST_INSOAP12BINDING] = info;
}


void StateMachine::defineStateSoap12Operation()
{
	poco_assert_dbg(_states.find(cat(Constants::SOAP12_NAMESPACE_URI, Constants::SOAP_OPERATION)) == _states.end());

	_states.try_emplace(cat(Constants::SOAP12_NAMESPACE_URI, Constants::SOAP_OPERATION), ST_INSOAP12OPERATION);

	StateSet aSet;
	aSet.insert(cat(Constants::WSDL_NAMESPACE_URI, Constants::WSDL_DOCUMENTATION));
	
	StateInfo info(ST_INSOAP12OPERATION, &XSDContentHandler::stateSoapOperationStart, &XSDContentHandler::stateSoapOperationEnd, aSet);
	_stateInfos[ST_INSOAP12OPERATION] = info;
}


void StateMachine::defineStateSoap12Header()
{
	poco_assert_dbg(_states.find(cat(Constants::SOAP12_NAMESPACE_URI, Constants::SOAP_HEADER)) == _states.end());

	_states.try_emplace(cat(Constants::SOAP12_NAMESPACE_URI, Constants::SOAP_HEADER), ST_INSOAPHEADER);

	StateSet aSet;
	aSet.insert(cat(Constants::SOAP12_NAMESPACE_URI, Constants::SOAP_HEADERFAULT));
	aSet.insert(cat(Constants::WSDL_NAMESPACE_URI, Constants::WSDL_DOCUMENTATION));
	
	StateInfo info(ST_INSOAP12HEADER, &XSDContentHandler::stateSoapHeaderStart, &XSDContentHandler::stateSoapHeaderEnd, aSet);
	_stateInfos[ST_INSOAP12HEADER] = info;
}


void StateMachine::defineStateSoap12HeaderFault()
{
	poco_assert_dbg(_states.find(cat(Constants::SOAP12_NAMESPACE_URI, Constants::SOAP_HEADERFAULT)) == _states.end());

	_states.try_emplace(cat(Constants::SOAP12_NAMESPACE_URI, Constants::SOAP_HEADERFAULT), ST_INSOAP12HEADERFAULT);

	StateSet aSet;
	aSet.insert(cat(Constants::WSDL_NAMESPACE_URI, Constants::WSDL_DOCUMENTATION));
	
	StateInfo info(ST_INSOAP12HEADERFAULT, &XSDContentHandler::stateSoapHeaderFaultStart, &XSDContentHandler::stateSoapHeaderFaultEnd, aSet);
	_stateInfos[ST_INSOAP12HEADERFAULT] = info;
}


void StateMachine::defineStateSoap12Body()
{
	poco_assert_dbg(_states.find(cat(Constants::SOAP12_NAMESPACE_URI, Constants::SOAP_BODY)) == _states.end());

	_states.try_emplace(cat(Constants::SOAP12_NAMESPACE_URI, Constants::SOAP_BODY), ST_INSOAP12BODY);

	StateSet aSet;
	aSet.insert(cat(Constants::WSDL_NAMESPACE_URI, Constants::WSDL_DOCUMENTATION));
	
	StateInfo info(ST_INSOAP12BODY, &XSDContentHandler::stateSoapBodyStart, &XSDContentHandler::stateSoapBodyEnd, aSet);
	_stateInfos[ST_INSOAP12BODY] = info;
}


void StateMachine::defineStateSoap12Fault()
{
	poco_assert_dbg(_states.find(cat(Constants::SOAP12_NAMESPACE_URI, Constants::SOAP_FAULT)) == _states.end());

	_states.try_emplace(cat(Constants::SOAP12_NAMESPACE_URI, Constants::SOAP_FAULT), ST_INSOAP12FAULT);

	StateSet aSet;
	aSet.insert(cat(Constants::WSDL_NAMESPACE_URI, Constants::WSDL_DOCUMENTATION));
	
	StateInfo info(ST_INSOAP12FAULT, &XSDContentHandler::stateSoapFaultStart, &XSDContentHandler::stateSoapFaultEnd, aSet);
	_stateInfos[ST_INSOAP12FAULT] = info;
}


void StateMachine::defineStateService()
{
	poco_assert_dbg(_states.find(cat(Constants::WSDL_NAMESPACE_URI, Constants::WSDL_SERVICE)) == _states.end());

	_states.try_emplace(cat(Constants::WSDL_NAMESPACE_URI, Constants::WSDL_SERVICE), ST_INSERVICE);

	StateSet aSet;
	aSet.insert(cat(Constants::WSDL_NAMESPACE_URI, Constants::WSDL_PORT));
	aSet.insert(cat(Constants::WSDL_NAMESPACE_URI, Constants::WSDL_DOCUMENTATION));
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_METAANY));
	
	StateInfo info(ST_INSERVICE, &XSDContentHandler::stateServiceStart, &XSDContentHandler::stateServiceEnd, aSet);
	_stateInfos[ST_INSERVICE] = info;
}


void StateMachine::defineStatePort()
{
	poco_assert_dbg(_states.find(cat(Constants::WSDL_NAMESPACE_URI, Constants::WSDL_PORT)) == _states.end());

	_states.try_emplace(cat(Constants::WSDL_NAMESPACE_URI, Constants::WSDL_PORT), ST_INPORT);

	StateSet aSet;
	aSet.insert(cat(Constants::SOAP_NAMESPACE_URI, Constants::SOAP_ADDRESS));
	aSet.insert(cat(Constants::WSDL_NAMESPACE_URI, Constants::WSDL_DOCUMENTATION));
	aSet.insert(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_METAANY));
	
	StateInfo info(ST_INPORT, &XSDContentHandler::statePortStart, &XSDContentHandler::statePortEnd, aSet);
	_stateInfos[ST_INPORT] = info;
}


void StateMachine::defineStateSoapAddress()
{
	poco_assert_dbg(_states.find(cat(Constants::SOAP_NAMESPACE_URI, Constants::SOAP_ADDRESS)) == _states.end());

	_states.try_emplace(cat(Constants::SOAP_NAMESPACE_URI, Constants::SOAP_ADDRESS), ST_INSOAPADDRESS);

	StateSet aSet;
	aSet.insert(cat(Constants::WSDL_NAMESPACE_URI, Constants::WSDL_DOCUMENTATION));
	
	StateInfo info(ST_INSOAPADDRESS, &XSDContentHandler::stateSoapAddressStart, &XSDContentHandler::stateSoapAddressEnd, aSet);
	_stateInfos[ST_INSOAPADDRESS] = info;
}


StateMachine::StateInfo::StateInfo():
	_state(ST_INUNINITIALIZED), 
	_start(&XSDContentHandler::stateUninitializedStart), 
	_end(&XSDContentHandler::stateUninitializedEnd)
{
}


StateMachine::StateInfo::StateInfo(StateMachine::State aState, StateMachine::StartMethod start, StateMachine::EndMethod end, const StateMachine::StateSet& successors):
	_state(aState), 
	_start(start), 
	_end(end), 
	_successors(successors)
{
	// check if any or anyAttribute is contained in successor, 
	// note that if the state is any or anyAttribute, they are automatically part of the successor
	if (_state == StateMachine::ST_INMETAANY || _successors.count(cat(Constants::XSD_NAMESPACE_URI, Constants::XSD_METAANY)) > 0)
	{
		_containsAnySuccessor = true;
	}
}


} // namespace Poco::XSD::Parser
