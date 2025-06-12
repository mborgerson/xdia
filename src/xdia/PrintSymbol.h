#pragma once

#include "util.h"

inline int myDebugBreak( int ){
    // DebugBreak();
    assert(0);
    return 0;
}
#define MAXELEMS(x)     (sizeof(x)/sizeof(x[0]))
#define SafeDRef(a, i)  ((i < MAXELEMS(a)) ? a[i] : a[myDebugBreak(i)])

#define MAX_TYPE_IN_DETAIL 5
#define MAX_RVA_LINES_BYTES_RANGE 0x100

extern const char * const rgBaseType[];
extern const char * const rgTags[];
extern const char * const rgFloatPackageStrings[];
extern const char * const rgProcessorStrings[];
extern const char * const rgDataKind[];
extern const char * const rgUdtKind[];
extern const char * const rgAccess[];
extern const char * const rgCallingConvention[];
extern const char * const rgLanguage[];
extern const char * const rgLocationTypeString[];

json PrintSymbolTag(IDiaSymbol *pSymbol);
json PrintSymbolAddress(IDiaSymbol *pSymbol);

json PrintPublicSymbol( IDiaSymbol* );
json PrintGlobalSymbol( IDiaSymbol* );
void PrintSymbol( IDiaSymbol* , DWORD );
void PrintSymTag( DWORD );
json PrintName( IDiaSymbol* );
void PrintUndName( IDiaSymbol* );
void PrintThunk( IDiaSymbol* );
void PrintCompilandDetails( IDiaSymbol* );
void PrintCompilandEnv( IDiaSymbol* );
void PrintLocation( IDiaSymbol* );
void PrintConst( IDiaSymbol* );
void PrintUDT( IDiaSymbol* );
void PrintSymbolType( IDiaSymbol* );
void PrintType( IDiaSymbol* );
void PrintBound( IDiaSymbol* );
void PrintData( IDiaSymbol* );
void PrintVariant( VARIANT );
void PrintUdtKind( IDiaSymbol* );
void PrintTypeInDetail( IDiaSymbol* , DWORD );
void PrintFunctionType( IDiaSymbol* );
void PrintSourceFile( IDiaSourceFile* );
void PrintLines( IDiaSession* , IDiaSymbol* );
void PrintLines( IDiaEnumLineNumbers* );
void PrintSource( IDiaSourceFile* );
void PrintSecContribs( IDiaSectionContrib* );
void PrintStreamData( IDiaEnumDebugStreamData* );
void PrintFrameData( IDiaFrameData* );

void PrintPropertyStorage( IDiaPropertyStorage* );

template<class T> void PrintGeneric( T t ){
  IDiaPropertyStorage* pPropertyStorage;
  
  if(t->QueryInterface( __uuidof(IDiaPropertyStorage), (void **)&pPropertyStorage ) == S_OK){
    PrintPropertyStorage(pPropertyStorage);
    pPropertyStorage->Release();
  }
}
