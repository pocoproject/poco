/*
 * << Haru Free PDF Library >> -- hpdf_fontdef_cnt.c
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

#include "hpdf.h"
#include "hpdf_conf.h"
#include "hpdf_utils.h"

/*----------------------------------------------------------------------------*/
/* According to https://github.com/adobe-type-tools/Adobe-CNS1/
* the following non-default widths are used:
* 1-98, 17601 proportional
* 13648–13742, 17603 half-width
*/
static const HPDF_CID_Width MING_LIU_W_ARRAY[] = {
    {    1, 500},
    {    2, 500},
    {    3, 500},
    {    4, 500},
    {    5, 500},
    {    6, 500},
    {    7, 500},
    {    8, 500},
    {    9, 500},
    {   10, 500},
    {   11, 500},
    {   12, 500},
    {   13, 500},
    {   14, 500},
    {   15, 500},
    {   16, 500},
    {   17, 500},
    {   18, 500},
    {   19, 500},
    {   20, 500},
    {   21, 500},
    {   22, 500},
    {   23, 500},
    {   24, 500},
    {   25, 500},
    {   26, 500},
    {   27, 500},
    {   28, 500},
    {   29, 500},
    {   30, 500},
    {   31, 500},
    {   32, 500},
    {   33, 500},
    {   34, 500},
    {   35, 500},
    {   36, 500},
    {   37, 500},
    {   38, 500},
    {   39, 500},
    {   40, 500},
    {   41, 500},
    {   42, 500},
    {   43, 500},
    {   44, 500},
    {   45, 500},
    {   46, 500},
    {   47, 500},
    {   48, 500},
    {   49, 500},
    {   50, 500},
    {   51, 500},
    {   52, 500},
    {   53, 500},
    {   54, 500},
    {   55, 500},
    {   56, 500},
    {   57, 500},
    {   58, 500},
    {   59, 500},
    {   60, 500},
    {   61, 500},
    {   62, 500},
    {   63, 500},
    {   64, 500},
    {   65, 500},
    {   66, 500},
    {   67, 500},
    {   68, 500},
    {   69, 500},
    {   70, 500},
    {   71, 500},
    {   72, 500},
    {   73, 500},
    {   74, 500},
    {   75, 500},
    {   76, 500},
    {   77, 500},
    {   78, 500},
    {   79, 500},
    {   80, 500},
    {   81, 500},
    {   82, 500},
    {   83, 500},
    {   84, 500},
    {   85, 500},
    {   86, 500},
    {   87, 500},
    {   88, 500},
    {   89, 500},
    {   90, 500},
    {   91, 500},
    {   92, 500},
    {   93, 500},
    {   94, 500},
    {   95, 500},
    {   96, 500},
    {   97, 500},
    {   98, 500},
    {13648, 500},
    {13649, 500},
    {13650, 500},
    {13651, 500},
    {13652, 500},
    {13653, 500},
    {13654, 500},
    {13655, 500},
    {13656, 500},
    {13657, 500},
    {13658, 500},
    {13659, 500},
    {13660, 500},
    {13661, 500},
    {13662, 500},
    {13663, 500},
    {13664, 500},
    {13665, 500},
    {13666, 500},
    {13667, 500},
    {13668, 500},
    {13669, 500},
    {13670, 500},
    {13671, 500},
    {13672, 500},
    {13673, 500},
    {13674, 500},
    {13675, 500},
    {13676, 500},
    {13677, 500},
    {13678, 500},
    {13679, 500},
    {13680, 500},
    {13681, 500},
    {13682, 500},
    {13683, 500},
    {13684, 500},
    {13685, 500},
    {13686, 500},
    {13687, 500},
    {13688, 500},
    {13689, 500},
    {13690, 500},
    {13691, 500},
    {13692, 500},
    {13693, 500},
    {13694, 500},
    {13695, 500},
    {13696, 500},
    {13697, 500},
    {13698, 500},
    {13699, 500},
    {13700, 500},
    {13701, 500},
    {13702, 500},
    {13703, 500},
    {13704, 500},
    {13705, 500},
    {13706, 500},
    {13707, 500},
    {13708, 500},
    {13709, 500},
    {13710, 500},
    {13711, 500},
    {13712, 500},
    {13713, 500},
    {13714, 500},
    {13715, 500},
    {13716, 500},
    {13717, 500},
    {13718, 500},
    {13719, 500},
    {13720, 500},
    {13721, 500},
    {13722, 500},
    {13723, 500},
    {13724, 500},
    {13725, 500},
    {13726, 500},
    {13727, 500},
    {13728, 500},
    {13729, 500},
    {13730, 500},
    {13731, 500},
    {13732, 500},
    {13733, 500},
    {13734, 500},
    {13735, 500},
    {13736, 500},
    {13737, 500},
    {13738, 500},
    {13739, 500},
    {13740, 500},
    {13741, 500},
    {13742, 500},
    {17601, 500},
    {17603, 500},
    {0xFFFF, 0}
};


static HPDF_STATUS
MingLiU_Init  (HPDF_FontDef   fontdef)
{
    HPDF_STATUS ret;

    HPDF_PTRACE ((" HPDF_FontDef_MingLiU_Init\n"));

    fontdef->ascent = 800;
    fontdef->descent = -199;
    fontdef->cap_height = 769;
    fontdef->font_bbox = HPDF_ToBox(0, -199, 1000, 800);
    fontdef->flags = HPDF_FONT_SYMBOLIC + HPDF_FONT_FIXED_WIDTH +
                HPDF_FONT_SERIF;
    fontdef->italic_angle = 0;
    fontdef->stemv = 78;
    if ((ret = HPDF_CIDFontDef_AddWidth (fontdef, MING_LIU_W_ARRAY)) !=
                HPDF_OK) {
        return ret;
    }

    fontdef->type = HPDF_FONTDEF_TYPE_CID;
    fontdef->valid = HPDF_TRUE;

    return HPDF_OK;
}


static HPDF_STATUS
MingLiU_Bold_Init  (HPDF_FontDef   fontdef)
{
    HPDF_STATUS ret = MingLiU_Init (fontdef);

    if (ret != HPDF_OK)
        return ret;

    return HPDF_CIDFontDef_ChangeStyle (fontdef, HPDF_TRUE, HPDF_FALSE);
}


static HPDF_STATUS
MingLiU_Italic_Init  (HPDF_FontDef   fontdef)
{
    HPDF_STATUS ret = MingLiU_Init (fontdef);

    if (ret != HPDF_OK)
        return ret;

    return HPDF_CIDFontDef_ChangeStyle (fontdef, HPDF_FALSE, HPDF_TRUE);
}

static HPDF_STATUS
MingLiU_BoldItalic_Init  (HPDF_FontDef   fontdef)
{
    HPDF_STATUS ret = MingLiU_Init (fontdef);

    if (ret != HPDF_OK)
        return ret;

    return HPDF_CIDFontDef_ChangeStyle (fontdef, HPDF_TRUE, HPDF_TRUE);
}


HPDF_EXPORT(HPDF_STATUS)
HPDF_UseCNTFonts   (HPDF_Doc   pdf)
{
    HPDF_FontDef fontdef;
    HPDF_STATUS ret;

    if (!HPDF_HasDoc (pdf))
        return HPDF_INVALID_DOCUMENT;

    /* MingLiU */
    fontdef = HPDF_CIDFontDef_New (pdf->mmgr,  "MingLiU",
                MingLiU_Init);

    if ((ret = HPDF_Doc_RegisterFontDef (pdf, fontdef)) != HPDF_OK)
        return ret;

    fontdef = HPDF_CIDFontDef_New (pdf->mmgr,  "MingLiU,Bold",
                MingLiU_Bold_Init);

    if ((ret = HPDF_Doc_RegisterFontDef (pdf, fontdef)) != HPDF_OK)
        return ret;

    fontdef = HPDF_CIDFontDef_New (pdf->mmgr,  "MingLiU,Italic",
                MingLiU_Italic_Init);

    if ((ret = HPDF_Doc_RegisterFontDef (pdf, fontdef)) != HPDF_OK)
        return ret;

    fontdef = HPDF_CIDFontDef_New (pdf->mmgr,  "MingLiU,BoldItalic",
                MingLiU_BoldItalic_Init);

    if ((ret = HPDF_Doc_RegisterFontDef (pdf, fontdef)) != HPDF_OK)
        return ret;

    return HPDF_OK;
}

