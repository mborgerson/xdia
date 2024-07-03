// PrintSymbol.cpp : Defines the printing procedures for the symbols
//
// This is a part of the Debug Interface Access SDK 
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
// This source code is only intended as a supplement to the
// Debug Interface Access SDK and related electronic documentation 
// provided with the library.
// See these sources for detailed information regarding the
// Debug Interface Access SDK API.
//

#include "stdafx.h"


#include <malloc.h>

#include "dia2.h"
#include "regs.h"
#include "PrintSymbol.h"

#define _alloca alloca

// Basic types
const char * const rgBaseType[] =
{
  "<NoType>",                         // btNoType = 0,
  "void",                             // btVoid = 1,
  "char",                             // btChar = 2,
  "wchar_t",                          // btWChar = 3,
  "signed char",
  "unsigned char",
  "int",                              // btInt = 6,
  "unsigned int",                     // btUInt = 7,
  "float",                            // btFloat = 8,
  "<BCD>",                            // btBCD = 9,
  "bool",                             // btBool = 10,
  "short",
  "unsigned short",
  "long",                             // btLong = 13,
  "unsigned long",                    // btULong = 14,
  "__int8",
  "__int16",
  "__int32",
  "__int64",
  "__int128",
  "unsigned __int8",
  "unsigned __int16",
  "unsigned __int32",
  "unsigned __int64",
  "unsigned __int128",
  "<currency>",                       // btCurrency = 25,
  "<date>",                           // btDate = 26,
  "VARIANT",                          // btVariant = 27,
  "<complex>",                        // btComplex = 28,
  "<bit>",                            // btBit = 29,
  "BSTR",                             // btBSTR = 30,
  "HRESULT",                          // btHresult = 31
  "char16_t",                         // btChar16 = 32
  "char32_t",                          // btChar32 = 33
  "char8_t",                          // btChar8  = 34
};

// Tags returned by Dia
const char * const rgTags[] =
{
  "(SymTagNull)",                     // SymTagNull
  "Executable (Global)",              // SymTagExe
  "Compiland",                        // SymTagCompiland
  "CompilandDetails",                 // SymTagCompilandDetails
  "CompilandEnv",                     // SymTagCompilandEnv
  "Function",                         // SymTagFunction
  "Block",                            // SymTagBlock
  "Data",                             // SymTagData
  "Annotation",                       // SymTagAnnotation
  "Label",                            // SymTagLabel
  "PublicSymbol",                     // SymTagPublicSymbol
  "UserDefinedType",                  // SymTagUDT
  "Enum",                             // SymTagEnum
  "FunctionType",                     // SymTagFunctionType
  "PointerType",                      // SymTagPointerType
  "ArrayType",                        // SymTagArrayType
  "BaseType",                         // SymTagBaseType
  "Typedef",                          // SymTagTypedef
  "BaseClass",                        // SymTagBaseClass
  "Friend",                           // SymTagFriend
  "FunctionArgType",                  // SymTagFunctionArgType
  "FuncDebugStart",                   // SymTagFuncDebugStart
  "FuncDebugEnd",                     // SymTagFuncDebugEnd
  "UsingNamespace",                   // SymTagUsingNamespace
  "VTableShape",                      // SymTagVTableShape
  "VTable",                           // SymTagVTable
  "Custom",                           // SymTagCustom
  "Thunk",                            // SymTagThunk
  "CustomType",                       // SymTagCustomType
  "ManagedType",                      // SymTagManagedType
  "Dimension",                        // SymTagDimension
  "CallSite",                         // SymTagCallSite
  "InlineSite",                       // SymTagInlineSite
  "BaseInterface",                    // SymTagBaseInterface
  "VectorType",                       // SymTagVectorType
  "MatrixType",                       // SymTagMatrixType
  "HLSLType",                         // SymTagHLSLType
  "Caller",                           // SymTagCaller,
  "Callee",                           // SymTagCallee,
  "Export",                           // SymTagExport,
  "HeapAllocationSite",               // SymTagHeapAllocationSite
  "CoffGroup",                        // SymTagCoffGroup
  "Inlinee",                          // SymTagInlinee
  "TaggedUnionCase",                  // SymTagTaggedUnionCase
};


// Processors
const char * const rgFloatPackageStrings[] =
{
  "hardware processor (80x87 for Intel processors)",    // CV_CFL_NDP
  "emulator",                                           // CV_CFL_EMU
  "altmath",                                            // CV_CFL_ALT
  "???"
};

const char * const rgProcessorStrings[] =
{
  "8080",                             //  CV_CFL_8080
  "8086",                             //  CV_CFL_8086
  "80286",                            //  CV_CFL_80286
  "80386",                            //  CV_CFL_80386
  "80486",                            //  CV_CFL_80486
  "Pentium",                          //  CV_CFL_PENTIUM
  "Pentium Pro/Pentium II",           //  CV_CFL_PENTIUMII/CV_CFL_PENTIUMPRO
  "Pentium III",                      //  CV_CFL_PENTIUMIII
  "???",
  "???",
  "???",
  "???",
  "???",
  "???",
  "???",
  "???",
  "MIPS (Generic)",                   //  CV_CFL_MIPSR4000
  "MIPS16",                           //  CV_CFL_MIPS16
  "MIPS32",                           //  CV_CFL_MIPS32
  "MIPS64",                           //  CV_CFL_MIPS64
  "MIPS I",                           //  CV_CFL_MIPSI
  "MIPS II",                          //  CV_CFL_MIPSII
  "MIPS III",                         //  CV_CFL_MIPSIII
  "MIPS IV",                          //  CV_CFL_MIPSIV
  "MIPS V",                           //  CV_CFL_MIPSV
  "???",
  "???",
  "???",
  "???",
  "???",
  "???",
  "???",
  "M68000",                           //  CV_CFL_M68000
  "M68010",                           //  CV_CFL_M68010
  "M68020",                           //  CV_CFL_M68020
  "M68030",                           //  CV_CFL_M68030
  "M68040",                           //  CV_CFL_M68040
  "???",
  "???",
  "???",
  "???",
  "???",
  "???",
  "???",
  "???",
  "???",
  "???",
  "???",
  "Alpha 21064",                      // CV_CFL_ALPHA, CV_CFL_ALPHA_21064
  "Alpha 21164",                      // CV_CFL_ALPHA_21164
  "Alpha 21164A",                     // CV_CFL_ALPHA_21164A
  "Alpha 21264",                      // CV_CFL_ALPHA_21264
  "Alpha 21364",                      // CV_CFL_ALPHA_21364
  "???",
  "???",
  "???",
  "???",
  "???",
  "???",
  "???",
  "???",
  "???",
  "???",
  "???",
  "PPC 601",                          // CV_CFL_PPC601
  "PPC 603",                          // CV_CFL_PPC603
  "PPC 604",                          // CV_CFL_PPC604
  "PPC 620",                          // CV_CFL_PPC620
  "PPC w/FP",                         // CV_CFL_PPCFP
  "PPC (Big Endian)",                 // CV_CFL_PPCBE
  "???",
  "???",
  "???",
  "???",
  "???",
  "???",
  "???",
  "???",
  "???",
  "???",
  "SH3",                              // CV_CFL_SH3
  "SH3E",                             // CV_CFL_SH3E
  "SH3DSP",                           // CV_CFL_SH3DSP
  "SH4",                              // CV_CFL_SH4
  "SHmedia",                          // CV_CFL_SHMEDIA
  "???",
  "???",
  "???",
  "???",
  "???",
  "???",
  "???",
  "???",
  "???",
  "???",
  "???",
  "ARM3",                             // CV_CFL_ARM3
  "ARM4",                             // CV_CFL_ARM4
  "ARM4T",                            // CV_CFL_ARM4T
  "ARM5",                             // CV_CFL_ARM5
  "ARM5T",                            // CV_CFL_ARM5T
  "ARM6",                             // CV_CFL_ARM6
  "ARM (XMAC)",                       // CV_CFL_ARM_XMAC
  "ARM (WMMX)",                       // CV_CFL_ARM_WMMX
  "???",
  "???",
  "???",
  "???",
  "???",
  "???",
  "???",
  "???",
  "Omni",                             // CV_CFL_OMNI
  "???",
  "???",
  "???",
  "???",
  "???",
  "???",
  "???",
  "???",
  "???",
  "???",
  "???",
  "???",
  "???",
  "???",
  "???",
  "Itanium",                          // CV_CFL_IA64, CV_CFL_IA64_1
  "Itanium (McKinley)",               // CV_CFL_IA64_2
  "???",
  "???",
  "???",
  "???",
  "???",
  "???",
  "???",
  "???",
  "???",
  "???",
  "???",
  "???",
  "???",
  "???",
  "CEE",                              // CV_CFL_CEE
  "???",
  "???",
  "???",
  "???",
  "???",
  "???",
  "???",
  "???",
  "???",
  "???",
  "???",
  "???",
  "???",
  "???",
  "???",
  "AM33",                             // CV_CFL_AM33
  "???",
  "???",
  "???",
  "???",
  "???",
  "???",
  "???",
  "???",
  "???",
  "???",
  "???",
  "???",
  "???",
  "???",
  "???",
  "M32R",                             // CV_CFL_M32R
  "???",
  "???",
  "???",
  "???",
  "???",
  "???",
  "???",
  "???",
  "???",
  "???",
  "???",
  "???",
  "???",
  "???",
  "???",
  "TriCore",                          // CV_CFL_TRICORE
  "???",
  "???",
  "???",
  "???",
  "???",
  "???",
  "???",
  "???",
  "???",
  "???",
  "???",
  "???",
  "???",
  "???",
  "???",
  "x64",                              // CV_CFL_X64
  "???",
  "???",
  "???",
  "???",
  "???",
  "???",
  "???",
  "???",
  "???",
  "???",
  "???",
  "???",
  "???",
  "???",
  "???",
  "EBC",                              // CV_CFL_EBC
  "???",
  "???",
  "???",
  "???",
  "???",
  "???",
  "???",
  "???",
  "???",
  "???",
  "???",
  "???",
  "???",
  "???",
  "???",
  "Thumb (CE)",                       // CV_CFL_THUMB
  "???",
  "???",
  "???",
  "ARM",                              // CV_CFL_ARMNT
  "???",
  "ARM64",                            // CV_CFL_ARM64
  "???",
  "???",
  "???",
  "???",
  "???",
  "???",
  "???",
  "???",
  "UNKNOWN",                          // CV_CFL_UNKNOWN
  "D3D11_SHADE",                      // CV_CFL_D3D11_SHADER
};

const char * const rgDataKind[] =
{
  "Unknown",
  "Local",
  "Static Local",
  "Param",
  "Object Ptr",
  "File Static",
  "Global",
  "Member",
  "Static Member",
  "Constant",
};

const char * const rgUdtKind[] =
{
  "struct",
  "class",
  "union",
  "interface",
  "tagged union",
};

const char * const rgAccess[] =
{
  "",                     // No access specifier
  "private",
  "protected",
  "public"
};

const char * const rgCallingConvention[] =
{
  "CV_CALL_NEAR_C      ",
  "CV_CALL_FAR_C       ",
  "CV_CALL_NEAR_PASCAL ",
  "CV_CALL_FAR_PASCAL  ",
  "CV_CALL_NEAR_FAST   ",
  "CV_CALL_FAR_FAST    ",
  "CV_CALL_SKIPPED     ",
  "CV_CALL_NEAR_STD    ",
  "CV_CALL_FAR_STD     ",
  "CV_CALL_NEAR_SYS    ",
  "CV_CALL_FAR_SYS     ",
  "CV_CALL_THISCALL    ",
  "CV_CALL_MIPSCALL    ",
  "CV_CALL_GENERIC     ",
  "CV_CALL_ALPHACALL   ",
  "CV_CALL_PPCCALL     ",
  "CV_CALL_SHCALL      ",
  "CV_CALL_ARMCALL     ",
  "CV_CALL_AM33CALL    ",
  "CV_CALL_TRICALL     ",
  "CV_CALL_SH5CALL     ",
  "CV_CALL_M32RCALL    ",
  "CV_ALWAYS_INLINED   ",
  "CV_CALL_NEAR_VECTOR ",
  "CV_CALL_SWIFT       ",
  "CV_CALL_RESERVED    "
};

const char * const rgLanguage[] =
{
  "C",                                // CV_CFL_C
  "C++",                              // CV_CFL_CXX
  "FORTRAN",                          // CV_CFL_FORTRAN
  "MASM",                             // CV_CFL_MASM
  "Pascal",                           // CV_CFL_PASCAL
  "Basic",                            // CV_CFL_BASIC
  "COBO",                            // CV_CFL_COBOL
  "LINK",                             // CV_CFL_LINK
  "CVTRES",                           // CV_CFL_CVTRES
  "CVTPGD",                           // CV_CFL_CVTPGD
  "C#",                               // CV_CFL_CSHARP
  "Visual Basic",                     // CV_CFL_VB
  "ILASM",                            // CV_CFL_ILASM
  "Java",                             // CV_CFL_JAVA
  "JScript",                          // CV_CFL_JSCRIPT
  "MSI",                             // CV_CFL_MSIL
  "HLS",                             // CV_CFL_HLSL
  "Objective-C",                      // CV_CFL_OBJC
  "Objective-C++",                    // CV_CFL_OBJCXX
  "Swift",                            // CV_CFL_SWIFT
  "ALIASOBJ",                         // CV_CFL_ALIASOBJ
  "Rust",                             // CV_CFL_RUST
  "Go",                               // CV_CFL_GO
};

const char * const rgLocationTypeString[] =
{
  "NUL",
  "static",
  "TLS",
  "RegRel",
  "ThisRel",
  "Enregistered",
  "BitField",
  "Slot",
  "IL Relative",
  "In MetaData",
  "Constant",
  "RegRelAliasIndir"
};


////////////////////////////////////////////////////////////
// Print a public symbol info: name, VA, RVA, SEG:OFF
//
void PrintPublicSymbol(IDiaSymbol *pSymbol)
{
  DWORD dwSymTag;
  DWORD dwRVA;
  DWORD dwSeg;
  DWORD dwOff;
  BSTR bstrName;
  
  if (pSymbol->get_symTag(&dwSymTag) != S_OK) {
    return;
  }

  if (pSymbol->get_relativeVirtualAddress(&dwRVA) != S_OK) {
    dwRVA = 0xFFFFFFFF;
  }

  pSymbol->get_addressSection(&dwSeg);
  pSymbol->get_addressOffset(&dwOff);

  printf("%s: [%08X][%04X:%08X] ", rgTags[dwSymTag], dwRVA, dwSeg, dwOff);
  
  if (dwSymTag == SymTagThunk) {
    if (pSymbol->get_name(&bstrName) == S_OK) {
      printf("%s\n", wc2c(bstrName));

      SysFreeString(bstrName);
    }

    else {
      if (pSymbol->get_targetRelativeVirtualAddress(&dwRVA) != S_OK) {
        dwRVA = 0xFFFFFFFF;
      }

      pSymbol->get_targetSection(&dwSeg);
      pSymbol->get_targetOffset(&dwOff);

      printf("target -> [%08X][%04X:%08X]\n", dwRVA, dwSeg, dwOff);
    }
  }

  else {
    // must be a function or a data symbol

    BSTR bstrUndname;

    if (pSymbol->get_name(&bstrName) == S_OK) {
      if (pSymbol->get_undecoratedName(&bstrUndname) == S_OK) {
        printf("%s(%s)\n", wc2c(bstrName), wc2c(bstrUndname));

        SysFreeString(bstrUndname);
      }

      else {
        printf("%s\n", wc2c(bstrName));
      }

      SysFreeString(bstrName);
    }
  }
}

////////////////////////////////////////////////////////////
// Print a global symbol info: name, VA, RVA, SEG:OFF
//
void PrintGlobalSymbol(IDiaSymbol *pSymbol)
{
  DWORD dwSymTag;
  DWORD dwRVA;
  DWORD dwSeg;
  DWORD dwOff;
  
  if (pSymbol->get_symTag(&dwSymTag) != S_OK) {
    return;
  }

  if (pSymbol->get_relativeVirtualAddress(&dwRVA) != S_OK) {
    dwRVA = 0xFFFFFFFF;
  }

  pSymbol->get_addressSection(&dwSeg);
  pSymbol->get_addressOffset(&dwOff);

  printf("%s: [%08X][%04X:%08X] ", rgTags[dwSymTag], dwRVA, dwSeg, dwOff);
  
  if (dwSymTag == SymTagThunk) {
    BSTR bstrName;

    if (pSymbol->get_name(&bstrName) == S_OK) {
      printf("%s\n", wc2c(bstrName));

      SysFreeString(bstrName);
    }

    else {
      if (pSymbol->get_targetRelativeVirtualAddress(&dwRVA) != S_OK) {
        dwRVA = 0xFFFFFFFF;
      }

      pSymbol->get_targetSection(&dwSeg);
      pSymbol->get_targetOffset(&dwOff);
      printf("target -> [%08X][%04X:%08X]\n", dwRVA, dwSeg, dwOff);
    }
  }

  else {
    BSTR bstrName;
    BSTR bstrUndname;

    if (pSymbol->get_name(&bstrName) == S_OK) {
      if (pSymbol->get_undecoratedName(&bstrUndname) == S_OK) {
        printf("%s(%s)\n", wc2c(bstrName), wc2c(bstrUndname));

        SysFreeString(bstrUndname);
      }

      else {
        printf("%s\n", wc2c(bstrName));
      }

      SysFreeString(bstrName);
    }
  }
}

////////////////////////////////////////////////////////////
// Print a callsite symbol info: SEG:OFF, RVA, type
//
void PrintCallSiteInfo(IDiaSymbol *pSymbol)
{
  DWORD dwISect, dwOffset;
  if (pSymbol->get_addressSection(&dwISect) == S_OK &&
    pSymbol->get_addressOffset(&dwOffset) == S_OK) {
    printf("[0x%04x:0x%08x]  ", dwISect, dwOffset);
  }

  DWORD rva;
  if (pSymbol->get_relativeVirtualAddress(&rva) == S_OK) {
    printf("0x%08X  ", rva);
  }

  IDiaSymbol *pFuncType;
  if (pSymbol->get_type(&pFuncType) == S_OK) {
    DWORD tag;
    if (pFuncType->get_symTag(&tag) == S_OK) {
      switch(tag)
      {
        case SymTagFunctionType:
          PrintFunctionType(pSymbol);
          break;
        case SymTagPointerType:
          PrintFunctionType(pFuncType);
          break;
        default:
          printf("???\n");
          break;
      }
    }
    pFuncType->Release();
  }
}

////////////////////////////////////////////////////////////
// Print a callsite symbol info: SEG:OFF, RVA, type
//
void PrintHeapAllocSite(IDiaSymbol *pSymbol)
{
  DWORD dwISect, dwOffset;
  if (pSymbol->get_addressSection(&dwISect) == S_OK &&
    pSymbol->get_addressOffset(&dwOffset) == S_OK) {
    printf("[0x%04x:0x%08x]  ", dwISect, dwOffset);
  }

  DWORD rva;
  if (pSymbol->get_relativeVirtualAddress(&rva) == S_OK) {
    printf("0x%08X  ", rva);
  }

  IDiaSymbol *pAllocType;
  if (pSymbol->get_type(&pAllocType) == S_OK) {
    PrintType(pAllocType);
    pAllocType->Release();
  }
}

////////////////////////////////////////////////////////////
// Print a COFF group symbol info: SEG:OFF, RVA, length, name
//
void PrintCoffGroup(IDiaSymbol *pSymbol)
{
  DWORD dwISect, dwOffset;
  if (pSymbol->get_addressSection(&dwISect) == S_OK &&
    pSymbol->get_addressOffset(&dwOffset) == S_OK) {
    printf("[0x%04x:0x%08x]  ", dwISect, dwOffset);
  }

  DWORD rva;
  if (pSymbol->get_relativeVirtualAddress(&rva) == S_OK) {
    printf("0x%08X, ", rva);
  }

  ULONGLONG ulLen;
  if (pSymbol->get_length(&ulLen) == S_OK) {
    printf("len = %08X, ", (ULONG)ulLen);
  }

  DWORD characteristics;
  if (pSymbol->get_characteristics(&characteristics) == S_OK) {
    printf("characteristics = %08X, ", characteristics);
  }

  PrintName(pSymbol);
}

////////////////////////////////////////////////////////////
// Print a symbol info: name, type etc.
//
void PrintSymbol(IDiaSymbol *pSymbol, DWORD dwIndent)
{
  IDiaSymbol *pType;
  DWORD dwSymTag;
  ULONGLONG ulLen;
  
  if (pSymbol->get_symTag(&dwSymTag) != S_OK) {
    printf("ERROR - PrintSymbol get_symTag() failed\n");
    return;
  }

  if (dwSymTag == SymTagFunction) {
    putchar('\n');
  }

  PrintSymTag(dwSymTag);

  for (DWORD i = 0; i < dwIndent; i++) {
    putchar(' ');
  }

  switch (dwSymTag) {
    case SymTagCompilandDetails:
      PrintCompilandDetails(pSymbol);
      break;

    case SymTagCompilandEnv:
      PrintCompilandEnv(pSymbol);
      break;

    case SymTagData:
      PrintData(pSymbol);
      break;

    case SymTagFunction:
    case SymTagBlock:
      PrintLocation(pSymbol);

      if (pSymbol->get_length(&ulLen) == S_OK) {
        printf(", len = %08X, ", (ULONG)ulLen);
      }

      if (dwSymTag == SymTagFunction) {
        DWORD dwCall;

        if (pSymbol->get_callingConvention(&dwCall) == S_OK) {
          printf(", %s", SafeDRef(rgCallingConvention, dwCall));
        }
      }

      PrintUndName(pSymbol);
      putchar('\n');

      if (dwSymTag == SymTagFunction)
      {
        BOOL f;

        for (DWORD i = 0; i < dwIndent; i++) {
          putchar(' ');
        }
        printf("                 Function attribute:");

        if ((pSymbol->get_isCxxReturnUdt(&f) == S_OK) && f) {
          printf(" return user defined type (C++ style)");
        }
        if ((pSymbol->get_constructor(&f) == S_OK) && f) {
          printf(" instance constructor");
        }
        if ((pSymbol->get_isConstructorVirtualBase(&f) == S_OK) && f) {
          printf(" instance constructor of a class with virtual base");
        }
        putchar('\n');

        for (DWORD i = 0; i < dwIndent; i++) {
          putchar(' ');
        }
        printf("                 Function info:");

        if ((pSymbol->get_hasAlloca(&f) == S_OK) && f) {
          printf(" alloca");
        }

        if ((pSymbol->get_hasSetJump(&f) == S_OK) && f) {
          printf(" setjmp");
        }

        if ((pSymbol->get_hasLongJump(&f) == S_OK) && f) {
          printf(" longjmp");
        }

        if ((pSymbol->get_hasInlAsm(&f) == S_OK) && f) {
          printf(" inlasm");
        }

        if ((pSymbol->get_hasEH(&f) == S_OK) && f) {
          printf(" eh");
        }

        if ((pSymbol->get_inlSpec(&f) == S_OK) && f) {
          printf(" inl_specified");
        }

        if ((pSymbol->get_hasSEH(&f) == S_OK) && f) {
          printf(" seh");
        }

        if ((pSymbol->get_isNaked(&f) == S_OK) && f) {
          printf(" naked");
        }

        if ((pSymbol->get_hasSecurityChecks(&f) == S_OK) && f) {
          printf(" gschecks");
        }

        if ((pSymbol->get_isSafeBuffers(&f) == S_OK) && f) {
          printf(" safebuffers");
        }

        if ((pSymbol->get_hasEHa(&f) == S_OK) && f) {
          printf(" asyncheh");
        }

        if ((pSymbol->get_noStackOrdering(&f) == S_OK) && f) {
          printf(" gsnostackordering");
        }

        if ((pSymbol->get_wasInlined(&f) == S_OK) && f) {
          printf(" wasinlined");
        }

        if ((pSymbol->get_strictGSCheck(&f) == S_OK) && f) {
          printf(" strict_gs_check");
        }

        putchar('\n');
      }

      IDiaEnumSymbols *pEnumChildren;

      if (SUCCEEDED(pSymbol->findChildren(SymTagNull, NULL, nsNone, &pEnumChildren))) {
        IDiaSymbol *pChild;
        ULONG celt = 0;

        while (SUCCEEDED(pEnumChildren->Next(1, &pChild, &celt)) && (celt == 1)) {
          PrintSymbol(pChild, dwIndent + 2);
          pChild->Release();
        }

        pEnumChildren->Release();
      }
      return;

    case SymTagAnnotation:
      PrintLocation(pSymbol);
      putchar('\n');
      break;

    case SymTagLabel:
      PrintLocation(pSymbol);
      printf(", ");
      PrintName(pSymbol);
      break;

    case SymTagEnum:
    case SymTagTypedef:
    case SymTagUDT:
    case SymTagBaseClass:
      PrintUDT(pSymbol);
      break;

    case SymTagFuncDebugStart:
    case SymTagFuncDebugEnd:
      PrintLocation(pSymbol);
      break;

    case SymTagFunctionArgType:
    case SymTagFunctionType:
    case SymTagPointerType:
    case SymTagArrayType:
    case SymTagBaseType:
      if (pSymbol->get_type(&pType) == S_OK) {
        PrintType(pType);
        pType->Release();
      }

      putchar('\n');
      break;

    case SymTagThunk:
      PrintThunk(pSymbol);
      break;

    case SymTagCallSite:
      PrintCallSiteInfo(pSymbol);
      break;

    case SymTagHeapAllocationSite:
      PrintHeapAllocSite(pSymbol);
      break;

    case SymTagCoffGroup:
      PrintCoffGroup(pSymbol);
      break;
        
    default:
      PrintName(pSymbol);

      if (pSymbol->get_type(&pType) == S_OK) {
        printf(" has type ");
        PrintType(pType);
        pType->Release();
      }
  }

  if ((dwSymTag == SymTagUDT) || (dwSymTag == SymTagAnnotation)) {
    IDiaEnumSymbols *pEnumChildren;

    putchar('\n');

    if (SUCCEEDED(pSymbol->findChildren(SymTagNull, NULL, nsNone, &pEnumChildren))) {
      IDiaSymbol *pChild;
      ULONG celt = 0;

      while (SUCCEEDED(pEnumChildren->Next(1, &pChild, &celt)) && (celt == 1)) {
        PrintSymbol(pChild, dwIndent + 2);
        pChild->Release();
      }

      pEnumChildren->Release();
    }
  }
  putchar('\n');
}

////////////////////////////////////////////////////////////
// Print the string coresponding to the symbol's tag property
//
void PrintSymTag(DWORD dwSymTag)
{
  printf("%-15s: ", SafeDRef(rgTags, dwSymTag));
}

////////////////////////////////////////////////////////////
// Print the name of the symbol
//
void PrintName(IDiaSymbol *pSymbol)
{
  BSTR bstrName;
  BSTR bstrUndName;

  if (pSymbol->get_name(&bstrName) != S_OK) {
    printf("(none)");
    return;
  }

  if (pSymbol->get_undecoratedName(&bstrUndName) == S_OK) {
    char *name = wc2c(bstrName);
    char *undName = wc2c(bstrUndName);
    if (strcmp(name, undName) == 0) {
      printf("%s", name);
    }
    else {
      printf("%s(%s)", undName, name);
    }

    SysFreeString(bstrUndName);
  }

  else {
    printf("%s", wc2c(bstrName));
  }

  SysFreeString(bstrName);
}

////////////////////////////////////////////////////////////
// Print the undecorated name of the symbol
//  - only SymTagFunction, SymTagData and SymTagPublicSymbol
//    can have this property set
//
void PrintUndName(IDiaSymbol *pSymbol)
{
  BSTR bstrName;

  if (pSymbol->get_undecoratedName(&bstrName) != S_OK) {
    if (pSymbol->get_name(&bstrName) == S_OK) {
      // Print the name of the symbol instead

      printf("%s", (bstrName[0] != L'\0') ? wc2c(bstrName) : "(none)");

      SysFreeString(bstrName);
    }

    else {
      printf("(none)");
    }

    return;
  }

  if (bstrName[0] != L'\0') {
    printf("%s", wc2c(bstrName));
  }

  SysFreeString(bstrName);
}

////////////////////////////////////////////////////////////
// Print a SymTagThunk symbol's info
//
void PrintThunk(IDiaSymbol *pSymbol)
{
  DWORD dwRVA;
  DWORD dwISect;
  DWORD dwOffset;

  if ((pSymbol->get_relativeVirtualAddress(&dwRVA) == S_OK) &&
      (pSymbol->get_addressSection(&dwISect) == S_OK) &&
      (pSymbol->get_addressOffset(&dwOffset) == S_OK)) {
    printf("[%08X][%04X:%08X]", dwRVA, dwISect, dwOffset);
  }

  if ((pSymbol->get_targetSection(&dwISect) == S_OK) &&
      (pSymbol->get_targetOffset(&dwOffset) == S_OK) &&
      (pSymbol->get_targetRelativeVirtualAddress(&dwRVA) == S_OK)) {
    printf(", target [%08X][%04X:%08X] ", dwRVA, dwISect, dwOffset);
  }

  else {
    printf(", target ");

    PrintName(pSymbol);
  }
}

////////////////////////////////////////////////////////////
// Print the compiland/module details: language, platform...
//
void PrintCompilandDetails(IDiaSymbol *pSymbol)
{
  DWORD dwLanguage;

  if (pSymbol->get_language(&dwLanguage) == S_OK) {
    printf("\n\tLanguage: %s\n", SafeDRef(rgLanguage, dwLanguage));
  }

  DWORD dwPlatform;

  if (pSymbol->get_platform(&dwPlatform) == S_OK) {
    printf("\tTarget processor: %s\n", SafeDRef(rgProcessorStrings, dwPlatform));
  }

  BOOL fEC;

  if (pSymbol->get_editAndContinueEnabled(&fEC) == S_OK) {
    if (fEC) {
      printf("\tCompiled for edit and continue: yes\n");
    }

    else {
      printf("\tCompiled for edit and continue: no\n");
    }
  }

  BOOL fDbgInfo;

  if (pSymbol->get_hasDebugInfo(&fDbgInfo) == S_OK) {
    if (fDbgInfo) {
      printf("\tCompiled without debugging info: no\n");
    }

    else {
      printf("\tCompiled without debugging info: yes\n");
    }
  }

  BOOL fLTCG;

  if (pSymbol->get_isLTCG(&fLTCG) == S_OK) {
    if (fLTCG) {
      printf("\tCompiled with LTCG: yes\n");
    }

    else {
      printf("\tCompiled with LTCG: no\n");
    }
  }

  BOOL fDataAlign;

  if (pSymbol->get_isDataAligned(&fDataAlign) == S_OK) {
    if (fDataAlign) {
      printf("\tCompiled with /bzalign: no\n");
    }

    else {
      printf("\tCompiled with /bzalign: yes\n");
    }
  }

  BOOL fManagedPresent;

  if (pSymbol->get_hasManagedCode(&fManagedPresent) == S_OK) {
    if (fManagedPresent) {
      printf("\tManaged code present: yes\n");
    }

    else {
      printf("\tManaged code present: no\n");
    }
  }

  BOOL fSecurityChecks;

  if (pSymbol->get_hasSecurityChecks(&fSecurityChecks) == S_OK) {
    if (fSecurityChecks) {
      printf("\tCompiled with /GS: yes\n");
    }

    else {
      printf("\tCompiled with /GS: no\n");
    }
  }

  BOOL fSdl;

  if (pSymbol->get_isSdl(&fSdl) == S_OK) {
    if (fSdl) {
      printf("\tCompiled with /sdl: yes\n");
    }

    else {
      printf("\tCompiled with /sdl: no\n");
    }
  }

  BOOL fHotPatch;

  if (pSymbol->get_isHotpatchable(&fHotPatch) == S_OK) {
    if (fHotPatch) {
      printf("\tCompiled with /hotpatch: yes\n");
    }

    else {
      printf("\tCompiled with /hotpatch: no\n");
    }
  }

  BOOL fCVTCIL;

  if (pSymbol->get_isCVTCIL(&fCVTCIL) == S_OK) {
    if (fCVTCIL) {
      printf("\tConverted by CVTCIL: yes\n");
    }

    else {
      printf("\tConverted by CVTCIL: no\n");
    }
  }

  BOOL fMSILModule;

  if (pSymbol->get_isMSILNetmodule(&fMSILModule) == S_OK) {
    if (fMSILModule) {
      printf("\tMSIL module: yes\n");
    }

    else {
      printf("\tMSIL module: no\n");
    }
  }

  DWORD dwVerMajor;
  DWORD dwVerMinor;
  DWORD dwVerBuild;
  DWORD dwVerQFE;

  if ((pSymbol->get_frontEndMajor(&dwVerMajor) == S_OK) &&
      (pSymbol->get_frontEndMinor(&dwVerMinor) == S_OK) &&
      (pSymbol->get_frontEndBuild(&dwVerBuild) == S_OK)) {
    printf("\tFrontend Version: Major = %u, Minor = %u, Build = %u",
            dwVerMajor,
            dwVerMinor,
            dwVerBuild);

    if (pSymbol->get_frontEndQFE(&dwVerQFE) == S_OK) {
      printf(", QFE = %u", dwVerQFE);
    }

    putchar('\n');
  }

  if ((pSymbol->get_backEndMajor(&dwVerMajor) == S_OK) &&
      (pSymbol->get_backEndMinor(&dwVerMinor) == S_OK) &&
      (pSymbol->get_backEndBuild(&dwVerBuild) == S_OK)) {
    printf("\tBackend Version: Major = %u, Minor = %u, Build = %u",
            dwVerMajor,
            dwVerMinor,
            dwVerBuild);

    if (pSymbol->get_backEndQFE(&dwVerQFE) == S_OK) {
      printf(", QFE = %u", dwVerQFE);
    }

    putchar('\n');
  }

  BSTR bstrCompilerName;

  if (pSymbol->get_compilerName(&bstrCompilerName) == S_OK) {
    if (bstrCompilerName != NULL) {
      printf("\tVersion string: %s", wc2c(bstrCompilerName));

      SysFreeString(bstrCompilerName);
    }
  }

  putchar('\n');
}

////////////////////////////////////////////////////////////
// Print the compilan/module env
//
void PrintCompilandEnv(IDiaSymbol *pSymbol)
{
  PrintName(pSymbol);
  printf(" =");

  VARIANT vt = { VT_EMPTY };

  if (pSymbol->get_value(&vt) == S_OK) {
    PrintVariant(vt);
    VariantClear((VARIANTARG *) &vt);
  }
}

////////////////////////////////////////////////////////////
// Print a string corespondig to a location type
//
void PrintLocation(IDiaSymbol *pSymbol)
{
  DWORD dwLocType;
  DWORD dwRVA, dwSect, dwOff, dwReg, dwBitPos, dwSlot;
  LONG lOffset;
  ULONGLONG ulLen;
  VARIANT vt = { VT_EMPTY };

  if (pSymbol->get_locationType(&dwLocType) != S_OK) {
    // It must be a symbol in optimized code

    printf("symbol in optmized code");
    return;
  }

  switch (dwLocType) {
    case LocIsStatic:
      if ((pSymbol->get_relativeVirtualAddress(&dwRVA) == S_OK) &&
          (pSymbol->get_addressSection(&dwSect) == S_OK) &&
          (pSymbol->get_addressOffset(&dwOff) == S_OK)) {
        printf("%s, [%08X][%04X:%08X]", SafeDRef(rgLocationTypeString, dwLocType), dwRVA, dwSect, dwOff);
      }
      break;

    case LocIsTLS:
    case LocInMetaData:
    case LocIsIlRel:
      if ((pSymbol->get_relativeVirtualAddress(&dwRVA) == S_OK) &&
          (pSymbol->get_addressSection(&dwSect) == S_OK) &&
          (pSymbol->get_addressOffset(&dwOff) == S_OK)) {
        printf("%s, [%08X][%04X:%08X]", SafeDRef(rgLocationTypeString, dwLocType), dwRVA, dwSect, dwOff);
      }
      break;

    case LocIsRegRel:
      if ((pSymbol->get_registerId(&dwReg) == S_OK) &&
          (pSymbol->get_offset(&lOffset) == S_OK)) {
        printf("%s Relative, [%08X]", SzNameC7Reg((USHORT) dwReg), lOffset);
      }
      break;

    case LocIsThisRel:
      if (pSymbol->get_offset(&lOffset) == S_OK) {
        printf("this+0x%X", lOffset);
      }
      break;

    case LocIsBitField:
      if ((pSymbol->get_offset(&lOffset) == S_OK) &&
          (pSymbol->get_bitPosition(&dwBitPos) == S_OK) &&
          (pSymbol->get_length(&ulLen) == S_OK)) {
        printf("this(bf)+0x%X:0x%X len(0x%X)", lOffset, dwBitPos, (ULONG)ulLen);
      }
      break;

    case LocIsEnregistered:
      if (pSymbol->get_registerId(&dwReg) == S_OK) {
        printf("enregistered %s", SzNameC7Reg((USHORT) dwReg));
      }
      break;

    case LocIsSlot:
      if (pSymbol->get_slot(&dwSlot) == S_OK) {
        printf("%s, [%08X]", SafeDRef(rgLocationTypeString, dwLocType), dwSlot);
      }
      break;

    case LocIsConstant:
      printf("constant");

      if (pSymbol->get_value(&vt) == S_OK) {
        PrintVariant(vt);
        VariantClear((VARIANTARG *) &vt);
      }
      break;

    case LocIsNull:
      break;

    default :
      printf("Error - invalid location type: 0x%X", dwLocType);
      break;
    }
}

////////////////////////////////////////////////////////////
// Print the type, value and the name of a const symbol
//
void PrintConst(IDiaSymbol *pSymbol)
{
  PrintSymbolType(pSymbol);

  VARIANT vt = { VT_EMPTY };

  if (pSymbol->get_value(&vt) == S_OK) {
    PrintVariant(vt);
    VariantClear((VARIANTARG *) &vt);
  }

  PrintName(pSymbol);
}

////////////////////////////////////////////////////////////
// Print the name and the type of an user defined type
//
void PrintUDT(IDiaSymbol *pSymbol)
{
  PrintName(pSymbol);
  PrintSymbolType(pSymbol);
}

////////////////////////////////////////////////////////////
// Print a string representing the type of a symbol
//
void PrintSymbolType(IDiaSymbol *pSymbol)
{
  IDiaSymbol *pType;

  if (pSymbol->get_type(&pType) == S_OK) {
    printf(", Type: ");
    PrintType(pType);
    pType->Release();
  }
}

////////////////////////////////////////////////////////////
// Print the information details for a type symbol
//
void PrintType(IDiaSymbol *pSymbol)
{
  IDiaSymbol *pBaseType;
  IDiaEnumSymbols *pEnumSym;
  IDiaSymbol *pSym;
  DWORD dwTag;
  BSTR bstrName;
  DWORD dwInfo;
  BOOL bSet;
  DWORD dwRank;
  LONG lCount = 0;
  ULONG celt = 1;

  if (pSymbol->get_symTag(&dwTag) != S_OK) {
    printf("ERROR - can't retrieve the symbol's SymTag\n");
    return;
  }

  if (pSymbol->get_name(&bstrName) != S_OK) {
    bstrName = NULL;
  }

  if (dwTag != SymTagPointerType) {
    if ((pSymbol->get_constType(&bSet) == S_OK) && bSet) {
      printf("const ");
    }

    if ((pSymbol->get_volatileType(&bSet) == S_OK) && bSet) {
       printf("volatile ");
    }

    if ((pSymbol->get_unalignedType(&bSet) == S_OK) && bSet) {
      printf("__unaligned ");
    }
  }

  ULONGLONG ulLen;

  pSymbol->get_length(&ulLen);

  switch (dwTag) {
    case SymTagUDT:
      PrintUdtKind(pSymbol);
      PrintName(pSymbol);
      break;

    case SymTagEnum:
      printf("enum ");
      PrintName(pSymbol);
      break;

    case SymTagFunctionType:
      printf("function ");
      break;

    case SymTagPointerType:
      if (pSymbol->get_type(&pBaseType) != S_OK) {
        printf("ERROR - SymTagPointerType get_type");
        if (bstrName != NULL) {
          SysFreeString(bstrName);
        }
        return;
      }

      PrintType(pBaseType);
      pBaseType->Release();

      if ((pSymbol->get_reference(&bSet) == S_OK) && bSet) {
        printf(" &");
      }

      else {
        printf(" *");
      }

      if ((pSymbol->get_constType(&bSet) == S_OK) && bSet) {
        printf(" const");
      }

      if ((pSymbol->get_volatileType(&bSet) == S_OK) && bSet) {
        printf(" volatile");
      }

      if ((pSymbol->get_unalignedType(&bSet) == S_OK) && bSet) {
        printf(" __unaligned");
      }
      break;

    case SymTagArrayType:
      if (pSymbol->get_type(&pBaseType) == S_OK) {
        PrintType(pBaseType);

        if (pSymbol->get_rank(&dwRank) == S_OK) {
          if (SUCCEEDED(pSymbol->findChildren(SymTagDimension, NULL, nsNone, &pEnumSym)) && (pEnumSym != NULL)) {
            while (SUCCEEDED(pEnumSym->Next(1, &pSym, &celt)) && (celt == 1)) {
              IDiaSymbol *pBound;

              printf("[");

              if (pSym->get_lowerBound(&pBound) == S_OK) {
                PrintBound(pBound);

                printf("..");

                pBound->Release();
              }

              pBound = NULL;

              if (pSym->get_upperBound(&pBound) == S_OK) {
                PrintBound(pBound);

                pBound->Release();
              }

              pSym->Release();
              pSym = NULL;

              printf("]");
            }

            pEnumSym->Release();
          }
        }

        else if (SUCCEEDED(pSymbol->findChildren(SymTagCustomType, NULL, nsNone, &pEnumSym)) &&
                 (pEnumSym != NULL) &&
                 (pEnumSym->get_Count(&lCount) == S_OK) &&
                 (lCount > 0)) {
          while (SUCCEEDED(pEnumSym->Next(1, &pSym, &celt)) && (celt == 1)) {
            printf("[");
            PrintType(pSym);
            printf("]");

            pSym->Release();
          }

          pEnumSym->Release();
        }

        else {
          DWORD dwCountElems;
          ULONGLONG ulLenArray;
          ULONGLONG ulLenElem;

          if (pSymbol->get_count(&dwCountElems) == S_OK) {
            printf("[0x%X]", dwCountElems);
          }

          else if ((pSymbol->get_length(&ulLenArray) == S_OK) &&
                   (pBaseType->get_length(&ulLenElem) == S_OK)) {
            if (ulLenElem == 0) {
              printf("[0x%X]", (ULONG)ulLenArray);
            }

            else {
              printf("[0x%X]", (ULONG)ulLenArray/ (ULONG)ulLenElem);
            }
          }
        }

        pBaseType->Release();
      }

      else {
        printf("ERROR - SymTagArrayType get_type\n");
        if (bstrName != NULL) {
          SysFreeString(bstrName);
        }
        return;
      }
      break;

    case SymTagBaseType:
      if (pSymbol->get_baseType(&dwInfo) != S_OK) {
        printf("SymTagBaseType get_baseType\n");
        if (bstrName != NULL) {
          SysFreeString(bstrName);
        }
        return;
      }

      switch (dwInfo) {
        case btUInt :
          printf("unsigned ");

        // Fall through

        case btInt :
          switch (ulLen) {
            case 1:
              if (dwInfo == btInt) {
                printf("signed ");
              }

              printf("char");
              break;

            case 2:
              printf("short");
              break;

            case 4:
              printf("int");
              break;

            case 8:
              printf("__int64");
              break;
          }

          dwInfo = 0xFFFFFFFF;
          break;

        case btFloat :
          switch (ulLen) {
            case 4:
              printf("float");
              break;

            case 8:
              printf("double");
              break;
          }

          dwInfo = 0xFFFFFFFF;
          break;
      }

      if (dwInfo == 0xFFFFFFFF) {
         break;
      }

      printf("%s", rgBaseType[dwInfo]);
      break;

    case SymTagTypedef:
      PrintName(pSymbol);
      break;

    case SymTagCustomType:
      {
        DWORD idOEM, idOEMSym;
        DWORD cbData = 0;
        DWORD count;

        if (pSymbol->get_oemId(&idOEM) == S_OK) {
          printf("OEMId = %X, ", idOEM);
        }

        if (pSymbol->get_oemSymbolId(&idOEMSym) == S_OK) {
          printf("SymbolId = %X, ", idOEMSym);
        }

        if (pSymbol->get_types(0, &count, NULL) == S_OK) {
          IDiaSymbol** rgpDiaSymbols = (IDiaSymbol**) _alloca(sizeof(IDiaSymbol *) * count);

          if (pSymbol->get_types(count, &count, rgpDiaSymbols) == S_OK) {
            for (ULONG i = 0; i < count; i++) {
              PrintType(rgpDiaSymbols[i]);
              rgpDiaSymbols[i]->Release();
            }
          }
        }

        // print custom data

        if ((pSymbol->get_dataBytes(cbData, &cbData, NULL) == S_OK) && (cbData != 0)) {
          printf(", Data: ");

          BYTE *pbData = new BYTE[cbData];

          pSymbol->get_dataBytes(cbData, &cbData, pbData);

          for (ULONG i = 0; i < cbData; i++) {
            printf("0x%02X ", pbData[i]);
          }

          delete [] pbData;
        }
      }
      break;

    case SymTagData: // This really is member data, just print its location
      PrintLocation(pSymbol);
      break;
  }

  if (bstrName != NULL) {
    SysFreeString(bstrName);
  }
}

////////////////////////////////////////////////////////////
// Print bound information
//
void PrintBound(IDiaSymbol *pSymbol)
{
  DWORD dwTag = 0;
  DWORD dwKind;

  if (pSymbol->get_symTag(&dwTag) != S_OK) {
    printf("ERROR - PrintBound() get_symTag");
    return;
  }

  if (pSymbol->get_locationType(&dwKind) != S_OK) {
    printf("ERROR - PrintBound() get_locationType");
    return;
  }

  if (dwTag == SymTagData && dwKind == LocIsConstant) {
    VARIANT v = { VT_EMPTY };

    if (pSymbol->get_value(&v) == S_OK) {
      PrintVariant(v);
      VariantClear((VARIANTARG *) &v);
    }
  }

  else {
    PrintName(pSymbol);
  }
}

////////////////////////////////////////////////////////////
//
void PrintData(IDiaSymbol *pSymbol)
{
  PrintLocation(pSymbol);

  DWORD dwDataKind;
  if (pSymbol->get_dataKind(&dwDataKind) != S_OK) {
    printf("ERROR - PrintData() get_dataKind");
    return;
  }

  printf(", %s", SafeDRef(rgDataKind, dwDataKind));
  PrintSymbolType(pSymbol);

  printf(", ");
  PrintName(pSymbol);
}

////////////////////////////////////////////////////////////
// Print a VARIANT
//
void PrintVariant(VARIANT var)
{
  switch (var.vt) {
    case VT_UI1:
    case VT_I1:
      printf(" 0x%X", var.bVal);
      break;

    case VT_I2:
    case VT_UI2:
    case VT_BOOL:
      printf(" 0x%X", var.iVal);
      break;

    case VT_I4:
    case VT_UI4:
    case VT_INT:
    case VT_UINT:
    case VT_ERROR:
      printf(" 0x%X", var.lVal);
      break;

    case VT_R4:
      printf(" %g", var.fltVal);
      break;

    case VT_R8:
      printf(" %g", var.dblVal);
      break;

    case VT_BSTR:
      printf(" \"%s\"", wc2c(var.bstrVal));
      break;

    default:
      printf(" ??");
    }
}

////////////////////////////////////////////////////////////
// Print a string corresponding to a UDT kind
//
void PrintUdtKind(IDiaSymbol *pSymbol)
{
  DWORD dwKind = 0;

  if (pSymbol->get_udtKind(&dwKind) == S_OK) {
    printf("%s ", rgUdtKind[dwKind]);
  }
}

void PrintDiaTagValue(DiaTagValue *value) {
  switch (value->valueSizeBytes) {
    case 0:
      printf("(none)");
      break;
    case 1:
      printf("0x%X", *((UINT8 *)&value->value));
      break;
    case 2:
      printf("0x%X", *((UINT16 *)&value->value));
      break;
    case 4:
      printf("0x%32X", *((UINT32 *)&value->value));
      break;
    case 8:
      printf("0x%64lX", *((UINT64 *)&value->value));
      break;
    case 16: {
      UINT64 lo = *((UINT64 *)(&value->value));
      UINT64 hi = *((UINT64 *)(&value->value[8]));

      if (hi == 0) {
          printf("0x%lX\n", lo);
      } else {
          printf("0x%lX%016lX\n", hi, lo);
      }

      break;
    }
    default:
      printf("??");
      break;
  }
}

void PrintIndent(DWORD dwIndent) {
  for (DWORD i = 0; i < dwIndent; i++) {
    putchar(' ');
  }
}

////////////////////////////////////////////////////////////
// Print type informations is details
//
void PrintTypeInDetail(IDiaSymbol *pSymbol, DWORD dwIndent)
{
  IDiaEnumSymbols *pEnumChildren;
  IDiaSymbol *pType;
  IDiaSymbol *pChild;
  DWORD dwSymTag;
  DWORD dwSymTagType;
  ULONG celt = 0;
  BOOL bFlag;

  if (dwIndent > MAX_TYPE_IN_DETAIL) {
    return;
  }

  if (pSymbol->get_symTag(&dwSymTag) != S_OK) {
    printf("ERROR - PrintTypeInDetail() get_symTag\n");
    return;
  }

  PrintSymTag(dwSymTag);

  PrintIndent(dwIndent);

  switch (dwSymTag) {
    case SymTagData:
      PrintData(pSymbol);

      if (pSymbol->get_type(&pType) == S_OK) {
        if (pType->get_symTag(&dwSymTagType) == S_OK) {
          if (dwSymTagType == SymTagUDT) {
            putchar('\n');
            PrintTypeInDetail(pType, dwIndent + 2);
          }
        }
        pType->Release();
      }
      break;

    case SymTagTypedef:
    case SymTagVTable:
      PrintSymbolType(pSymbol);
      break;

    case SymTagEnum:
    case SymTagUDT:
      PrintUDT(pSymbol);
      putchar('\n');

      if (SUCCEEDED(pSymbol->findChildren(SymTagNull, NULL, nsNone, &pEnumChildren))) {
        while (SUCCEEDED(pEnumChildren->Next(1, &pChild, &celt)) && (celt == 1)) {
          PrintTypeInDetail(pChild, dwIndent + 2);

          pChild->Release();
        }

        pEnumChildren->Release();
      }
      return;
      break;

    case SymTagFunction:
      PrintFunctionType(pSymbol);
      return;
      break;

    case SymTagPointerType:
      PrintName(pSymbol);
      printf(" has type ");
      PrintType(pSymbol);
      break;

    case SymTagArrayType:
    case SymTagBaseType:
    case SymTagFunctionArgType:
    case SymTagUsingNamespace:
    case SymTagCustom:
    case SymTagFriend:
      PrintName(pSymbol);
      PrintSymbolType(pSymbol);
      break;

    case SymTagVTableShape:
    case SymTagBaseClass:
      PrintName(pSymbol);

      if ((pSymbol->get_virtualBaseClass(&bFlag) == S_OK) && bFlag) {
        IDiaSymbol *pVBTableType;
        LONG ptrOffset;
        DWORD dispIndex;

        if ((pSymbol->get_virtualBaseDispIndex(&dispIndex) == S_OK) &&
            (pSymbol->get_virtualBasePointerOffset(&ptrOffset) == S_OK)) {
          printf(" virtual, offset = 0x%X, pointer offset = %d, virtual base pointer type = ", dispIndex, ptrOffset);

          if (pSymbol->get_virtualBaseTableType(&pVBTableType) == S_OK) {
            PrintType(pVBTableType);
            pVBTableType->Release();
          }

          else {
            printf("(unknown)");
          }
        }
      }

      else {
        LONG offset;

        if (pSymbol->get_offset(&offset) == S_OK) {
          printf(", offset = 0x%X", offset);
        }
      }

      putchar('\n');

      if (SUCCEEDED(pSymbol->findChildren(SymTagNull, NULL, nsNone, &pEnumChildren))) {
        while (SUCCEEDED(pEnumChildren->Next(1, &pChild, &celt)) && (celt == 1)) {
          PrintTypeInDetail(pChild, dwIndent + 2);
          pChild->Release();
        }

        pEnumChildren->Release();
      }
      break;

    case SymTagFunctionType:
      if (pSymbol->get_type(&pType) == S_OK) {
        PrintType(pType);
      }
      break;

    case SymTagThunk:
      // Happens for functions which only have S_PROCREF
      PrintThunk(pSymbol);
      break;

    case SymTagTaggedUnionCase: {
      PrintName(pSymbol);

      IDiaSymbol11* pSymbol11 = NULL;
      IDiaSymbol* pTagType = NULL;
      DWORD tagOffset = 0;
      DiaTagValue mask;

      if (SUCCEEDED(pSymbol->QueryInterface(__uuidof(IDiaSymbol11), (void**)&pSymbol11)) && pSymbol11->get_discriminatedUnionTag(&pTagType, &tagOffset, &mask) == S_OK) {
        printf("\nTag            :   ");
        PrintIndent(dwIndent);
        printf("this+0x%X, Type: ", tagOffset);
        PrintType(pTagType);

        printf("\nTagMask        :   ");
        PrintIndent(dwIndent);
        PrintDiaTagValue(&mask);
      }

      putchar('\n');

      DWORD rangeSize = 0;
      if (pSymbol11 && pSymbol11->get_tagRanges(0, &rangeSize, NULL) == S_OK) {
        DiaTagValue *pTagValues = new DiaTagValue[rangeSize];
        if (pSymbol11->get_tagRanges(rangeSize, &rangeSize, pTagValues) == S_OK) {
          for (DWORD i = 0; i < rangeSize; i++) {
            if (i % 2 == 0) {
              printf("TagRange       :   ");
              PrintIndent(dwIndent);
              printf("low = ");
            } else {
              printf(", high = ");
            }

            PrintDiaTagValue(&pTagValues[i]);

            if (i % 2 == 1) {
              putchar('\n');
            }
          }
        }
      }

      if (SUCCEEDED(pSymbol->findChildren(SymTagNull, NULL, nsNone, &pEnumChildren))) {
        while (SUCCEEDED(pEnumChildren->Next(1, &pChild, &celt)) && (celt == 1)) {
          PrintTypeInDetail(pChild, dwIndent + 2);

          pChild->Release();
        }

        pEnumChildren->Release();
      }
      break;
    }

    default:
      printf("ERROR - PrintTypeInDetail() invalid SymTag\n");
    }

    putchar('\n');
}

////////////////////////////////////////////////////////////
// Print a function type
//
void PrintFunctionType(IDiaSymbol *pSymbol)
{
  DWORD dwAccess = 0;

  if (pSymbol->get_access(&dwAccess) == S_OK) {
    printf("%s ", SafeDRef(rgAccess, dwAccess));
  }

  BOOL bIsStatic = FALSE;

  if ((pSymbol->get_isStatic(&bIsStatic) == S_OK) && bIsStatic) {
    printf("static ");
  }

  IDiaSymbol *pFuncType;

  if (pSymbol->get_type(&pFuncType) == S_OK) {
    IDiaSymbol *pReturnType;

    if (pFuncType->get_type(&pReturnType) == S_OK) {
      PrintType(pReturnType);
      putchar(' ');

      BSTR bstrName;

      if (pSymbol->get_name(&bstrName) == S_OK) {
        printf("%s", wc2c(bstrName));

        SysFreeString(bstrName);
      }

      IDiaEnumSymbols *pEnumChildren;

      if (SUCCEEDED(pFuncType->findChildren(SymTagNull, NULL, nsNone, &pEnumChildren))) {
        IDiaSymbol *pChild;
        ULONG celt = 0;
        ULONG nParam = 0;

        printf("(");

        while (SUCCEEDED(pEnumChildren->Next(1, &pChild, &celt)) && (celt == 1)) {
          IDiaSymbol *pType;

          if (pChild->get_type(&pType) == S_OK) {
            if (nParam++) {
              printf(", ");
            }

            PrintType(pType);
            pType->Release();
          }

          pChild->Release();
        }

        pEnumChildren->Release();

        printf(")\n");
      }

      pReturnType->Release();
    }

    pFuncType->Release();
  }
}

////////////////////////////////////////////////////////////
//
void PrintSourceFile(IDiaSourceFile *pSource)
{
  BSTR bstrSourceName;

  if (pSource->get_fileName(&bstrSourceName) == S_OK) {
    printf("\t%s", wc2c(bstrSourceName));

    SysFreeString(bstrSourceName);
  }

  else {
    printf("ERROR - PrintSourceFile() get_fileName");
    return;
  }

  BYTE checksum[256];
  DWORD cbChecksum = sizeof(checksum);

  if (pSource->get_checksum(cbChecksum, &cbChecksum, checksum) == S_OK) {
    printf(" (");

    DWORD checksumType;

    if (pSource->get_checksumType(&checksumType) == S_OK) {
      switch (checksumType) {
        case CHKSUM_TYPE_NONE :
          printf("None");
          break;

        case CHKSUM_TYPE_MD5 :
          printf("MD5");
          break;

        case CHKSUM_TYPE_SHA1 :
          printf("SHA1");
          break;

        default :
          printf("0x%X", checksumType);
          break;
      }

      if (cbChecksum != 0) {
        printf(": ");
      }
    }

    for (DWORD ib = 0; ib < cbChecksum; ib++) {
      printf("%02X", checksum[ib]);
    }

    printf(")");
  }
}

////////////////////////////////////////////////////////////
//
void PrintLines(IDiaSession *pSession, IDiaSymbol *pFunction)
{
  DWORD dwSymTag;

  if ((pFunction->get_symTag(&dwSymTag) != S_OK) || (dwSymTag != SymTagFunction)) {
    printf("ERROR - PrintLines() dwSymTag != SymTagFunction");
    return;
  }

  BSTR bstrName;

  if (pFunction->get_name(&bstrName) == S_OK) {
    printf("\n** %s\n\n", wc2c(bstrName));

    SysFreeString(bstrName);
  }

  ULONGLONG ulLength;

  if (pFunction->get_length(&ulLength) != S_OK) {
    printf("ERROR - PrintLines() get_length");
    return;
  }

  DWORD dwRVA;
  IDiaEnumLineNumbers *pLines;

  if (pFunction->get_relativeVirtualAddress(&dwRVA) == S_OK) {
    if (SUCCEEDED(pSession->findLinesByRVA(dwRVA, static_cast<DWORD>(ulLength), &pLines))) {
      PrintLines(pLines);
      pLines->Release();
    }
  }

  else {
    DWORD dwSect;
    DWORD dwOffset;

    if ((pFunction->get_addressSection(&dwSect) == S_OK) &&
        (pFunction->get_addressOffset(&dwOffset) == S_OK)) {
      if (SUCCEEDED(pSession->findLinesByAddr(dwSect, dwOffset, static_cast<DWORD>(ulLength), &pLines))) {
        PrintLines(pLines);
        pLines->Release();
      }
    }
  }
}

////////////////////////////////////////////////////////////
//
void PrintLines(IDiaEnumLineNumbers *pLines)
{
  IDiaLineNumber *pLine;
  DWORD celt;
  DWORD dwRVA;
  DWORD dwSeg;
  DWORD dwOffset;
  DWORD dwLinenum;
  DWORD dwSrcId;
  DWORD dwLength;

  DWORD dwSrcIdLast = (DWORD)(-1);

  while (SUCCEEDED(pLines->Next(1, &pLine, &celt)) && (celt == 1)) {
    if ((pLine->get_relativeVirtualAddress(&dwRVA) == S_OK) &&
        (pLine->get_addressSection(&dwSeg) == S_OK) &&
        (pLine->get_addressOffset(&dwOffset) == S_OK) &&
        (pLine->get_lineNumber(&dwLinenum) == S_OK) &&
        (pLine->get_sourceFileId(&dwSrcId) == S_OK) &&
        (pLine->get_length(&dwLength) == S_OK)) {
      printf("\tline %u at [%08X][%04X:%08X], len = 0x%X", dwLinenum, dwRVA, dwSeg, dwOffset, dwLength);

      if (dwSrcId != dwSrcIdLast) {
        IDiaSourceFile *pSource;

        if (pLine->get_sourceFile(&pSource) == S_OK) {
          PrintSourceFile(pSource);

          dwSrcIdLast = dwSrcId;

          pSource->Release();
        }
      }

      pLine->Release();

      putchar('\n');
    }
  }
}

////////////////////////////////////////////////////////////
// Print the section contribution data: name, Sec::Off, length
void PrintSecContribs(IDiaSectionContrib *pSegment)
{
  DWORD dwRVA;
  DWORD dwSect;
  DWORD dwOffset;
  DWORD dwLen;
  IDiaSymbol *pCompiland;
  BSTR bstrName;

  if ((pSegment->get_relativeVirtualAddress(&dwRVA) == S_OK) &&
      (pSegment->get_addressSection(&dwSect) == S_OK) &&
      (pSegment->get_addressOffset(&dwOffset) == S_OK) &&
      (pSegment->get_length(&dwLen) == S_OK) &&
      (pSegment->get_compiland(&pCompiland) == S_OK) &&
      (pCompiland->get_name(&bstrName) == S_OK)) {
    printf("  %08X  %04X:%08X  %08X  %s\n", dwRVA, dwSect, dwOffset, dwLen, wc2c(bstrName));

    pCompiland->Release();

    SysFreeString(bstrName);
  }
}

////////////////////////////////////////////////////////////
// Print a debug stream data
//
void PrintStreamData(IDiaEnumDebugStreamData *pStream)
{
  BSTR bstrName;

  if (pStream->get_name(&bstrName) != S_OK) {
    printf("ERROR - PrintStreamData() get_name\n");
  }

  else {
    printf("Stream: %s", wc2c(bstrName));

    SysFreeString(bstrName);
  }

  LONG dwElem;

  if (pStream->get_Count(&dwElem) != S_OK) {
      printf("ERROR - PrintStreamData() get_Count\n");
  }

  else {
    printf("(%u)\n", dwElem);
  }

  DWORD cbTotal = 0;

  BYTE data[1024];
  DWORD cbData;
  ULONG celt = 0;

  while (SUCCEEDED(pStream->Next(1, sizeof(data), &cbData, (BYTE *) &data, &celt)) && (celt == 1)) {
    DWORD i;

    for (i = 0; i < cbData; i++) {
      printf("%02X ", data[i]);

      if (i && (i % 8 == 7) && (i+1 < cbData)) {
        printf("- ");
      }
    }

    printf("| ");

    for (i = 0; i < cbData; i++) {
      printf("%c", isprint(data[i]) ? data[i] : '.');
    }

    putchar('\n');

    cbTotal += cbData;
  }

  printf("Summary :\n\tNo of Elems = %u\n", dwElem);
  if (dwElem != 0) {
    printf("\tSizeof(Elem) = %u\n", cbTotal / dwElem);
  }
  putchar('\n');
}

////////////////////////////////////////////////////////////
// Print the FPO info for a given symbol;
//
void PrintFrameData(IDiaFrameData *pFrameData)
{
  DWORD dwSect;
  DWORD dwOffset;
  DWORD cbBlock;
  DWORD cbLocals;                      // Number of bytes reserved for the function locals
  DWORD cbParams;                      // Number of bytes reserved for the function arguments
  DWORD cbMaxStack;
  DWORD cbProlog;
  DWORD cbSavedRegs;
  BOOL bSEH;
  BOOL bEH;
  BOOL bStart;

  if ((pFrameData->get_addressSection(&dwSect) == S_OK) &&
      (pFrameData->get_addressOffset(&dwOffset) == S_OK) &&
      (pFrameData->get_lengthBlock(&cbBlock) == S_OK) &&
      (pFrameData->get_lengthLocals(&cbLocals) == S_OK) &&
      (pFrameData->get_lengthParams(&cbParams) == S_OK) &&
      (pFrameData->get_maxStack(&cbMaxStack) == S_OK) &&
      (pFrameData->get_lengthProlog(&cbProlog) == S_OK) &&
      (pFrameData->get_lengthSavedRegisters(&cbSavedRegs) == S_OK) &&
      (pFrameData->get_systemExceptionHandling(&bSEH) == S_OK) &&
      (pFrameData->get_cplusplusExceptionHandling(&bEH) == S_OK) &&
      (pFrameData->get_functionStart(&bStart) == S_OK)) {
    printf("%04X:%08X   %8X %8X %8X %8X %8X %8X %c   %c   %c",
            dwSect, dwOffset, cbBlock, cbLocals, cbParams, cbMaxStack, cbProlog, cbSavedRegs,
            bSEH ? L'Y' : L'N',
            bEH ? L'Y' : L'N',
            bStart ? L'Y' : L'N');

    BSTR bstrProgram;

    if (pFrameData->get_program(&bstrProgram) == S_OK) {
      printf(" %s", wc2c(bstrProgram));

      SysFreeString(bstrProgram);
    }

    putchar('\n');
  }
}

////////////////////////////////////////////////////////////
// Print all the valid properties associated to a symbol
//
void PrintPropertyStorage(IDiaPropertyStorage *pPropertyStorage)
{
  IEnumSTATPROPSTG *pEnumProps;

  if (SUCCEEDED(pPropertyStorage->Enum(&pEnumProps))) {
    STATPROPSTG prop;
    DWORD celt = 1;

    while (SUCCEEDED(pEnumProps->Next(celt, &prop, &celt)) && (celt == 1)) {
      PROPSPEC pspec = { PRSPEC_PROPID, prop.propid };
      PROPVARIANT vt = { VT_EMPTY };

      if (SUCCEEDED(pPropertyStorage->ReadMultiple(1, &pspec, &vt))) {
        switch (vt.vt) {
          case VT_BOOL:
            printf("%32s:\t %s\n", wc2c(prop.lpwstrName), vt.bVal ? "true" : "false");
            break;

          case VT_I2:
            printf("%32s:\t %d\n", wc2c(prop.lpwstrName), vt.iVal);
            break;

          case VT_UI2:
            printf("%32s:\t %u\n", wc2c(prop.lpwstrName), vt.uiVal);
            break;

          case VT_I4:
            printf("%32s:\t %d\n", wc2c(prop.lpwstrName), vt.lVal);
            break;

          case VT_UI4:
            printf("%32s:\t 0x%0X\n", wc2c(prop.lpwstrName), vt.ulVal);
            break;

          case VT_UI8:
            printf("%32s:\t 0x%lX\n", wc2c(prop.lpwstrName), vt.uhVal.QuadPart);
            break;

          case VT_BSTR:
            printf("%32s:\t %s\n", wc2c(prop.lpwstrName), wc2c(vt.bstrVal));
            break;

          case VT_UNKNOWN:
            printf("%32s:\t %p\n", wc2c(prop.lpwstrName), vt.punkVal);
            break;

          case VT_SAFEARRAY:
            break;
        }

        VariantClear((VARIANTARG *) &vt);
      }

      SysFreeString( prop.lpwstrName );
    }

    pEnumProps->Release();
  }
}
