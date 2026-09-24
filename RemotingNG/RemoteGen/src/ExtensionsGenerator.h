//
// ExtensionsGenerator.h
//
// Definition of the ExtensionsGenerator class.
//
// Copyright (c) 2006-2014, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef RemoteGen_ExtensionsGenerator_INCLUDED
#define RemoteGen_ExtensionsGenerator_INCLUDED


#include "BundleActivatorGenerator.h"
#include <ostream>
#include <vector>


class ExtensionsGenerator
	/// ExtensionsGenerator generates an extensions.xml file
{
public:
	ExtensionsGenerator(std::ostream& out);
		/// Creates the ExtensionsGenerator.

	~ExtensionsGenerator();
		/// Destroys the ExtensionsGenerator.

	void generate(const std::vector<BundleActivatorGenerator::BundleService>& bundleServices);

private:
	std::ostream& _out;
};


#endif // RemoteGen_ExtensionsGenerator_INCLUDED
