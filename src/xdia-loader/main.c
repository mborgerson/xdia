#include <stdlib.h>
#include "../pe-loader/common.h"

char *command_line_a;
wchar_t *command_line_w;

int main(int argc, const char *argv[])
{
    assert(argc == 2 && "usage: xdia.exe <pdb>");
    command_line_a = g_strdup_printf("xdia.exe %s", argv[1]);
    command_line_w = c2wc(command_line_a);

    load_pe(getenv("MSDIA_PATH") ?: "msdia140.dll");
    load_pe(getenv("XDIA_PATH") ?: "xdia.exe");

    return 0;
}
