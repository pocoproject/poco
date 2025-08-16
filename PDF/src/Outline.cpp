//
// Outline.cpp
//
// Library: PDF
// Package: PDFCore
// Module:  Outline
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/PDF/Outline.h"
#include "Poco/PDF/PDFException.h"

#include <hpdf.h>

namespace Poco {
namespace PDF {


Outline::Outline(HPDF_Doc* pPDF, const HPDF_Outline& outline, const std::string& name):
	Resource<HPDF_Outline>(pPDF, outline, name)
{
	open();
}

Outline::~Outline() = default;

Outline::Outline(const Outline& other) = default;

Outline& Outline::operator=(const Outline& outline)
{
	Outline tmp(outline);
	swap(tmp);
	return *this;
}


void Outline::open()
{
	HPDF_Outline_SetOpened(handle(), HPDF_TRUE);
}


void Outline::close()
{
	HPDF_Outline_SetOpened(handle(), HPDF_FALSE);
}


void Outline::destination(const Destination& dest)
{
	HPDF_Outline_SetDestination(handle(), dest);
}


} } // namespace Poco::PDF
