//
// StateMachine.h
//
// Library: XSD/Parser
// Package: XSDParser
// Module:  StateMachine
//
// Definition of the StateMachine class.
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties, 
// copied or duplicated in any form, in whole or in part.
//


#ifndef XSDParser_StateMachine_INCLUDED
#define XSDParser_StateMachine_INCLUDED


#include "Poco/XSD/Parser/XSDParser.h"
#include "Poco/SAX/Attributes.h"
#include "Poco/String.h"
#include <set>
#include <vector>
#include <map>


namespace Poco {
namespace XSD {
namespace Parser {


class XSDContentHandler;


class XSDParser_API StateMachine
	/// class StateMachine. Defines all possible states.
{
public:
	enum State
	{
		ST_INALL = 0,
		ST_INANNOTATION,
		ST_INANY,
		ST_INANYATTRIBUTE,
		ST_INAPPINFO,
		ST_INATTRIBUTE,
		ST_INATTRIBUTEGROUP,
		ST_INCHOICE,
		ST_INCOMPLEXTYPE,
		ST_INDOCUMENT,
		ST_INDOCUMENTATION,
		ST_INELEMENT,
		ST_INSIMPLEEXTENSION,
		ST_INCOMPLEXEXTENSION,
		ST_INFIELD,
		ST_INGROUP,
		ST_INXSDIMPORT,
		ST_ININCLUDE,
		ST_INKEY,
		ST_INKEYREF,
		ST_INLIST,
		ST_INNOTATION,
		ST_INREDEFINE,
		ST_INSIMPLETYPERESTRICTION,
		ST_INSIMPLECONTENTRESTRICTION,
		ST_INCOMPLEXRESTRICTION,
		ST_INSCHEMA,
		ST_INSELECTOR,
		ST_INSEQUENCE,
		ST_INUNION,
		ST_INUNIQUE,
		ST_INENUMERATION,
		ST_INFRACTIONDIGITS,
		ST_INLENGTH,
		ST_INMAXEXCLUSIVE,
		ST_INMAXINCLUSIVE,
		ST_INMAXLENGTH,
		ST_INMINEXCLUSIVE,
		ST_INMININCLUSIVE,
		ST_INMINLENGTH,
		ST_INPATTERN,
		ST_INTOTALDIGITS,
		ST_INUNINITIALIZED,
		ST_INWHITESPACE,
		ST_INMETAANY,
		ST_INDEFINITIONS,    // begin WSDL
		ST_INWSDLIMPORT,
		ST_INTYPES,
		ST_INMESSAGE,
		ST_INPART,
		ST_INPORTTYPE,
		ST_INOPERATION,
		ST_INWSDLDOCUMENTATION,
		ST_ININPUT,
		ST_INOUTPUT,
		ST_INFAULT,
		ST_INBINDING,
		ST_INSOAPBINDING,
		ST_INSOAPOPERATION,
		ST_INSOAPHEADER,
		ST_INSOAPHEADERFAULT,
		ST_INSOAPBODY,
		ST_INSOAPFAULT,
		ST_INSOAP12BINDING,
		ST_INSOAP12OPERATION,
		ST_INSOAP12HEADER,
		ST_INSOAP12HEADERFAULT,
		ST_INSOAP12BODY,
		ST_INSOAP12FAULT,
		ST_INSERVICE,
		ST_INPORT,
		ST_INSOAPADDRESS,    // end WSDL
		ST_INCOMPLEXCONTENT, // INCOMPLEXCONTENT must be before SimpleContent/-TYPE and at the end of the enum!
		ST_INSIMPLECONTENT,
		ST_INSIMPLETYPE,
	};

	typedef void (XSDContentHandler::*StartMethod)(const std::string&, const std::string&, const std::string&, const CompactAttributes&);
	typedef void (XSDContentHandler::*EndMethod)(const std::string&, const std::string&, const std::string&);

	using StateSet = std::set<std::string>;

	struct StateInfo
	{
		State       _state;
		StartMethod _start;
		EndMethod   _end;
		bool        _containsAnySuccessor;
		StateSet    _successors;
		
		StateInfo();
		StateInfo(State _state, StartMethod start, EndMethod end, const StateSet& successors);
		bool isValidSuccessor(const std::string& uri, const std::string& localName) const;
		bool containsAny() const;
	};


	using StateInfos = std::vector<StateInfo>;
		/// the position of the vector equals the int value of the enum State

	using StateMap = std::map<std::string, StateMachine::State>;
		/// Maps an element name to the assigned State.
		/// Due to performance reasons we define the concatenation of namespace URI and localname of the xsd element
		/// as the key.

	StateMachine();
		/// Creates the StateMachine.

	virtual ~StateMachine();
		/// Destroys the StateMachine.

	const StateMachine::StateInfo& stateInfo(StateMachine::State aState) const;

	StateMachine::State state(const std::string& uri, const std::string& elementName, StateMachine::State parent) const;
		/// Returns the state for an xml element name.

private:
	void initialize();
	void defineStateAll();
	void defineStateAnnotation();
	void defineStateAny();
	void defineStateAnyAttribute();
	void defineStateAppInfo();
	void defineStateAttribute();
	void defineStateAttributeGroup();
	void defineStateChoice();
	void defineStateComplexContent();
	void defineStateComplexType();
	void defineStateDocument();
	void defineStateDocumentation();
	void defineStateElement();
	void defineStateSimpleExtension();
	void defineStateComplexExtension();
	void defineStateField();
	void defineStateGroup();
	void defineStateXSDImport();
	void defineStateInclude();
	void defineStateKey();
	void defineStateKeyref();
	void defineStateList();
	void defineStateNotation();
	void defineStateRedefine();
	void defineStateSimpleTypeRestriction();
	void defineStateSimpleContentRestriction();
	void defineStateComplexRestriction();
	void defineStateSchema();
	void defineStateSelector();
	void defineStateSequence();
	void defineStateSimpleContent();
	void defineStateSimpleType();
	void defineStateUnion();
	void defineStateUnique();
	void defineStateEnumeration();
	void defineStateFractionDigits();
	void defineStateLength();
	void defineStateMaxExclusive();
	void defineStateMaxInclusive();
	void defineStateMaxLength();
	void defineStateMinExclusive();
	void defineStateMinInclusive();
	void defineStateMinLength();
	void defineStatePattern();
	void defineStateTotalDigits();
	void defineStateUninitialized();
	void defineStateWhiteSpace();
	void defineStateMetaAny();
	void defineStateDefinitions();
	void defineStateWSDLImport();
	void defineStateTypes();
	void defineStateMessage();
	void defineStatePart();
	void defineStatePortType();
	void defineStateOperation();
	void defineStateWSDLDocumentation();
	void defineStateInput();
	void defineStateOutput();
	void defineStateFault();
	void defineStateBinding();
	void defineStateSoapBinding();
	void defineStateSoapOperation();
	void defineStateSoapHeader();
	void defineStateSoapHeaderFault();
	void defineStateSoapBody();
	void defineStateSoapFault();
	void defineStateSoap12Binding();
	void defineStateSoap12Operation();
	void defineStateSoap12Header();
	void defineStateSoap12HeaderFault();
	void defineStateSoap12Body();
	void defineStateSoap12Fault();
	void defineStateService();
	void defineStatePort();
	void defineStateSoapAddress();

private:
	StateInfos _stateInfos;
	StateMap   _states;
};


//
// inlines
//
inline const StateMachine::StateInfo& StateMachine::stateInfo(StateMachine::State aState) const
{
	return _stateInfos[aState];
}


inline bool StateMachine::StateInfo::isValidSuccessor(const std::string& uri, const std::string& localName) const
{
	if (_containsAnySuccessor)
		return true;
	else
		return _successors.find(cat(uri, localName)) != _successors.end();
}


inline bool StateMachine::StateInfo::containsAny() const
{
	return _containsAnySuccessor;
}


} } } // namespace Poco::XSD::Parser


#endif // XSDParser_StateMachine_INCLUDED
