//
// OSPCommand.h
//
// $Id: //poco/1.4/OSP/Shell/include/Poco/OSP/Shell/OSPCommand.h#1 $
//
// Library: OSP/Shell
// Package: Shell
// Module:  OSPCommand
//
// Definition of the OSPCommand class.
//
// Copyright (c) 2009, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties, 
// copied or duplicated in any form, in whole or in part.
//


#ifndef OSP_Shell_OSPCommand_INCLUDED
#define OSP_Shell_OSPCommand_INCLUDED


#include "Poco/OSP/Shell/AbstractCommand.h"
#include "Poco/OSP/BundleContext.h"
#include "Poco/OSP/Bundle.h"
#include <vector>


namespace Poco {
namespace OSP {
namespace Shell {


class OSPShell_API OSPCommand: public AbstractCommand
	/// OSPCommand is the base class for all
	/// OSP-specific commands.
	///
	/// It provides various helper functions that
	/// make implementing commands that deal with OSP
	/// internals easier.
{
public:
	OSPCommand(Poco::OSP::BundleContext::Ptr pContext, const std::string& name);
		/// Creates the OSPCommand, using the given
		/// bundle context and name.
		///
		/// The name is used to load the command description
		/// and usage string from the bundle properties, using
		/// the <name>.description and <name>.usage properties.
		
	~OSPCommand();
		/// Destroys the OSPCommand.

	Poco::OSP::BundleContext::Ptr context() const;
		/// Returns the bundle context.
		
protected:
	Poco::OSP::Bundle::Ptr findBundle(const std::string& symbolicNameOrId) const;
		/// Finds a bundle, either by symbolic name or ID.
		///
		/// If the given argument is an integer, it is treated as bundle ID.
		/// Otherwise, it is treated as symbolic name.
		///
		/// Returns a pointer to the bundle that matches the symbolic name or
		/// id, or a null pointer otherwise.
		
	static bool match(const std::string& pattern, const std::string& subject);
		/// Returns true if the given subject matches the given pattern.
		///
		/// Every subject matches an empty pattern.

	static bool match(const std::vector<std::string>& patterns, const std::string& subject);
		/// Returns true if the given subject matches at least one of the given pattern.
		///
		/// Every subject matches an empty pattern.
		
private:
	Poco::OSP::BundleContext::Ptr _pContext;
};


//
// inlines
//
inline Poco::OSP::BundleContext::Ptr OSPCommand::context() const
{
	return _pContext;
}


} } } // namespace Poco::OSP::Shell


#endif // OSP_Shell_OSPCommand_INCLUDED
