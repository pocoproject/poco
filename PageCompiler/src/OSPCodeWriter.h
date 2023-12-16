//
// OSPCodeWriter.h
//
// Copyright (c) 2008, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef OSPCodeWriter_INCLUDED
#define OSPCodeWriter_INCLUDED


#include "CodeWriter.h"


class OSPCodeWriter: public CodeWriter
	/// Code generator for OSP Web request handlers.
{
public:
	OSPCodeWriter(const Page& page, const std::string& clazz);
		/// Creates the CodeWriter, using the given Page.

	~OSPCodeWriter() override;
		/// Destroys the PageReader.

protected:
	void writeHeaderIncludes(std::ostream& ostr) override;
	void writeHandlerClass(std::ostream& ostr) override;
	void writeHandlerMembers(std::ostream& ostr) override;
	void writeFactoryClass(std::ostream& ostr) override;
	void writeImplIncludes(std::ostream& ostr) override;
	void writeConstructor(std::ostream& ostr) override;
	void writeFactory(std::ostream& ostr) override;
	void writeSession(std::ostream& ostr) override;
};


#endif // CodeWriter_INCLUDED
