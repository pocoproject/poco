//
// PropertyFileConfiguration.cpp
//
// $Id: //poco/1.4/Util/src/PropertyFileConfiguration.cpp#1 $
//
// Library: Util
// Package: Configuration
// Module:  PropertyFileConfiguration
//
// Copyright (c) 2004-2009, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Util/PropertyFileConfiguration.h"
#include "Poco/Exception.h"
#include "Poco/String.h"
#include "Poco/Path.h"
#include "Poco/FileStream.h"
#include "Poco/LineEndingConverter.h"
#include "Poco/Ascii.h"


using Poco::trim;
using Poco::Path;


namespace Poco {
namespace Util {


PropertyFileConfiguration::PropertyFileConfiguration() : 
	_preserveComment(false)
{
}


PropertyFileConfiguration::PropertyFileConfiguration(std::istream& istr, bool preserveComment) : 
	_preserveComment(preserveComment)
{
	load(istr, preserveComment);
}

	
PropertyFileConfiguration::PropertyFileConfiguration(const std::string& path, bool preserveComment) : 
	_preserveComment(preserveComment)
{
	load(path, preserveComment);
}


PropertyFileConfiguration::~PropertyFileConfiguration()
{
}

	
void PropertyFileConfiguration::load(std::istream& istr, bool preserveComment)
{
	_preserveComment = preserveComment;
	clear();
	_fileContent.clear();
	_keyFileContentItMap.clear();

	while (!istr.eof()) parseLine(istr);
}

	
void PropertyFileConfiguration::load(const std::string& path, bool preserveComment)
{
	Poco::FileInputStream istr(path);
	if (istr.good())
		load(istr, preserveComment);
	else
		throw Poco::OpenFileException(path);
}


void PropertyFileConfiguration::save(std::ostream& ostr) const
{
	if (_preserveComment)
	{
		// Check the starting char of each line in _fileContent.
		// If the char is a comment sign, write the line out directly.
		// Otherwise, use this line as key to get the value from parent's map and write out.
		for (FileContent::const_iterator it = _fileContent.begin(); it != _fileContent.end(); ++it)
		{
			if (isComment((*it)[0])) ostr << *it;
			else outputKeyValue(ostr, *it, getString(*it));
		}
	} else
	{
		MapConfiguration::iterator it = begin();
		MapConfiguration::iterator ed = end();
		while (it != ed)
		{
			outputKeyValue(ostr, it->first, it->second);
			++it;
		}
	}
}


void PropertyFileConfiguration::save(const std::string& path) const
{
	Poco::FileOutputStream ostr(path);
	if (ostr.good())
	{
		Poco::OutputLineEndingConverter lec(ostr);
		save(lec);
		lec.flush();
		ostr.flush();
		if (!ostr.good()) throw Poco::WriteFileException(path);
	}
	else throw Poco::CreateFileException(path);
}


void PropertyFileConfiguration::parseLine(std::istream& istr)
{
	skipSpace(istr);

	if (!istr.eof())
	{
		if (isComment(istr.peek()))
		{
			// Save
			if (_preserveComment) saveComment(istr);
			else skipLine(istr);
		}
		else saveKeyValue(istr);
	}
}


void PropertyFileConfiguration::skipSpace(std::istream& istr) const
{
	while (!istr.eof() && Poco::Ascii::isSpace(istr.peek())) istr.get();
}


int PropertyFileConfiguration::readChar(std::istream& istr)
{
	for (;;)
	{
		int c = istr.get();
		if (c == '\\')
		{
			c = istr.get();
			switch (c)
			{
			case 't':
				return '\t';
			case 'r':
				return '\r';
			case 'n':
				return '\n';
			case 'f':
				return '\f';
			case '\r':
				if (istr.peek() == '\n')
					istr.get();
				continue;
			case '\n':
				continue;
			default:
				return c;
			}
		}
		else if (c == '\n' || c == '\r')
			return 0;
		else
			return c;
	}
}


void PropertyFileConfiguration::setRaw(const std::string& key, const std::string& value)
{
	MapConfiguration::setRaw(key, value);
	// Insert the key to the end of _fileContent and update _keyFileContentItMap.
	if (_preserveComment) 
	{
		FileContent::iterator fit = _fileContent.insert(_fileContent.end(), key);
		_keyFileContentItMap[key] = fit;		
	}
}


void PropertyFileConfiguration::removeRaw(const std::string& key)
{
	MapConfiguration::removeRaw(key);
	// remove the key from _fileContent and _keyFileContentItMap.
	if (_preserveComment)
	{
		_fileContent.erase(_keyFileContentItMap[key]);
		_keyFileContentItMap.erase(key);
	}
}

bool PropertyFileConfiguration::isComment(int c) const
{
	return c == '#' || c == '!';
}


void PropertyFileConfiguration::saveComment(std::istream& istr)
{
	std::string comment;

	int c = istr.get();
	while (!isNewLine(c))
	{
		comment += (char) c;
		c = istr.get();
	}
	comment += (char) c;

	_fileContent.push_back(comment);
}


void PropertyFileConfiguration::skipLine(std::istream& istr) const
{
	int c = istr.get();
	while (!isNewLine(c)) c = istr.get();
}


void PropertyFileConfiguration::saveKeyValue(std::istream& istr)
{
	int c = istr.get();

	std::string key;
	while (!isNewLine(c) && !isKeyValueSeparator(c))
	{ 
		key += (char) c;
		c = istr.get(); 
	}

	std::string value;
	if (isKeyValueSeparator(c))
	{
		c = readChar(istr);
		while (!istr.eof() && c)
		{ 
			value += (char) c; 
			c = readChar(istr);
		}
	}

	setRaw(trim(key), trim(value));
}


bool PropertyFileConfiguration::isNewLine(int c) const
{
	return c == std::char_traits<char>::eof() || c == '\n' || c == '\r';
}


void PropertyFileConfiguration::outputKeyValue(std::ostream& ostr, const std::string& key, const std::string& value) const
{
	ostr << key << ": ";

	for (std::string::const_iterator its = value.begin(); its != value.end(); ++its)
	{
		switch (*its)
		{
		case '\t':
			ostr << "\\t";
			break;
		case '\r':
			ostr << "\\r";
			break;
		case '\n':
			ostr << "\\n";
			break;
		case '\f':
			ostr << "\\f";
			break;
		case '\\':
			ostr << "\\\\";
			break;
		default:
			ostr << *its;
			break;
		}
	}
	
	ostr << "\n";
}


bool PropertyFileConfiguration::isKeyValueSeparator(int c) const
{
	return c == '=' || c == ':';
}


} } // namespace Poco::Util
