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


namespace Poco {
namespace JSON {


Object::Object(bool preserveInsOrder):
	_preserveInsOrder(preserveInsOrder),
	_modified(false)
{
}


Object::Object(const Object& copy) : _values(copy._values),
	_preserveInsOrder(copy._preserveInsOrder),
	_pStruct(!copy._modified ? copy._pStruct : 0),
	_modified(copy._modified)
{
	if (_preserveInsOrder)
	{
		// need to update pointers in _keys to point to copied _values
		for (KeyPtrList::const_iterator it = copy._keys.begin(); it != copy._keys.end(); ++it)
		{
			ValueMap::const_iterator itv = _values.find(**it);
			poco_assert (itv != _values.end());
			_keys.push_back(&itv->first);
		}
	}
}


#ifdef POCO_ENABLE_CPP11


Object::Object(Object&& other) :
	_values(std::move(other._values)),
	_keys(std::move(other._keys)),
	_preserveInsOrder(other._preserveInsOrder),
	_pStruct(!other._modified ? other._pStruct : 0),
	_modified(other._modified)
{
}


Object &Object::operator= (Object &&other)
{
	if (&other != this)
	{
		_values = std::move(other._values);
		_keys = std::move(other._keys);
		_preserveInsOrder = other._preserveInsOrder;
		_pStruct = !other._modified ? other._pStruct : 0;
		_modified = other._modified;
	}
	return *this;
}


#endif // POCO_ENABLE_CPP11


Object::~Object()
{
}


Object &Object::operator= (const Object &other)
{
	if (&other != this)
	{
		_values = other._values;
		_keys = other._keys;
		_preserveInsOrder = other._preserveInsOrder;
		_pStruct = !other._modified ? other._pStruct : 0;
		_modified = other._modified;
	}
	return *this;
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


void Object::getNames(std::vector<std::string>& names) const
{
	names.clear();
	for (ValueMap::const_iterator it = _values.begin(); it != _values.end(); ++it)
	{
		names.push_back(it->first);
	}
}


void Object::stringify(std::ostream& out, unsigned int indent, int step) const
{
	if (step < 0) step = indent;

	if (!_preserveInsOrder)
		doStringify(_values, out, indent, step);
	else
		doStringify(_keys, out, indent, step);
}


const std::string& Object::getKey(KeyPtrList::const_iterator& iter) const
{
	ValueMap::const_iterator it = _values.begin();
	ValueMap::const_iterator end = _values.end();
	for (; it != end; ++it)
	{
		if (it->first == **iter) return it->first;
	}

	throw NotFoundException(**iter);
}


void Object::set(const std::string& key, const Dynamic::Var& value)
{
	std::pair<ValueMap::iterator, bool> ret = _values.insert(ValueMap::value_type(key, value));
	if (!ret.second) ret.first->second = value;
	if (_preserveInsOrder)
	{
		KeyPtrList::iterator it = _keys.begin();
		KeyPtrList::iterator end = _keys.end();
		for (; it != end; ++it)
		{
			if (key == **it) return;
		}
		_keys.push_back(&ret.first->first);
	}
	_modified = true;
}


Poco::DynamicStruct Object::makeStruct(const Object::Ptr& obj)
{
	Poco::DynamicStruct ds;

	ConstIterator it  = obj->begin();
	ConstIterator end = obj->end();
	for (; it != end; ++it)
	{
		if (obj->isObject(it))
		{
			Object::Ptr pObj = obj->getObject(it->first);
			DynamicStruct str = makeStruct(pObj);
			ds.insert(it->first, str);
		}
		else if (obj->isArray(it))
		{
			Array::Ptr pArr = obj->getArray(it->first);
			std::vector<Poco::Dynamic::Var> v = Poco::JSON::Array::makeArray(pArr);
			ds.insert(it->first, v);
		}
		else
			ds.insert(it->first, it->second);
	}

	return ds;
}


void Object::resetDynStruct() const
{
	if (!_pStruct)
		_pStruct = new Poco::DynamicStruct;
	else
		_pStruct->clear();
}


Object::operator const Poco::DynamicStruct& () const
{
	if (!_values.size())
	{
		resetDynStruct();
	}
	else if (_modified)
	{
		ValueMap::const_iterator it = _values.begin();
		ValueMap::const_iterator end = _values.end();
		resetDynStruct();
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


void Object::clear()
{
	_values.clear();
	_keys.clear();
	_pStruct = 0;
	_modified = true;
}


} } // namespace Poco::JSON
