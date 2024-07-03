#ifndef COMMON_H
#define COMMON_H

#include <wchar.h>
#include <assert.h>
#include <errno.h>
#include <malloc.h>
#include <signal.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <ucontext.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <glib.h>

#include <windef.h>
#include <winbase.h>
#include <winnt.h>
#include <winnls.h>
#include <wtypes.h>
#include <appmodel.h>

#ifndef WINE_NTSTATUS_DECLARED
#define WINE_NTSTATUS_DECLARED
typedef LONG NTSTATUS;
#endif
#include "rtlsupportapi.h"

#define DEBUG 0

#if DEBUG
#define LOG(fmt, ...)                             \
    do {                                          \
        fprintf(stderr, fmt "\n", ##__VA_ARGS__); \
    } while (0)
#else
#define LOG(fmt, ...)
#endif

#define ALIGN_UP(x, a) ((x + (a - 1)) & ~(a - 1))

typedef struct ModuleExport {
    const char *name;
    int ordinal;
    uintptr_t handle;
} ModuleExport;

typedef struct Module {
    const char *name;
    ModuleExport *exports;
    int num_exports;
    void *base;
} Module;

#define E(n) { #n, -1, (uintptr_t)&n }
#define EO(o, n) { #n, o, (uintptr_t)&n }

extern Module kernel32_module;
extern Module oleaut32_module;
extern Module ole32_module;
extern Module shlwapi_module;
extern Module advapi32_module;

uint8_t *load_pe(const char *path);
uintptr_t resolve_import_by_name(const char *dll, const char *name);
uintptr_t resolve_import_by_ordinal(const char *dll, int ordinal);

#ifdef __cplusplus
extern "C" {
#endif

char *wc2c(const wchar_t *);
wchar_t *c2wc(const char *);

#ifdef __cplusplus
};
#endif

#endif
