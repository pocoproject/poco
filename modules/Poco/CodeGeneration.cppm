//
// CodeGeneration.cppm
//
// C++ module file
//
// Copyright (c) 2026, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//

module;

#ifdef ENABLE_CODEGENERATION
#include "Poco/CodeGeneration/CodeGeneration.h"
#include "Poco/CodeGeneration/CodeGenerator.h"
#include "Poco/CodeGeneration/CppGenerator.h"
#include "Poco/CodeGeneration/GeneratorEngine.h"
#include "Poco/CodeGeneration/MethodPropertyFilter.h"
#include "Poco/CodeGeneration/Utility.h"
#endif

export module Poco:CodeGeneration;

export namespace Poco::CodeGeneration {
	#ifdef ENABLE_CODEGENERATION
	using Poco::CodeGeneration::CodeGenerator;
	using Poco::CodeGeneration::CppGenerator;
	using Poco::CodeGeneration::GeneratorEngine;
	using Poco::CodeGeneration::MethodPropertyFilter;
	using Poco::CodeGeneration::RecursionGuard;
	using Poco::CodeGeneration::Utility;
	#endif
}
