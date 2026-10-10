/*
 * << Haru Free PDF Library >> -- hpdf_u3d.h
 *
 * URL: http://libharu.org
 *
 * Copyright (c) 1999-2006 Takeshi Kanno <takeshi_kanno@est.hi-ho.ne.jp>
 * Copyright (c) 2007-2009 Antony Dovgal <tony@daylessday.org>
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.
 * It is provided "as is" without express or implied warranty.
 *
 */

#ifndef _HPDF_U3D_H
#define _HPDF_U3D_H

#include "hpdf_objects.h"

#ifdef __cplusplus
extern "C" {
#endif

HPDF_EXPORT(HPDF_JavaScript) HPDF_CreateJavaScript(HPDF_Doc pdf, const char *code);
HPDF_EXPORT(HPDF_JavaScript) HPDF_LoadJSFromFile  (HPDF_Doc pdf, const char *filename);


HPDF_EXPORT(HPDF_U3D) HPDF_LoadU3DFromFile (HPDF_Doc pdf, const char *filename);
HPDF_EXPORT(HPDF_Image) HPDF_LoadU3DFromMem (HPDF_Doc pdf, const HPDF_BYTE *buffer, HPDF_UINT size);
HPDF_EXPORT(HPDF_Dict) HPDF_Create3DView (HPDF_MMgr mmgr, const char *name);
HPDF_EXPORT(HPDF_STATUS) HPDF_U3D_Add3DView(HPDF_U3D u3d, HPDF_Dict view);
HPDF_EXPORT(HPDF_STATUS) HPDF_U3D_SetDefault3DView(HPDF_U3D u3d, const char *name);
HPDF_EXPORT(HPDF_STATUS) HPDF_U3D_AddOnInstanciate(HPDF_U3D u3d, HPDF_JavaScript javaScript);
HPDF_EXPORT(HPDF_Dict) HPDF_3DView_CreateNode(HPDF_Dict view, const char *name);
HPDF_EXPORT(HPDF_STATUS) HPDF_3DViewNode_SetOpacity(HPDF_Dict node, HPDF_REAL opacity);
HPDF_EXPORT(HPDF_STATUS) HPDF_3DViewNode_SetVisibility(HPDF_Dict node, HPDF_BOOL visible);
HPDF_EXPORT(HPDF_STATUS) HPDF_3DViewNode_SetMatrix(HPDF_Dict node, HPDF_3DMatrix Mat3D);
HPDF_EXPORT(HPDF_STATUS) HPDF_3DView_AddNode(HPDF_Dict view, HPDF_Dict node);
HPDF_EXPORT(HPDF_STATUS) HPDF_3DView_SetLighting(HPDF_Dict view, const char *scheme);
HPDF_EXPORT(HPDF_STATUS) HPDF_3DView_SetBackgroundColor(HPDF_Dict view, HPDF_REAL r, HPDF_REAL g, HPDF_REAL b);
HPDF_EXPORT(HPDF_STATUS) HPDF_3DView_SetPerspectiveProjection(HPDF_Dict view, HPDF_REAL fov);
HPDF_EXPORT(HPDF_STATUS) HPDF_3DView_SetOrthogonalProjection(HPDF_Dict view, HPDF_REAL mag);
HPDF_EXPORT(HPDF_STATUS) HPDF_3DView_SetCamera(HPDF_Dict view, HPDF_REAL coox, HPDF_REAL cooy, HPDF_REAL cooz, HPDF_REAL c2cx, HPDF_REAL c2cy, HPDF_REAL c2cz, HPDF_REAL roo, HPDF_REAL roll);
HPDF_EXPORT(HPDF_STATUS) HPDF_3DView_SetCameraByMatrix(HPDF_Dict view, HPDF_3DMatrix Mat3D, HPDF_REAL co);
HPDF_EXPORT(HPDF_STATUS) HPDF_3DView_SetCrossSectionOn(HPDF_Dict view, HPDF_Point3D center, HPDF_REAL Roll, HPDF_REAL Pitch, HPDF_REAL opacity, HPDF_BOOL  showintersection);
HPDF_EXPORT(HPDF_STATUS) HPDF_3DView_SetCrossSectionOff(HPDF_Dict view);

HPDF_Dict
HPDF_3DView_New    ( HPDF_MMgr  mmgr,
					 HPDF_Xref  xref,
					 HPDF_U3D	u3d,
					 const char *name);
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _HPDF_U3D_H */

