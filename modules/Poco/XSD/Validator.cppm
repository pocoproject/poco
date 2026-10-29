//
// Validator.cppm
//
// C++ module file
//
// Copyright (c) 2026, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//

module;

#ifdef ENABLE_XSD_VALIDATOR
#include "Poco/XSD/Validator/Validator.h"
#endif

export module Poco:XSD.Validator;

export namespace Poco::XSD::Validator {
	#ifdef ENABLE_XSD_VALIDATOR
	using Poco::XSD::Validator::Validator;
	#endif
}
