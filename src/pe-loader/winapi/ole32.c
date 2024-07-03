#include "../common.h"

int WINAPI StringFromGUID2(REFGUID rguid, LPOLESTR lpsz, int cchMax)
{
    assert(0);
    return 0;
}

WINOLE32API HRESULT WINAPI CoInitialize(LPVOID lpReserved)
{
    return 0;
}

HRESULT WINAPI CoCreateGuid(
GUID *pguid
)
{
    assert(0);
    return 0;
}

static ModuleExport ole32_exports[] = {
    E(StringFromGUID2),
    E(CoInitialize),
    E(CoCreateGuid),
};

Module ole32_module = { "ole32.dll", ole32_exports, ARRAYSIZE(ole32_exports) };
