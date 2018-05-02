//
// PropertyListConfiguration.h
//
// Library: Util
// Package: Configuration
// Module:  PropertyListConfiguration
//
// Definition of the PropertyListConfiguration class.
//
// Copyright (c) 2017-2018, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Util_PropertyListConfiguration_INCLUDED
#define Util_PropertyListConfiguration_INCLUDED


#include "Poco/Util/Util.h"


#ifndef POCO_UTIL_NO_XMLCONFIGURATION


#include "Poco/Util/AbstractConfiguration.h"
#include "Poco/DOM/Document.h"
#include "Poco/DOM/AutoPtr.h"


namespace Poco {
namespace Util {


class Util_API PropertyListConfiguration: public AbstractConfiguration
{
public:
	PropertyListConfiguration();
		/// Creates an empty PropertyListConfiguration.

	PropertyListConfiguration(const std::string& path);
		/// Creates an PropertyListConfiguration and loads the XMLplist document from
		/// the given path.
	PropertyListConfiguration(std::istream& istr);
		/// Creates an PropertyFileConfiguration and loads the configuration data
		/// from the given stream, which must be in properties list format.

	void load(const std::string& path);
		/// Loads the configuration data from the given file, which
		/// must be in properties list format.

	void load(std::istream& istr);
		/// Loads the configuration data from the given stream, which
		/// must be in properties list format.

	void save(std::ostream& ostr) const;
		/// Writes the configuration data to the given stream.

	void save(const std::string& path) const;
		/// Writes the configuration data to the given file.

	virtual void setInt(const std::string& key, int value);
	virtual void setDouble(const std::string& key, double value);
	virtual void setBool(const std::string& key, bool value);

	virtual void setData(const std::string& key, std::istream &istr);
		/// set base64 data
	virtual bool getData(const std::string& key, std::ostream &ostr);
		/// get base64 data
protected:
	virtual bool getRaw(const std::string& key, std::string& value) const;
	virtual void setRaw(const std::string& key, const std::string& value);
	virtual void enumerate(const std::string& key, Keys& range) const;
	virtual void removeRaw(const std::string& key);
	~PropertyListConfiguration();

private:
	Poco::XML::Node* findNode(const std::string& key, bool create = false);
	const Poco::XML::Node* findNode(const std::string& key, bool create = false) const;
	static Poco::XML::Node* findNode(const std::string& key, Poco::XML::Node* dict, bool create = false);

	Poco::XML::AutoPtr<Poco::XML::Element> _pRoot;
	Poco::XML::AutoPtr<Poco::XML::Document> _pDocument;		
};


} } // namespace Poco::Util


#endif // POCO_UTIL_NO_XMLplistCONFIGURATION


#endif // Util_PropertyListConfiguration_INCLUDED
