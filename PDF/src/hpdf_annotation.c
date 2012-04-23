/*
 * << Haru Free PDF Library 2.0.0 >> -- hpdf_annotation.c
 *
 * Copyright (c) 1999-2006 Takeshi Kanno <takeshi_kanno@est.hi-ho.ne.jp>
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.
 * It is provided "as is" without express or implied warranty.
 *
 */

#include "hpdf_conf.h"
#include "hpdf_utils.h"
#include "hpdf_annotation.h"
#include "hpdf.h"

const static char  *HPDF_ANNOT_TYPE_NAMES[] = {
                                        "Text",
                                        "Link",
                                        "Sound",
                                        "FreeText",
                                        "Stamp",
                                        "Square",
                                        "Circle",
                                        "StrikeOut",
                                        "Highlight",
                                        "Underline",
                                        "Ink",
                                        "FileAttachment",
                                        "Popup"
                                        };

const static char  *HPDF_ANNOT_ICON_NAMES_NAMES[] = {
                                        "Comment",
                                        "Key",
                                        "Note",
                                        "Help",
                                        "NewParagraph",
                                        "Paragraph",
                                        "Insert"
                                        };


static HPDF_BOOL
CheckSubType (HPDF_Annotation  annot,
              HPDF_AnnotType  type);


/*----------------------------------------------------------------------------*/
/*------ HPDF_Annotation -----------------------------------------------------*/


HPDF_Annotation
HPDF_Annotation_New  (HPDF_MMgr       mmgr,
                      HPDF_Xref       xref,
                      HPDF_AnnotType  type,
                      HPDF_Rect       rect)
{
    HPDF_Annotation annot;
    HPDF_Array array;
    HPDF_STATUS ret = HPDF_OK;
    HPDF_REAL tmp;

    HPDF_PTRACE((" HPDF_Annotation_New\n"));

    annot = HPDF_Dict_New (mmgr);
    if (!annot)
        return NULL;

    if (HPDF_Xref_Add (xref, annot) != HPDF_OK)
        return NULL;

    array = HPDF_Array_New (mmgr);
    if (!array)
        return NULL;

    if (HPDF_Dict_Add (annot, "Rect", array) != HPDF_OK)
        return NULL;

    if (rect.top < rect.bottom) {
        tmp = rect.top;
        rect.top = rect.bottom;
        rect.bottom = tmp;
    }

    ret += HPDF_Array_AddReal (array, rect.left);
    ret += HPDF_Array_AddReal (array, rect.bottom);
    ret += HPDF_Array_AddReal (array, rect.right);
    ret += HPDF_Array_AddReal (array, rect.top);

    ret += HPDF_Dict_AddName (annot, "Type", "Annot");
    ret += HPDF_Dict_AddName (annot, "Subtype",
                    HPDF_ANNOT_TYPE_NAMES[(HPDF_INT)type]);

    if (ret != HPDF_OK)
        return NULL;

    annot->header.obj_class |= HPDF_OSUBCLASS_ANNOTATION;

    return annot;
}


HPDF_STATUS
HPDF_Annotation_SetBorderStyle  (HPDF_Annotation  annot,
                                 HPDF_BSSubtype   subtype,
                                 HPDF_REAL        width,
                                 HPDF_UINT16      dash_on,
                                 HPDF_UINT16      dash_off,
                                 HPDF_UINT16      dash_phase)
{
    HPDF_Dict bs;
    HPDF_Array dash;
    HPDF_STATUS ret;

    HPDF_PTRACE((" HPDF_Annotation_SetBoderStyle\n"));

    bs = HPDF_Dict_New (annot->mmgr);
    if (!bs)
        return HPDF_Error_GetCode (annot->error);

    if ((ret = HPDF_Dict_Add (annot, "BS", bs)) != HPDF_OK)
        return ret;

    if (subtype == HPDF_BS_DASHED) {
        dash = HPDF_Array_New (annot->mmgr);
        if (!dash)
            return HPDF_Error_GetCode (annot->error);

        if ((ret = HPDF_Dict_Add (bs, "D", dash)) != HPDF_OK)
            return ret;

        ret += HPDF_Dict_AddName (bs, "Type", "Border");
        ret += HPDF_Array_AddReal (dash, dash_on);
        ret += HPDF_Array_AddReal (dash, dash_off);

        if (dash_phase  != 0)
            ret += HPDF_Array_AddReal (dash, dash_off);
    }

    switch (subtype) {
        case HPDF_BS_SOLID:
            ret += HPDF_Dict_AddName (bs, "S", "S");
            break;
        case HPDF_BS_DASHED:
            ret += HPDF_Dict_AddName (bs, "S", "D");
            break;
        case HPDF_BS_BEVELED:
            ret += HPDF_Dict_AddName (bs, "S", "B");
            break;
        case HPDF_BS_INSET:
            ret += HPDF_Dict_AddName (bs, "S", "I");
            break;
        case HPDF_BS_UNDERLINED:
            ret += HPDF_Dict_AddName (bs, "S", "U");
            break;
        default:
            return  HPDF_SetError (annot->error, HPDF_ANNOT_INVALID_BORDER_STYLE, 0);
    }

    if (width != HPDF_BS_DEF_WIDTH)
        ret += HPDF_Dict_AddReal (bs, "W", width);

    if (ret != HPDF_OK)
        return HPDF_Error_GetCode (annot->error);

    return HPDF_OK;
}


HPDF_Annotation
HPDF_LinkAnnot_New  (HPDF_MMgr         mmgr,
                     HPDF_Xref         xref,
                     HPDF_Rect         rect,
                     HPDF_Destination  dst)
{
    HPDF_Annotation annot;

    HPDF_PTRACE((" HPDF_LinkAnnot_New\n"));

    annot = HPDF_Annotation_New (mmgr, xref, HPDF_ANNOT_LINK, rect);
    if (!annot)
        return NULL;

    if (HPDF_Dict_Add (annot, "Dest", dst) != HPDF_OK)
        return NULL;

    return annot;
}


HPDF_Annotation
HPDF_URILinkAnnot_New  (HPDF_MMgr          mmgr,
                        HPDF_Xref          xref,
                        HPDF_Rect          rect,
                        const char   *uri)
{
    HPDF_Annotation annot;
    HPDF_Dict action;
    HPDF_STATUS ret;

    HPDF_PTRACE((" HPDF_URILinkAnnot_New\n"));

    annot = HPDF_Annotation_New (mmgr, xref, HPDF_ANNOT_LINK, rect);
    if (!annot)
        return NULL;

    /* create action dictionary */
    action = HPDF_Dict_New (mmgr);
    if (!action)
        return NULL;

    ret = HPDF_Dict_Add (annot, "A", action);
    if (ret != HPDF_OK)
        return NULL;

    ret += HPDF_Dict_AddName (action, "Type", "Action");
    ret += HPDF_Dict_AddName (action, "S", "URI");
    ret += HPDF_Dict_Add (action, "URI", HPDF_String_New (mmgr, uri, NULL));

    if (ret != HPDF_OK)
        return NULL;

    return annot;
}

HPDF_EXPORT(HPDF_STATUS)
HPDF_LinkAnnot_SetBorderStyle  (HPDF_Annotation  annot,
                                HPDF_REAL        width,
                                HPDF_UINT16      dash_on,
                                HPDF_UINT16      dash_off)
{
    HPDF_Array array;
    HPDF_STATUS ret;

    HPDF_PTRACE((" HPDF_LinkAnnot_SetBorderStyle\n"));

    if (!CheckSubType (annot, HPDF_ANNOT_LINK))
        return HPDF_INVALID_ANNOTATION;

    if (width < 0)
        return HPDF_RaiseError (annot->error, HPDF_INVALID_PARAMETER, 0);

    array = HPDF_Array_New (annot->mmgr);
    if (!array)
        return HPDF_CheckError (annot->error);

    if ((ret = HPDF_Dict_Add (annot, "Border", array)) != HPDF_OK)
        return HPDF_CheckError (annot->error);

    ret += HPDF_Array_AddNumber (array, 0);
    ret += HPDF_Array_AddNumber (array, 0);
    ret += HPDF_Array_AddReal (array, width);

    if (ret != HPDF_OK)
        return HPDF_CheckError (annot->error);

    if (dash_on && dash_off) {
        HPDF_Array dash = HPDF_Array_New (annot->mmgr);
        if (!dash)
            return HPDF_CheckError (annot->error);

        if ((ret = HPDF_Array_Add (array, dash)) != HPDF_OK)
            return HPDF_CheckError (annot->error);

        ret += HPDF_Array_AddNumber (dash, dash_on);
        ret += HPDF_Array_AddNumber (dash, dash_off);
    }

    if (ret != HPDF_OK)
        return HPDF_CheckError (annot->error);

    return ret;
}

HPDF_EXPORT(HPDF_STATUS)
HPDF_LinkAnnot_SetHighlightMode  (HPDF_Annotation           annot,
                                  HPDF_AnnotHighlightMode  mode)
{
    HPDF_STATUS ret;

    HPDF_PTRACE((" HPDF_LinkAnnot_SetHighlightMode\n"));

    if (!CheckSubType (annot, HPDF_ANNOT_LINK))
        return HPDF_INVALID_ANNOTATION;

    switch (mode) {
        case HPDF_ANNOT_NO_HIGHTLIGHT:
            ret = HPDF_Dict_AddName (annot, "H", "N");
            break;
        case HPDF_ANNOT_INVERT_BORDER:
            ret = HPDF_Dict_AddName (annot, "H", "O");
            break;
        case HPDF_ANNOT_DOWN_APPEARANCE:
            ret = HPDF_Dict_AddName (annot, "H", "P");
            break;
        default:  /* HPDF_ANNOT_INVERT_BOX */
            /* default value */
            HPDF_Dict_RemoveElement (annot, "H");
            ret = HPDF_OK;
    }

    if (ret != HPDF_OK)
        return HPDF_CheckError (annot->error);

    return ret;
}

HPDF_Annotation
HPDF_TextAnnot_New  (HPDF_MMgr       mmgr,
                     HPDF_Xref       xref,
                     HPDF_Rect       rect,
                     const char     *text,
                     HPDF_Encoder    encoder)
{
    HPDF_Annotation annot;
    HPDF_String s;

    HPDF_PTRACE((" HPDF_TextAnnot_New\n"));

    annot = HPDF_Annotation_New (mmgr, xref, HPDF_ANNOT_TEXT_NOTES, rect);
    if (!annot)
        return NULL;

    s = HPDF_String_New (mmgr, text, encoder);
    if (!s)
        return NULL;

    if (HPDF_Dict_Add (annot, "Contents", s) != HPDF_OK)
        return NULL;

    return annot;
}


HPDF_EXPORT(HPDF_STATUS)
HPDF_TextAnnot_SetIcon  (HPDF_Annotation  annot,
                         HPDF_AnnotIcon   icon)
{
    HPDF_PTRACE((" HPDF_TextAnnot_SetIcon\n"));

    if (!CheckSubType (annot, HPDF_ANNOT_TEXT_NOTES))
        return HPDF_INVALID_ANNOTATION;

    if (icon < 0 || icon >= HPDF_ANNOT_ICON_EOF)
        return HPDF_RaiseError (annot->error, HPDF_ANNOT_INVALID_ICON,
                (HPDF_STATUS)icon);

    if (HPDF_Dict_AddName (annot, "Name",
        HPDF_ANNOT_ICON_NAMES_NAMES[(HPDF_INT)icon]) != HPDF_OK)
        return HPDF_CheckError (annot->error);

    return HPDF_OK;
}


HPDF_EXPORT(HPDF_STATUS)
HPDF_TextAnnot_SetOpened  (HPDF_Annotation  annot,
                           HPDF_BOOL        opened)
{
    HPDF_Boolean b;

    HPDF_PTRACE((" HPDF_TextAnnot_SetOpend\n"));

    if (!CheckSubType (annot, HPDF_ANNOT_TEXT_NOTES))
        return HPDF_INVALID_ANNOTATION;

    b = HPDF_Boolean_New (annot->mmgr, opened);
    if (!b)
        return HPDF_CheckError (annot->error);

    return  HPDF_Dict_Add (annot, "Open", b);
}


HPDF_BOOL
HPDF_Annotation_Validate (HPDF_Annotation  annot)
{
    HPDF_PTRACE((" HPDF_Annotation_Validate\n"));

    if (!annot)
        return HPDF_FALSE;

    if (annot->header.obj_class !=
                (HPDF_OSUBCLASS_ANNOTATION | HPDF_OCLASS_DICT))
        return HPDF_FALSE;

    return HPDF_TRUE;
}

static HPDF_BOOL
CheckSubType (HPDF_Annotation  annot,
              HPDF_AnnotType  type)
{
    HPDF_Name subtype;

    HPDF_PTRACE((" HPDF_Annotation_CheckSubType\n"));

    if (!HPDF_Annotation_Validate (annot))
        return HPDF_FALSE;

    subtype = HPDF_Dict_GetItem (annot, "Subtype", HPDF_OCLASS_NAME);

    if (!subtype || HPDF_StrCmp (subtype->value,
                HPDF_ANNOT_TYPE_NAMES[(HPDF_INT)type]) != 0) {
        HPDF_RaiseError (annot->error, HPDF_INVALID_ANNOTATION, 0);
        return HPDF_FALSE;
    }

    return HPDF_TRUE;
}

