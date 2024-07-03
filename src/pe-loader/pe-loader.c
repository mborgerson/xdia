#include "common.h"

#include <asm/prctl.h>
#include <sys/prctl.h>

#include "winternl.h"

int arch_prctl(int code, unsigned long *addr);

int tls_index = 0;
void *tls_array[32];

RTL_USER_PROCESS_PARAMETERS user_params;
PEB peb = {
    .ProcessParameters = &user_params,
};
TEB teb = {
    .Tib = {
        .Self = &teb,
    },
    .Peb = &peb,
    .ThreadLocalStoragePointer = tls_array,
};

Module *modules[7] = { &kernel32_module, &oleaut32_module, &ole32_module,
                       &shlwapi_module,  &advapi32_module, };

static PIMAGE_NT_HEADERS64 get_image_nt_headers(void *image_base)
{
    PIMAGE_DOS_HEADER dos = (void *)image_base;
    assert(dos->e_magic == IMAGE_DOS_SIGNATURE);

    PIMAGE_NT_HEADERS64 hdr = (void *)(image_base + dos->e_lfanew);
    assert(hdr->Signature == IMAGE_NT_SIGNATURE);

    return hdr;
}

static uintptr_t resolve_import_by_name_or_ordinal(const char *dll,
                                                   const char *name,
                                                   int ordinal)
{
    for (int module_idx = 0;
         (module_idx < ARRAYSIZE(modules)) && modules[module_idx];
         module_idx++) {
        Module *module = modules[module_idx];
        if (!strcmp(module->name, dll)) {
            if (module->base) {
                assert(ordinal < 0 &&
                       "Image export lookup by ordinal not implemented");

                PIMAGE_NT_HEADERS64 hdr = get_image_nt_headers(module->base);
                PIMAGE_DATA_DIRECTORY exports =
                    &hdr->OptionalHeader
                         .DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT];
                if (!exports->Size) {
                    break;
                }
                PIMAGE_EXPORT_DIRECTORY export_directory =
                    (void *)(module->base + exports->VirtualAddress);
                assert(export_directory->NumberOfNames ==
                       export_directory->NumberOfFunctions);

                DWORD *export_names =
                    (void *)(module->base + export_directory->AddressOfNames);
                DWORD *export_addresses =
                    (void *)(module->base +
                             export_directory->AddressOfFunctions);

                for (int i = 0; i < export_directory->NumberOfNames; i++) {
                    const char *export_name =
                        (char *)(module->base + export_names[i]);
                    if (!strcmp(name, export_name)) {
                        return (uintptr_t)(module->base + export_addresses[i]);
                    }
                }
            } else if (module->exports) {
                for (int export_idx = 0; export_idx < module->num_exports;
                     export_idx++) {
                    ModuleExport *export = &module->exports[export_idx];
                    if ((name && export->name && !strcmp(name, export->name)) ||
                        (ordinal >= 0 && ordinal == export->ordinal)) {
                        return export->handle;
                    }
                }
            }
        }
    }

    fprintf(stderr, "Failed to resolve %s.%d:%s\n", dll, ordinal, name);
    assert(0);
    return 0;
}

uintptr_t resolve_import_by_name(const char *dll, const char *name)
{
    return resolve_import_by_name_or_ordinal(dll, name, -1);
}

uintptr_t resolve_import_by_ordinal(const char *dll, int ordinal)
{
    return resolve_import_by_name_or_ordinal(dll, NULL, ordinal);
}

#if DEBUG
static void log_file_header(PIMAGE_FILE_HEADER hdr)
{
    LOG("IMAGE_FILE_HEADER {\n"
        "Machine: %x\n"
        "NumberOfSections: %x\n"
        "TimeDateStamp: %x\n"
        "PointerToSymbolTable: %x\n"
        "NumberOfSymbols: %x\n"
        "SizeOfOptionalHeader: %x\n"
        "Characteristics: %x\n"
        "}",
        hdr->Machine, hdr->NumberOfSections, hdr->TimeDateStamp,
        hdr->PointerToSymbolTable, hdr->NumberOfSymbols,
        hdr->SizeOfOptionalHeader, hdr->Characteristics);
}

static void log_optional_header(PIMAGE_OPTIONAL_HEADER64 hdr)
{
    LOG("IMAGE_OPTIONAL_HEADER64 {\n"
        "MajorLinkerVersion: %x\n"
        "MinorLinkerVersion: %x\n"
        "SizeOfCode: %x\n"
        "SizeOfInitializedData: %x\n"
        "SizeOfUninitializedData: %x\n"
        "AddressOfEntryPoint: %x\n"
        "BaseOfCode: %x\n"
        "ImageBase: %zx\n"
        "SectionAlignment: %x\n"
        "FileAlignment: %x\n"
        "MajorOperatingSystemVersion: %x\n"
        "MinorOperatingSystemVersion: %x\n"
        "MajorImageVersion: %x\n"
        "MinorImageVersion: %x\n"
        "MajorSubsystemVersion: %x\n"
        "MinorSubsystemVersion: %x\n"
        "Win32VersionValue: %x\n"
        "SizeOfImage: %x\n"
        "SizeOfHeaders: %x\n"
        "CheckSum: %x\n"
        "Subsystem: %x\n"
        "DllCharacteristics: %x\n"
        "SizeOfStackReserve: %zx\n"
        "SizeOfStackCommit: %zx\n"
        "SizeOfHeapReserve: %zx\n"
        "SizeOfHeapCommit: %zx\n"
        "LoaderFlags: %x\n"
        "NumberOfRvaAndSizes: %x\n"
        "}",
        hdr->MajorLinkerVersion, hdr->MinorLinkerVersion, hdr->SizeOfCode,
        hdr->SizeOfInitializedData, hdr->SizeOfUninitializedData,
        hdr->AddressOfEntryPoint, hdr->BaseOfCode, hdr->ImageBase,
        hdr->SectionAlignment, hdr->FileAlignment,
        hdr->MajorOperatingSystemVersion, hdr->MinorOperatingSystemVersion,
        hdr->MajorImageVersion, hdr->MinorImageVersion,
        hdr->MajorSubsystemVersion, hdr->MinorSubsystemVersion,
        hdr->Win32VersionValue, hdr->SizeOfImage, hdr->SizeOfHeaders,
        hdr->CheckSum, hdr->Subsystem, hdr->DllCharacteristics,
        hdr->SizeOfStackReserve, hdr->SizeOfStackCommit, hdr->SizeOfHeapReserve,
        hdr->SizeOfHeapCommit, hdr->LoaderFlags, hdr->NumberOfRvaAndSizes);
}

static void log_section_header(PIMAGE_SECTION_HEADER s)
{
    LOG("IMAGE_SECTION_HEADER {\n"
        "Name: %.*s\n"
        "VirtualSize: %x\n"
        "VirtualAddress: %x\n"
        "SizeOfRawData: %x\n"
        "PointerToRawData: %x\n"
        "PointerToRelocations: %x\n"
        "PointerToLinenumbers: %x\n"
        "NumberOfRelocations: %hx\n"
        "NumberOfLinenumbers: %hx\n"
        "Characteristics: %x\n"
        "}",
        (int)sizeof(s->Name), s->Name, s->Misc.VirtualSize, s->VirtualAddress,
        s->SizeOfRawData, s->PointerToRawData, s->PointerToRelocations,
        s->PointerToLinenumbers, s->NumberOfRelocations, s->NumberOfLinenumbers,
        s->Characteristics);
}

static void log_data_directory_entry(PIMAGE_DATA_DIRECTORY dd)
{
    LOG("IMAGE_DATA_DIRECTORY {\n"
        "VirtualAddress = %x\n"
        "Size = %x\n"
        "}",
        dd->VirtualAddress, dd->Size);
}

static void log_image_import_descriptor(void *idata, PIMAGE_IMPORT_DESCRIPTOR desc)
{
    LOG("IMAGE_IMPORT_DESCRIPTOR {\n"
        "Characteristics: %x\n"
        "TimeDateStamp: %x\n"
        "ForwarderChain: %x\n"
        "Name: %x %s\n"
        "FirstThunk: %x\n"
        "}",
        desc->Characteristics, desc->TimeDateStamp, desc->ForwarderChain,
        desc->Name, (char *)(idata + desc->Name), desc->FirstThunk);
}

static void log_export_directory(void *idata, PIMAGE_EXPORT_DIRECTORY export_directory)
{
    LOG("PIMAGE_EXPORT_DIRECTORY {\n"
        "Characteristics: %x\n"
        "TimeDateStamp: %x\n"
        "MajorVersion: %x\n"
        "MinorVersion: %x\n"
        "Name: %s\n"
        "Base: %x\n"
        "NumberOfFunctions: %x\n"
        "NumberOfNames: %x\n"
        "AddressOfFunctions: %x\n"
        "AddressOfNames: %x\n"
        "AddressOfNameOrdinals: %x\n"
        "}",
        export_directory->Characteristics, export_directory->TimeDateStamp,
        export_directory->MajorVersion, export_directory->MinorVersion,
        (char *)(idata + export_directory->Name), export_directory->Base,
        export_directory->NumberOfFunctions, export_directory->NumberOfNames,
        export_directory->AddressOfFunctions, export_directory->AddressOfNames,
        export_directory->AddressOfNameOrdinals);
}

static void log_tls_directory(IMAGE_TLS_DIRECTORY64 *tls_directory)
{
    LOG("IMAGE_TLS_DIRECTORY64 {\n"
        "StartAddressOfRawData: %lx\n"
        "EndAddressOfRawData: %lx\n"
        "AddressOfIndex: %lx\n"
        "AddressOfCallBacks: %lx\n"
        "SizeOfZeroFill: %x\n"
        "Characteristics: %x"
        "}",
        tls_directory->StartAddressOfRawData,
        tls_directory->EndAddressOfRawData, tls_directory->AddressOfIndex,
        tls_directory->AddressOfCallBacks, tls_directory->SizeOfZeroFill,
        tls_directory->Characteristics);
}
#endif

uint8_t *load_pe(const char *path)
{
    //
    // Load image headers and section data
    //

    LOG("Loading %s", path);
    FILE *f = fopen(path, "rb");
    assert(f && "failed to open");

    // Get image size
    fseek(f, 0, SEEK_END);
    size_t fsize = ftell(f);
    fseek(f, 0, SEEK_SET);

    // mmap file for loading
    uint8_t *fdata =
        (uint8_t *)mmap(0, fsize, PROT_READ, MAP_PRIVATE, fileno(f), 0);
    assert(fdata && "mmap");
    fclose(f);

    PIMAGE_NT_HEADERS64 hdr = get_image_nt_headers(fdata);
    assert(hdr->FileHeader.SizeOfOptionalHeader ==
           sizeof(IMAGE_OPTIONAL_HEADER64));
    assert(hdr->OptionalHeader.Magic == IMAGE_NT_OPTIONAL_HDR64_MAGIC);

    // Allocate loaded image
    uint8_t *idata;
    int r = posix_memalign((void *)&idata, hdr->OptionalHeader.SectionAlignment,
                           hdr->OptionalHeader.SizeOfImage);
    assert(!r && "posix_memalign");
    LOG("Allocated image base at %p\n", idata);

    // Copy image header and section data
    LOG("Copying %d bytes of image header", hdr->OptionalHeader.SizeOfHeaders);
    memcpy(idata, fdata, hdr->OptionalHeader.SizeOfHeaders);

    PIMAGE_SECTION_HEADER sec = IMAGE_FIRST_SECTION(hdr);
    uintptr_t sec_rva = ALIGN_UP(hdr->OptionalHeader.SizeOfHeaders,
                                 hdr->OptionalHeader.SectionAlignment);
    for (int i = 0; i < hdr->FileHeader.NumberOfSections; i++) {
        LOG("Section %.*s", (int)sizeof(sec->Name), sec->Name);
        sec_rva = ALIGN_UP(sec_rva, hdr->OptionalHeader.SectionAlignment);
        assert(sec->VirtualAddress == sec_rva);

        // Copy section data
        size_t num_bytes_to_copy =
            min(sec->Misc.VirtualSize, sec->SizeOfRawData);
        memcpy(idata + sec_rva, fdata + sec->PointerToRawData,
               num_bytes_to_copy);
        LOG("Copy [%lx, %lx)", sec_rva, sec_rva + num_bytes_to_copy);
        sec_rva += num_bytes_to_copy;

        // Clear remainder of section data
        size_t num_bytes_to_clear = sec->Misc.VirtualSize - num_bytes_to_copy;
        if (num_bytes_to_clear) {
            memset(idata + sec_rva, 0, num_bytes_to_clear);
            LOG("Clear [%lx, %lx)", sec_rva, sec_rva + num_bytes_to_clear);
            sec_rva += num_bytes_to_clear;
        }

        sec++;
    }

    // Free mapped raw file
    munmap(fdata, fsize);
    fdata = NULL;

    //
    // Begin patching loaded image
    //
    hdr = get_image_nt_headers(idata);

#if DEBUG
    log_file_header(&hdr->FileHeader);
    log_optional_header(&hdr->OptionalHeader);
    sec = IMAGE_FIRST_SECTION(hdr);
    for (int i = 0; i < hdr->FileHeader.NumberOfSections; i++) {
        log_section_header(sec);
        sec++;
    }
    for (int i = 0; i < ARRAYSIZE(hdr->OptionalHeader.DataDirectory); i++) {
        log_data_directory_entry(&hdr->OptionalHeader.DataDirectory[i]);
    }
#endif

    //
    // Relocate image
    //

    LOG("Rebasing image from %lx to %p", hdr->OptionalHeader.ImageBase, idata);
    ptrdiff_t base_delta = (ptrdiff_t)idata - hdr->OptionalHeader.ImageBase;

    PIMAGE_DATA_DIRECTORY basereloc =
        &hdr->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC];
    for (uintptr_t offset = 0; offset < basereloc->Size;) {
        PIMAGE_BASE_RELOCATION block =
            (void *)(idata + basereloc->VirtualAddress + offset);
        LOG("Processing Base Relocation Block at %lx\n"
            "VirtualAddress %x\n"
            "SizeOfBlock %x",
            basereloc->VirtualAddress + offset, block->VirtualAddress,
            block->SizeOfBlock);

        PWORD TypeOffset = (void *)(block + 1);
        size_t num_entries =
            (block->SizeOfBlock - sizeof(IMAGE_BASE_RELOCATION)) / sizeof(WORD);
        for (int i = 0; i < num_entries; i++) {
            int rtype = TypeOffset[i] >> 12;
            int roffset = TypeOffset[i] & 0xfff;
            LOG("- %d [%04x] Type %d offset %x", i, TypeOffset[i], rtype,
                roffset);
            switch (rtype) {
            case IMAGE_REL_BASED_ABSOLUTE:
                // Skip. Used for alignment.
                break;
            case IMAGE_REL_BASED_DIR64: {
                uintptr_t reloc_rva = block->VirtualAddress + roffset;
                uint64_t *target = (void *)(idata + reloc_rva);
                uint64_t old_value = *target;
                uint64_t new_value = old_value + base_delta;
                LOG("  - IMAGE_REL_BASED_DIR64 @%lx: %016lx -> %016lx",
                    reloc_rva, old_value, new_value);
                *target = new_value;
                break;
            }
            default:
                assert(!"Unhandled relocation type");
                break;
            }
        }

        offset += block->SizeOfBlock;
        assert(offset <= basereloc->Size);
    }

    hdr->OptionalHeader.ImageBase = (ULONGLONG)idata;
    LOG("Image rebased");

    //
    // Patch imports
    //

    PIMAGE_DATA_DIRECTORY imports =
        &hdr->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT];
    for (uintptr_t offset = 0; offset < imports->Size;) {
        PIMAGE_IMPORT_DESCRIPTOR desc =
            (void *)(idata + imports->VirtualAddress + offset);
        if (desc->Characteristics == 0) {
            LOG("Reached end of import descriptor table");
            break;
        }
#if DEBUG
        log_image_import_descriptor(idata, desc);
#endif
        uint64_t *lookup = (void *)(idata + desc->Characteristics);
        uint64_t *address = (void *)(idata + desc->FirstThunk);
        for (int i = 0; lookup[i]; i++) {
            if (lookup[i] & IMAGE_ORDINAL_FLAG64) {
                int ordinal = IMAGE_ORDINAL64(lookup[i]);
                LOG("- %016lx  Ordinal:%x Address:%lx", lookup[i], ordinal, address[i]);
                address[i] = resolve_import_by_ordinal((char *)(idata + desc->Name), ordinal);
            } else {
                PIMAGE_IMPORT_BY_NAME ibn = (void *)idata + lookup[i];
                LOG("- Hint:%04x Name:%s Address:%lx", ibn->Hint, ibn->Name, address[i]);
                address[i] = resolve_import_by_name((char *)(idata + desc->Name),
                                                    (char *)ibn->Name);
            }
        }
        desc->TimeDateStamp = 0x12345678; // Bound

        offset += sizeof(IMAGE_IMPORT_DESCRIPTOR);
        assert(offset <= imports->Size);
    }

#if DEBUG
    //
    // Examine exports
    //

    PIMAGE_DATA_DIRECTORY exports =
        &hdr->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT];
    if (exports->Size) {
        PIMAGE_EXPORT_DIRECTORY export_directory =
            (void *)(idata + exports->VirtualAddress);
        log_export_directory(idata, export_directory);
        assert(export_directory->NumberOfNames ==
               export_directory->NumberOfFunctions);
        DWORD *names = (void *)(idata + export_directory->AddressOfNames);
        DWORD *addresses =
            (void *)(idata + export_directory->AddressOfFunctions);
        for (int i = 0; i < export_directory->NumberOfNames; i++) {
            const char *name = (char *)(idata + names[i]);
            LOG("- %s @ %x", name, addresses[i]);
        }
    }
#endif

    //
    // Update section protection
    //

    LOG("Marking headers read-only");
    r = mprotect(idata,
                 ALIGN_UP(hdr->OptionalHeader.SizeOfHeaders,
                          hdr->OptionalHeader.SectionAlignment),
                 PROT_READ);
    assert(r == 0);

    LOG("Setting section protection");
    sec = IMAGE_FIRST_SECTION(hdr);
    for (int i = 0; i < hdr->FileHeader.NumberOfSections; i++) {
        int prot = PROT_READ;
        if (sec->Characteristics & IMAGE_SCN_MEM_WRITE)
            prot |= PROT_WRITE;
        if (sec->Characteristics & IMAGE_SCN_MEM_EXECUTE)
            prot |= PROT_EXEC;
        LOG("Marking section %.*s %s%s%s", (int)sizeof(sec->Name), sec->Name,
            (prot & PROT_READ) ? "r" : "", (prot & PROT_WRITE) ? "w" : "",
            (prot & PROT_EXEC) ? "x" : "");
        r = mprotect(idata + sec->VirtualAddress,
                     ALIGN_UP(sec->Misc.VirtualSize,
                              hdr->OptionalHeader.SectionAlignment),
                     prot);
        assert(r == 0);
        sec++;
    }

    Module *module = NULL;
    for (int i = 0; i < ARRAYSIZE(modules); i++) {
        if (!modules[i]) {
            module = malloc(sizeof(*module));
            module->name = strdup(basename(path));
            module->base = idata;
            module->exports = NULL;
            module->num_exports = 0;
            modules[i] = module;
            break;
        }
    }
    assert(module);

    //
    // Thread-Local Storage
    //

    LOG("Checking TLS");
    PIMAGE_DATA_DIRECTORY tls =
        &hdr->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_TLS];
    if (tls->Size) {
        PIMAGE_TLS_DIRECTORY64 tls_directory =
            (void *)(idata + tls->VirtualAddress);
#if DEBUG
        log_tls_directory(tls_directory);
#endif
        assert(tls_directory->Characteristics == IMAGE_SCN_ALIGN_16BYTES);
        assert(tls_directory->SizeOfZeroFill == 0);

        // FIXME: Move to module
        void *tls_data;

        size_t num_bytes_to_copy = tls_directory->EndAddressOfRawData -
                                   tls_directory->StartAddressOfRawData + 1;

        int r = posix_memalign(&tls_data, 0x10, num_bytes_to_copy);
        assert(!r && "posix_memalign");
        LOG("Allocated TLS data at %p", tls_data);

        memcpy(tls_data, (void*)tls_directory->StartAddressOfRawData, num_bytes_to_copy);
        assert(tls_index < (ARRAYSIZE(tls_array) - 1));
        tls_array[tls_index] = tls_data;
        *(uint32_t *)tls_directory->AddressOfIndex = tls_index;
        tls_index += 1;

        r = arch_prctl(ARCH_SET_GS, (void*)&teb);
        assert(!r && "arch_prctl");

        if (tls_directory->AddressOfCallBacks) {
            PIMAGE_TLS_CALLBACK *callbacks = (void*)tls_directory->AddressOfCallBacks;
            for (int i = 0; callbacks[i]; i++) {
                LOG("Executing TLS Callback %d @ %p", i, callbacks[i]);
                callbacks[i](idata, DLL_PROCESS_ATTACH, 0);
            }
        }
        LOG("TLS Callbacks complete");
    }

    // Jump to entrypoint
    typedef BOOL(WINAPI * NativeEntryPointFunc)(HINSTANCE, DWORD, LPVOID);

    NativeEntryPointFunc entrypoint =
        (void *)(idata + hdr->OptionalHeader.AddressOfEntryPoint);
    LOG("Jumping to entrypoint!");
    bool result = entrypoint((HINSTANCE)path, DLL_PROCESS_ATTACH, 0);
    LOG("Entrypoint returned %d\n", result);
    assert(result == 1);

    return idata;
}
