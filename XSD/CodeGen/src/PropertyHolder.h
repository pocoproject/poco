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

	PropertyHolder(const PropertyHolder&) = default;

	PropertyHolder(PropertyHolder&&) = default;

	virtual ~PropertyHolder() = default;

	PropertyHolder& operator = (const PropertyHolder&) = default;

	PropertyHolder& operator = (PropertyHolder&&) = default;

	void insert(const std::string& id, const std::string& val);

	void update(const std::string& id, const std::string& val);

	[[nodiscard]] bool has(const std::string& id) const;

	[[nodiscard]] const std::map<std::string, std::string>& getAll() const;

	[[nodiscard]] const std::string& get(const std::string& id) const;

private:
	std::map<std::string, std::string> _props;
};


//
// inlines
//
inline void PropertyHolder::insert(const std::string& id, const std::string& val)
{
	_props.try_emplace(id, val);
}


inline void PropertyHolder::update(const std::string& id, const std::string& val)
{
	_props.insert_or_assign(id, val);
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
