//
// RefCountedObject.h
//
// Library: XML
// Package: DOM
// Module:  DOM
//
// Import Poco::RefCountedObject into the XML namespace.
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef DOM_DOMRefCountedObject_INCLUDED
#define DOM_DOMRefCountedObject_INCLUDED


#include "Poco/XML/XML.h"
#include "Poco/Mutex.h"
#include "Poco/RefCountedObject.h"


namespace Poco {
namespace XML {


typedef Poco::WeakRefCountedObject WeakRefCountedObject;


} } // namespace Poco::XML


#endif // DOM_DOMRefCountedObject_INCLUDED
