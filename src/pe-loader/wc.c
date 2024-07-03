#include "common.h"

char *wc2c(const wchar_t *s)
{
    return g_utf16_to_utf8(s, -1, NULL, NULL, NULL);
}

wchar_t *c2wc(const char *s)
{
    return g_utf8_to_utf16(s, -1, NULL, NULL, NULL);
}
