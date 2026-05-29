//
// RenderingBinder.h
//
// Library: Data
// Package: DataCore
// Module:  RenderingBinder
//
// Definition of the RenderingBinder class.
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Data_RenderingBinder_INCLUDED
#define Data_RenderingBinder_INCLUDED


#include "Poco/Data/Data.h"
#include "Poco/Data/AbstractBinder.h"
#include <cstddef>
#include <map>
#include <string>
#include <vector>


namespace Poco {
namespace Data {


class Data_API RenderingBinder: public AbstractBinder
	/// AbstractBinder subclass that captures bind() traffic as SQL-literal
	/// strings instead of forwarding the values to a database driver.
	///
	/// Used by Utility::boundSQL (and any other code that wants to render
	/// the SQL a Statement would execute) to obtain fidelity-perfect
	/// renderings of every binding — including STL containers (one entry
	/// per row) and user types with custom TypeHandler specializations,
	/// which call back into pBinder->bind() and thus flow through the
	/// overrides here.
	///
	/// PD_IN and PD_IN_OUT bindings are captured by formatting the value
	/// with Utility's formatting primitives. PD_OUT bindings store the
	/// literal "?" placeholder so the rendered SQL keeps a valid shape
	/// (there is no input value to substitute).
	///
	/// This binder never talks to a database. Constructing and exercising
	/// it is a pure in-memory operation; the cost is paid only by callers
	/// that explicitly request diagnostic rendering.
{
public:
	explicit RenderingBinder(std::size_t maxRows = 1);
		/// Constructs the binder. maxRows caps how many bulk rows we retain
		/// per binding position. Containers larger than maxRows are
		/// truncated; totalRows() still reports the actual count so callers
		/// can emit a "+N more rows" suffix.

	~RenderingBinder() override;

	void reset() override;
		/// No-op. AbstractBinding::reset() cascades into this for scalar
		/// bindings (Binding<T>::reset() calls pBinder->reset()); clearing
		/// here would wipe captures from earlier walked bindings during a
		/// multi-binding walk. The binder is single-use - scoped to one
		/// render call and discarded afterwards, so there is nothing to
		/// clean up.

	// --- Scalar bind() overrides ---
	void bind(std::size_t pos, const Poco::Int8&  val, Direction dir = PD_IN) override;
	void bind(std::size_t pos, const Poco::UInt8& val, Direction dir = PD_IN) override;
	void bind(std::size_t pos, const Poco::Int16& val, Direction dir = PD_IN) override;
	void bind(std::size_t pos, const Poco::UInt16& val, Direction dir = PD_IN) override;
	void bind(std::size_t pos, const Poco::Int32& val, Direction dir = PD_IN) override;
	void bind(std::size_t pos, const Poco::UInt32& val, Direction dir = PD_IN) override;
	void bind(std::size_t pos, const Poco::Int64& val, Direction dir = PD_IN) override;
	void bind(std::size_t pos, const Poco::UInt64& val, Direction dir = PD_IN) override;
#ifndef POCO_INT64_IS_LONG
	void bind(std::size_t pos, const long& val, Direction dir = PD_IN) override;
	void bind(std::size_t pos, const unsigned long& val, Direction dir = PD_IN) override;
#endif
	void bind(std::size_t pos, const bool& val, Direction dir = PD_IN) override;
	void bind(std::size_t pos, const float& val, Direction dir = PD_IN) override;
	void bind(std::size_t pos, const double& val, Direction dir = PD_IN) override;
	void bind(std::size_t pos, const char& val, Direction dir = PD_IN) override;
	void bind(std::size_t pos, const char* const& pVal, Direction dir = PD_IN) override;
	void bind(std::size_t pos, const std::string& val, Direction dir = PD_IN) override;
	void bind(std::size_t pos, const UTF16String& val, Direction dir = PD_IN) override;
	void bind(std::size_t pos, const BLOB& val, Direction dir = PD_IN) override;
	void bind(std::size_t pos, const CLOB& val, Direction dir = PD_IN) override;
	void bind(std::size_t pos, const DateTime& val, Direction dir = PD_IN) override;
	void bind(std::size_t pos, const Date& val, Direction dir = PD_IN) override;
	void bind(std::size_t pos, const Time& val, Direction dir = PD_IN) override;
	void bind(std::size_t pos, const UUID& val, Direction dir = PD_IN) override;
	void bind(std::size_t pos, const NullData& val, Direction dir = PD_IN) override;

	// --- Container bind() overrides ---
	void bind(std::size_t pos, const std::vector<Poco::Int8>& val, Direction dir = PD_IN) override;
	void bind(std::size_t pos, const std::deque<Poco::Int8>& val, Direction dir = PD_IN) override;
	void bind(std::size_t pos, const std::list<Poco::Int8>& val, Direction dir = PD_IN) override;
	void bind(std::size_t pos, const std::vector<Poco::UInt8>& val, Direction dir = PD_IN) override;
	void bind(std::size_t pos, const std::deque<Poco::UInt8>& val, Direction dir = PD_IN) override;
	void bind(std::size_t pos, const std::list<Poco::UInt8>& val, Direction dir = PD_IN) override;
	void bind(std::size_t pos, const std::vector<Poco::Int16>& val, Direction dir = PD_IN) override;
	void bind(std::size_t pos, const std::deque<Poco::Int16>& val, Direction dir = PD_IN) override;
	void bind(std::size_t pos, const std::list<Poco::Int16>& val, Direction dir = PD_IN) override;
	void bind(std::size_t pos, const std::vector<Poco::UInt16>& val, Direction dir = PD_IN) override;
	void bind(std::size_t pos, const std::deque<Poco::UInt16>& val, Direction dir = PD_IN) override;
	void bind(std::size_t pos, const std::list<Poco::UInt16>& val, Direction dir = PD_IN) override;
	void bind(std::size_t pos, const std::vector<Poco::Int32>& val, Direction dir = PD_IN) override;
	void bind(std::size_t pos, const std::deque<Poco::Int32>& val, Direction dir = PD_IN) override;
	void bind(std::size_t pos, const std::list<Poco::Int32>& val, Direction dir = PD_IN) override;
	void bind(std::size_t pos, const std::vector<Poco::UInt32>& val, Direction dir = PD_IN) override;
	void bind(std::size_t pos, const std::deque<Poco::UInt32>& val, Direction dir = PD_IN) override;
	void bind(std::size_t pos, const std::list<Poco::UInt32>& val, Direction dir = PD_IN) override;
	void bind(std::size_t pos, const std::vector<Poco::Int64>& val, Direction dir = PD_IN) override;
	void bind(std::size_t pos, const std::deque<Poco::Int64>& val, Direction dir = PD_IN) override;
	void bind(std::size_t pos, const std::list<Poco::Int64>& val, Direction dir = PD_IN) override;
	void bind(std::size_t pos, const std::vector<Poco::UInt64>& val, Direction dir = PD_IN) override;
	void bind(std::size_t pos, const std::deque<Poco::UInt64>& val, Direction dir = PD_IN) override;
	void bind(std::size_t pos, const std::list<Poco::UInt64>& val, Direction dir = PD_IN) override;
#ifndef POCO_INT64_IS_LONG
	void bind(std::size_t pos, const std::vector<long>& val, Direction dir = PD_IN) override;
	void bind(std::size_t pos, const std::deque<long>& val, Direction dir = PD_IN) override;
	void bind(std::size_t pos, const std::list<long>& val, Direction dir = PD_IN) override;
#endif
	void bind(std::size_t pos, const std::vector<bool>& val, Direction dir = PD_IN) override;
	void bind(std::size_t pos, const std::deque<bool>& val, Direction dir = PD_IN) override;
	void bind(std::size_t pos, const std::list<bool>& val, Direction dir = PD_IN) override;
	void bind(std::size_t pos, const std::vector<float>& val, Direction dir = PD_IN) override;
	void bind(std::size_t pos, const std::deque<float>& val, Direction dir = PD_IN) override;
	void bind(std::size_t pos, const std::list<float>& val, Direction dir = PD_IN) override;
	void bind(std::size_t pos, const std::vector<double>& val, Direction dir = PD_IN) override;
	void bind(std::size_t pos, const std::deque<double>& val, Direction dir = PD_IN) override;
	void bind(std::size_t pos, const std::list<double>& val, Direction dir = PD_IN) override;
	void bind(std::size_t pos, const std::vector<char>& val, Direction dir = PD_IN) override;
	void bind(std::size_t pos, const std::deque<char>& val, Direction dir = PD_IN) override;
	void bind(std::size_t pos, const std::list<char>& val, Direction dir = PD_IN) override;
	void bind(std::size_t pos, const std::vector<std::string>& val, Direction dir = PD_IN) override;
	void bind(std::size_t pos, const std::deque<std::string>& val, Direction dir = PD_IN) override;
	void bind(std::size_t pos, const std::list<std::string>& val, Direction dir = PD_IN) override;
	void bind(std::size_t pos, const std::vector<UTF16String>& val, Direction dir = PD_IN) override;
	void bind(std::size_t pos, const std::deque<UTF16String>& val, Direction dir = PD_IN) override;
	void bind(std::size_t pos, const std::list<UTF16String>& val, Direction dir = PD_IN) override;
	void bind(std::size_t pos, const std::vector<BLOB>& val, Direction dir = PD_IN) override;
	void bind(std::size_t pos, const std::deque<BLOB>& val, Direction dir = PD_IN) override;
	void bind(std::size_t pos, const std::list<BLOB>& val, Direction dir = PD_IN) override;
	void bind(std::size_t pos, const std::vector<CLOB>& val, Direction dir = PD_IN) override;
	void bind(std::size_t pos, const std::deque<CLOB>& val, Direction dir = PD_IN) override;
	void bind(std::size_t pos, const std::list<CLOB>& val, Direction dir = PD_IN) override;
	void bind(std::size_t pos, const std::vector<DateTime>& val, Direction dir = PD_IN) override;
	void bind(std::size_t pos, const std::deque<DateTime>& val, Direction dir = PD_IN) override;
	void bind(std::size_t pos, const std::list<DateTime>& val, Direction dir = PD_IN) override;
	void bind(std::size_t pos, const std::vector<Date>& val, Direction dir = PD_IN) override;
	void bind(std::size_t pos, const std::deque<Date>& val, Direction dir = PD_IN) override;
	void bind(std::size_t pos, const std::list<Date>& val, Direction dir = PD_IN) override;
	void bind(std::size_t pos, const std::vector<Time>& val, Direction dir = PD_IN) override;
	void bind(std::size_t pos, const std::deque<Time>& val, Direction dir = PD_IN) override;
	void bind(std::size_t pos, const std::list<Time>& val, Direction dir = PD_IN) override;
	void bind(std::size_t pos, const std::vector<UUID>& val, Direction dir = PD_IN) override;
	void bind(std::size_t pos, const std::deque<UUID>& val, Direction dir = PD_IN) override;
	void bind(std::size_t pos, const std::list<UUID>& val, Direction dir = PD_IN) override;
	void bind(std::size_t pos, const std::vector<NullData>& val, Direction dir = PD_IN) override;
	void bind(std::size_t pos, const std::deque<NullData>& val, Direction dir = PD_IN) override;
	void bind(std::size_t pos, const std::list<NullData>& val, Direction dir = PD_IN) override;

	// --- Capture readout ---
	const std::vector<std::string>& renderedAt(std::size_t pos) const;
		/// Returns the captured renderings for the binding at pos, indexed
		/// by row. Scalar bindings have exactly one entry. Container
		/// bindings have up to min(container.size(), maxRows) entries.
		/// Throws Poco::NotFoundException if pos was never bound.

	bool hasPosition(std::size_t pos) const;
		/// Returns true if the position was captured.

	std::size_t totalRows() const;
		/// Maximum container size seen across all captured positions, or 1
		/// if no container bindings were captured.

	void setTotalRows(std::size_t n);
		/// Sets totalRows explicitly. Used by callers that drive the binder
		/// row-by-row via existing AbstractBinding instances (where the
		/// binder receives scalar bind() calls rather than container ones,
		/// so cannot infer the row count from the value) and need to
		/// communicate the actual total so the truncation suffix renders
		/// correctly.

	std::size_t maxRows() const;
		/// Returns the maxRows cap passed to the constructor.

private:
	template <typename T>
	void recordScalar(std::size_t pos, const T& val, Direction dir);

	template <typename C>
	void recordContainer(std::size_t pos, const C& container, Direction dir);

	std::size_t _maxRows;
	std::map<std::size_t, std::vector<std::string>> _values;
	std::size_t _totalRows;
};


//
// inlines
//
inline std::size_t RenderingBinder::totalRows() const
{
	return _totalRows;
}


inline void RenderingBinder::setTotalRows(std::size_t n)
{
	_totalRows = n;
}


inline std::size_t RenderingBinder::maxRows() const
{
	return _maxRows;
}


} } // namespace Poco::Data


#endif // Data_RenderingBinder_INCLUDED
