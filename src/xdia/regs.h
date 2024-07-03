#pragma once

#define WIN32_LEAN_AND_MEAN     // Exclude rarely-used stuff from Windows headers
#include <dia/dia2.h>

extern const char * const rgRegX86[];
extern const char * const rgRegAMD64[];
extern const char * const rgRegMips[];
extern const char * const rgReg68k[];
extern const char * const rgRegAlpha[];
extern const char * const rgRegPpc[];
extern const char * const rgRegSh[];
extern const char * const rgRegArm[];

typedef struct MapIa64Reg{
    CV_HREG_e  iCvReg;
    const char* wszRegName;
}MapIa64Reg;
extern const MapIa64Reg mpIa64regSz[];
int cmpIa64regSz( const void* , const void* );

extern DWORD g_dwMachineType;
const char* SzNameC7Reg( USHORT , DWORD );
const char* SzNameC7Reg( USHORT );
