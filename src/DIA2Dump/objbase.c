#define INITGUID
#define COBJMACROS

#include "../pe-loader/common.h"
#include <unknwn.h>
#include <dia2.h>
#include "../pe-loader/uuids.h"

// WINOLE32API HRESULT WINAPI CoInitialize(LPVOID lpReserved)
// {
//     return 0;
// }

void WINAPI DECLSPEC_HOTPATCH CoUninitialize(void)
{
}

HRESULT WINAPI DECLSPEC_HOTPATCH CoCreateInstance(REFCLSID rclsid,
                                                  IUnknown *outer,
                                                  DWORD cls_context,
                                                  REFIID riid, void **obj)
{
    typedef HRESULT(WINAPI * DllGetClassObjectFunc)(REFCLSID, REFIID, LPVOID *);
    DllGetClassObjectFunc DllGetClassObject =
        (DllGetClassObjectFunc)GetProcAddress((HMODULE) "msdia140.dll",
                                              "DllGetClassObject");

    IClassFactory *ClassFactory = NULL;
    HRESULT hresult =
        DllGetClassObject(rclsid, &IID_IClassFactory, (LPVOID *)&ClassFactory);
    assert(hresult == ERROR_SUCCESS && "DllGetClassObject");

    hresult = IClassFactory_CreateInstance(ClassFactory, NULL, riid, obj);
    assert(hresult == ERROR_SUCCESS && "IClassFactory_CreateInstance");
    IClassFactory_AddRef(ClassFactory);

    return 0;
}
