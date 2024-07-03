#include "../common.h"

extern char *command_line_a;
extern wchar_t *command_line_w;

typedef struct FlsSlot {
    bool occupied;
    PFLS_CALLBACK_FUNCTION callback;
    void *data;
} FlsSlot;

FlsSlot fls_slots[256];

int get_free_fls_slot(void) {
    for (int i = 0; i < ARRAYSIZE(fls_slots); i++) {
        if (!fls_slots[i].occupied) return i;
    }
    assert(!"no free fls slot");
    return -1;
}

uintptr_t resolve_import_by_name(const char *dll, const char *symbol);
uintptr_t resolve_import_by_ordinal(const char *dll, int ordinal);

typedef struct FileMapping {
    FILE *f;
    void *mapped;
    size_t mapped_offset, mapped_len;
} FileMapping;

FileMapping *file_mappings[10];

DWORD last_error = ERROR_SUCCESS;

WINBASEAPI BOOL WINAPI CloseHandle(HANDLE handle)
{
    LOG("%s", __func__);
    assert(0);
    return 0;
}

INT WINAPI DECLSPEC_HOTPATCH CompareStringW(LCID lcid, DWORD flags,
                                            const WCHAR *str1, int len1,
                                            const WCHAR *str2, int len2)
{
    LOG("%s", __func__);
    assert(0);
    return 0;
}

HANDLE WINAPI DECLSPEC_HOTPATCH CreateFileMappingW(HANDLE file,
                                                   LPSECURITY_ATTRIBUTES sa,
                                                   DWORD protect,
                                                   DWORD size_high,
                                                   DWORD size_low, LPCWSTR name)
{
    g_autofree gchar *name_utf8 = name ? g_utf16_to_utf8(name, -1, NULL, NULL, NULL) : NULL;

    assert(protect == PAGE_READONLY);
    assert(size_high == size_low && size_low == 0);

    LOG("%s(%p, %p, %x, %x, %x, %s)", __func__, file, sa, protect, size_high, size_low, name_utf8);
    
    FileMapping *fm = malloc(sizeof(FileMapping));
    memset(fm, 0, sizeof(*fm));
    fm->f = (FILE *)file;

    bool added = false;
    for (int i = 0; i < ARRAYSIZE(file_mappings); i++) {
        if (!file_mappings[i]) {
            file_mappings[i] = fm;
            added = true;
            break;
        }
    }
    assert(added);

    return fm;
}

HANDLE WINAPI DECLSPEC_HOTPATCH CreateFileW(LPCWSTR filename, DWORD access,
                                            DWORD sharing,
                                            LPSECURITY_ATTRIBUTES sa,
                                            DWORD creation, DWORD attributes,
                                            HANDLE template)
{
    g_autofree gchar *filename_utf8 = g_utf16_to_utf8(filename, -1, NULL, NULL, NULL);
    LOG("%s(%s, %x, %x, , %x, %x, %p)", __func__, filename_utf8, access, sharing, creation, attributes, template);
    assert(access == GENERIC_READ);
    FILE *f = fopen(filename_utf8, "rb");
    assert(f);
    return (HANDLE)f;
}

WINBASEAPI void WINAPI DeleteCriticalSection(CRITICAL_SECTION *lpCrit)
{
    LOG("%s", __func__);
    // assert(0);
}

BOOL WINAPI DECLSPEC_HOTPATCH DeleteFileW(LPCWSTR path)
{
    LOG("%s", __func__);
    assert(0);
    return 0;
}

BOOL WINAPI DECLSPEC_HOTPATCH DeviceIoControl(HANDLE handle, DWORD code,
                                              void *in_buff, DWORD in_count,
                                              void *out_buff, DWORD out_count,
                                              DWORD *returned,
                                              OVERLAPPED *overlapped)
{
    LOG("%s", __func__);
    assert(0);
    return 0;
}

WINBASEAPI void *WINAPI EncodePointer(void *)
{
    LOG("%s", __func__);
    assert(0);
    return 0;
}

WINBASEAPI void WINAPI EnterCriticalSection(CRITICAL_SECTION *lpCrit)
{
    LOG("%s", __func__);
    // assert(0);
}

void WINAPI ExitProcess(DWORD status)
{
    LOG("%s", __func__);
    assert(0);
}

DWORD WINAPI DECLSPEC_HOTPATCH ExpandEnvironmentStringsW(LPCWSTR src,
                                                         LPWSTR dst, DWORD len)
{
    LOG("%s", __func__);
    assert(0);
    return 0;
}

BOOL WINAPI DECLSPEC_HOTPATCH FindClose(HANDLE handle)
{
    LOG("%s", __func__);
    assert(0);
    return 0;
}

HANDLE WINAPI DECLSPEC_HOTPATCH FindFirstFileExA(const char *filename,
                                                 FINDEX_INFO_LEVELS level,
                                                 void *data,
                                                 FINDEX_SEARCH_OPS search_op,
                                                 void *filter, DWORD flags)
{
    LOG("%s", __func__);
    assert(0);
    return 0;
}

BOOL WINAPI DECLSPEC_HOTPATCH FindNextFileA(HANDLE handle,
                                            WIN32_FIND_DATAA *data)
{
    LOG("%s", __func__);
    assert(0);
    return 0;
}

HRSRC WINAPI DECLSPEC_HOTPATCH FindResourceExW(HMODULE module, LPCWSTR type,
                                               LPCWSTR name, WORD lang)
{
    LOG("%s", __func__);
    assert(0);
    return 0;
}

BOOL WINAPI DECLSPEC_HOTPATCH FlushFileBuffers(HANDLE file)
{
    LOG("%s", __func__);
    assert(0);
    return 0;
}

BOOL WINAPI DECLSPEC_HOTPATCH FreeEnvironmentStringsW(LPWSTR ptr)
{
    LOG("%s", __func__);
    return TRUE;
}

BOOL WINAPI DECLSPEC_HOTPATCH FreeLibrary(HINSTANCE module)
{
    LOG("%s", __func__);
    assert(0);
    return 0;
}

UINT WINAPI GetACP(void)
{
    LOG("%s", __func__);
    return 1252;
}

LPSTR WINAPI GetCommandLineA(void)
{
    LOG("%s", __func__);
    return command_line_a;
}

LPWSTR WINAPI GetCommandLineW(void)
{
    LOG("%s", __func__);
    return command_line_w;
}

UINT WINAPI DECLSPEC_HOTPATCH GetConsoleCP(void)
{
    LOG("%s", __func__);
    assert(0);
    return 0;
}

BOOL WINAPI DECLSPEC_HOTPATCH GetConsoleMode(HANDLE handle, DWORD *mode)
{
    LOG("%s", __func__);
    assert(0);
    return 0;
}

BOOL WINAPI DECLSPEC_HOTPATCH GetCPInfo(UINT codepage, CPINFO *cpinfo)
{
    LOG("%s(%d, %p)", __func__, codepage, cpinfo);
    assert(codepage == 1252);
    cpinfo->MaxCharSize = 2;
    memset(cpinfo->DefaultChar, '?', sizeof(cpinfo->DefaultChar));
    memset(cpinfo->LeadByte, 0, sizeof(cpinfo->LeadByte));
    return 0;
}

UINT WINAPI DECLSPEC_HOTPATCH GetCurrentDirectoryW(UINT buflen, LPWSTR buf)
{
    LOG("%s", __func__);
    assert(0);
    return 0;
}

WINBASEAPI HANDLE WINAPI GetCurrentProcess(void)
{
    LOG("%s", __func__);
    // assert(0);
    return (HANDLE)1;
}

DWORD WINAPI GetCurrentProcessId(void)
{
    LOG("%s", __func__);
    return (DWORD)getpid();
}

DWORD WINAPI GetCurrentThreadId(void)
{
    LOG("%s", __func__);
    return (DWORD)gettid();
}

UINT WINAPI DECLSPEC_HOTPATCH GetDriveTypeW(LPCWSTR root)
{
    LOG("%s", __func__);
    assert(0);
    return 0;
}

LPWSTR WINAPI DECLSPEC_HOTPATCH GetEnvironmentStringsW(void)
{
    LOG("%s", __func__);
    return L"PATH=C:\\Windows\0\0";
}

DWORD WINAPI DECLSPEC_HOTPATCH GetFileAttributesW(LPCWSTR name)
{
    LOG("%s", __func__);
    assert(0);
    return 0;
}

DWORD WINAPI DECLSPEC_HOTPATCH GetFileSize(HANDLE file, LPDWORD size_high)
{
    LOG("%s", __func__);
    assert(0);
    return 0;
}

BOOL WINAPI DECLSPEC_HOTPATCH GetFileSizeEx(HANDLE file, PLARGE_INTEGER size)
{
    // FIXME: 64b

    LOG("%s", __func__);
    
    FILE *f = (FILE*)file;
    int starting_pos = ftell(f);

    fseek(f, 0, SEEK_END);
    size->HighPart = 0;
    size->LowPart = ftell(f);

    fseek(f, starting_pos, SEEK_SET);

    return TRUE;
}

DWORD WINAPI DECLSPEC_HOTPATCH GetFileType(HANDLE file)
{
    LOG("%s(%p)", __func__, file);
    if (0 <= (uintptr_t)file && (uintptr_t)file <= 2) {
        return FILE_TYPE_CHAR;
    } else {
        return FILE_TYPE_DISK;
    }
}

DWORD WINAPI DECLSPEC_HOTPATCH GetFullPathNameA(LPCSTR name, DWORD len,
                                                LPSTR buffer, LPSTR *lastpart)
{
    LOG("%s", __func__);
    assert(0);
    return 0;
}

DWORD WINAPI DECLSPEC_HOTPATCH GetFullPathNameW(LPCWSTR name, DWORD len,
                                                LPWSTR buffer, LPWSTR *lastpart)
{
    g_autofree gchar *name_utf8 = g_utf16_to_utf8(name, -1, NULL, NULL, NULL);
    LOG("%s(%s, %d, %p, %p)", __func__, name_utf8, len, buffer, lastpart);

    g_autofree gchar *out_utf8 = g_strdup_printf("C:\\%s", name_utf8);
    size_t chars_to_copy = strlen(out_utf8) + 1;
    size_t bytes_to_copy = chars_to_copy * sizeof(WCHAR);
    assert(len >= chars_to_copy);
    gunichar2 *out_utf16 = g_utf8_to_utf16(out_utf8, -1, NULL, NULL, NULL);
    assert(buffer);
    memcpy(buffer, out_utf16, bytes_to_copy);
    assert(lastpart == NULL);
    return strlen(out_utf8);
}

WINBASEAPI DWORD WINAPI GetLastError(void)
{
    LOG("%s -> %d", __func__, last_error);
    DWORD error = last_error;
    return error;
}

DWORD WINAPI DECLSPEC_HOTPATCH GetModuleFileNameA(HMODULE module,
                                                  LPSTR filename, DWORD size)
{
    LOG("%s(%p, %p, %x)", __func__, module, filename, size);
    assert(size);
    if (module == 0) {
        const char *dummy = "dummy.exe";
        strncpy(filename, dummy, size);
        if (size < (strlen(dummy) + 1)) {
            filename[size - 1] = 0;
        }
        return strlen(dummy);
    }
    assert(0);
    return 0;
}

DWORD WINAPI DECLSPEC_HOTPATCH GetModuleFileNameW(HMODULE module,
                                                  LPWSTR filename, DWORD size)
{
    LOG("%s(%s)", __func__, module);

    if (module == NULL) {
        wchar_t *module = L"dummy.exe";
        int module_len = 10;
        if (filename) {
            assert(size >= 10);
            memcpy(filename, module, 10 * sizeof(WCHAR));
        }
        return module_len;
    }

    assert(0);
    return 0;
}

BOOL WINAPI DECLSPEC_HOTPATCH GetModuleHandleExW(DWORD flags, LPCWSTR name,
                                                 HMODULE *module)
{
    LOG("%s(%d, %p, %p)", __func__, flags, name, module);
    assert(0);
    return FALSE;
}

HMODULE WINAPI DECLSPEC_HOTPATCH GetModuleHandleW(LPCWSTR module)
{
    if (module == NULL) {
        return (HMODULE)"dummy.exe";
    }

    g_autofree gchar *module_utf8 = g_utf16_to_utf8(module, -1, NULL, NULL, NULL);
    LOG("%s(%s)", __func__, module_utf8);
    if (!strcasecmp(module_utf8, "kernel32.dll")) {
        return (HMODULE)"KERNEL32.dll";
    } else {
        assert(0);
    }
    return NULL;
}

UINT WINAPI GetOEMCP(void)
{
    LOG("%s", __func__);
    assert(0);
    return 0;
}

FARPROC WINAPI DECLSPEC_HOTPATCH GetProcAddress(HMODULE module, LPCSTR function)
{
    LOG("%s", __func__);

    uintptr_t f = (uintptr_t)function;
    bool is_by_ordinal = (f >> 16) == 0;
    int ordinal = is_by_ordinal ? f & 0xffff : 0;
    if (is_by_ordinal) {
        LOG("GetProcAddress(%s, %p)", (char*)module, function);
    } else {
        LOG("GetProcAddress(%s, %s)", (char*)module, function);
    }

    FARPROC out;
    if (is_by_ordinal) {
        out = (FARPROC)resolve_import_by_ordinal((char*)module, ordinal);
    } else {
        out = (FARPROC)resolve_import_by_name((char*)module, function);
    }
    LOG("--> %p", out);
    return out;
}

HANDLE WINAPI GetProcessHeap(void)
{
    LOG("%s", __func__);
    return (HANDLE)"ProcessHeap";
}

void WINAPI DECLSPEC_HOTPATCH GetStartupInfoW(STARTUPINFOW *info)
{
    LOG("%s", __func__);
    info->cb              = sizeof(*info);
    info->lpReserved      = NULL;
    info->lpDesktop       = (LPWSTR)L"desktop";
    info->lpTitle         = (LPWSTR)L"window";
    info->dwX             = 0;
    info->dwY             = 0;
    info->dwXSize         = 640;
    info->dwYSize         = 480;
    info->dwXCountChars   = 80;
    info->dwYCountChars   = 40;
    info->dwFillAttribute = 0;
    info->dwFlags         = 0;
    info->wShowWindow     = TRUE;
    info->cbReserved2     = 0;
    info->lpReserved2     = NULL;
    info->hStdInput   = (HANDLE)0;
    info->hStdOutput  = (HANDLE)1;
    info->hStdError   = (HANDLE)2;
}

HANDLE WINAPI DECLSPEC_HOTPATCH GetStdHandle(DWORD std_handle)
{
    LOG("%s(%d)", __func__, std_handle);
    switch (std_handle) {
    case STD_INPUT_HANDLE: return 0;
    case STD_OUTPUT_HANDLE: return (HANDLE)1;
    case STD_ERROR_HANDLE: return (HANDLE)2;
    default: assert(0);
    }
    return NULL;
}

BOOL WINAPI DECLSPEC_HOTPATCH GetStringTypeW(DWORD type, const WCHAR *src,
                                             INT count, WORD *chartype)
{
    LOG("%s", __func__);
    assert(0);
    return FALSE;
}

void WINAPI DECLSPEC_HOTPATCH GetSystemInfo(SYSTEM_INFO *si)
{
    LOG("%s", __func__);
    long page_size = sysconf(_SC_PAGESIZE);

    si->wProcessorArchitecture = PROCESSOR_ARCHITECTURE_AMD64;
    si->dwPageSize = page_size;
    si->lpMinimumApplicationAddress = 0;
    si->lpMaximumApplicationAddress = (void*)0xFFFFFFFFFFFFFFFFULL;
    si->dwActiveProcessorMask = 1;
    si->dwNumberOfProcessors = 1;
    si->dwProcessorType = PROCESSOR_AMD_X8664;
    si->dwAllocationGranularity = page_size;
}

void WINAPI DECLSPEC_HOTPATCH GetSystemTimeAsFileTime(FILETIME *lpSystemTimeAsFileTime)
{
    LOG("%s", __func__);
    time_t seconds_since_epoch = 0;
    time(&seconds_since_epoch);
    uint64_t timestamp = 116444736000000000 + seconds_since_epoch * 10000000;
    lpSystemTimeAsFileTime->dwLowDateTime = 0xFFFFFFFF & timestamp;
    lpSystemTimeAsFileTime->dwHighDateTime = timestamp >> 32;
}

WINBASEAPI LPVOID WINAPI HeapAlloc(
    HANDLE heap, DWORD flags,
    SIZE_T
        size) //__WINE_ALLOC_SIZE(3) __WINE_DEALLOC(HeapFree,3) __WINE_MALLOC;
{
    LOG("%s(%s, 0x%x, %zd)", __func__, (char*)heap, flags, size);
    
    uint8_t *data = malloc(size + sizeof(SIZE_T));
    uint8_t *r = data + sizeof(SIZE_T);
    *(SIZE_T *)data = size;

    if (flags & HEAP_ZERO_MEMORY) {
        memset(r, 0, size);
    }

    LOG("-> %p", r);
    return r;
}

WINBASEAPI BOOL WINAPI HeapFree(HANDLE heap, DWORD flags, LPVOID ptr)
{
    LOG("%s(%s, 0x%x, %p)", __func__, (char*)heap, flags, ptr);
    free((uint8_t *)ptr - sizeof(SIZE_T));
    return FALSE;
}

WINBASEAPI LPVOID WINAPI
HeapReAlloc(HANDLE heap, DWORD flags, LPVOID ptr,
            SIZE_T size) //__WINE_ALLOC_SIZE(4) __WINE_DEALLOC(HeapFree,3);
{
    LOG("%s(%s, 0x%x, %p, %zd)", __func__, (char*)heap, flags, ptr, size);

    void *allocation = (uint8_t *)ptr - sizeof(SIZE_T);
    SIZE_T original_size = *(SIZE_T *)allocation;

    uint8_t *data = realloc(allocation, size);
    uint8_t *r = data + sizeof(SIZE_T);
    *(SIZE_T *)data = size;

    if ((flags & HEAP_ZERO_MEMORY) && (size > original_size)) {
        memset(data + original_size, 0, size - original_size);
    }

    return r;
}

WINBASEAPI SIZE_T WINAPI HeapSize(HANDLE heap, DWORD flags, LPCVOID ptr)
{
    LOG("%s(%s, 0x%x, %p)", __func__, (char*)heap, flags, ptr);
    return *(SIZE_T *)((uint8_t *)ptr - sizeof(SIZE_T));
}

WINBASEAPI void WINAPI InitializeCriticalSection(CRITICAL_SECTION *lpCrit)
{
    LOG("%s", __func__);
    // assert(0);
}

BOOL WINAPI DECLSPEC_HOTPATCH InitializeCriticalSectionEx( CRITICAL_SECTION *crit, DWORD spincount,
                                                           DWORD flags )
{
    LOG("%s", __func__);
    crit->SpinCount = 0;
    return TRUE;
}

WINBASEAPI BOOL WINAPI
InitializeCriticalSectionAndSpinCount(CRITICAL_SECTION *lpCrit, DWORD count)
{
    LOG("%s", __func__);
    lpCrit->SpinCount = count;
    return FALSE;
}

WINBASEAPI VOID WINAPI InitializeSListHead(PSLIST_HEADER header)
{
    LOG("%s", __func__);
    memset(header, 0, sizeof(*header));
}

WINBASEAPI PSLIST_ENTRY WINAPI InterlockedFlushSList(PSLIST_HEADER header)
{
    LOG("%s", __func__);
    assert(0);
    return NULL;
}

BOOL WINAPI IsDebuggerPresent(void)
{
    LOG("%s", __func__);
    assert(0);
    return FALSE;
}

BOOL WINAPI DECLSPEC_HOTPATCH IsProcessorFeaturePresent(DWORD feature)
{
    LOG("%s", __func__);
    assert(0);
    return FALSE;
}

BOOL WINAPI DECLSPEC_HOTPATCH IsValidCodePage(UINT codepage)
{
    LOG("%s(%d)", __func__, codepage);
    return TRUE;
}

INT WINAPI DECLSPEC_HOTPATCH LCMapStringW(LCID lcid, DWORD flags,
                                          const WCHAR *src, int srclen,
                                          WCHAR *dst, int dstlen)
{
    LOG("%s", __func__);
    assert(0);
    return 0;
}

WINBASEAPI void WINAPI LeaveCriticalSection(CRITICAL_SECTION *lpCrit)
{
    LOG("%s", __func__);
    // assert(0);
}

HMODULE WINAPI DECLSPEC_HOTPATCH LoadLibraryExA(LPCSTR name, HANDLE file,
                                                DWORD flags)
{
    LOG("%s(%s, %p, 0x%x)", __func__, name, file, flags);
    if (!strcasecmp(name, "OLEAUT32.dll")) {
        return (HMODULE)"OLEAUT32.dll";
    } else if (!memcmp(name, "api-ms-win-", 11) || !strcasecmp(name, "kernel32")) {
        return (HMODULE)"KERNEL32.dll";
    } else if (!strcasecmp(name, "ADVAPI32.dll")) {
        return (HMODULE)"ADVAPI32.dll";
    } else if (!strcasecmp(name, "msdia140.dll")) {

        // static int is_loaded = 0;

        // if (!is_loaded) {
        //     const char *msdia140_dll_path = getenv("MSDIA_PATH");
        //     if (!msdia140_dll_path) {
        //         msdia140_dll_path = "msdia140.dll";
        //     }
        //     load_pe(msdia140_dll_path);
        //     is_loaded = 1;
        // }

        return (HMODULE)"msdia140.dll";
    } else {
        assert(0);
    }
    return NULL;
}

HMODULE WINAPI DECLSPEC_HOTPATCH LoadLibraryA( LPCSTR name )
{
    return LoadLibraryExA( name, 0, 0 );
}

HMODULE WINAPI DECLSPEC_HOTPATCH LoadLibraryExW(LPCWSTR name, HANDLE file,
                                                DWORD flags)
{
    g_autofree char *utf8_name = g_utf16_to_utf8(name, -1, NULL, NULL, NULL);
    LOG("%s(%s, %p, 0x%x)", __func__, utf8_name, file, flags);
    return (HMODULE)LoadLibraryExA(utf8_name, file, flags);
}

HGLOBAL WINAPI DECLSPEC_HOTPATCH LoadResource(HINSTANCE module, HRSRC rsrc)
{
    LOG("%s", __func__);
    assert(0);
    return NULL;
}

HLOCAL WINAPI DECLSPEC_HOTPATCH LocalAlloc(UINT flags, SIZE_T size)
{
    LOG("%s(0x%x, %zd)", __func__, flags, size);
    
    void *data = malloc(size);

    if (flags & LMEM_ZEROINIT) {
        memset(data, 0, size);
    }

    LOG("-> %p", data);
    return data;
}

HLOCAL WINAPI DECLSPEC_HOTPATCH LocalFree(HLOCAL handle)
{
    LOG("%s(%p)", __func__, handle);
    free(handle);
    return NULL;
}

LPVOID WINAPI DECLSPEC_HOTPATCH LockResource(HGLOBAL handle)
{
    LOG("%s", __func__);
    assert(0);
    return NULL;
}

LPVOID WINAPI DECLSPEC_HOTPATCH MapViewOfFileEx(HANDLE handle, DWORD access,
                                                DWORD offset_high,
                                                DWORD offset_low, SIZE_T count,
                                                LPVOID addr)
{
    LOG("%s(%p, %x, %x, %x, %zx, %p)", __func__, handle, access, offset_high, offset_low, count, addr);

    FileMapping *fm = (FileMapping *)handle;

    assert(offset_high == 0); // FIXME: 64b
    assert(access == FILE_MAP_READ);
    assert(count);

    assert(!fm->mapped);

    uint8_t *fdata =
        (uint8_t *)mmap(addr, count, PROT_READ, MAP_PRIVATE, fileno(fm->f), offset_low);
    assert(fdata && "mmap");

    if (addr && fdata != addr) {
        munmap(fdata, count);
        last_error = ERROR_INVALID_ADDRESS;
        LOG("Mapping failed! Could not map to desired address");
        return 0;
    }

    fm->mapped = fdata;
    fm->mapped_offset = offset_low;
    fm->mapped_len = count;

    LOG("-> %p", fm->mapped);

    return fdata;
}

LPVOID WINAPI DECLSPEC_HOTPATCH MapViewOfFile(HANDLE mapping, DWORD access,
                                              DWORD offset_high,
                                              DWORD offset_low, SIZE_T count)
{
    LOG("%s(%p, %x, %x, %x, %zx)", __func__, mapping, access, offset_high, offset_low, count);

    assert(0);
    return NULL;
}

INT WINAPI DECLSPEC_HOTPATCH MultiByteToWideChar(UINT codepage, DWORD flags,
                                                 const char *src, INT srclen,
                                                 WCHAR *dst, INT dstlen)
{
    assert(srclen != 0);

    glong items_written;
    LOG("%s(%d, %x, %s, %d, %p, %d)", __func__, codepage, flags, src, srclen, dst, dstlen);
    g_autofree gunichar2 *src_utf16 = g_utf8_to_utf16(src, srclen, NULL, &items_written, NULL);

    if (dst) {
        assert(dstlen >= items_written);
        memcpy(dst, src_utf16, items_written * sizeof(WCHAR));
    }
    return items_written;
}

WINBASEAPI BOOL WINAPI QueryPerformanceCounter(LARGE_INTEGER *counter)
{
    LOG("%s", __func__);
    counter->QuadPart = 1;
    return TRUE;
}

WINBASEAPI void WINAPI RaiseException(DWORD code, DWORD flags, DWORD count,
                                      const ULONG_PTR *args)
{
    LOG("%s", __func__);
    assert(0);
}

BOOL WINAPI ReadConsoleW(HANDLE handle, void *buffer, DWORD length,
                         DWORD *count, void *reserved)
{
    LOG("%s", __func__);
    assert(0);
    return FALSE;
}

BOOL WINAPI DECLSPEC_HOTPATCH ReadFile(HANDLE file, LPVOID buffer, DWORD count,
                                       LPDWORD result, LPOVERLAPPED overlapped)
{
    LOG("%s(%p, %p, %d, %p, %p)", __func__, file, buffer, count, result, overlapped);
    assert(!overlapped);

    int r = fread(buffer, 1, count, (FILE*)file);
    if (result) {
        *result = r;
    }
    return r > 0;
}

NTSYSAPI void WINAPI RtlCaptureContext(CONTEXT *context)
{
    LOG("%s", __func__);
    assert(0);
}

PRUNTIME_FUNCTION WINAPI RtlLookupFunctionEntry(ULONG_PTR pc, ULONG_PTR *base,
                                                UNWIND_HISTORY_TABLE *table)
{
    LOG("%s", __func__);
    assert(0);
    return NULL;
}

PVOID WINAPI RtlPcToFileHeader(PVOID pc, PVOID *address)
{
    LOG("%s", __func__);
    assert(0);
    return NULL;
}

void WINAPI RtlUnwindEx(PVOID end_frame, PVOID target_ip, EXCEPTION_RECORD *rec,
                        PVOID retval, CONTEXT *context, UNWIND_HISTORY_TABLE *table)
{
    LOG("%s", __func__);
    assert(0);
}

PEXCEPTION_ROUTINE WINAPI RtlVirtualUnwind(
    ULONG type, ULONG_PTR base, ULONG_PTR pc, RUNTIME_FUNCTION *func,
    CONTEXT *context, PVOID *handler_data, ULONG_PTR *frame_ret,
    KNONVOLATILE_CONTEXT_POINTERS *ctx_ptr)
{
    LOG("%s", __func__);
    assert(0);
    return NULL;
}

BOOL WINAPI DECLSPEC_HOTPATCH SetEndOfFile(HANDLE file)
{
    LOG("%s", __func__);
    assert(0);
    return FALSE;
}

BOOL WINAPI DECLSPEC_HOTPATCH SetEnvironmentVariableW(LPCWSTR name,
                                                      LPCWSTR value)
{
    LOG("%s", __func__);
    assert(0);
    return FALSE;
}

BOOL WINAPI DECLSPEC_HOTPATCH SetFileAttributesW(LPCWSTR name, DWORD attributes)
{
    LOG("%s", __func__);
    assert(0);
    return FALSE;
}

BOOL WINAPI DECLSPEC_HOTPATCH SetFilePointerEx(HANDLE file,
                                               LARGE_INTEGER distance,
                                               LARGE_INTEGER *newpos,
                                               DWORD method)
{
    LOG("%s(%d %d, %p, %d)", __func__, distance.HighPart, distance.LowPart, newpos, method);
    FILE *f = (FILE *)file;

    int whence;
    if (method == FILE_BEGIN) {
        whence = SEEK_SET;
    } else if (method == FILE_CURRENT) {
        whence = SEEK_CUR;
    } else if (method == FILE_END) {
        whence = SEEK_END;
    } else {
        assert(0);
    }

    assert(distance.HighPart == 0);
    int r = fseek(f, distance.LowPart, whence);
    if (r == -1) {
        assert(0);
        return FALSE;
    }

    if (newpos) {
        newpos->HighPart = 0;
        newpos->LowPart = ftell(f);
    }

    return TRUE;
}

void WINAPI SetLastError(DWORD err)
{
    LOG("%s(%d)", __func__, err);
    last_error = err;
}

BOOL WINAPI DECLSPEC_HOTPATCH SetStdHandle(DWORD std_handle, HANDLE handle)
{
    LOG("%s", __func__);
    assert(0);
    return FALSE;
}

LPTOP_LEVEL_EXCEPTION_FILTER WINAPI DECLSPEC_HOTPATCH
SetUnhandledExceptionFilter(LPTOP_LEVEL_EXCEPTION_FILTER filter)
{
    LOG("%s", __func__);
    // assert(0);
    return NULL;
}

DWORD WINAPI DECLSPEC_HOTPATCH SizeofResource(HINSTANCE module, HRSRC rsrc)
{
    LOG("%s", __func__);
    assert(0);
    return 0;
}

BOOL WINAPI DECLSPEC_HOTPATCH TerminateProcess(HANDLE handle, DWORD exit_code)
{
    LOG("%s(%p, %d)", __func__, handle, exit_code);

    // XXX: blink will crash on macOS ARM64 during libc exit() with
    //        PC 4ffffffffff7 jp +46 48 7a 2e 2f 78 64 69 61
    fflush(stdout);
    _exit(exit_code);

    return FALSE;
}

DWORD WINAPI DECLSPEC_HOTPATCH TlsAlloc(void)
{
    LOG("%s", __func__);
    assert(0);
    return 0;
}

DWORD WINAPI FlsAlloc(
  PFLS_CALLBACK_FUNCTION lpCallback
)
{
    LOG("%s", __func__);
    int slot = get_free_fls_slot();
    fls_slots[slot].occupied = true;
    fls_slots[slot].callback = lpCallback;
    LOG("Allocated slot %d", slot);
    return slot;
}

WINBASEAPI BOOL WINAPI FlsSetValue(DWORD dwFlsIndex, PVOID lpFlsData)
{
    LOG("%s", __func__);
    assert(dwFlsIndex < ARRAYSIZE(fls_slots));
    fls_slots[dwFlsIndex].data = lpFlsData;
    return true;
}

PVOID WINAPI DECLSPEC_HOTPATCH FlsGetValue( DWORD index )
{
    LOG("%s", __func__);
    assert(index < ARRAYSIZE(fls_slots));
    assert(fls_slots[index].occupied);
    return fls_slots[index].data;
}

BOOL WINAPI DECLSPEC_HOTPATCH TlsFree(DWORD index)
{
    LOG("%s", __func__);
    assert(0);
    return FALSE;
}

LPVOID WINAPI DECLSPEC_HOTPATCH TlsGetValue(DWORD index)
{
    LOG("%s", __func__);
    assert(0);
}

BOOL WINAPI DECLSPEC_HOTPATCH TlsSetValue(DWORD index, LPVOID value)
{
    LOG("%s", __func__);
    assert(0);
    return FALSE;
}

LONG WINAPI UnhandledExceptionFilter(EXCEPTION_POINTERS *epointers)
{
    LOG("%s", __func__);
    assert(0);
    return 0;
}

BOOL WINAPI DECLSPEC_HOTPATCH UnmapViewOfFile(const void *addr)
{
    LOG("%s", __func__);

    FileMapping *fm = NULL;
    for (int i = 0; i < ARRAYSIZE(file_mappings); i++) {
        if (file_mappings[i] && file_mappings[i]->mapped == addr) {
            fm = file_mappings[i];
        }
    }
    assert(fm);
    assert(fm->mapped);

    LOG("Unmapping %p of %zd bytes", fm->mapped, fm->mapped_len);
    munmap(fm->mapped, fm->mapped_len);

    fm->mapped = NULL;
    fm->mapped_offset = 0;
    fm->mapped_len = 0;

    return TRUE;
}

LPVOID WINAPI DECLSPEC_HOTPATCH VirtualAlloc(void *addr, SIZE_T size,
                                             DWORD type, DWORD protect)
{
    LOG("%s(%p, %zx, %x, %x)", __func__, addr, size, type, protect);
    // assert(!addr);

    // FIXME: This doesn't really implement the MEM_COMMIT/MEM_RESERVE behavior...
    if (addr) {
        return addr;
    }

    assert(protect == PAGE_READWRITE);

    void *d = malloc(size);
    memset(d, 0, size);
    return d;
}

BOOL WINAPI DECLSPEC_HOTPATCH VirtualFree(void *addr, SIZE_T size, DWORD type)
{
    LOG("%s", __func__);
    free(addr);
    return FALSE;
}

BOOL WINAPI DECLSPEC_HOTPATCH VirtualProtect(void *addr, SIZE_T size,
                                             DWORD new_prot, DWORD *old_prot)
{
    LOG("%s", __func__);
    assert(0);
    return FALSE;
}

SIZE_T WINAPI DECLSPEC_HOTPATCH VirtualQuery(LPCVOID addr,
                                             PMEMORY_BASIC_INFORMATION info,
                                             SIZE_T len)
{
    LOG("%s", __func__);
    assert(0);
    return 0;
}

INT WINAPI DECLSPEC_HOTPATCH WideCharToMultiByte(UINT codepage, DWORD flags,
                                                 LPCWSTR src, INT srclen,
                                                 LPSTR dst, INT dstlen,
                                                 LPCSTR defchar, BOOL *used)
{
    LOG("%s(%d, 0x%x, %p, %d)", __func__, codepage, flags, src, srclen);

    bool null_terminated = srclen < 0;

    assert(sizeof(WCHAR) == 2);

    for (int i = 0; i < srclen; i++) {
        assert(sizeof(src[i]) == 2);
        LOG("%c", src[i]);
    }

    g_autofree char *utf8 = g_utf16_to_utf8(src, srclen, NULL, NULL, NULL);

    size_t bytes_to_copy = strlen(utf8) + null_terminated;

    if (dst == 0 || dstlen == 0) {
        return bytes_to_copy;
    }

    assert(dstlen >= 0);

    memcpy(dst, utf8, MIN(bytes_to_copy, dstlen));

    if (dstlen < bytes_to_copy) {
        last_error = ERROR_INSUFFICIENT_BUFFER;
        return 0;
    }

    return bytes_to_copy;
}

BOOL WINAPI DECLSPEC_HOTPATCH WriteConsoleW(HANDLE handle, const void *buffer,
                                            DWORD length, DWORD *written,
                                            void *reserved)
{
    LOG("%s", __func__);
    assert(0);
    return FALSE;
}

BOOL WINAPI DECLSPEC_HOTPATCH WriteFile(HANDLE file, LPCVOID buffer,
                                        DWORD count, LPDWORD result,
                                        LPOVERLAPPED overlapped)
{
    LOG("%s(%p, %p, %d, %p)", __func__, file, buffer, count, result);
    if (file == (HANDLE)1 || file == (HANDLE)2) {
        size_t num_written = fwrite(buffer, 1, count, stdout);
        if (result) {
            *result = num_written;
        }
        return TRUE;
    }
    assert(0);
    return FALSE;
}

BOOL WINAPI CopyFileW(LPCWSTR lpExistingFileName, LPCWSTR lpNewFileName,
               BOOL bFailIfExists)
{
    g_autofree gchar *lpExistingFileName_utf8 = g_utf16_to_utf8(lpExistingFileName, -1, NULL, NULL, NULL);
    g_autofree gchar *lpNewFileName_utf8 = g_utf16_to_utf8(lpNewFileName, -1, NULL, NULL, NULL);
    LOG("%s(%s, %s, %d)", __func__, lpExistingFileName_utf8, lpNewFileName_utf8, bFailIfExists);
    assert(0);
    return TRUE;
}

BOOL WINAPI CreateDirectoryW(
  LPCWSTR               lpPathName,
  LPSECURITY_ATTRIBUTES lpSecurityAttributes
)
{
    g_autofree gchar *lpPathName_utf8 = g_utf16_to_utf8(lpPathName, -1, NULL, NULL, NULL);
    LOG("%s(%s, %p)", __func__, lpPathName_utf8, lpSecurityAttributes);
    assert(0);
    return TRUE;
}

DWORD WINAPI GetTempPathW(DWORD nBufferLength, LPWSTR lpBuffer)
{
    assert(0);
    return 0;
}

void WINAPI GetNativeSystemInfo(
  LPSYSTEM_INFO lpSystemInfo
)
{
    assert(0);
}

UINT WINAPI GetConsoleOutputCP(void)
{
    assert(0);
    return 0;
}

BOOL WINAPI FlsFree(
  DWORD dwFlsIndex
)
{
    assert(0);
    return 0;
}

HANDLE WINAPI FindFirstFileExW(LPCWSTR lpFileName,
                               FINDEX_INFO_LEVELS fInfoLevelId,
                               LPVOID lpFindFileData,
                               FINDEX_SEARCH_OPS fSearchOp,
                               LPVOID lpSearchFilter, DWORD dwAdditionalFlags)
{
    assert(0);
    return NULL;
}

BOOL WINAPI FindNextFileW(
  HANDLE             hFindFile,
  LPWIN32_FIND_DATAW lpFindFileData
)
{
    assert(0);
    return FALSE;
}

void WINAPI ReleaseSRWLockExclusive(
PSRWLOCK SRWLock
)
{
    // assert(0);
}

void WINAPI AcquireSRWLockExclusive(
  PSRWLOCK SRWLock
)
{
    // assert(0);
}

BOOLEAN WINAPI TryAcquireSRWLockExclusive(
  PSRWLOCK SRWLock
)
{
    assert(0);
    return TRUE;
}

void WINAPI InitializeSRWLock(
  PSRWLOCK SRWLock
)
{
    // assert(0);
}

void WINAPI ReleaseSRWLockShared(
  PSRWLOCK SRWLock
)
{
    // assert(0);
}

void WINAPI AcquireSRWLockShared(
  PSRWLOCK SRWLock
)
{
    // assert(0);
}

void WINAPI Sleep(
  DWORD dwMilliseconds
)
{
    assert(0);
}

HANDLE WINAPI CreateFileMappingA(
HANDLE                hFile,
LPSECURITY_ATTRIBUTES lpFileMappingAttributes,
DWORD                 flProtect,
DWORD                 dwMaximumSizeHigh,
DWORD                 dwMaximumSizeLow,
LPCSTR                lpName
)
{
    assert(0);
    return NULL;
}

NTSYSAPI VOID WINAPI RtlUnwind(
PVOID             TargetFrame,
PVOID             TargetIp,
PEXCEPTION_RECORD ExceptionRecord,
PVOID             ReturnValue
)
{
    assert(0);
}

PTP_WORK WINAPI CreateThreadpoolWork(
PTP_WORK_CALLBACK    pfnwk,
PVOID                pv,
PTP_CALLBACK_ENVIRON pcbe
)
{
    assert(0);
    return NULL;
}

void WINAPI SubmitThreadpoolWork(
  PTP_WORK pwk
)
{
    assert(0);
}

void WINAPI WaitForThreadpoolWorkCallbacks(
PTP_WORK pwk,
BOOL     fCancelPendingCallbacks
)
{
    assert(0);
}

void WINAPI CloseThreadpoolWork(
  PTP_WORK pwk
)
{
    assert(0);
}

LONG WINAPI GetCurrentPackageId(UINT32 *bufferLength, BYTE *buffer)
{
    assert(0);
    return 0;
}

void WINAPI GetSystemTimePreciseAsFileTime(
    LPFILETIME lpSystemTimeAsFileTime
)
{
    assert(0);
}

DWORD WINAPI GetTempPath2W(DWORD BufferLength, LPWSTR Buffer)
{
    assert(0);
    return 0;
}

BOOL WINAPI AreFileApisANSI()
{
    LOG("%s", __func__);
    return TRUE;
}

LONG WINAPI AppPolicyGetProcessTerminationMethod(HANDLE token, AppPolicyProcessTerminationMethod *policy)
{
    // FIXME("%p, %p\n", token, policy);

    if(policy)
        *policy = AppPolicyProcessTerminationMethod_TerminateProcess;

    return ERROR_SUCCESS;
}

static ModuleExport kernel32_exports[] = {
    E(AreFileApisANSI),
    E(GetTempPath2W),
    E(GetSystemTimePreciseAsFileTime),
    E(GetCurrentPackageId),
    E(CloseThreadpoolWork),
    E(WaitForThreadpoolWorkCallbacks),
    E(SubmitThreadpoolWork),
    E(CreateThreadpoolWork),
    E(RtlUnwind),
    E(CreateFileMappingA),
    E(Sleep),
    E(InitializeSRWLock),
    E(AcquireSRWLockShared),
    E(ReleaseSRWLockShared),
    E(AcquireSRWLockExclusive),
    E(CloseHandle),
    E(CompareStringW),
    E(CreateDirectoryW),
    E(CreateFileMappingW),
    E(CreateFileW),
    E(CopyFileW),
    E(DeleteCriticalSection),
    E(DeleteFileW),
    E(DeviceIoControl),
    E(EncodePointer),
    E(EnterCriticalSection),
    E(ExitProcess),
    E(ExpandEnvironmentStringsW),
    E(FindClose),
    E(FindFirstFileExA),
    E(FindFirstFileExW),
    E(FindNextFileA),
    E(FindNextFileW),
    E(FindResourceExW),
    E(FlushFileBuffers),
    E(FlsAlloc),
    E(FlsGetValue),
    E(FlsFree),
    E(FlsSetValue),
    E(FreeEnvironmentStringsW),
    E(FreeLibrary),
    E(GetACP),
    E(GetCommandLineA),
    E(GetCommandLineW),
    E(GetConsoleCP),
    E(GetConsoleOutputCP),
    E(GetConsoleMode),
    E(GetCPInfo),
    E(GetCurrentDirectoryW),
    E(GetCurrentProcess),
    E(GetCurrentProcessId),
    E(GetCurrentThreadId),
    E(GetDriveTypeW),
    E(GetEnvironmentStringsW),
    E(GetFileAttributesW),
    E(GetFileSize),
    E(GetFileSizeEx),
    E(GetFileType),
    E(GetFullPathNameA),
    E(GetFullPathNameW),
    E(GetLastError),
    E(GetModuleFileNameA),
    E(GetModuleFileNameW),
    E(GetModuleHandleExW),
    E(GetModuleHandleW),
    E(GetNativeSystemInfo),
    E(GetOEMCP),
    E(GetProcAddress),
    E(GetProcessHeap),
    E(GetStartupInfoW),
    E(GetStdHandle),
    E(GetStringTypeW),
    E(GetSystemInfo),
    E(GetSystemTimeAsFileTime),
    E(GetTempPathW),
    E(HeapAlloc),
    E(HeapFree),
    E(HeapReAlloc),
    E(HeapSize),
    E(InitializeCriticalSection),
    E(InitializeCriticalSectionEx),
    E(InitializeCriticalSectionAndSpinCount),
    E(InitializeSListHead),
    E(InterlockedFlushSList),
    E(IsDebuggerPresent),
    E(IsProcessorFeaturePresent),
    E(IsValidCodePage),
    E(LCMapStringW),
    E(LeaveCriticalSection),
    E(LoadLibraryA),
    E(LoadLibraryExA),
    E(LoadLibraryExW),
    E(LoadResource),
    E(LocalAlloc),
    E(LocalFree),
    E(LockResource),
    E(MapViewOfFile),
    E(MapViewOfFileEx),
    E(MultiByteToWideChar),
    E(QueryPerformanceCounter),
    E(RaiseException),
    E(ReadConsoleW),
    E(ReadFile),
    E(ReleaseSRWLockExclusive),
    E(RtlCaptureContext),
    E(RtlLookupFunctionEntry),
    E(RtlPcToFileHeader),
    E(RtlUnwindEx),
    E(RtlVirtualUnwind),
    E(SetEndOfFile),
    E(SetEnvironmentVariableW),
    E(SetFileAttributesW),
    E(SetFilePointerEx),
    E(SetLastError),
    E(SetStdHandle),
    E(SetUnhandledExceptionFilter),
    E(SizeofResource),
    E(TerminateProcess),
    E(TlsAlloc),
    E(TlsFree),
    E(TlsGetValue),
    E(TlsSetValue),
    E(TryAcquireSRWLockExclusive),
    E(UnhandledExceptionFilter),
    E(UnmapViewOfFile),
    E(VirtualAlloc),
    E(VirtualFree),
    E(VirtualProtect),
    E(VirtualQuery),
    E(WideCharToMultiByte),
    E(WriteConsoleW),
    E(WriteFile),
    E(AppPolicyGetProcessTerminationMethod),
};

Module kernel32_module = {"KERNEL32.dll", kernel32_exports,
                          ARRAYSIZE(kernel32_exports)};
