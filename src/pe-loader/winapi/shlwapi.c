#include "../common.h"

BOOL WINAPI PathFileExistsW(
LPCWSTR pszPath
)
{
    assert(0);
    return TRUE;
}

LPCWSTR WINAPI PathFindFileNameW(
  LPCWSTR pszPath
)
{
    assert(0);
    return NULL;
}

static ModuleExport shlwapi_exports[] = {
    E(PathFileExistsW),
    E(PathFindFileNameW),
};

Module shlwapi_module = { "SHLWAPI.dll", shlwapi_exports, ARRAYSIZE(shlwapi_exports) };
