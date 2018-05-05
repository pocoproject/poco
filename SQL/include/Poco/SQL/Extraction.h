//
// Extraction.h
//
// Library: Data
// Package: DataCore
// Module:  Extraction
//
// Definition of the Extraction class.
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef SQL_Extraction_INCLUDED
#define SQL_Extraction_INCLUDED


#include "Poco/SQL/SQL.h"
#include "Poco/Types.h"
#include "Poco/SQL/AbstractExtraction.h"
#include "Poco/SQL/Preparation.h"
#include "Poco/SQL/TypeHandler.h"
#include "Poco/SQL/Column.h"
#include "Poco/SQL/Position.h"
#include "Poco/SQL/SQLException.h"
#include <set>
#include <vector>
#include <list>
#include <deque>
#include <map>
#include <cstddef>


namespace Poco {
namespace SQL {


	template <typename CollTp, typename ElemTp = typename CollTp::value_type>
	struct DefaultInitializer
	{
		static void appendElem(CollTp& coll, const ElemTp& defVal)
		{
			coll.push_back(defVal);
		}
	};

	template <typename CollTp>
	struct DefaultInitializer<CollTp, CLOB>
	{
		static void appendElem(CollTp& coll, const CLOB& defVal)
		{
			CLOB v(defVal.rawContent(), defVal.size());
			coll.push_back(v);
		}
	};

	template <typename CollTp>
	struct DefaultInitializer<CollTp, BLOB>
	{
		static void appendElem(CollTp& coll, const BLOB& defVal)
		{
			BLOB v(defVal.rawContent(), defVal.size());
			coll.push_back(v);
		}
	};

	template <typename CollTp>
	inline void appendElem(CollTp& coll, const typename CollTp::value_type& defVal)
	{
		DefaultInitializer<CollTp, typename CollTp::value_type>::appendElem(coll, defVal);
	}



template <class T>
class Extraction: public AbstractExtraction
	/// Concrete Data Type specific extraction of values from a query result set.
{
public:
	typedef T                   ValType;
	typedef SharedPtr<ValType>  ValPtr;
	typedef Extraction<ValType> Type;
	typedef SharedPtr<Type>     Ptr;

	Extraction(T& rResult, const Position& pos = Position(0)):
		AbstractExtraction(typeid(T).name(), static_cast<Limit::SizeT>(Limit::LIMIT_UNLIMITED), pos.value()),
		_rResult(rResult),
		_default(),
		_extracted(false),
		_null(false)
		/// Creates an Extraction object at specified position.
		/// Uses an empty object T as default value.
	{
	}

	Extraction(T& rResult, const T& def, const Position& pos = Position(0)):
		AbstractExtraction(typeid(T).name(), Limit::LIMIT_UNLIMITED, pos.value()),
		_rResult(rResult),
		_default(def),
		_extracted(false),
		_null(false)
		/// Creates an Extraction object at specified position.
		/// Uses the provided def object as default value.
	{
	}

	~Extraction()
		/// Destroys the Extraction object.
	{
	}

	std::size_t numOfColumnsHandled() const
	{
		return TypeHandler<T>::size();
	}

	std::size_t numOfRowsHandled() const
	{
		return _extracted ? 1u : 0;
	}

	std::size_t numOfRowsAllowed() const
	{
		return 1u;
	}

	bool isNull(std::size_t /*row*/ = 0) const
	{
		return _null;
	}

	std::size_t extract(std::size_t pos)
	{
		if (_extracted) throw ExtractException("value already extracted");
		_extracted = true;
		AbstractExtractor::Ptr pExt = getExtractor();
		TypeHandler<T>::extract(pos, _rResult, _default, pExt);
		_null = isValueNull<T>(_rResult, pExt->isNull(pos));
		
		return 1u;
	}

	void reset()
	{
		_extracted = false;
	}

	bool canExtract() const
	{
		return !_extracted;
	}

	AbstractPreparation::Ptr createPreparation(AbstractPreparator::Ptr& pPrep, std::size_t pos)
	{
		return new Preparation<T>(pPrep, pos, _rResult);
	}

private:
	T&   _rResult;
	T    _default;
	bool _extracted;
	bool _null;
};


template <class T>
class Extraction<std::vector<T> >: public AbstractExtraction
	/// Vector Data Type specialization for extraction of values from a query result set.
{
public:

	typedef std::vector<T>      ValType;
	typedef SharedPtr<ValType>  ValPtr;
	typedef Extraction<ValType> Type;
	typedef SharedPtr<Type>     Ptr;

	Extraction(std::vector<T>& rResult, const Position& pos = Position(0)):
		AbstractExtraction(typeid(std::vector<T>).name(), Limit::LIMIT_UNLIMITED, pos.value()),
		_rResult(rResult),
		_default()
	{
		_rResult.clear();
	}

	Extraction(std::vector<T>& rResult, const T& def, const Position& pos = Position(0)):
		AbstractExtraction(typeid(std::vector<T>).name(), static_cast<Limit::SizeT>(Limit::LIMIT_UNLIMITED), pos.value()),
		_rResult(rResult),
		_default(def)
	{
		_rResult.clear();
	}

	virtual ~Extraction()
	{
	}

	std::size_t numOfColumnsHandled() const
	{
		return TypeHandler<T>::size();
	}

	std::size_t numOfRowsHandled() const
	{
		return static_cast<std::size_t>(_rResult.size());
	}

	std::size_t numOfRowsAllowed() const
	{
		return getLimit();
	}

	bool isNull(std::size_t row) const
	{
		try
		{
			return _nulls.at(row);
		}
		catch (std::out_of_range& ex)
		{
			throw RangeException(ex.what());
		}
	}

	std::size_t extract(std::size_t pos)
	{
		AbstractExtractor::Ptr pExt = getExtractor();
		appendElem(_rResult, _default);
		TypeHandler<T>::extract(pos, _rResult.back(), _default, pExt);
		_nulls.push_back(isValueNull(_rResult.back(), pExt->isNull(pos)));
		return 1u;
	}

	AbstractPreparation::Ptr createPreparation(AbstractPreparator::Ptr& pPrep, std::size_t pos)
	{
		return new Preparation<T>(pPrep, pos, _default);
	}

	void reset()
	{
		_nulls.clear();
	}

protected:

	const std::vector<T>& result() const
	{
		return _rResult;
	}

private:
	std::vector<T>&  _rResult;
	T                _default;
	std::deque<bool> _nulls;
};


template <>
class Extraction<std::vector<bool> >: public AbstractExtraction
	/// Vector bool specialization for extraction of values from a query result set.
{
public:
	typedef std::vector<bool>   ValType;
	typedef SharedPtr<ValType>  ValPtr;
	typedef Extraction<ValType> Type;
	typedef SharedPtr<Type>     Ptr;

	Extraction(std::vector<bool>& rResult, const Position& pos = Position(0)):
		AbstractExtraction(typeid(std::vector<bool>).name(), static_cast<Limit::SizeT>(Limit::LIMIT_UNLIMITED), pos.value()),
		_rResult(rResult),
		_default()
	{
		_rResult.clear();
	}

	Extraction(std::vector<bool>& rResult, const bool& def, const Position& pos = Position(0)):
		AbstractExtraction(typeid(std::vector<bool>).name(), static_cast<Limit::SizeT>(Limit::LIMIT_UNLIMITED), pos.value()),
		_rResult(rResult),
		_default(def)
	{
		_rResult.clear();
	}

	virtual ~Extraction()
	{
	}

	std::size_t numOfColumnsHandled() const
	{
		return TypeHandler<bool>::size();
	}

	std::size_t numOfRowsHandled() const
	{
		return static_cast<std::size_t>(_rResult.size());
	}

	std::size_t numOfRowsAllowed() const
	{
		return getLimit();
	}

	bool isNull(std::size_t row) const
	{
		try
		{
			return _nulls.at(row);
		}
		catch (std::out_of_range& ex)
		{
			throw RangeException(ex.what());
		}
	}

	std::size_t extract(std::size_t pos)
	{
		AbstractExtractor::Ptr pExt = getExtractor();

		bool tmp = _default;
		TypeHandler<bool>::extract(pos, tmp, _default, pExt);
		_rResult.push_back(tmp);
		_nulls.push_back(pExt->isNull(pos));
		return 1u;
	}

	AbstractPreparation::Ptr createPreparation(AbstractPreparator::Ptr& pPrep, std::size_t pos)
	{
		return new Preparation<bool>(pPrep, pos, _default);
	}

	void reset()
	{
		_nulls.clear();
	}

protected:

	const std::vector<bool>& result() const
	{
		return _rResult;
	}

private:
	std::vector<bool>& _rResult;
	bool               _default;
	std::deque<bool>   _nulls;
};


template <class T>
class Extraction<std::list<T> >: public AbstractExtraction
	/// List Data Type specialization for extraction of values from a query result set.
{
public:
	typedef std::list<T>        ValType;
	typedef SharedPtr<ValType>  ValPtr;
	typedef Extraction<ValType> Type;
	typedef SharedPtr<Type>     Ptr;

	Extraction(std::list<T>& rResult, const Position& pos = Position(0)):
		AbstractExtraction(typeid(std::list<T>).name(), Limit::LIMIT_UNLIMITED, pos.value()),
		_rResult(rResult),
		_default()
	{
		_rResult.clear();
	}

	Extraction(std::list<T>& rResult, const T& def, const Position& pos = Position(0)):
		AbstractExtraction(typeid(std::list<T>).name(), static_cast<Limit::SizeT>(Limit::LIMIT_UNLIMITED), pos.value()),
		_rResult(rResult),
		_default(def)
	{
		_rResult.clear();
	}

	virtual ~Extraction()
	{
	}

	std::size_t numOfColumnsHandled() const
	{
		return TypeHandler<T>::size();
	}

	std::size_t numOfRowsHandled() const
	{
		return _rResult.size();
	}

	std::size_t numOfRowsAllowed() const
	{
		return getLimit();
	}

	bool isNull(std::size_t row) const
	{
		try
		{
			return _nulls.at(row);
		}
		catch (std::out_of_range& ex)
		{
			throw RangeException(ex.what());
		}
	}

	std::size_t extract(std::size_t pos)
	{
		AbstractExtractor::Ptr pExt = getExtractor();
		appendElem(_rResult, _default);
		TypeHandler<T>::extract(pos, _rResult.back(), _default, pExt);
		_nulls.push_back(isValueNull(_rResult.back(), pExt->isNull(pos)));
		return 1u;
	}

	AbstractPreparation::Ptr createPreparation(AbstractPreparator::Ptr& pPrep, std::size_t pos)
	{
		return new Preparation<T>(pPrep, pos, _default);
	}

	void reset()
	{
		_nulls.clear();
	}

protected:

	const std::list<T>& result() const
	{
		return _rResult;
	}

private:
	std::list<T>&    _rResult;
	T                _default;
	std::deque<bool> _nulls;
};


template <class T>
class Extraction<std::deque<T> >: public AbstractExtraction
	/// Deque Data Type specialization for extraction of values from a query result set.
{
public:
	typedef std::deque<T>       ValType;
	typedef SharedPtr<ValType>  ValPtr;
	typedef Extraction<ValType> Type;
	typedef SharedPtr<Type>     Ptr;

	Extraction(std::deque<T>& rResult, const Position& pos = Position(0)):
		AbstractExtraction(typeid(std::deque<T>).name(), Limit::LIMIT_UNLIMITED, pos.value()),
		_rResult(rResult),
		_default()
	{
		_rResult.clear();
	}

	Extraction(std::deque<T>& rResult, const T& def, const Position& pos = Position(0)):
		AbstractExtraction(typeid(std::deque<T>).name(), static_cast<Limit::SizeT>(Limit::LIMIT_UNLIMITED), pos.value()),
		_rResult(rResult),
		_default(def)
	{
		_rResult.clear();
	}

	virtual ~Extraction()
	{
	}

	std::size_t numOfColumnsHandled() const
	{
		return TypeHandler<T>::size();
	}

	std::size_t numOfRowsHandled() const
	{
		return _rResult.size();
	}

	std::size_t numOfRowsAllowed() const
	{
		return getLimit();
	}

	bool isNull(std::size_t row) const
	{
		try
		{
			return _nulls.at(row);
		}
		catch (std::out_of_range& ex)
		{
			throw RangeException(ex.what());
		}
	}

	std::size_t extract(std::size_t pos)
	{
		AbstractExtractor::Ptr pExt = getExtractor();
		appendElem(_rResult, _default);
		TypeHandler<T>::extract(pos, _rResult.back(), _default, pExt);
		_nulls.push_back(isValueNull(_rResult.back(), pExt->isNull(pos)));
		return 1u;
	}

	AbstractPreparation::Ptr createPreparation(AbstractPreparator::Ptr& pPrep, std::size_t pos)
	{
		return new Preparation<T>(pPrep, pos, _default);
	}

	void reset()
	{
		_nulls.clear();
	}

protected:

	const std::deque<T>& result() const
	{
		return _rResult;
	}

private:
	std::deque<T>&   _rResult;
	T                _default;
	std::deque<bool> _nulls;
};


template <class C>
class InternalExtraction: public Extraction<C>
	/// Container Data Type specialization extension for extraction of values from a query result set.
	///
	/// This class is intended for PocoData internal use - it is used by StatementImpl
	/// to automatically create internal Extraction in cases when statement returns data and no external storage
	/// was supplied. It is later used by RecordSet to retrieve the fetched data after statement execution.
	/// It takes ownership of the Column pointer supplied as constructor argument. Column object, in turn
	/// owns the data container pointer.
	///
	/// InternalExtraction objects can not be copied or assigned.
{
public:
	typedef typename C::value_type ValType;
	typedef SharedPtr<ValType>     ValPtr;
	typedef Extraction<ValType>    Type;
	typedef SharedPtr<Type>        Ptr;


	InternalExtraction(C& rResult, Column<C>* pColumn, const Position& pos = Position(0)):
		Extraction<C>(rResult, ValType(), pos),
		_pColumn(pColumn)
		/// Creates InternalExtraction.
	{
	}

	~InternalExtraction()
		/// Destroys InternalExtraction.
	{
		delete _pColumn;
	}

	void reset()
	{
		Extraction<C>::reset();
		_pColumn->reset();
	}	

	const ValType& value(int index) const
	{
		try
		{
			return Extraction<C>::result().at(index);
		}
		catch (std::out_of_range& ex)
		{
			throw RangeException(ex.what());
		}
	}

	bool isNull(std::size_t row) const
	{
		return Extraction<C>::isNull(row);
	}

	const Column<C>& column() const
	{
		return *_pColumn;
	}

private:
	InternalExtraction();
	InternalExtraction(const InternalExtraction&);
	InternalExtraction& operator = (const InternalExtraction&);

	Column<C>* _pColumn;
};


template <class T>
class Extraction<std::set<T> >: public AbstractExtraction
	/// Set Data Type specialization for extraction of values from a query result set.
{
public:
	typedef std::set<T>                ValType;
	typedef SharedPtr<ValType>         ValPtr;
	typedef Extraction<ValType>        Type;
	typedef SharedPtr<Type>            Ptr;
	typedef typename ValType::iterator Iterator;

	Extraction(std::set<T>& rResult, const Position& pos = Position(0)):
		AbstractExtraction(typeid(std::set<T>).name(), Limit::LIMIT_UNLIMITED, pos.value()),
		_rResult(rResult),
		_default()
	{
		_rResult.clear();
	}

	Extraction(std::set<T>& rResult, const T& def, const Position& pos = Position(0)):
		AbstractExtraction(typeid(std::set<T>).name(), Limit::LIMIT_UNLIMITED, pos.value()),
		_rResult(rResult),
		_default(def)
	{
		_rResult.clear();
	}

	~Extraction()
	{
	}

	std::size_t numOfColumnsHandled() const
	{
		return TypeHandler<T>::size();
	}

	std::size_t numOfRowsHandled() const
	{
		return static_cast<std::size_t>(_rResult.size());
	}

	std::size_t numOfRowsAllowed() const
	{
		return getLimit();
	}

	std::size_t extract(std::size_t pos)
	{
		T tmp;
		TypeHandler<T>::extract(pos, tmp, _default, getExtractor());
		_rResult.insert(tmp);
		return 1u;
	}

	AbstractPreparation::Ptr createPreparation(AbstractPreparator::Ptr& pPrep, std::size_t pos)
	{
		return new Preparation<T>(pPrep, pos, _default);
	}

private:
	std::set<T>& _rResult;
	T            _default;
};


template <class T>
class Extraction<std::multiset<T> >: public AbstractExtraction
	/// Multiset Data Type specialization for extraction of values from a query result set.
{
public:
	typedef std::multiset<T>    ValType;
	typedef SharedPtr<ValType>  ValPtr;
	typedef Extraction<ValType> Type;
	typedef SharedPtr<Type>     Ptr;

	Extraction(std::multiset<T>& rResult, const Position& pos = Position(0)):
		AbstractExtraction(typeid(std::multiset<T>).name(), Limit::LIMIT_UNLIMITED, pos.value()),
		_rResult(rResult),
		_default()
	{
		_rResult.clear();
	}

	Extraction(std::multiset<T>& rResult, const T& def, const Position& pos = Position(0)):
		AbstractExtraction(typeid(std::multiset<T>).name(), Limit::LIMIT_UNLIMITED, pos.value()),
		_rResult(rResult),
		_default(def)
	{
		_rResult.clear();
	}

	~Extraction()
	{
	}

	std::size_t numOfColumnsHandled() const
	{
		return TypeHandler<T>::size();
	}

	std::size_t numOfRowsHandled() const
	{
		return static_cast<std::size_t>(_rResult.size());
	}

	std::size_t numOfRowsAllowed() const
	{
		return getLimit();
	}

	std::size_t extract(std::size_t pos)
	{
		T tmp;
		TypeHandler<T>::extract(pos, tmp, _default, getExtractor());
		_rResult.insert(tmp);
		return 1u;
	}

	AbstractPreparation::Ptr createPreparation(AbstractPreparator::Ptr& pPrep, std::size_t pos)
	{
		return new Preparation<T>(pPrep, pos, _default);
	}

private:
	std::multiset<T>& _rResult;
	T                 _default;
};


template <class K, class V>
class Extraction<std::map<K, V> >: public AbstractExtraction
	/// Map Data Type specialization for extraction of values from a query result set.
{
public:
	typedef std::map<K, V>      ValType;
	typedef SharedPtr<ValType>  ValPtr;
	typedef Extraction<ValType> Type;
	typedef SharedPtr<Type>     Ptr;

	Extraction(std::map<K, V>& rResult, const Position& pos = Position(0)):
		AbstractExtraction(typeid(std::map<K, V>).name(), Limit::LIMIT_UNLIMITED, pos.value()),
		_rResult(rResult),
		_default()
	{
		_rResult.clear();
	}

	Extraction(std::map<K, V>& rResult, const V& def, const Position& pos = Position(0)):
		AbstractExtraction(typeid(std::map<K, V>).name(), Limit::LIMIT_UNLIMITED, pos.value()),
		_rResult(rResult),
		_default(def)
	{
		_rResult.clear();
	}

	~Extraction()
	{
	}

	std::size_t numOfColumnsHandled() const
	{
		return TypeHandler<V>::size();
	}

	std::size_t numOfRowsHandled() const
	{
		return static_cast<std::size_t>(_rResult.size());
	}

	std::size_t numOfRowsAllowed() const
	{
		return getLimit();
	}

	std::size_t extract(std::size_t pos)
	{
		V tmp;
		TypeHandler<V>::extract(pos, tmp, _default, getExtractor());
		_rResult.insert(std::make_pair(tmp(), tmp));
		return 1u;
	}

	AbstractPreparation::Ptr createPreparation(AbstractPreparator::Ptr& pPrep, std::size_t pos)
	{
		return new Preparation<V>(pPrep, pos, _default);
	}

private:
	std::map<K, V>& _rResult;
	V               _default;
};


template <class K, class V>
class Extraction<std::multimap<K, V> >: public AbstractExtraction
	/// Multimap Data Type specialization for extraction of values from a query result set.
{
public:
	typedef std::multimap<K, V> ValType;
	typedef SharedPtr<ValType>  ValPtr;
	typedef Extraction<ValType> Type;
	typedef SharedPtr<Type>     Ptr;

	Extraction(std::multimap<K, V>& rResult, const Position& pos = Position(0)):
		AbstractExtraction(typeid(std::multimap<K, V>).name(), Limit::LIMIT_UNLIMITED, pos.value()),
		_rResult(rResult),
		_default()
	{
		_rResult.clear();
	}

	Extraction(std::multimap<K, V>& rResult, const V& def, const Position& pos = Position(0)):
		AbstractExtraction(typeid(std::multimap<K, V>).name(), Limit::LIMIT_UNLIMITED, pos.value()),
		_rResult(rResult),
		_default(def)
	{
		_rResult.clear();
	}

	~Extraction()
	{
	}

	std::size_t numOfColumnsHandled() const
	{
		return TypeHandler<V>::size();
	}

	std::size_t numOfRowsHandled() const
	{
		return static_cast<std::size_t>(_rResult.size());
	}

	std::size_t numOfRowsAllowed() const
	{
		return getLimit();
	}

	std::size_t extract(std::size_t pos)
	{
		V tmp;
		TypeHandler<V>::extract(pos, tmp, _default, getExtractor());
		_rResult.insert(std::make_pair(tmp(), tmp));
		return 1u;
	}

	AbstractPreparation::Ptr createPreparation(AbstractPreparator::Ptr& pPrep, std::size_t pos)
	{
		return new Preparation<V>(pPrep, pos, _default);
	}

private:
	std::multimap<K, V>& _rResult;
	V                    _default;
};


namespace Keywords {


template <typename T>
inline AbstractExtraction::Ptr into(T& t)
	/// Convenience function to allow for a more compact creation of an extraction object.
{
	return new Extraction<T>(t);
}


template <>
inline AbstractExtraction::Ptr into(long& t)
	/// Convenience function to allow for a more compact creation of an extraction object.
{
#ifndef POCO_LONG_IS_64_BIT
	return new Extraction<long>(t);
#else
	return new Extraction<Poco::Int64>(reinterpret_cast<Poco::Int64&>(t));
#endif
}


template <>
inline AbstractExtraction::Ptr into(unsigned long& t)
	/// Convenience function to allow for a more compact creation of an extraction object.
{
#ifndef POCO_LONG_IS_64_BIT
	return new Extraction<unsigned long>(t);
#else
	return new Extraction<Poco::UInt64>(reinterpret_cast<Poco::UInt64&>(t));
#endif
}


template <typename T>
inline AbstractExtraction::Ptr into(T& t, const Position& pos)
	/// Convenience function to allow for a more compact creation of an extraction object
	/// with multiple recordset support.
{
	return new Extraction<T>(t, pos);
}


template <typename T>
inline AbstractExtraction::Ptr into(T& t, const Position& pos, const T& def)
	/// Convenience function to allow for a more compact creation of an extraction object
	/// with multiple recordset support and the given default
{
	return new Extraction<T>(t, def, pos);
}


inline AbstractExtractionVecVec& into(AbstractExtractionVecVec& evv)
	/// Convenience dummy function (for syntax purposes only).
{
	return evv;
}


inline AbstractExtractionVec& into(AbstractExtractionVec& ev)
	/// Convenience dummy function (for syntax purposes only).
{
	return ev;
}


} // namespace Keywords


} } // namespace Poco::SQL


#endif // Data_Extraction_INCLUDED
