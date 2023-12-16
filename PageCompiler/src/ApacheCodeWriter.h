//
// ApacheCodeWriter.h
//
// Copyright (c) 2008, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef ApacheCodeWriter_INCLUDED
#define ApacheCodeWriter_INCLUDED


#include "CodeWriter.h"


class ApacheCodeWriter: public CodeWriter
	/// Code generator for ApacheConnector request handlers.
{
public:
	ApacheCodeWriter(const Page& page, const std::string& clazz);
		/// Creates the CodeWriter, using the given Page.

	~ApacheCodeWriter() override;
		/// Destroys the PageReader.

protected:
	void writeHeaderIncludes(std::ostream& ostr) override;
	void writeFactoryClass(std::ostream& ostr) override;
	void writeImplIncludes(std::ostream& ostr) override;
	void writeFactory(std::ostream& ostr) override;
	void writeManifest(std::ostream& ostr) override;
};


#endif // CodeWriter_INCLUDED
