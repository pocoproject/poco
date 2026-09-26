//
// PropertyHolder.h
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef CodeGen_PropertyHolder_H_INCLUDED
#define CodeGen_PropertyHolder_H_INCLUDED


#include "Poco/Exception.h"
#include <map>
#include <string>


class PropertyHolder
{
public:
	using Properties = std::map<std::string, std::string>;

	PropertyHolder();

	virtual ~PropertyHolder();

	void insert(const std::string& id, const std::string& val);

	void update(const std::string& id, const std::string& val);

	bool has(const std::string& id) const;

	const std::map<std::string, std::string>& getAll() const;

	const std::string& get(const std::string& id) const;

private:
	std::map<std::string, std::string> _props;
};


//
// inlines
//
inline void PropertyHolder::insert(const std::string& id, const std::string& val)
{
	_props.insert(std::make_pair(id, val));
}


inline void PropertyHolder::update(const std::string& id, const std::string& val)
{
	std::pair<Properties::iterator, bool> aPair = _props.insert(std::make_pair(id, val));
	if (!aPair.second)
		aPair.first->second = val;
}


inline bool PropertyHolder::has(const std::string& id) const
{
	return _props.find(id) != _props.end();
}


inline const std::map<std::string, std::string>& PropertyHolder::getAll() const
{
	return _props;
}


#endif // CodeGen_PropertyHolder_H_INCLUDED
