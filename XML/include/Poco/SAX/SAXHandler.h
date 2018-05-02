//
// DeclHandler.h
//
// Library: XML
// Package: SAX
// Module:  SAX
//
// SAX2-ext SAXHandler Interface.
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef SAX_SAXHandler_INCLUDED
#define SAX_SAXHandler_INCLUDED


#include "Poco/XML/XML.h"
#include "Poco/RefPtr.h"
#include "Poco/DOM/RefCountedObject.h"


namespace Poco {
namespace XML {


class XML_API SAXHandler: virtual public Poco::XML::WeakRefCountedObject
	/// This class is parent for all SAX handlers.
{
public:
	typedef Poco::RefPtr<SAXHandler> Ptr;

protected:

	~SAXHandler() {}
};


} } // namespace Poco::XML


#endif // SAX_SAXHandler_INCLUDED
