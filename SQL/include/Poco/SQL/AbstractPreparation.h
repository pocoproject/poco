//
// AbstractPreparation.h
//
// Library: Data
// Package: DataCore
// Module:  AbstractPreparation
//
// Definition of the AbstractPreparation class.
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef SQL_AbstractPreparation_INCLUDED
#define SQL_AbstractPreparation_INCLUDED


#include "Poco/SQL/SQL.h"
#include "Poco/SQL/AbstractPreparator.h"
#include "Poco/SharedPtr.h"
#include <cstddef>


namespace Poco {
namespace SQL {


class Poco_SQL_API AbstractPreparation
	/// Interface for calling the appropriate AbstractPreparator method
{
public:
	typedef SharedPtr<AbstractPreparation> Ptr;
	typedef AbstractPreparator::Ptr PreparatorPtr;

	AbstractPreparation(PreparatorPtr pPreparator);
		/// Creates the AbstractPreparation.

	virtual ~AbstractPreparation();
		/// Destroys the AbstractPreparation.

	virtual void prepare() = 0;
		/// Prepares data.

protected:
	AbstractPreparation();
	AbstractPreparation(const AbstractPreparation&);
	AbstractPreparation& operator = (const AbstractPreparation&);

	PreparatorPtr preparation();
		/// Returns the preparation object

	PreparatorPtr _pPreparator;
};


//
// inlines
//
inline AbstractPreparation::PreparatorPtr AbstractPreparation::preparation()
{
	return _pPreparator;
}


} } // namespace Poco::SQL


#endif // Data_AbstractPreparation_INCLUDED
