//
// AbstractSessionImpl.h
//
// Library: Data
// Package: DataCore
// Module:  AbstractSessionImpl
//
// Definition of the AbstractSessionImpl class.
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Data_AbstractSessionImpl_INCLUDED
#define Data_AbstractSessionImpl_INCLUDED


#include "Poco/Data/Data.h"
#include "Poco/Data/SessionImpl.h"
#include "Poco/Data/DataException.h"
#include <map>


namespace Poco {
namespace Data {


template <class C>
class AbstractSessionImpl: public SessionImpl
	/// A partial implementation of SessionImpl, providing
	/// features and properties management.
	///
	/// To implement a certain feature or property, a subclass
	/// must provide setter and getter methods and register
	/// them with addFeature() or addProperty().
{
public:
	typedef void (C::*FeatureSetter)(const std::string&, bool);
		/// The setter method for a feature.

	typedef bool (C::*FeatureGetter)(const std::string&) const;
		/// The getter method for a feature.

	typedef void (C::*PropertySetter)(const std::string&, const Poco::Any&);
		/// The setter method for a property.

	typedef Poco::Any (C::*PropertyGetter)(const std::string&) const;
		/// The getter method for a property.

	AbstractSessionImpl(const std::string& connectionString,
		std::size_t timeout = LOGIN_TIMEOUT_DEFAULT): SessionImpl(connectionString, timeout),
			_storage(std::string("deque")),
			_bulk(false),
			_emptyStringIsNull(false),
			_forceEmptyString(false),
			_sqlParse(false),
			_autoCommit(true)
		/// Creates the AbstractSessionImpl.
		///
		/// Adds "storage" property and sets the default internal storage container
		/// type to std::deque.
		/// The storage is created by statements automatically whenever a query
		/// returning results is executed but external storage is provided by the user.
		/// Storage type can be reconfigured at runtime both globally (for the
		/// duration of the session) and locally (for a single statement execution only).
		/// See StatementImpl for details on how this property is used at runtime.
		///
		/// Adds "handle" property which, if set by the back end, returns native handle
		/// for the back end DB.
		///
		/// Adds "bulk" feature and sets it to false.
		/// Bulk feature determines whether the session is capable of bulk operations.
		/// Connectors that are capable of it must set this feature prior to attempting
		/// bulk operations.
		///
		/// Adds "emptyStringIsNull" feature and sets it to false. This feature should be
		/// set to true in order to modify the behavior of the databases that distinguish
		/// between zero-length character strings as nulls. Setting this feature to true
		/// shall disregard any difference between empty character strings and nulls,
		/// causing the framework to treat them the same (i.e. behave like Oracle).
		///
		/// Adds "forceEmptyString" feature and sets it to false. This feature should be set
		/// to true in order to force the databases that do not distinguish empty strings from
		/// nulls (e.g. Oracle) to always report empty string.
		///
		/// The "emptyStringIsNull" and "forceEmptyString" features are mutually exclusive.
		/// While these features can not both be true at the same time, they can both be false,
		/// resulting in default underlying database behavior.
		///
		/// Adds "sqlParse" feature and sets it to false; this property enables parsing of the SQL queries,
		/// to help the Data framework to determine whether to start a transaction automatically in
		/// non-autocomit mode (ie. not to start transaction if all the queries in an SQL statement are SELECTs).
		/// Note that the property is not a bullet-proof way to ensure this behavior, because not all SQL dialects
		/// are supported by the parser. When enabled, the parsing has performance cost, but it is otherwise
		/// non-intrusive, ie. on parse failure Statement only internally records parsing errors, but does not throw.
		/// See Poco::Data::Statement documentation for more information.
		/// This property has no effect when Poco::Data library is compiled with POCO_DATA_NO_SQL_PARSER.
		///
		/// Adds "autoCommit" feature and sets it to true. This property enables automatic commit.
		/// Setting this feature to  true renders the `sqlParse` property meaningless, because every query
		/// is automatically commited.
	{
		addProperty("storage",
			&AbstractSessionImpl<C>::setStorage,
			&AbstractSessionImpl<C>::getStorage);

		addProperty("handle",
			&AbstractSessionImpl<C>::setHandle,
			&AbstractSessionImpl<C>::getHandle);

		addFeature("bulk",
			&AbstractSessionImpl<C>::setBulk,
			&AbstractSessionImpl<C>::getBulk);

		addFeature("emptyStringIsNull",
			&AbstractSessionImpl<C>::setEmptyStringIsNull,
			&AbstractSessionImpl<C>::getEmptyStringIsNull);

		addFeature("forceEmptyString",
			&AbstractSessionImpl<C>::setForceEmptyString,
			&AbstractSessionImpl<C>::getForceEmptyString);

		addFeature("sqlParse",
			&AbstractSessionImpl<C>::setSQLParse,
			&AbstractSessionImpl<C>::getSQLParse);

		addFeature("autoCommit",
			&AbstractSessionImpl<C>::setAutoCommit,
			&AbstractSessionImpl<C>::getAutoCommit);
	}

	~AbstractSessionImpl()
		/// Destroys the AbstractSessionImpl.
	{
	}

	bool hasFeature(const std::string& name) const
		/// Looks a feature up in the features map
		/// and returns true if there is one.
	{
		auto it = _features.find(name);
		return it != _features.end() &&
			it->second.getter &&
			it->second.setter;
	}

	void setFeature(const std::string& name, bool state)
		/// Looks a feature up in the features map
		/// and calls the feature's setter, if there is one.
	{
		typename FeatureMap::const_iterator it = _features.find(name);
		if (it != _features.end())
		{
			if (it->second.setter)
				(static_cast<C*>(this)->*it->second.setter)(name, state);
			else
				throw NotImplementedException("set", name);
		}
		else throw NotSupportedException(name);
	}

	bool getFeature(const std::string& name) const
		/// Looks a feature up in the features map
		/// and calls the feature's getter, if there is one.
	{
		typename FeatureMap::const_iterator it = _features.find(name);
		if (it != _features.end())
		{
			if (it->second.getter)
				return (static_cast<const C*>(this)->*it->second.getter)(name);
			else
				throw NotImplementedException("get", name);
		}
		else throw NotSupportedException(name);
	}

	bool hasProperty(const std::string& name) const
		/// Looks a property up in the properties map
		/// and returns true if there is one.
	{
		auto it = _properties.find(name);
		return it != _properties.end() &&
			it->second.getter &&
			it->second.setter;
	}

	void setProperty(const std::string& name, const Poco::Any& value)
		/// Looks a property up in the properties map
		/// and calls the property's setter, if there is one.
	{
		typename PropertyMap::const_iterator it = _properties.find(name);
		if (it != _properties.end())
		{
			if (it->second.setter)
				(static_cast<C*>(this)->*it->second.setter)(name, value);
			else
				throw NotImplementedException("set", name);
		}
		else throw NotSupportedException(name);
	}

	Poco::Any getProperty(const std::string& name) const
		/// Looks a property up in the properties map
		/// and calls the property's getter, if there is one.
	{
		typename PropertyMap::const_iterator it = _properties.find(name);
		if (it != _properties.end())
		{
			if (it->second.getter)
				return (static_cast<const C*>(this)->*it->second.getter)(name);
			else
				throw NotImplementedException("set", name);
		}
		else throw NotSupportedException(name);
	}

	void setStorage(const std::string& value)
		/// Sets the storage type.
	{
		_storage = value;
	}

	void setStorage(const std::string& name, const Poco::Any& value)
		/// Sets the storage type.
	{
		_storage = Poco::RefAnyCast<std::string>(value);
	}

	Poco::Any getStorage(const std::string& name="") const
		/// Returns the storage type
	{
		return _storage;
	}

	void setHandle(const std::string& name, const Poco::Any& handle)
		/// Sets the native session handle.
	{
		_handle = handle;
	}

	Poco::Any getHandle(const std::string& name="") const
		/// Returns the native session handle.
	{
		return _handle;
	}

	void setBulk(const std::string& name, bool bulk)
		/// Sets the execution type.
	{
		_bulk = bulk;
	}

	bool getBulk(const std::string& name="") const
		/// Returns the execution type
	{
		return _bulk;
	}

	void setEmptyStringIsNull(const std::string& name, bool emptyStringIsNull)
		/// Sets the behavior regarding empty variable length strings.
		/// Those are treated as NULL by Oracle and as empty string by
		/// most other databases.
		/// When this feature is true, empty strings are treated as NULL.
	{
		if (emptyStringIsNull && _forceEmptyString)
			throw InvalidAccessException("Features mutually exclusive");

		_emptyStringIsNull = emptyStringIsNull;
	}

	bool getEmptyStringIsNull(const std::string& name="") const
		/// Returns the setting for the behavior regarding empty variable
		/// length strings. See setEmptyStringIsNull(const std::string&, bool)
		/// and this class documentation for feature rationale and details.
	{
		return _emptyStringIsNull;
	}

	void setForceEmptyString(const std::string&, bool forceEmptyString)
		/// Sets the behavior regarding empty variable length strings.
		/// Those are treated as NULL by Oracle and as empty string by
		/// most other databases.
		/// When this feature is true, both empty strings and NULL values
		/// are reported as empty strings.
	{
		if (forceEmptyString && _emptyStringIsNull)
			throw InvalidAccessException("Features mutually exclusive");

		_forceEmptyString = forceEmptyString;
	}

	bool getForceEmptyString(const std::string& name="") const
		/// Returns the setting for the behavior regarding empty variable
		/// length strings. See setForceEmptyString(const std::string&, bool)
		/// and this class documentation for feature rationale and details.
	{
		return _forceEmptyString;
	}

	void setSQLParse(const std::string&, bool doParse)
		/// Enables the SQL parsing. Value must be of type bool.
		/// When SQL parsing is enabled (default), the Statement attempts
		/// to parse the SQL prior to executing it. The parsing is done
		/// in non-autocommit mode only, with purpose to determine the
		/// type of query and whether to start the transaction automatically.
		///
		/// See Statement documentation for more information.
	{
		_sqlParse = doParse;
	}


	bool getSQLParse(const std::string& name = "") const
		/// Returns the value of the SQL parsing flag.
	{
		return _sqlParse;
	}

protected:
	void addFeature(const std::string& name, FeatureSetter setter, FeatureGetter getter)
		/// Adds a feature to the map of supported features.
		///
		/// The setter or getter can be null, in case setting or getting a feature
		/// is not supported.
	{
		Feature feature;
		feature.setter = setter;
		feature.getter = getter;
		_features[name] = feature;
	}

	void addProperty(const std::string& name, PropertySetter setter, PropertyGetter getter)
		/// Adds a property to the map of supported properties.
		///
		/// The setter or getter can be null, in case setting or getting a property
		/// is not supported.
	{
		Property property;
		property.setter = setter;
		property.getter = getter;
		_properties[name] = property;
	}

	// most, if not all, back ends support the autocommit feature
	// these handlers are added in this class by default,
	// but an implementation can easily replace them by registering
	// early its own handlers with:
	//    addFeature("autoCommit", setter, getter)");
	//
	// these are here to be used by any back end DBMS client that
	// does not provide its own autocommit get/set capabilities

	void setAutoCommit(const std::string&, bool autoCommit)
		/// Enables automatic commit. When this feature is true,
		/// every query is automatically commited. When false,
		/// every query starts a transaction, except SELECT queries
		/// (if properly detected by parser, see set/getSQLParse() and
		/// Statement::checkBeginTransaction() documentation).
	{
		_autoCommit = autoCommit;
	}

	bool getAutoCommit(const std::string& name = "") const
		/// Returns the value of the automatic commit flag.
		/// See setAutoCommit() documentation for more details.
	{
		return _autoCommit;
	}

private:
	struct Feature
	{
		FeatureSetter setter;
		FeatureGetter getter;
	};

	struct Property
	{
		PropertySetter setter;
		PropertyGetter getter;
	};

	using FeatureMap = std::map<std::string, Feature>;
	using PropertyMap = std::map<std::string, Property>;

	FeatureMap  _features;
	PropertyMap _properties;
	std::string _storage;
	bool        _bulk;
	bool        _emptyStringIsNull;
	bool        _forceEmptyString;
	bool        _sqlParse;
	bool        _autoCommit;
	Poco::Any   _handle;
};


} } // namespace Poco::Data


#endif // Data_AbstractSessionImpl_INCLUDED
