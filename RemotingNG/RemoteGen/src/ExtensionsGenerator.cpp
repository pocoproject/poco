//
// ExtensionsGenerator.cpp
//
// Copyright (c) 2006-2014, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "ExtensionsGenerator.h"


ExtensionsGenerator::ExtensionsGenerator(std::ostream& out): 
	_out(out)
{
}


ExtensionsGenerator::~ExtensionsGenerator()
{
}


void ExtensionsGenerator::generate(const std::vector<BundleActivatorGenerator::BundleService>& bundleServices)
{
	_out << "<extensions>" << std::endl;
	_out << "\t<!--<extension point=\"com.appinf.osp.remoting.service\" serviceName=\"<OSP service name>\" ";
	_out <<	"serviceLocation=\"<protocol>://<host>:<port>/[soap|binary]/<classname>/<objectid>\">-->" << std::endl;

	std::vector<BundleActivatorGenerator::BundleService>::const_iterator it = bundleServices.begin();
	for (; it != bundleServices.end(); ++it)
	{
		if (it->clientCode)
		{
			_out << "\t<extension point=\"com.appinf.osp.remoting.service\" serviceName=\"";
			_out <<	"com.sample." << "The" << it->pStruct->name() << "\" ";
			_out <<	"serviceLocation=\"http://localhost:22080/soap/";
			_out << it->pStruct->name();
			_out << "/The" << it->pStruct->name() << "\"/>" << std::endl;
		}
	}
	_out << "</extensions>" << std::endl;
}
