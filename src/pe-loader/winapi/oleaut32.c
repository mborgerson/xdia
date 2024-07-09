#include "../common.h"

BSTR WINAPI SysAllocStringLen(const OLECHAR *str, unsigned int len)
{
    size_t size = len * sizeof(WCHAR);
    BSTR bstr = malloc(size + sizeof(WCHAR));
    if (str) {
        memcpy(bstr, str, size);
        bstr[len] = 0;
    } else {
        memset(bstr, 0, size + sizeof(WCHAR));
    }

    return bstr;
}

SAFEARRAY* WINAPI SafeArrayCreateVector(VARTYPE vt, LONG lLbound, ULONG cElements)
{
    assert(0);
    return NULL;
}

BSTR WINAPI SysAllocString(LPCOLESTR str)
{
#if DEBUG
    char *str_utf8 = wc2c(str);
    LOG("%s(%s)", __func__, str_utf8);
    free(str_utf8);
#endif
    return SysAllocStringLen(str, u_strlen(str));
}

void WINAPI DECLSPEC_HOTPATCH SysFreeString(BSTR str)
{
    LOG("%s(%p)", __func__, str);
    free(str);
}

HRESULT WINAPI LoadTypeLibEx(
    LPCOLESTR szFile,  /* [in] Name of file to load from */
    REGKIND  regkind,  /* [in] Specify kind of registration */
    ITypeLib **pptLib) /* [out] Pointer to pointer to loaded type library */
{
    assert(0);
    return 0;
}

HRESULT WINAPI UnRegisterTypeLib(
    REFGUID libid,  /* [in] Guid of the library */
    WORD wVerMajor, /* [in] major version */
    WORD wVerMinor, /* [in] minor version */
    LCID lcid,  /* [in] locale id */
    SYSKIND syskind)
{
    assert(0);
    return 0;
}

/*
 * VARIANT
 *
 * Copyright 1998 Jean-Claude Cote
 * Copyright 2003 Jon Griffiths
 * Copyright 2005 Daniel Remenak
 * Copyright 2006 Google (Benjamin Arai)
 *
 * The algorithm for conversion from Julian days to day/month/year is based on
 * that devised by Henry Fliegel, as implemented in PostgreSQL, which is
 * Copyright 1994-7 Regents of the University of California
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA
 */

#include "oleaut32/variant.h"

/******************************************************************************
 * Check if a variants type is valid.
 */
static inline HRESULT VARIANT_ValidateType(VARTYPE vt)
{
  VARTYPE vtExtra = vt & VT_EXTRA_TYPE;

  vt &= VT_TYPEMASK;

  if (!(vtExtra & (VT_VECTOR|VT_RESERVED)))
  {
    if (vt < VT_VOID || vt == VT_RECORD || vt == VT_CLSID)
    {
      if ((vtExtra & (VT_BYREF|VT_ARRAY)) && vt <= VT_NULL)
        return DISP_E_BADVARTYPE;
      if (vt != (VARTYPE)15)
        return S_OK;
    }
  }
  return DISP_E_BADVARTYPE;
}

HRESULT WINAPI DECLSPEC_HOTPATCH VariantClear(VARIANTARG* pVarg)
{
  HRESULT hres;

  // TRACE("(%s)\n", debugstr_variant(pVarg));

  hres = VARIANT_ValidateType(V_VT(pVarg));

  if (SUCCEEDED(hres))
  {
    if (!V_ISBYREF(pVarg))
    {
      if (V_ISARRAY(pVarg) || V_VT(pVarg) == VT_SAFEARRAY)
      {
        hres = SafeArrayDestroy(V_ARRAY(pVarg));
      }
      else if (V_VT(pVarg) == VT_BSTR)
      {
        SysFreeString(V_BSTR(pVarg));
      }
      else if (V_VT(pVarg) == VT_RECORD)
      {
        assert(0);
        // IRecordInfo *rec_info = V_RECORDINFO(pVarg);
        // if (rec_info)
        // {
        //   IRecordInfo_RecordClear(rec_info, V_RECORD(pVarg));
        //   IRecordInfo_Release(rec_info);
        // }
      }
      else if (V_VT(pVarg) == VT_DISPATCH ||
               V_VT(pVarg) == VT_UNKNOWN)
      {
        // FIXME
        // if (V_UNKNOWN(pVarg))
        //   IUnknown_Release(V_UNKNOWN(pVarg));
        // assert(0);
      }
    }
    V_VT(pVarg) = VT_EMPTY;
  }
  return hres;
}

HRESULT WINAPI VariantCopy(VARIANTARG* pvargDest, const VARIANTARG* pvargSrc)
{
  HRESULT hres = S_OK;

  // TRACE("(%s,%s)\n", debugstr_variant(pvargDest), debugstr_variant(pvargSrc));

  if (V_TYPE(pvargSrc) == VT_CLSID || /* VT_CLSID is a special case */
      FAILED(VARIANT_ValidateType(V_VT(pvargSrc))))
    return DISP_E_BADVARTYPE;

  if (pvargSrc != pvargDest &&
      SUCCEEDED(hres = VariantClear(pvargDest)))
  {
    *pvargDest = *pvargSrc; /* Shallow copy the value */

    if (!V_ISBYREF(pvargSrc))
    {
      switch (V_VT(pvargSrc))
      {
      case VT_BSTR:
        V_BSTR(pvargDest) = SysAllocString(V_BSTR(pvargSrc));
        // V_BSTR(pvargDest) = SysAllocStringByteLen((char*)V_BSTR(pvargSrc), SysStringByteLen(V_BSTR(pvargSrc)));
        if (!V_BSTR(pvargDest))
          hres = E_OUTOFMEMORY;
        break;
      case VT_RECORD:
        assert(0);
        // hres = VARIANT_CopyIRecordInfo(pvargDest, pvargSrc);
        break;
      case VT_DISPATCH:
      case VT_UNKNOWN:
        V_UNKNOWN(pvargDest) = V_UNKNOWN(pvargSrc);
        if (V_UNKNOWN(pvargSrc)) {
          // IUnknown_AddRef(V_UNKNOWN(pvargSrc));
          assert(0);
        }
        break;
      default:
        if (V_ISARRAY(pvargSrc))
          // hres = SafeArrayCopy(V_ARRAY(pvargSrc), &V_ARRAY(pvargDest));
            assert(0);
      }
    }
  }
  return hres;
}


HRESULT WINAPI SafeArrayPutElement(SAFEARRAY *psa, LONG *rgIndices, void *pvData)
{
    assert(0);
    return 0;
}

HRESULT WINAPI SafeArrayAccessData(
  SAFEARRAY  *psa,
  void HUGEP **ppvData
)
{
    assert(0);
    return 0;
}

HRESULT WINAPI SafeArrayDestroy(
  SAFEARRAY *psa
)
{
    assert(0);
    return 0;
}

HRESULT WINAPI SafeArrayUnaccessData(
  SAFEARRAY *psa
)
{
    assert(0);
    return 0;    
}


static ModuleExport oleaut32_exports[] = {
    EO(2,   SysAllocString),
    EO(4,   SysAllocStringLen),
    EO(6,   SysFreeString),
    EO(9,   VariantClear),
    EO(10,  VariantCopy),
    EO(16,  SafeArrayDestroy),
    EO(23,  SafeArrayAccessData),
    EO(24,  SafeArrayUnaccessData),
    EO(26,  SafeArrayPutElement),
    EO(183, LoadTypeLibEx),
    EO(186, UnRegisterTypeLib),
    EO(411, SafeArrayCreateVector),
};

Module oleaut32_module = {"OLEAUT32.dll", oleaut32_exports,
                          ARRAYSIZE(oleaut32_exports)};
