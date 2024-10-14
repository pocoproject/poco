//
// Object.cpp
//
// Library: JSON
// Package: JSON
// Module:  Object
//
// Copyright (c) 2012, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/JSON/Object.h"
#include <iostream>
#include <sstream>


using Poco::Dynamic::Var;

// Explicitly instatiated shared pointer in JSON library is required to
// have known instance of the pointer to be used with VarHolder when
// compiling with -fvisibility=hidden

#if defined(POCO_OS_FAMILY_WINDOWS)
template class JSON_API Poco::SharedPtr<Poco::JSON::Object>;
#else
template class Poco::SharedPtr<Poco::JSON::Object>;
#endif

namespace Poco {

namespace JSON {


Object::Object(int options):
	_preserveInsOrder((options & Poco::JSON_PRESERVE_KEY_ORDER) != 0),
	_escapeUnicode((options & Poco::JSON_ESCAPE_UNICODE) != 0),
	_lowercaseHex((options & Poco::JSON_LOWERCASE_HEX) != 0),
	_modified(false)
{
}


Object::Object(const Object& other) : _values(other._values),
	_preserveInsOrder(other._preserveInsOrder),
	_escapeUnicode(other._escapeUnicode),
	_lowercaseHex(other._lowercaseHex),
	_pStruct(!other._modified ? other._pStruct : 0),
	_modified(other._modified)
{
	syncKeys(other._keys);
}


Object::Object(Object&& other) noexcept:
	_values(std::move(other._values)),
	_keys(std::move(other._keys)),
	_preserveInsOrder(other._preserveInsOrder),
	_escapeUnicode(other._escapeUnicode),
	_lowercaseHex(other._lowercaseHex),
	_pStruct(std::move(other._pStruct)),
	_pOrdStruct(std::move(other._pOrdStruct)),
	_modified(other._modified)
{
}


Object::~Object()
{
}


Object &Object::operator = (const Object &other)
{
	if (&other != this)
	{
		_values = other._values;
		_keys = other._keys;
		_preserveInsOrder = other._preserveInsOrder;
		_escapeUnicode = other._escapeUnicode;
		_lowercaseHex = other._lowercaseHex;
		_pStruct = !other._modified ? other._pStruct : 0;
		_modified = other._modified;
	}
	return *this;
}


Object& Object::operator = (Object&& other) noexcept
{
	_values = std::move(other._values);
	_keys = std::move(other._keys);
	_preserveInsOrder = other._preserveInsOrder;
	_escapeUnicode = other._escapeUnicode;
	_lowercaseHex = other._lowercaseHex;
	_pStruct = std::move(other._pStruct);
	_pOrdStruct = std::move(other._pOrdStruct);
	_modified = other._modified;

	return *this;
}


void Object::syncKeys(const KeyList& keys)
{
	if(_preserveInsOrder)
	{
		// update iterators in _keys to point to copied _values
		for(KeyList::const_iterator it = keys.begin(); it != keys.end(); ++it)
		{
			ValueMap::const_iterator itv = _values.find((*it)->first);
			poco_assert (itv != _values.end());
			_keys.push_back(itv);
		}
	}
}


Var Object::get(const std::string& key) const
{
	ValueMap::const_iterator it = _values.find(key);
	if (it != _values.end())
	{
		return it->second;
	}

	return Var();
}


Array::Ptr Object::getArray(const std::string& key) const
{
	ValueMap::const_iterator it = _values.find(key);
	if ((it != _values.end()) && (it->second.type() == typeid(Array::Ptr)))
	{
		return it->second.extract<Array::Ptr>();
	}

	return 0;
}


Object::Ptr Object::getObject(const std::string& key) const
{
	ValueMap::const_iterator it = _values.find(key);
	if ((it != _values.end()) && (it->second.type() == typeid(Object::Ptr)))
	{
		return it->second.extract<Object::Ptr>();
	}

	return 0;
}


void Object::getNames(NameList& names) const
{
	names.clear();
	if (_preserveInsOrder)
	{
		for(KeyList::const_iterator it = _keys.begin(); it != _keys.end(); ++it)
		{
			names.push_back((*it)->first);
		}
	}
	else
	{
		for(ValueMap::const_iterator it = _values.begin(); it != _values.end(); ++it)
		{
			names.push_back(it->first);
		}
	}
}


Object::NameList Object::getNames() const
{
	NameList names;
	getNames(names);
	return names;
}


void Object::stringify(std::ostream& out, unsigned int indent, int step) const
{
	if (step < 0) step = indent;

	if (!_preserveInsOrder)
		doStringify(_values, out, indent, step);
	else
		doStringify(_keys, out, indent, step);
}


const std::string& Object::getKey(KeyList::const_iterator& iter) const
{
	ValueMap::const_iterator it = _values.begin();
	ValueMap::const_iterator end = _values.end();
	for (; it != end; ++it)
	{
		if (it == *iter) return it->first;
	}

	throw NotFoundException((*iter)->first);
}


Object& Object::set(const std::string& key, const Dynamic::Var& value)
{
	std::pair<ValueMap::iterator, bool> ret = _values.insert(ValueMap::value_type(key, value));
	if (!ret.second) ret.first->second = value;
	if (_preserveInsOrder)
	{
		KeyList::iterator it = _keys.begin();
		KeyList::iterator end = _keys.end();
		for (; it != end; ++it)
		{
			if (key == (*it)->first) return *this;
		}
		_keys.push_back(ret.first);
	}
	_modified = true;
	return *this;
}


Poco::DynamicStruct Object::makeStruct(const Object::Ptr& obj)
{
	return makeStructImpl<Poco::DynamicStruct>(obj);
}


Poco::OrderedDynamicStruct Object::makeOrderedStruct(const Object::Ptr& obj)
{
	return makeStructImpl<Poco::OrderedDynamicStruct>(obj);
}

/*
void Object::resetOrdDynStruct() const
{
	if (!_pOrdStruct)
		_pOrdStruct = new Poco::OrderedDynamicStruct;
	else
		_pOrdStruct->clear();
}
*/


/*
void Object::resetDynStruct() const
{
	if (!_pStruct)
		_pStruct = new Poco::DynamicStruct;
	else
		_pStruct->clear();
}*/


Object::operator const Poco::DynamicStruct& () const
{
	if (!_values.size())
	{
		resetDynStruct(_pStruct);
	}
	else if (_modified)
	{
		ValueMap::const_iterator it = _values.begin();
		ValueMap::const_iterator end = _values.end();
		resetDynStruct(_pStruct);
		for (; it != end; ++it)
		{
			if (isObject(it))
			{
				_pStruct->insert(it->first, makeStruct(getObject(it->first)));
			}
			else if (isArray(it))
			{
				_pStruct->insert(it->first, Poco::JSON::Array::makeArray(getArray(it->first)));
			}
			else
			{
				_pStruct->insert(it->first, it->second);
			}
		}
	}

	return *_pStruct;
}


Object::operator const Poco::OrderedDynamicStruct& () const
{
	if (!_values.size())
	{
		resetDynStruct(_pOrdStruct);
	}
	else if (_modified)
	{
		if (_preserveInsOrder)
		{
			KeyList::const_iterator it = _keys.begin();
			KeyList::const_iterator end = _keys.end();
			resetDynStruct(_pOrdStruct);
			for (; it != end; ++it)
			{
				if (isObject((*it)->first))
				{
					_pOrdStruct->insert((*it)->first, makeOrderedStruct(getObject((*it)->first)));
				}
				else if (isArray((*it)->first))
				{
					_pOrdStruct->insert((*it)->first, Poco::JSON::Array::makeArray(getArray((*it)->first)));
				}
				else
				{
					_pOrdStruct->insert((*it)->first, (*it)->second);
				}
			}
		}
		else
		{
			ValueMap::const_iterator it = _values.begin();
			ValueMap::const_iterator end = _values.end();
			resetDynStruct(_pOrdStruct);
			for (; it != end; ++it)
			{
				if (isObject(it))
				{
					_pOrdStruct->insert(it->first, makeOrderedStruct(getObject(it->first)));
				}
				else if (isArray(it))
				{
					_pOrdStruct->insert(it->first, Poco::JSON::Array::makeArray(getArray(it->first)));
				}
				else
				{
					_pOrdStruct->insert(it->first, it->second);
				}
			}
		}
	}

	return *_pOrdStruct;
}


void Object::clear()
{
	_values.clear();
	_keys.clear();
	_pStruct = 0;
	_modified = true;
}


} } // namespace Poco::JSON
