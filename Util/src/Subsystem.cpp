//
// Subsystem.cpp
//
// Library: Util
// Package: Application
// Module:  Subsystem
//
// Copyright (c) 2004-2025, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Util/Subsystem.h"


namespace Poco {
namespace Util {


void Subsystem::reinitialize(Application& app)
{
	uninitialize();
	initialize(app);
}


void Subsystem::defineOptions(OptionSet& options)
{
}


} } // namespace Poco::Util
