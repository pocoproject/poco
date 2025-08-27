//
// Page.h
//
// Library: PDF
// Package: PDFCore
// Module:  Page
//
// Definition of the Page class.
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef PDF_Declarations_INCLUDED
#define PDF_Declarations_INCLUDED

#include <vector>

// HPDF forward declatations to avoid including hpdf.h in header files

struct _HPDF_TransMatrix;
using HPDF_TransMatrix =_HPDF_TransMatrix;

struct _HPDF_Rect;
using HPDF_Rect =_HPDF_Rect;

struct _HPDF_Point;
using HPDF_Point =_HPDF_Point;

//enum _HPDF_LineJoin;
//using HPDF_LineJoin =_HPDF_LineJoin;

struct _HPDF_DashMode;
using HPDF_DashMode =_HPDF_DashMode;

struct _HPDF_RGBColor;
using HPDF_RGBColor =_HPDF_RGBColor;

struct _HPDF_CMYKColor;
using HPDF_CMYKColor =_HPDF_CMYKColor;

struct _HPDF_TextWidth;
using HPDF_TextWidth = _HPDF_TextWidth;

struct _HPDF_Doc_Rec;
using HPDF_Doc_Rec = _HPDF_Doc_Rec;
using HPDF_Doc = _HPDF_Doc_Rec*;

using HPDF_BYTE = unsigned char;
using HPDF_UINT16 = unsigned short;
using HPDF_UINT32 = unsigned int;
using HPDF_STATUS = unsigned long;

struct _HPDF_Dict_Rec;
using HPDF_Dict = _HPDF_Dict_Rec*;

struct _HPDF_Array_Rec;
using HPDF_Array = _HPDF_Array_Rec*;

using HPDF_EmbeddedFile = HPDF_Dict;
using HPDF_NameDict = HPDF_Dict;
using HPDF_NameTree = HPDF_Dict;
using HPDF_Pages = HPDF_Dict;
using HPDF_Page = HPDF_Dict;
using HPDF_Annotation = HPDF_Dict;
using HPDF_3DMeasure = HPDF_Dict;
using HPDF_ExData = HPDF_Dict;
using HPDF_XObject = HPDF_Dict;
using HPDF_Image = HPDF_Dict;
using HPDF_Outline = HPDF_Dict;
using HPDF_EncryptDict = HPDF_Dict;
using HPDF_Action = HPDF_Dict;
using HPDF_ExtGState = HPDF_Dict;
using HPDF_Destination = HPDF_Array;
using HPDF_U3D = HPDF_Dict;
using HPDF_OutputIntent = HPDF_Dict;
using HPDF_JavaScript = HPDF_Dict;

using HPDF_Font = HPDF_Dict;

struct _HPDF_Encoder_Rec;
using HPDF_Encoder_Rec = _HPDF_Encoder_Rec;
using HPDF_Encoder = HPDF_Encoder_Rec*;

namespace Poco::PDF {

using TransMatrix = HPDF_TransMatrix;
using Rectangle = HPDF_Rect;
using Point = HPDF_Point;
using DashMode = HPDF_DashMode;
using RGBColor = HPDF_RGBColor;
using CMYKColor = HPDF_CMYKColor;
using PatternVec = std::vector<HPDF_UINT16>;
using TextWidth = HPDF_TextWidth;

// Note: constants must match the ones in hpdf_consts.h
static constexpr int HPDF_ENABLE_READ = 0;
static constexpr int HPDF_ENABLE_PRINT = 4;
static constexpr int HPDF_ENABLE_EDIT_ALL = 8;
static constexpr int HPDF_ENABLE_COPY = 16;
static constexpr int HPDF_ENABLE_EDIT = 32;

// Note: constants must match the ones in hpdf_consts.h
static constexpr int HPDF_COMP_NONE		= 0x00;
static constexpr int HPDF_COMP_TEXT		= 0x01;
static constexpr int HPDF_COMP_IMAGE	= 0x02;
static constexpr int HPDF_COMP_METADATA	= 0x04;
static constexpr int HPDF_COMP_ALL		= 0x0F;

// Note: values must match the ones in hpdf_types.h
enum LineCap {
	HPDF_BUTT_END = 0,
	HPDF_ROUND_END,
	HPDF_PROJECTING_SCUARE_END,
	HPDF_LINECAP_EOF
};

// Note: values must match the ones in hpdf_types.h
enum LineJoin {
	HPDF_MITER_JOIN = 0,
	HPDF_ROUND_JOIN,
	HPDF_BEVEL_JOIN,
	HPDF_LINEJOIN_EOF
};

}

#endif // PDF_Declarations_INCLUDED

