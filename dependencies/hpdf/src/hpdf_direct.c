#include "hpdf_conf.h"
#include "hpdf_utils.h"
#include "hpdf_objects.h"


HPDF_Direct
HPDF_Direct_New  (HPDF_MMgr  mmgr,
                  HPDF_BYTE  *value,
                  HPDF_UINT  len)
{
    HPDF_Direct obj;

    obj  = HPDF_GetMem (mmgr, sizeof(HPDF_Direct_Rec));

    if (obj) {
        HPDF_MemSet(&obj->header, 0, sizeof(HPDF_Obj_Header));
        obj->header.obj_class = HPDF_OCLASS_DIRECT;

        obj->mmgr = mmgr;
        obj->error = mmgr->error;
        obj->value = NULL;
        obj->len = 0;
        if (HPDF_Direct_SetValue (obj, value, len) != HPDF_OK) {
            HPDF_FreeMem (mmgr, obj);
            return NULL;
        }
    }

    return obj;
}

HPDF_STATUS
HPDF_Direct_Write  (HPDF_Direct   obj,
                    HPDF_Stream   stream)
{
    return HPDF_Stream_Write(stream, obj->value, obj->len);
}


HPDF_STATUS
HPDF_Direct_SetValue  (HPDF_Direct  obj,
                       HPDF_BYTE    *value,
                       HPDF_UINT    len)
{
    if (len > HPDF_LIMIT_MAX_STRING_LEN)
        return HPDF_SetError (obj->error, HPDF_BINARY_LENGTH_ERR, 0);

    if (obj->value) {
        HPDF_FreeMem (obj->mmgr, obj->value);
        obj->len = 0;
    }

    obj->value = HPDF_GetMem (obj->mmgr, len);
    if (!obj->value)
        return HPDF_Error_GetCode (obj->error);

    HPDF_MemCpy (obj->value, value, len);
    obj->len = len;

    return HPDF_OK;
}


void
HPDF_Direct_Free  (HPDF_Direct  obj)
{
    if (!obj)
        return;

    if (obj->value)
        HPDF_FreeMem (obj->mmgr, obj->value);

    HPDF_FreeMem (obj->mmgr, obj);
}
