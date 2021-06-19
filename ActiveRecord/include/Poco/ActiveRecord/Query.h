//
// Query.h
//
// Library: ActiveRecord
// Package: ActiveRecord
// Module:  Query
//
// Copyright (c) 2020, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef ActiveRecord_select_INCLUDED
#define ActiveRecord_select_INCLUDED


#include "Poco/ActiveRecord/ActiveRecord.h"
#include "Poco/ActiveRecord/Context.h"
#include "Poco/Data/Session.h"
#include "Poco/Data/Statement.h"


namespace Poco {
namespace ActiveRecord {


template <typename ActRec>
class Query
	/// A Query is used to retrieve ActiveRecord objects from a table.
	///
	/// As the name implies, Query supports selection of database rows
	/// based on a WHERE clause (see where()).
	///
	/// Furthermore, results can be sorted (see orderBy()) and
	/// filtered based on a lambda expression (see filter()).
	///
	/// Optional result paging is controlled by offset() and limit().
	/// The total number of results is available via totalResults().
{
public:
	explicit Query(Context::Ptr pContext):
		_pContext(pContext),
		_select(pContext->session())
	{
		select();
	}

	Query() = delete;
	Query(const Query&) = delete;
	~Query() = default;
	Query& operator = (const Query&) = delete;

	Query& where(const std::string& clause)
		/// Specify a WHERE clause (without the WHERE keyword)
		/// to select only rows matching the clause.
		///
		/// Placeholders (?) can be used in the clause. For each
		/// placeholder, an actual value must be bound before
		/// the query is executed (see bind()).
	{
		_select << " WHERE " << fixPlaceholders(clause);
		return *this;
	}

	Query& orderBy(const std::string& order)
		/// Specify a column name and optional direction (ASC, DESC)
		/// to order the result by.
	{
		_select << " ORDER BY " << order;
		return *this;
	}

	template <typename T>
	Query& bind(const T& value)
		/// Bind a value to a placeholder in the where() clause.
		///
		/// For each placeholder (?) in the where() clause,
		/// a value must be bound with bind() before the query
		/// can be executed.
	{
		_select, Poco::Data::Keywords::bind(value);
		return *this;
	}

	Query& offset(std::size_t offset)
		/// Specify the index or offset of the first row
		/// to return for paging.
	{
		_offset = offset;
		return *this;
	}

	Query& limit(std::size_t limit)
		/// Specify the maximum number of rows to return for paging.
	{
		_limit = limit;
		return *this;
	}

	Query& filter(const std::function<bool(const ActRec&)>& fn)
		/// Specify a lambda expression for filtering results.
		///
		/// The lamda takes a const reference to the ActiveRecord
		/// (template argument) as parameter and must return a
		/// bool. If the lambda returns true, the respective ActiveRecord
		/// is included in the query result.
	{
		_filter = fn;
		return *this;
	}

	Query& filter(std::function<bool(const ActRec&)>&& fn)
	{
		_filter = std::move(fn);
		return *this;
	}

	std::vector<typename ActRec::Ptr> execute()
		/// Execute the query and return a vector with the
		/// results.
	{
		std::vector<typename ActRec::Ptr> result;

		typename ActRec::Ptr pObject = new ActRec;
		ActRec::queryInto(_select, *pObject);
		_select, Poco::Data::Keywords::limit(1);

		std::size_t index = 0;
		while (!_select.done())
		{
			if (_select.execute())
			{
				if (!_filter || _filter(*pObject))
				{
					if (index >= _offset && (_limit == 0 || result.size() < _limit))
					{
						typename ActRec::Ptr pClone = new ActRec(*pObject);
						result.push_back(ActRec::withContext(pClone, _pContext));
					}
					index++;
				}
			}
		}
		_totalResults = index;

		return result;
	}

	std::size_t totalResults() const
		/// In case of a paged query, returns the total number of results
		/// that would be returned without paging.
	{
		return _totalResults;
	}

	void reset()
		/// Resets the query so that it can be executed again, with
		/// potentially different parameters.
	{
		_offset = 0;
		_limit = 0;
		_totalResults = 0;
		std::function<bool(const ActRec&)> emptyFilter;
		_filter.swap(emptyFilter);
		_select = Poco::Data::Statement(_pContext->session());
		select();
	}

protected:
	void select()
	{
		_select << "SELECT ";
		const auto& columns = ActRec::columns();
		bool needComma = false;
		for (const auto& c: columns)
		{
			if (needComma) _select << ", ";
			_select << c;
			needComma = true;
		}
		_select << " FROM " << ActRec::table();
	}

	std::string fixPlaceholders(const std::string& clause)
	{
		auto pSPP = _pContext->statementPlaceholderProvider();

		std::string result;
		auto it = clause.begin();
		auto end = clause.end();
		while (it != end)
		{
			switch (*it)
			{
			case '"':
				result += *it++;
				while (it != end && *it != '"') result += *it++;
				if (it != end) result += *it++;
				break;

			case '\'':
				result += *it++;
				while (it != end && *it != '\'') result += *it++;
				if (it != end) result += *it++;
				break;

			case '?':
				result += pSPP->next();
				++it;
				break;

			default:
				result += *it++;
				break;
			}
		}
		return result;
	}

private:
	Context::Ptr _pContext;
	Poco::Data::Statement _select;
	std::function<bool(const ActRec&)> _filter;
	std::size_t _offset = 0;
	std::size_t _limit = 0;
	std::size_t _totalResults = 0;
};


} } // namespace Poco::ActiveRecord


#endif // ActiveRecord_select_INCLUDED
