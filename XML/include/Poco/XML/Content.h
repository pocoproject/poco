//
// Content.h
//
// $Id$
//
// Library: XML
// Package: XML
// Module:  Content
//
// Definition of the Content enum.
//
// Copyright (c) 2004-2015, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
// copyright : Copyright (c) 2009-2013 Code Synthesis Tools CC
// license   : MIT; see accompanying LICENSE file


#ifndef POCO_XML_CONTENT
#define POCO_XML_CONTENT


namespace Poco
{
namespace XML
{


/// XML content model. C++11 enum class emulated for C++98.
struct Content
{
	enum value
	{
		//  element   characters  whitespaces        notes
		Empty,   //    no          no        ignored
		Simple,  //    no          yes       preserved   content accumulated
		Complex, //    yes         no        ignored
		Mixed    //    yes         yes       preserved
	};

	Content(value v)
		: v_(v)
	{
	}

	operator value() const
	{
		return v_;
	}

private:
	value v_;
};


}
}

#endif // XML_CONTENT
