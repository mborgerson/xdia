#include <stdlib.h>

#include "../pe-loader/common.h"

char *command_line_a = "dummy.exe";
wchar_t *command_line_w = L"dummy.exe";

int wmain(int, const char *argv[]);

int main(int argc, const char *argv[])
{
    load_pe(getenv("MSDIA_PATH") ?: "msdia140.dll");

    int r = wmain(argc, argv);

    // XXX: blink will crash on macOS ARM64 during libc exit() with
    //        PC 4ffffffffff7 jp +46 48 7a 2e 2f 78 64 69 61
    fflush(stdout);
    _exit(r);

    return r;
}
