//
// PropertyFileConfiguration.cpp
//
// Library: Util
// Package: Configuration
// Module:  PropertyFileConfiguration
//
// Copyright (c) 2004-2025, Applied Informatics Software Engineering GmbH.
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
#include <set>

using Poco::trim;
using Poco::Path;


namespace Poco {
namespace Util {


PropertyFileConfiguration::PropertyFileConfiguration(std::istream& istr)
{
	load(istr);
}


PropertyFileConfiguration::PropertyFileConfiguration(const std::string& path)
{
	load(path);
}


void PropertyFileConfiguration::load(std::istream& istr)
{
	AbstractConfiguration::ScopedLock lock(*this);

	clear();
	std::set<std::string> includeStack;
	loadStream(istr, "", includeStack);
}


void PropertyFileConfiguration::load(const std::string& path)
{
	Poco::Path p(path);
	p.makeAbsolute();
	std::string basePath = p.parent().toString();
	Poco::FileInputStream istr(path);
	if (!istr.good())
		throw Poco::OpenFileException(path);
	AbstractConfiguration::ScopedLock lock(*this);
	clear();
	std::set<std::string> includeStack;
	includeStack.insert(p.toString());
	loadStream(istr, basePath, includeStack);
}


void PropertyFileConfiguration::loadStream(std::istream& istr, const std::string& basePath, std::set<std::string>& includeStack)
{
	for (;;)
	{
		if (!istr.good())
		{
			if (istr.eof())
				break;
			throw Poco::IOException("Broken input stream");
		}
		parseLine(istr, basePath, includeStack);
	}
}


void PropertyFileConfiguration::save(std::ostream& ostr) const
{
	AbstractConfiguration::ScopedLock lock(*this);

	MapConfiguration::iterator it = begin();
	MapConfiguration::iterator ed = end();
	while (it != ed)
	{
		ostr << it->first << ": ";
		for (auto ch: it->second)
		{
			switch (ch)
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
				ostr << ch;
				break;
			}
		}
		ostr << "\n";
		++it;
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


void PropertyFileConfiguration::parseLine(std::istream& istr, const std::string& basePath, std::set<std::string>& includeStack)
{
	constexpr int eof = std::char_traits<char>::eof();

	int c = istr.get();
	while (c != eof && Poco::Ascii::isSpace(c)) c = istr.get();
	if (c != eof)
	{
		if (c == '#' || c == '!')
		{
			std::string line;
			line += (char) c;
			while (c != eof && c != '\n' && c != '\r') { c = istr.get(); if (c != eof && c != '\n' && c != '\r') line += (char) c; }

			static const std::string includeDirective = "!include";
			if (line.size() > includeDirective.size() &&
				line.compare(0, includeDirective.size(), includeDirective) == 0 &&
				Poco::Ascii::isSpace(line[includeDirective.size()]))
			{
				std::string includePath = Poco::trim(line.substr(includeDirective.size()));
				if (includePath.empty())
					throw Poco::SyntaxException("Missing path in !include directive");
				Poco::Path p(includePath);
				if (p.isRelative() && !basePath.empty())
					p = Poco::Path(basePath).resolve(p);

				Poco::Path absPath(p);
				absPath.makeAbsolute();
				const std::string absPathStr = absPath.toString();

				if (includeStack.find(absPathStr) != includeStack.end())
				{
					throw Poco::FileException("Cyclic property file include detected", absPathStr);
				}

				includeStack.insert(absPathStr);
				try
				{
					Poco::FileInputStream includeIstr(p.toString());
					if (!includeIstr.good())
						throw Poco::OpenFileException(p.toString());
					loadStream(includeIstr, p.parent().makeAbsolute().toString(), includeStack);
				}
				catch (...)
				{
					includeStack.erase(absPathStr);
					throw;
				}
				includeStack.erase(absPathStr);
			}
		}
		else
		{
			std::string key;
			while (c != eof && c != '=' && c != ':' && c != '\r' && c != '\n') { key += (char) c; c = istr.get(); }
			std::string value;
			if (c == '=' || c == ':')
			{
				c = readChar(istr);
				while (c != eof && c) { value += (char) c; c = readChar(istr); }
			}
			setRaw(trim(key), trim(value));
		}
	}
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


} } // namespace Poco::Util
