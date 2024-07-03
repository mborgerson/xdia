#include <stdio.h>
#include <assert.h>
#include <guiddef.h>
#include <objbase.h>
#include <unknwn.h>
#include <dia/dia2.h>

#include "util.h"
#include "PrintSymbol.h"

const wchar_t *g_szFilename;
IDiaDataSource *g_pDiaDataSource;
IDiaSession *g_pDiaSession;
IDiaSymbol *g_pGlobalSymbol;
DWORD g_dwMachineType = CV_CFL_80386;

bool LoadDataFromPdb(const wchar_t *szFilename, IDiaDataSource **ppSource,
                     IDiaSession **ppSession, IDiaSymbol **ppGlobal);
void DumpAllPdbInfo(IDiaSession *pSession, IDiaSymbol *pGlobal);
json DumpAllGlobals(IDiaSymbol *pGlobal);

int wmain( int argc, wchar_t *argv[ ] )
{
  json publics;

  assert(argc == 2);
  g_szFilename = argv[1];

  if (!LoadDataFromPdb(g_szFilename, &g_pDiaDataSource, &g_pDiaSession, &g_pGlobalSymbol)) {
    return -1;
  }

  DumpAllPdbInfo(g_pDiaSession, g_pGlobalSymbol);

  return 0;
}

bool LoadDataFromPdb(const wchar_t *szFilename, IDiaDataSource **ppSource,
                     IDiaSession **ppSession, IDiaSymbol **ppGlobal)
{
  HINSTANCE hModule = LoadLibrary(TEXT("msdia140.dll"));
  if (!hModule) {
    fprintf(stderr, "LoadLibrary(msdia140.dll) failed\n");
    return false;
  }

  HRESULT hr = CoInitialize(NULL);
  if (FAILED(hr)) {
    fprintf(stderr, "CoInitialize failed (%08X)", hr);
    return false;
  }

  typedef HRESULT(WINAPI * DllGetClassObjectFunc)(REFCLSID, REFIID, LPVOID *);
  DllGetClassObjectFunc DllGetClassObject =
      (DllGetClassObjectFunc)GetProcAddress(hModule, "DllGetClassObject");

  IClassFactory *ClassFactory = NULL;
  hr = DllGetClassObject(__uuidof(DiaSource), IID_IClassFactory,
                         (LPVOID *)&ClassFactory);
  if (FAILED(hr)) {
    fprintf(stderr, "DllGetClassObject failed (%08X)\n", hr);
    return false;
  }

  hr = ClassFactory->CreateInstance(NULL, __uuidof(IDiaDataSource), (void**)ppSource);
  if (FAILED(hr)) {
    fprintf(stderr, "IClassFactory_CreateInstance failed (%08X)\n", hr);
    return false;
  }
  ClassFactory->AddRef();

  // Open and prepare a program database (.pdb) file as a debug data source
  hr = (*ppSource)->loadDataFromPdb(szFilename);
  if (FAILED(hr)) {
    fprintf(stderr, "loadDataFromPdb failed (%08X)\n", hr);
    return false;
  }

  // Open a session for querying symbols
  hr = (*ppSource)->openSession(ppSession);
  if (FAILED(hr)) {
    fprintf(stderr, "openSession failed (%08X)\n", hr);
    return false;
  }

  // Retrieve a reference to the global scope
  hr = (*ppSession)->get_globalScope(ppGlobal);
  if (hr != S_OK) {
    fprintf(stderr, "get_globalScope failed (%08X)\n", hr);
    return false;
  }

  // Set Machine type for getting correct register names
  DWORD dwMachType = 0;
  if ((*ppGlobal)->get_machineType(&dwMachType) == S_OK) {
    switch (dwMachType) {
      case IMAGE_FILE_MACHINE_I386:
        g_dwMachineType = CV_CFL_80386;
        break;
      case IMAGE_FILE_MACHINE_IA64:
        g_dwMachineType = CV_CFL_IA64;
        break;
      case IMAGE_FILE_MACHINE_AMD64:
        g_dwMachineType = CV_CFL_AMD64;
        break;
    }
  }

  return true;
}

////////////////////////////////////////////////////////////
// Dump all the data stored in a PDB
//
void DumpAllPdbInfo(IDiaSession *pSession, IDiaSymbol *pGlobal)
{
  json j;

  // DumpAllMods(pGlobal);
  // DumpAllPublics(pGlobal);
  // DumpAllSymbols(pGlobal);
  j["globals"] = DumpAllGlobals(pGlobal);
  // DumpAllTypes(pGlobal);
  // DumpAllFiles(pSession, pGlobal);
  // DumpAllLines(pSession, pGlobal);
  // DumpAllSecContribs(pSession);
  // DumpAllDebugStreams(pSession);
  // DumpAllInjectedSources(pSession);
  // DumpAllFPO(pSession);
  // DumpAllOEMs(pGlobal);

  puts(j.dump(2).c_str());
}

////////////////////////////////////////////////////////////
// Dump all the global symbols - SymTagFunction,
//  SymTagThunk and SymTagData
//
json DumpAllGlobals(IDiaSymbol *pGlobal)
{
  json j;

  IDiaEnumSymbols *pEnumSymbols;
  IDiaSymbol *pSymbol;
  enum SymTagEnum dwSymTags[] = { SymTagFunction, SymTagThunk, SymTagData };
  ULONG celt = 0;

  for (size_t i = 0; i < ARRAYSIZE(dwSymTags); i++, pEnumSymbols = NULL) {
    if (SUCCEEDED(pGlobal->findChildren(dwSymTags[i], NULL, nsNone, &pEnumSymbols))) {
      while (SUCCEEDED(pEnumSymbols->Next(1, &pSymbol, &celt)) && (celt == 1)) {
        j.push_back(PrintGlobalSymbol(pSymbol));

        pSymbol->Release();
      }

      pEnumSymbols->Release();
    }

    else {
      fprintf(stderr, "ERROR - DumpAllGlobals() returned no symbols\n");

      return false;
    }
  }

  return j;
}
