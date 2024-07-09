#include "../common.h"
#include "evntprov.h"

ULONG EVNTAPI EventRegister(LPCGUID ProviderId, PENABLECALLBACK EnableCallback,
                            PVOID CallbackContext, PREGHANDLE RegHandle)
{
    LOG("EventRegister stubbed");
    return 0;
}

LSTATUS WINAPI RegOpenKeyExW(HKEY hKey, LPCWSTR lpSubKey, DWORD ulOptions,
                      REGSAM samDesired, PHKEY phkResult)
{
    LOG("%s(%p, %p, %x, ", __func__, hKey, lpSubKey, ulOptions);
#if DEBUG
    if (lpSubKey) {
        char *lpSubKey_utf8 = wc2c(lpSubKey);
        LOG("-> %s", lpSubKey_utf8);
        free(lpSubKey_utf8);
    }
#endif
    return ERROR_FILE_NOT_FOUND;
}

LSTATUS WINAPI RegQueryValueExW(HKEY hKey, LPCWSTR lpValueName,
                                LPDWORD lpReserved, LPDWORD lpType,
                                LPBYTE lpData, LPDWORD lpcbData)
{
    LOG("%s(%p, %p, %p, %p, %p, %p)", __func__, hKey, lpValueName, lpReserved, lpType, lpData, lpcbData);
#if DEBUG
    if (lpValueName) {
        char *lpValueName_utf8 = wc2c(lpValueName);
        LOG("-> %s", lpValueName_utf8);
        free(lpValueName_utf8);
    }
#endif

    assert(0);
    return 0;
}

static ModuleExport advapi32_exports[] = {
    E(EventRegister),
    E(RegOpenKeyExW),
    E(RegQueryValueExW),
};

Module advapi32_module = { "ADVAPI32.dll", advapi32_exports, ARRAYSIZE(advapi32_exports) };
