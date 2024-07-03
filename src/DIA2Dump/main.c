#include <stdlib.h>

#include "../pe-loader/common.h"

char *command_line_a = "dummy.exe";
wchar_t *command_line_w = L"dummy.exe";

int wmain(int, const char *argv[]);

int main(int argc, const char *argv[])
{
    const char *msdia140_dll_path = getenv("MSDIA_PATH");
    if (!msdia140_dll_path) {
        msdia140_dll_path = "msdia140.dll";
    }
    load_pe(msdia140_dll_path);

    int r = wmain(argc, argv);

    // XXX: blink will crash on macOS ARM64 during libc exit() with
    //        PC 4ffffffffff7 jp +46 48 7a 2e 2f 78 64 69 61
    fflush(stdout);
    _exit(r);

    return r;
}
