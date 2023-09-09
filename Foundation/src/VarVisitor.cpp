//
// VarVisitor.cpp
//
// Library: Foundation
// Package: Dynamic
// Module:  VarVisitor
//
//
// Copyright (c) 2023, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//

#include "Poco/Dynamic/VarVisitor.h"

namespace Poco {
namespace Dynamic {

bool Visitor::visit(const Var &x) const
{
	bool wasHendled = false;
	auto it = _handlers.find(x.type());
	if (it != _handlers.end())
	{
		it->second(x);
		wasHendled = true;
	}
	return wasHendled;
}

} } // namespace Poco::Dynamic
