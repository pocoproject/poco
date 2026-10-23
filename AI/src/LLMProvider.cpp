//
// LLMProvider.cpp
//
// Library: AI
// Package: Providers
// Module:  LLMProvider
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/AI/LLMProvider.h"
#include "Poco/Exception.h"


namespace Poco::AI {


LLMProvider::LLMProvider()
{
}


LLMProvider::~LLMProvider()
{
}


std::vector<std::string> LLMProvider::listModels()
{
	throw Poco::NotImplementedException("The provider has no model listing");
}


} // namespace Poco::AI
