//
// DOMObject.h
//
// Library: XML
// Package: DOM
// Module:  DOM
//
// Definition of the DOMObject class.
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef DOM_DOMObject_INCLUDED
#define DOM_DOMObject_INCLUDED


#include "Poco/XML/XML.h"
#include "Poco/RefCountedObject.h"
#include "Poco/AutoPtr.h"


namespace Poco {
namespace XML {


class XML_API DOMObject: public RefCountedObject
	/// The base class for all objects in the Document Object Model.
	///
	/// DOMObject can not be created on the stack and every DOMObject
	/// pointer must be wrapped in AutoPtr. Using bare DOMObject
	/// pointers may result in memory leaks or dangling pointers.
	///
	/// While DOMObjects are safe for use in multithreaded programs,
	/// a DOMObject or one of its subclasses must not be accessed
	/// from multiple threads simultaneously.
{
public:
	typedef AutoPtr<DOMObject> Ptr;

	DOMObject();
		/// Creates the DOMObject.

protected:
	virtual ~DOMObject();
		/// Destroys the DOMObject.

private:
	DOMObject(const DOMObject&);
	DOMObject& operator = (const DOMObject&);
	DOMObject(DOMObject&&);
	DOMObject& operator=(DOMObject&&);
};


} } // namespace Poco::XML


#endif // DOM_DOMObject_INCLUDED
