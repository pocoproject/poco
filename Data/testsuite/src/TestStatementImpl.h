//
// TestStatementImpl.h
//
// Definition of the TestStatementImpl class.
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Data_Test_TestStatementImpl_INCLUDED
#define Data_Test_TestStatementImpl_INCLUDED


#include "Poco/Data/StatementImpl.h"
#include "Poco/Data/MetaColumn.h"
#include "Poco/SharedPtr.h"
#include "Poco/Event.h"
#include "Binder.h"
#include "Extractor.h"
#include "Preparator.h"
#include <atomic>


namespace Poco::Data::Test {


class SessionImpl;


class TestStatementImpl: public Poco::Data::StatementImpl
	/// A no-op implementation of TestStatementImpl for testing.
{
public:
	enum class HoldPoint
		/// Where holdNextExecution() makes the next execution wait. The first call
		/// at that point, on any thread, takes the hold.
	{
		COMPILE,
			/// In compileImpl(), before the execution has changed the state.
		HAS_NEXT,
			/// In hasNext(), while the state is bound.
		AFFECTED_ROW_COUNT
			/// In affectedRowCount(), which an execution with a limit calls
			/// after it has set the state to done or paused.
	};

	TestStatementImpl(SessionImpl& rSession, bool throwOnHasNext = false);
		/// Creates the TestStatementImpl.

	~TestStatementImpl();
		/// Destroys the TestStatementImpl.

	[[nodiscard]] static int instances();
		/// Returns the number of TestStatementImpl objects alive.

	static void holdNextExecution(HoldPoint point = HoldPoint::HAS_NEXT);
		/// Makes the next execution of a TestStatementImpl wait at the given point
		/// until releaseExecution() is called.

	[[nodiscard]] static bool waitUntilHeld(long milliseconds);
		/// Waits until the execution held by holdNextExecution() waits.
		/// Returns false if it does not within the given time.

	static void releaseExecution();
		/// Lets the held execution go on, and cancels a hold that no execution has reached yet.

protected:
	std::size_t columnsReturned() const;
		/// Returns number of columns returned by query.

	int affectedRowCount() const;
		/// Returns the number of affected rows.
		/// Used to find out the number of rows affected by insert or update.

	const MetaColumn& metaColumn(std::size_t pos) const;
		/// Returns column meta data.

	bool hasNext();
		/// Returns true if a call to next() will return data.

	std::size_t next();
		/// Retrieves the next row or set of rows from the resultset.
		/// Will throw, if the resultset is empty.

	bool canBind() const;
		/// Returns true if a valid statement is set and we can bind.

	bool canCompile() const;
		/// Returns true if statement was not compiled.

	void compileImpl();
		/// Compiles the statement, doesn't bind yet

	void bindImpl();
		/// Binds parameters

	AbstractExtraction::ExtractorPtr extractor();
		/// Returns the concrete extractor used by the statement.

	AbstractBinding::BinderPtr binder();
		/// Returns the concrete binder used by the statement.

private:
	static void waitIfHeld(HoldPoint point);
		/// Waits until releaseExecution() is called if the next execution
		/// is to be held at the given point.

	Poco::SharedPtr<Binder>     _ptrBinder;
	Poco::SharedPtr<Extractor>  _ptrExtractor;
	Poco::SharedPtr<Preparator> _ptrPreparation;
	bool                        _compiled;
	bool                        _throwOnHasNext = false;

	static std::atomic<int>     _instances;
	static std::atomic<bool>    _holdNext;
	static std::atomic<HoldPoint> _holdPoint;
	static Poco::Event          _held;
	static Poco::Event          _released;
};


//
// inlines
//
inline AbstractExtraction::ExtractorPtr TestStatementImpl::extractor()
{
	return _ptrExtractor;
}


inline AbstractBinding::BinderPtr TestStatementImpl::binder()
{
	return _ptrBinder;
}


inline bool TestStatementImpl::canCompile() const
{
	return !_compiled;
}


} // namespace Poco::Data::Test


#endif // Data_Test_TestStatementImpl_INCLUDED
