#pragma once
#include "dia2.h"

extern IDiaDataSource *g_pDiaDataSource;
extern IDiaSession *g_pDiaSession;
extern IDiaSymbol *g_pGlobalSymbol;
extern DWORD g_dwMachineType;

void PrintHelpOptions();
bool ParseArg(int , const char *argv[]);

void Cleanup();
bool LoadDataFromPdb(const char *, IDiaDataSource **, IDiaSession **, IDiaSymbol **);

void DumpAllPdbInfo(IDiaSession *, IDiaSymbol *);
bool DumpAllMods(IDiaSymbol *);
bool DumpAllPublics(IDiaSymbol *);
bool DumpCompiland(IDiaSymbol *, const char *);
bool DumpAllSymbols(IDiaSymbol *);
bool DumpAllGlobals(IDiaSymbol *);
bool DumpAllTypes(IDiaSymbol *);
bool DumpAllUDTs(IDiaSymbol *);
bool DumpAllEnums(IDiaSymbol *);
bool DumpAllTypedefs(IDiaSymbol *);
bool DumpAllOEMs(IDiaSymbol *);
bool DumpAllFiles(IDiaSession *, IDiaSymbol *);
bool DumpAllLines(IDiaSession *, IDiaSymbol *);
bool DumpAllLines(IDiaSession *, DWORD, DWORD);
bool DumpAllSecContribs(IDiaSession *);
bool DumpAllDebugStreams(IDiaSession *);
bool DumpAllInjectedSources(IDiaSession *);
bool DumpInjectedSource(IDiaSession *, const char *);
bool DumpAllSourceFiles(IDiaSession *, IDiaSymbol *);
bool DumpAllFPO(IDiaSession *);
bool DumpFPO(IDiaSession *, DWORD);
bool DumpFPO(IDiaSession *, IDiaSymbol *, const char *);
bool DumpSymbolWithRVA(IDiaSession *, DWORD, const char *);
bool DumpSymbolsWithRegEx(IDiaSymbol *, const char *, const char *);
bool DumpSymbolWithChildren(IDiaSymbol *, const wchar_t *);
bool DumpLines(IDiaSession *, DWORD);
bool DumpLines(IDiaSession *, IDiaSymbol *, const char *);
bool DumpType(IDiaSymbol *, const char *);
bool DumpLinesForSourceFile(IDiaSession *, const char *, DWORD);
bool DumpPublicSymbolsSorted(IDiaSession *, DWORD, DWORD, bool);
bool DumpLabel(IDiaSession *, DWORD);
bool DumpAnnotations(IDiaSession *, DWORD);
bool DumpMapToSrc(IDiaSession *, DWORD);
bool DumpMapFromSrc(IDiaSession *, DWORD);

HRESULT GetTable(IDiaSession *, REFIID, void **);

///////////////////////////////////////////////////////////////////
// Functions defined in regs.cpp
const char *SzNameC7Reg(USHORT, DWORD);
const char *SzNameC7Reg(USHORT);
