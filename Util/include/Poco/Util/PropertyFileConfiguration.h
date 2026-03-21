//
// PropertyFileConfiguration.h
//
// Library: Util
// Package: Configuration
// Module:  PropertyFileConfiguration
//
// Definition of the PropertyFileConfiguration class.
//
// Copyright (c) 2004-2025, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Util_PropertyFileConfiguration_INCLUDED
#define Util_PropertyFileConfiguration_INCLUDED


#include "Poco/Util/Util.h"
#include "Poco/Util/MapConfiguration.h"
#include <istream>
#include <ostream>
#include <set>


namespace Poco::Util {


class Util_API PropertyFileConfiguration: public MapConfiguration
	/// This implementation of a Configuration reads properties
	/// from a Java-style properties file.
	///
	/// The file syntax is implemented as follows.
	///   - a line starting with a hash '#' or exclamation mark '!' is treated as a comment and ignored
	///     (with the exception of the !include directive described below)
	///   - every other line denotes a property assignment in the form
	///     <key> = <value> or
	///     <key> : <value>
	///
	/// Keys and values may contain special characters represented by the following escape sequences:
	///   - \t: tab (0x09)
	///   - \n: line feed (0x0a)
	///   - \r: carriage return (0x0d)
	///   - \f: form feed (0x0c)
	///
	/// For every other sequence that starts with a backslash, the backslash is removed.
	/// Therefore, the sequence \a would just yield an 'a'.
	///
	/// A value can spread across multiple lines if the last character in a line (the character
	/// immediately before the carriage return or line feed character) is a single backslash.
	///
	/// A line of the form
	///   !include <path>
	/// (where <path> is a relative or absolute file path) includes another properties file.
	/// Relative paths are resolved relative to the directory of the including file.
	/// ${variable} references in include paths are expanded, either from
	/// properties already loaded in the same file, or from an optional parent
	/// configuration passed via the constructor.
	///
	/// Property names are case sensitive. Leading and trailing whitespace is
	/// removed from both keys and values. A property name can neither contain
	/// a colon ':' nor an equal sign '=' character.
{
public:
	PropertyFileConfiguration(AbstractConfiguration::Ptr pParentConfig = nullptr);
		/// Creates an empty PropertyFileConfiguration.
		/// If pParentConfig is not null, it is used to expand ${variable}
		/// references in !include directive paths.

	PropertyFileConfiguration(std::istream& istr, AbstractConfiguration::Ptr pParentConfig = nullptr);
		/// Creates an PropertyFileConfiguration and loads the configuration data
		/// from the given stream, which must be in properties file format.
		/// If pParentConfig is not null, it is used to expand ${variable}
		/// references in !include directive paths.

	PropertyFileConfiguration(const std::string& path, AbstractConfiguration::Ptr pParentConfig = nullptr);
		/// Creates an PropertyFileConfiguration and loads the configuration data
		/// from the given file, which must be in properties file format.
		/// If pParentConfig is not null, it is used to expand ${variable}
		/// references in !include directive paths.

	void load(std::istream& istr);
		/// Loads the configuration data from the given stream, which
		/// must be in properties file format.

	void load(const std::string& path);
		/// Loads the configuration data from the given file, which
		/// must be in properties file format.

	void save(std::ostream& ostr) const;
		/// Writes the configuration data to the given stream.
		///
		/// The data is written as a sequence of statements in the form
		/// <key>: <value>
		/// separated by a newline character.

	void save(const std::string& path) const;
		/// Writes the configuration data to the given file.

protected:
	~PropertyFileConfiguration() = default;

private:
	void loadStream(std::istream& istr, const std::string& basePath, std::set<std::string>& includeStack);
	void parseLine(std::istream& istr, const std::string& basePath, std::set<std::string>& includeStack);
	static int readChar(std::istream& istr);

	AbstractConfiguration::Ptr _pParentConfig;
};


} // namespace Poco::Util


#endif // Util_PropertyFileConfiguration_INCLUDED
