//
// Encoder.cpp
//
// Library: PDF
// Package: PDFCore
// Module:  Encoder
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/PDF/Encoder.h"
#include "Poco/Exception.h"

#include <hpdf.h>

namespace Poco {
namespace PDF {


Encoder::Encoder(HPDF_Doc* pPDF,
	const HPDF_Encoder& encoder,
	const std::string& name):
	Resource<HPDF_Encoder>(pPDF, encoder, name)
{
}

Encoder::~Encoder() = default;

Encoder::Type Encoder::getType() const
{
	return static_cast<Type>(HPDF_Encoder_GetType(handle()));
}


Encoder::ByteType Encoder::getByteType(const std::string& text, int index) const
{
	if (index < 0)
		throw InvalidArgumentException("Negative values not allowed.");

	return static_cast<ByteType>(HPDF_Encoder_GetByteType(handle(),
														  text.c_str(),
														  static_cast<HPDF_UINT>(index)));
}


Encoder::WriteMode Encoder::writeMode()
{
	return static_cast<WriteMode>(HPDF_Encoder_GetWritingMode(handle()));
}



} } // namespace Poco::PDF
