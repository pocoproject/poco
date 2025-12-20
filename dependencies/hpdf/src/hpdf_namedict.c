/*
 * << Haru Free PDF Library >> -- hpdf_namedict.c
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

#include "hpdf_conf.h"
#include "hpdf_utils.h"
#include "hpdf_consts.h"
#include "hpdf_info.h"
#include "hpdf_namedict.h"
#include "hpdf.h"

#ifndef HPDF_UNUSED
#define HPDF_UNUSED(a) ((void)(a))
#endif

static const char * const HPDF_NAMEDICT_KEYS[] = {
                                        "EmbeddedFiles"
                                        };

HPDF_NameDict
HPDF_NameDict_New  (HPDF_MMgr  mmgr,
                    HPDF_Xref  xref)
{
    HPDF_NameDict ndict;

    HPDF_PTRACE((" HPDF_NameDict_New\n"));

    ndict = HPDF_Dict_New (mmgr);
    if (!ndict)
        return NULL;

    if (HPDF_Xref_Add (xref, ndict) != HPDF_OK)
        return NULL;

    ndict->header.obj_class |= HPDF_OSUBCLASS_NAMEDICT;

    return ndict;
}

HPDF_NameTree
HPDF_NameDict_GetNameTree  (HPDF_NameDict     namedict,
                            HPDF_NameDictKey  key)
{
    if (!namedict)
        return NULL;
    return HPDF_Dict_GetItem (namedict, HPDF_NAMEDICT_KEYS[key], HPDF_OCLASS_DICT);
}

HPDF_STATUS
HPDF_NameDict_SetNameTree  (HPDF_NameDict     namedict,
                            HPDF_NameDictKey  key,
                            HPDF_NameTree     ntree)
{
    return HPDF_Dict_Add (namedict, HPDF_NAMEDICT_KEYS[key], ntree);
}

HPDF_BOOL
HPDF_NameDict_Validate  (HPDF_NameDict  namedict)
{
    if (!namedict)
        return HPDF_FALSE;

    if (namedict->header.obj_class != (HPDF_OSUBCLASS_NAMEDICT |
                HPDF_OCLASS_DICT)) {
        HPDF_SetError (namedict->error, HPDF_INVALID_OBJECT, 0);
        return HPDF_FALSE;
    }

    return HPDF_TRUE;
}


/*------- NameTree -------*/

HPDF_NameTree
HPDF_NameTree_New  (HPDF_MMgr  mmgr,
                    HPDF_Xref  xref)
{
    HPDF_STATUS ret = HPDF_OK;
    HPDF_NameTree ntree;
    HPDF_Array items;

    HPDF_PTRACE((" HPDF_NameTree_New\n"));

    ntree = HPDF_Dict_New (mmgr);
    if (!ntree)
        return NULL;

    if (HPDF_Xref_Add (xref, ntree) != HPDF_OK)
        return NULL;

    ntree->header.obj_class |= HPDF_OSUBCLASS_NAMETREE;

    items = HPDF_Array_New (mmgr);
    if (!ntree)
        return NULL;

    ret += HPDF_Dict_Add (ntree, "Names", items);
    if (ret != HPDF_OK)
        return NULL;

    return ntree;
}

HPDF_STATUS
HPDF_NameTree_Add  (HPDF_NameTree  tree,
                    HPDF_String    name,
                    void          *obj)
{
    HPDF_Array items;
    HPDF_INT32 i, icount;

    if (!tree || !name)
        return HPDF_INVALID_PARAMETER;

    items = HPDF_Dict_GetItem (tree, "Names", HPDF_OCLASS_ARRAY);
    if (!items)
        return HPDF_INVALID_OBJECT;

    /* "The keys shall be sorted in lexical order" -- 7.9.6, Name Trees.
     * Since we store keys sorted, it's best to do a linear insertion sort
     * Find the first element larger than 'key', and insert 'key' and then
     * 'obj' into the items. */

    icount = HPDF_Array_Items(items);

    for (i = 0; i < icount; i += 2) {
        HPDF_String elem = HPDF_Array_GetItem( items, i, HPDF_OCLASS_STRING );
        if (HPDF_String_Cmp(name, elem) < 0) {
            HPDF_Array_Insert(items, elem, name);
            HPDF_Array_Insert(items, elem, obj);
            return HPDF_OK;
        }
    }

    /* Items list is empty */
    HPDF_Array_Add(items, name);
    HPDF_Array_Add(items, obj);
    return HPDF_OK;
}

HPDF_STATUS
HPDF_NameTree_Add_String  (HPDF_NameTree  tree,
                           const char    *name,
                           const char    *value)
{
    HPDF_String strname;
    HPDF_String strvalue;

    strname = HPDF_String_New(tree->mmgr, name, NULL);
    if (!strname)
        return HPDF_FAILED_TO_ALLOC_MEM;
    strvalue = HPDF_String_New(tree->mmgr, value, NULL);
    if (!strvalue) {
        HPDF_String_Free(strname);
        return HPDF_FAILED_TO_ALLOC_MEM;
    }
    return HPDF_NameTree_Add(tree, strname, strvalue);
}

HPDF_BOOL
HPDF_NameTree_Validate  (HPDF_NameTree  nametree)
{
    if (!nametree)
        return HPDF_FALSE;

    if (nametree->header.obj_class != (HPDF_OSUBCLASS_NAMETREE |
                HPDF_OCLASS_DICT)) {
        HPDF_SetError (nametree->error, HPDF_INVALID_OBJECT, 0);
        return HPDF_FALSE;
    }

    return HPDF_TRUE;
}


/*------- EmbeddedFile -------*/

HPDF_EmbeddedFile
HPDF_EmbeddedFile_New  (HPDF_MMgr  mmgr,
                        HPDF_Xref  xref,
                        const char *file)
{
    HPDF_STATUS ret = HPDF_OK;
    HPDF_Dict ef;               /* the dictionary for the embedded file: /Type /EF */
    HPDF_String name;           /* the name of the file: /F (name) */
    HPDF_String ufname;         /* the unicode name of the file: /UF (name) */
    HPDF_Dict eff;              /* ef has an /EF <<blah>> key - this is it */
    HPDF_Dict filestream;       /* the stream that /EF <</F _ _ R>> refers to */
    HPDF_Stream stream;

    ef = HPDF_Dict_New (mmgr);
    if (!ef)
        return NULL;
    if (HPDF_Xref_Add (xref, ef) != HPDF_OK)
        return NULL;

    filestream = HPDF_DictStream_New (mmgr, xref);
    if (!filestream)
        return NULL;
    stream = HPDF_FileReader_New (mmgr, file);
    if (!stream)
        return NULL;
    HPDF_Stream_Free(filestream->stream);
    filestream->stream = stream;
    filestream->filter = HPDF_STREAM_FILTER_FLATE_DECODE;

    eff = HPDF_Dict_New (mmgr);
    if (!eff)
        return NULL;

    name = HPDF_String_New (mmgr, file, NULL);
    if (!name)
        return NULL;

    ufname = HPDF_String_New (mmgr, file, NULL);
    if (!ufname)
        return NULL;

    ret += HPDF_Dict_AddName (ef, "Type", "Filespec");
    ret += HPDF_Dict_Add (ef, "F", name);
    ret += HPDF_Dict_Add (ef, "UF", ufname);
    ret += HPDF_Dict_Add (ef, "EF", eff);
    ret += HPDF_Dict_Add (eff, "F", filestream);
    ret += HPDF_Dict_AddName (filestream, "Type", "EmbeddedFile");

    if (ret != HPDF_OK)
        return NULL;

    return ef;
}

HPDF_BOOL
HPDF_EmbeddedFile_Validate  (HPDF_EmbeddedFile  emfile)
{
    HPDF_UNUSED (emfile);
    return HPDF_TRUE;
}

static HPDF_Dict
HPDF_EmbeddedFile_GetFileStream  (HPDF_EmbeddedFile  emfile)
{
    HPDF_Dict ef;

    ef = HPDF_Dict_GetItem(emfile, "EF", HPDF_OCLASS_DICT);
    if (!ef)
        return NULL;

    return  HPDF_Dict_GetItem(ef, "F", HPDF_OCLASS_DICT);
}

HPDF_EXPORT(HPDF_STATUS)
HPDF_EmbeddedFile_SetName  (HPDF_EmbeddedFile  emfile,
                            const char        *new_name)
{
    HPDF_Dict filestream;
    HPDF_String name;           /* the name of the file: /F (name) */
    HPDF_String ufname;         /* the unicode name of the file: /UF (name) */
    HPDF_STATUS status;

    filestream = HPDF_EmbeddedFile_GetFileStream(emfile);
    if (!filestream)
        return HPDF_INVALID_OBJECT;

    name = HPDF_String_New (emfile->mmgr, new_name, NULL);
    if (!name)
        return HPDF_FAILED_TO_ALLOC_MEM;

    ufname = HPDF_String_New (emfile->mmgr, new_name, NULL);
    if (!ufname)
        return HPDF_FAILED_TO_ALLOC_MEM;

    status = HPDF_Dict_Add (emfile, "F", name);
    if (status == HPDF_OK)
        status = HPDF_Dict_Add (emfile, "UF", ufname);

    return status;
}

HPDF_EXPORT(HPDF_STATUS)
HPDF_EmbeddedFile_SetDescription (HPDF_EmbeddedFile  emfile,
                                  const char        *new_description)
{
    HPDF_Dict filestream;
    HPDF_String description;           /* the name of the file: /F (name) */

    filestream = HPDF_EmbeddedFile_GetFileStream(emfile);
    if (!filestream)
        return HPDF_INVALID_OBJECT;

    description = HPDF_String_New (emfile->mmgr, new_description, NULL);
    if (!description)
        return HPDF_FAILED_TO_ALLOC_MEM;

    return HPDF_Dict_Add (emfile, "Desc", description);
}

HPDF_EXPORT(HPDF_STATUS)
HPDF_EmbeddedFile_SetSubtype  (HPDF_EmbeddedFile  emfile,
                               const char        *subtype)
{
    HPDF_Dict filestream;

    filestream = HPDF_EmbeddedFile_GetFileStream(emfile);
    if (!filestream)
        return HPDF_INVALID_OBJECT;

    return HPDF_Dict_AddName (filestream, "Subtype", subtype);
}

HPDF_EXPORT(HPDF_STATUS)
HPDF_EmbeddedFile_SetAFRelationship  (HPDF_EmbeddedFile    emfile,
                                      HPDF_AFRelationship  relationship)
{
    // This array has to be kept coherent with HPDF_AFRelationship enum.
    static const char *relationships[] = {
        "Source",
        "Data",
        "Alternative",
        "Supplement",
        "EncryptedPayload",
        "FormData",
        "Schema",
        "Unspecified"
      };
    const char *str_relationship = NULL;

    if (relationship < 0 || relationship > HPDF_AFRELATIONSHIP_UNSPECIFIED) {
        return HPDF_INVALID_PARAMETER;
    }
    return HPDF_Dict_AddName(emfile, "AFRelationship", relationships[relationship]);
}

static HPDF_Dict
HPDF_EmbeddedFile_GetParams  (HPDF_EmbeddedFile  emfile)
{
    HPDF_Dict filestream;
    HPDF_Dict params;

    filestream = HPDF_EmbeddedFile_GetFileStream(emfile);
    if (!filestream)
        return NULL;

    params = HPDF_Dict_GetItem(filestream, "Params", HPDF_OCLASS_DICT);
    if (!params) {
        params = HPDF_Dict_New (emfile->mmgr);
        if (!params || HPDF_Dict_Add (filestream, "Params", params) != HPDF_OK)
            return NULL;
    }
    return params;
}

HPDF_EXPORT(HPDF_STATUS)
HPDF_EmbeddedFile_SetSize  (HPDF_EmbeddedFile  emfile,
                            HPDF_UINT64        size)
{
    HPDF_Dict params;

    params = HPDF_EmbeddedFile_GetParams(emfile);
    if (!params)
        return emfile->error->error_no;

    return HPDF_Dict_AddNumber(params, "Size", (HPDF_INT32)size);
}

HPDF_EXPORT(HPDF_STATUS)
HPDF_EmbeddedFile_SetCreationDate  (HPDF_EmbeddedFile  emfile,
                                    HPDF_Date          creationDate)
{
    HPDF_Dict params;

    params = HPDF_EmbeddedFile_GetParams(emfile);
    if (!params)
        return emfile->error->error_no;

    return HPDF_Info_SetInfoDateAttr(params, HPDF_INFO_CREATION_DATE, creationDate);
}

HPDF_EXPORT(HPDF_STATUS)
HPDF_EmbeddedFile_SetLastModificationDate  (HPDF_EmbeddedFile  emfile,
                                            HPDF_Date          lastModificationDate)
{
    HPDF_Dict params;

    params = HPDF_EmbeddedFile_GetParams(emfile);
    if (!params)
        return emfile->error->error_no;

    return HPDF_Info_SetInfoDateAttr(params, HPDF_INFO_MOD_DATE, lastModificationDate);
}
