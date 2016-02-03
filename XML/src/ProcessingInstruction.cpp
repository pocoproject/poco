//
// ProcessingInstruction.cpp
//
// $Id: //poco/1.4/XML/src/ProcessingInstruction.cpp#1 $
//
// Library: XML
// Package: DOM
// Module:  DOM
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/DOM/ProcessingInstruction.h"


namespace Poco {
namespace XML {


ProcessingInstruction::ProcessingInstruction(Document* pOwnerDocument, const XMLString& rTarget, const XMLString& rData): 
	AbstractNode(pOwnerDocument),
	_target(rTarget),
	_data(rData)
{
}


ProcessingInstruction::ProcessingInstruction(Document* pOwnerDocument, const ProcessingInstruction& processingInstruction): 
	AbstractNode(pOwnerDocument, processingInstruction),
	_target(processingInstruction._target),
	_data(processingInstruction._data)
{
}


ProcessingInstruction::~ProcessingInstruction()
{
}


void ProcessingInstruction::setData(const XMLString& rData)
{
	_data = rData;
}


const XMLString& ProcessingInstruction::nodeName() const
{
	return _target;
}


const XMLString& ProcessingInstruction::getNodeValue() const
{
	return _data;
}


void ProcessingInstruction::setNodeValue(const XMLString& rData)
{
	setData(rData);
}


unsigned short ProcessingInstruction::nodeType() const
{
	return Node::PROCESSING_INSTRUCTION_NODE;
}


Node* ProcessingInstruction::copyNode(bool deep, Document* pOwnerDocument) const
{
	return new ProcessingInstruction(pOwnerDocument, *this);
}


} } // namespace Poco::XML
