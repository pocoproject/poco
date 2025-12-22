//
// Destination.cpp
//
// Library: PDF
// Package: PDFCore
// Module:  Destination
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/PDF/Destination.h"

#include <hpdf.h>

namespace Poco {
namespace PDF {


Destination::Destination(HPDF_Doc* pPDF,
	const HPDF_Destination& destination,
	const std::string& name):
	Resource<HPDF_Destination>(pPDF, destination, name)
{
}

Destination::~Destination() = default;

void Destination::positionAndZoom(float x, float y, float zoom)
{
	HPDF_Destination_SetXYZ(handle(), x, y, zoom);
}


void Destination::fit()
{
	HPDF_Destination_SetFit(handle());
}


void Destination::fitHorizontal(float top)
{
	HPDF_Destination_SetFitH(handle(), top);
}


void Destination::fitVertical(float left)
{
	HPDF_Destination_SetFitV(handle(), left);
}


void Destination::fitRectangle(float left, float top, float right, float bottom)
{
	HPDF_Destination_SetFitR(handle(), left, bottom, right, top);
}


void Destination::fitWindow()
{
	HPDF_Destination_SetFitB(handle());
}


void Destination::fitWindowHorizontal(float top)
{
	HPDF_Destination_SetFitBH(handle(), top);
}


void Destination::fitWindowVertical(float left)
{
	HPDF_Destination_SetFitBV(handle(), left);
}



} } // namespace Poco::PDF
